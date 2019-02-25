#include "interface.h"
#include "inf_motion_aa.h"
//下料y轴
#pragma warning(disable:4244)
void read_SM103_position(void)
{
	long position;
	WaitForSingleObject(SM103_mutex,INFINITE);
	mn_get_enccounter(LINE_NO_1, DEVICE_SM103_No,&position);
	SM103_POSITION = (position);
	ReleaseMutex(SM103_mutex);
	return;
}

int SM103_home_search(void)
{
	int		errcode, timeout;
	BYTE	status, HomeMode, EZcount;
	
	HomeMode=0;
	EZcount =0;

	if(SM103_SERVO_ALARM==ON )	return(SM103_ALARM_ERR);
	if(SM103_SERVO_READY==ON)	return(SM103_NOT_READY_ERR);
	if(SM103_SERVO_CHECK==OFF)	return(SM103_NOT_READY_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	SpeedPar103_home.Drive_Speed = (long)(SM103_AXIS_HOME_SPEED); // unit = pulses per second PPS
	SpeedPar103_home.Acc		  = (long)(SM103_AXIS_HOME_ACCEL_TIME);
	SpeedPar103_home.Start_Speed = (int)(sqrt((double)SpeedPar103_home.Drive_Speed));	

	if (SpeedPar103_home.Start_Speed<2000)SpeedPar103_home.Start_Speed=2000;

	//=== HomeMode -0 =======================================================
	timeout = 1000;
	while (!SM103_LMT_N && timeout-- > 0 )
	{
		SM103_jog_ccw();
		Sleep(50);
	}
	if (timeout <= 0)
	{
		SM103_motor_stop();
		return(SM103_HOMING_FAILURE_ERR);
	}	
	Sleep(50);
	log("before set home");
	if((errcode = mn_home_start(LINE_NO_1,DEVICE_SM103_No, SpeedPar103_home, MOVE_DIRECTION_FORWARD,HomeMode,EZcount)) != SUCCESS)
		return(SM103_MOTION_COMMAND_ERR);
	log("after set home");
	timeout=5000;//50s
	status = OFF;
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if(EmergencyStopFlag==ON){
			SM103_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		if(HomeMachine_StopFlag==ON){ 
			SM103_motor_stop(); 
			return(0x00); 
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM103_No, &status);
		Sleep(10);
	}
	if(timeout <= 0x00){
		SM103_motor_stop();
		return(SM103_HOMING_FAILURE_ERR);
	}
	else{
		SM103_HOMED_FLAG = ON;
		log("sm103 home flag=%d",SM103_HOMED_FLAG);
		return(0);
	}

}
int SM103_motor_stop(void)
{
	WaitForSingleObject(SM103_mutex, INFINITE);
	mn_stop_move(LINE_NO_1, DEVICE_SM103_No, SUDDEN_STOP);
	ReleaseMutex(SM103_mutex);
	return(0);
}
int SM103_encoder_reset()
{
	int  errcode;
	long position;

	if((errcode=SM103_home_search()) != 0x00) return(errcode);
	log("err=%d",errcode);
	if(HomeMachine_StopFlag==ON){ SM103_motor_stop(); return(0x00); }

	Sleep(2000);
	WaitForSingleObject(SM103_mutex,INFINITE);

	mn_set_enccounter(LINE_NO_1, DEVICE_SM103_No, 0x00); 
	mn_get_enccounter(LINE_NO_1, DEVICE_SM103_No, &position);
	SM103_POSITION = (position);
	ReleaseMutex(SM103_mutex);
	return(0x00);
}
int SM103_jog_cw(void)
{
	int  errcode;
//	log("alram:%d,ready:%d,servo check:%d",SM103_SERVO_ALARM,SM103_SERVO_READY,SM103_SERVO_CHECK);
	if(SM103_SERVO_ALARM==ON)	return(SM103_ALARM_ERR);
	if(SM103_SERVO_READY==ON)	return(SM103_NOT_READY_ERR);
	if(SM103_SERVO_CHECK==OFF)	return(SM103_NOT_READY_ERR);

	SpeedPar103_jog.Drive_Speed = (long)(SM103_AXIS_JOG_SPEED);
	SpeedPar103_jog.Acc = (long)(SM103_AXIS_JOG_ACCEL_TIME);
	SpeedPar103_jog.Start_Speed = (int)(sqrt((double)SpeedPar103_jog.Drive_Speed ));

	if (SpeedPar103_jog.Start_Speed<2000)  SpeedPar103_jog.Start_Speed = 2000;
	DWORD dwrt = WaitForSingleObject(SM103_mutex, LONG_MAX);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM103_No, 
		SpeedPar103_jog,MOVE_DIRECTION_FORWARD)) != SUCCESS){
			ReleaseMutex(SM103_mutex);
			return(SM103_MOTION_COMMAND_ERR);
	}
	int rt = ReleaseMutex(SM103_mutex);
	return(0x00);
}
int SM103_jog_ccw(void)
{
	
	int  errcode;

	if(SM103_SERVO_ALARM==ON )	return(SM103_ALARM_ERR);
	if(SM103_SERVO_READY==ON)	return(SM103_NOT_READY_ERR);
	if(SM103_SERVO_CHECK==OFF)	return(SM103_NOT_READY_ERR);

	SpeedPar103_jog.Drive_Speed = (long)(SM103_AXIS_JOG_SPEED);
	SpeedPar103_jog.Acc		 = (long)(SM103_AXIS_JOG_ACCEL_TIME);
	SpeedPar103_jog.Start_Speed = (int)(sqrt((double)SpeedPar103_jog.Start_Speed));

	if (SpeedPar103_jog.Start_Speed<2000) SpeedPar103_jog.Start_Speed = 2000;
	WaitForSingleObject(SM103_mutex,INFINITE);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM103_No, 
		SpeedPar103_jog,MOVE_DIRECTION_REVERSE)) != SUCCESS){
			ReleaseMutex(SM103_mutex);
			return(SM103_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM103_mutex);
	return(0x00);
}  
int SM103_relative_movement(int target, int speed, int acceleration, int deceleration)//diandong
{
	
	long	distance;
	int		timeout, errcode;
	BYTE	status;


	if(SM103_SERVO_ALARM==ON )	return(SM103_ALARM_ERR);
	if(SM103_SERVO_READY==ON)	return(SM103_NOT_READY_ERR);
	if(SM103_SERVO_CHECK==OFF)	return(SM103_NOT_READY_ERR);
	if(SM103_LMT_N		==ON )	return(SM103_N_LIMIT_ERR);
	if(SM103_LMT_P		==ON )	return(SM103_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	read_SM103_position();//获取当前位
	distance = (target - SM103_POSITION); //target 目标位
	if(abs(distance) < 1) return(0);//ok

	SpeedPar103_pos.Drive_Speed = speed;
	SpeedPar103_pos.Acc		 = acceleration;
	SpeedPar103_pos.Dec		 = deceleration;
	SpeedPar103_pos.Start_Speed = (int)(sqrt((double)speed));

	if (SpeedPar103_pos.Start_Speed<2000) SpeedPar103_pos.Start_Speed = 2000;
	WaitForSingleObject(SM103_mutex,INFINITE);
	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM103_No, //风险，distance过大,需增加判断distance
		SpeedPar103_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
			ReleaseMutex(SM103_mutex);
			return(SM103_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM103_mutex);

	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM103_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM103_LMT_N==ON)||(SM103_LMT_P==ON)){
			SM103_motor_stop();
			if(SM103_LMT_P==ON) return(SM103_P_LIMIT_ERR);
			return(SM103_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM103_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM103_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕

	timeout = 1000; //10s
	while((abs(target-SM103_POSITION) > 4)&&(timeout-- >= 0x00)){
		if((SM103_LMT_N==ON)||(SM103_LMT_P==ON)){
			SM103_motor_stop();
			if(SM103_LMT_P==ON) return(SM103_P_LIMIT_ERR);
			return(SM103_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM103_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		read_SM103_position();
		Sleep(10);
	}//判断是否运动到目标值
	if(timeout < 0x00) return(SM103_NOT_IN_POSITION_ERR);
	return(0);
}
int SM103_move_to_home_position(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= SM103_HOME_OFFSET;
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_pass_position(void)
{
	int accel, speed,  y;
	double target;
	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= SM103_PASS_OFFSET+(LINE_Y_PASS_NUM*30*LINE_Y_RATIO);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM103_move_to_ng_position(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= SM103_NG_OFFSET-(LINE_Y_NG_NUM*30*LINE_Y_RATIO);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM103_move_product_home_position(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= SM103_PRODUCT_HOME_OFFSET;
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM103_move_to_lens_loading_position(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(AA_LENS_Y_LOADING_POS*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_relative_short_move(int MoveDistance)
{
	int accel, speed, target, y;

	read_SM103_position();
	target	= SM103_POSITION + MoveDistance; 
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM103_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}

int SM103_move_to_load_position1(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(SM103_POSITION_1*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_load_position2(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(SM103_POSITION_2*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_load_position3(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(SM103_POSITION_3*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_load_position4(void)//SM103_focus_x_offset_move_from_cone_position(void)
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	read_SM103_position();
	target	= (int)(SM103_POSITION_4*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_load_position5(void)//SM103_focus_x_move_to_laser_position
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(SM103_POSITION_5*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_move_to_load_position6(void)//SM103_focus_x_move_to_cone_position
{
	int accel, speed, target, y;

	if(SM103_HOMED_FLAG	==OFF)	return(SM103_NOT_HOMED_YET_WARNING);

	target	= (int)(SM103_POSITION_6*SM103_PositionRatio);
	speed	= (long)(SM103_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM103_AXIS_MOVE_ACCEL_TIME);

	if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM103_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM103_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM103_initialization(void)//SM103_focus_x_initialization
{
	int ret;

	if((SM103_HOMED_FLAG==ON)||(SM103_LMT_P==ON)){
		if((ret=SM103_move_to_home_position())	!= 0) return(ret);
	}
	else{
		if((ret=SM103_encoder_reset())	!= 0) return(ret);
	}
	return(0x00);
}

