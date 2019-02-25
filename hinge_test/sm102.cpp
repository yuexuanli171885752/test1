#include "interface.h"
#include "inf_motion_aa.h"
//堆垛下料升降电机

void read_SM102_position(void)
{
	long position;
	WaitForSingleObject(SM102_mutex,INFINITE);
	mn_get_enccounter(LINE_NO_1, DEVICE_SM102_No,&position);
	SM102_POSITION = (position);
	ReleaseMutex(SM102_mutex);
	return;
}

int SM102_home_search(void)
{
	int		errcode, timeout;
	BYTE	status, HomeMode, EZcount;

	HomeMode=0;
	EZcount =0;

	if(SM102_SERVO_ALARM==ON )	return(SM102_ALARM_ERR);
	if(SM102_SERVO_READY==ON)	return(SM102_NOT_READY_ERR);
	if(SM102_SERVO_CHECK==OFF)	return(SM102_NOT_READY_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	SpeedPar102_home.Drive_Speed = (long)(SM102_AXIS_HOME_SPEED); // unit = pulses per second PPS
	SpeedPar102_home.Acc		 = (long)(SM102_AXIS_HOME_ACCEL_TIME);
	SpeedPar102_home.Start_Speed = (int)(sqrt((double)SpeedPar102_home.Drive_Speed));	

	if (SpeedPar102_home.Start_Speed<2000)SpeedPar102_home.Start_Speed=2000;

	//=== Home PHASE-1 =======================================================
	
	timeout = 1000;
	
	while (!SM102_LMT_N && timeout-- > 0 )
	{
		SM102_jog_ccw();
		Sleep(50);
	}
	if (timeout <= 0)
	{
		SM102_motor_stop();
		return(SM102_HOMING_FAILURE_ERR);
	}	
	Sleep(50);
	log("before set home");
	if((errcode = mn_home_start(LINE_NO_1,DEVICE_SM102_No, SpeedPar102_home, MOVE_DIRECTION_FORWARD,HomeMode,EZcount)) != SUCCESS)
		return(SM102_MOTION_COMMAND_ERR);
	log("after set home");
	timeout=5000;//50s
	status = OFF;
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if(EmergencyStopFlag==ON){
			SM102_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		if(HomeMachine_StopFlag==ON){ 
			SM102_motor_stop(); 
			return(0x00); 
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM102_No, &status);
		Sleep(10);
	}
	if(timeout <= 0x00){
		SM102_motor_stop();
		return(SM102_HOMING_FAILURE_ERR);
	}
	else{
		SM102_HOMED_FLAG = ON;
		log("sm101 home flag=%d",SM102_HOMED_FLAG);
		return(0);
	}

}
int SM102_motor_stop(void)
{
	WaitForSingleObject(SM102_mutex, INFINITE);
	mn_stop_move(LINE_NO_1, DEVICE_SM102_No, SUDDEN_STOP);
	ReleaseMutex(SM102_mutex);
	return(0);
}
int SM102_encoder_reset()
{
	int  errcode;
	long position;

	if((errcode=SM102_home_search()) != 0x00) return(errcode);
	log("err=%d",errcode);
	if(HomeMachine_StopFlag==ON){ SM102_motor_stop(); return(0x00); }

	Sleep(4000);
	WaitForSingleObject(SM102_mutex,INFINITE);

	mn_set_enccounter(LINE_NO_1, DEVICE_SM102_No, 0x00); 
	mn_get_enccounter(LINE_NO_1, DEVICE_SM102_No, &position);
	SM102_POSITION = (position);
	ReleaseMutex(SM102_mutex);
	return(0x00);
}
int SM102_jog_cw(void)
{
	int  errcode;

	if(SM102_SERVO_ALARM==ON)	return(SM102_ALARM_ERR);
	if(SM102_SERVO_READY==ON)	return(SM102_NOT_READY_ERR);
	if(SM102_SERVO_CHECK==OFF)	return(SM102_NOT_READY_ERR);

	SpeedPar102_jog.Drive_Speed = (long)(SM102_AXIS_JOG_SPEED);
	SpeedPar102_jog.Acc = (long)(SM102_AXIS_JOG_ACCEL_TIME);
	SpeedPar102_jog.Start_Speed = (int)(sqrt((double)SpeedPar102_jog.Drive_Speed ));

	if (SpeedPar102_jog.Start_Speed<2000)  SpeedPar102_jog.Start_Speed = 2000;
	DWORD dwrt = WaitForSingleObject(SM102_mutex, LONG_MAX);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM102_No, 
		SpeedPar102_jog,MOVE_DIRECTION_FORWARD)) != SUCCESS){
			ReleaseMutex(SM102_mutex);
			return(SM102_MOTION_COMMAND_ERR);
	}
	int rt = ReleaseMutex(SM102_mutex);
	return(0x00);
}
int SM102_jog_ccw(void)
{
	
	int  errcode;

	if(SM102_SERVO_ALARM==ON )	return(SM102_ALARM_ERR);
	if(SM102_SERVO_READY==ON)	return(SM102_NOT_READY_ERR);
	if(SM102_SERVO_CHECK==OFF)	return(SM102_NOT_READY_ERR);

	SpeedPar102_jog.Drive_Speed = (long)(SM102_AXIS_JOG_SPEED);
	SpeedPar102_jog.Acc		 = (long)(SM102_AXIS_JOG_ACCEL_TIME);
	SpeedPar102_jog.Start_Speed = (int)(sqrt((double)SpeedPar102_jog.Start_Speed));

	if (SpeedPar102_jog.Start_Speed<2000) SpeedPar102_jog.Start_Speed = 2000;
	WaitForSingleObject(SM102_mutex,INFINITE);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM102_No, 
		SpeedPar102_jog,MOVE_DIRECTION_REVERSE)) != SUCCESS){
			ReleaseMutex(SM102_mutex);
			return(SM102_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM102_mutex);
	return(0x00);
}  
int SM102_relative_movement(int target, int speed, int acceleration, int deceleration)//diandong
{
	
	long	distance;
	int		timeout, errcode;
	BYTE	status;

	if(SM102_SERVO_ALARM==ON )	return(SM102_ALARM_ERR);
	if(SM102_SERVO_READY==ON)	return(SM102_NOT_READY_ERR);
	if(SM102_SERVO_CHECK==OFF)	return(SM102_NOT_READY_ERR);
	if(SM102_LMT_N		==ON )	return(SM102_N_LIMIT_ERR);
	if(SM102_LMT_P		==ON )	return(SM102_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	read_SM102_position();//获取当前位
	distance = (target - SM102_POSITION); //target 目标位
	if(abs(distance) < 1) return(0);//ok

	SpeedPar102_pos.Drive_Speed = speed;
	SpeedPar102_pos.Acc		    = acceleration;
	SpeedPar102_pos.Dec		    = deceleration;
	SpeedPar102_pos.Start_Speed = (int)(sqrt((double)speed));

	if (SpeedPar102_pos.Start_Speed<2000) SpeedPar102_pos.Start_Speed = 2000;
	WaitForSingleObject(SM102_mutex,INFINITE);
	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM102_No, //风险，distance过大,需增加判断distance
		SpeedPar102_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
			ReleaseMutex(SM102_mutex);
			return(SM102_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM102_mutex);

	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM102_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM102_LMT_N==ON)||(SM102_LMT_P==ON)){
			SM102_motor_stop();
			if(SM102_LMT_P==ON) return(SM102_P_LIMIT_ERR);
			return(SM102_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM102_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM102_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕

	timeout = 1000; //10s
	while((abs(target-SM102_POSITION) > 4)&&(timeout-- >= 0x00)){
		if((SM102_LMT_N==ON)||(SM102_LMT_P==ON)){
			SM102_motor_stop();
			if(SM102_LMT_P==ON) return(SM102_P_LIMIT_ERR);
			return(SM102_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM102_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		read_SM102_position();
		Sleep(10);
	}//判断是否运动到目标值
	if(timeout < 0x00) return(SM102_NOT_IN_POSITION_ERR);
	return(0);
}
int SM102_move_to_home_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= SM102_HOME_OFFSET;
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_lens_loading_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(AA_LENS_Y_LOADING_POS*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_relative_short_move(int MoveDistance)
{
	int accel, speed, target, y;

	read_SM102_position();
	target	= SM102_POSITION + MoveDistance; 
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM102_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}
int SM102_move_to_stack1_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= SM102_STACK1;
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM102_move_to_stack2_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= SM102_STACK2;
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM102_move_to_stack3_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= SM102_STACK3;
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM102_move_to_load_position1(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(SM102_POSITION_1*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_load_position2(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(SM102_POSITION_2*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_load_position3(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(SM102_POSITION_3*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_load_position4(void)//SM102_focus_x_offset_move_from_cone_position(void)
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	read_SM102_position();
	target	= (int)(SM102_POSITION_4*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_load_position5(void)//SM102_focus_x_move_to_laser_position
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(SM102_POSITION_5*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_move_to_load_position6(void)//SM102_focus_x_move_to_cone_position
{
	int accel, speed, target, y;

	if(SM102_HOMED_FLAG	==OFF)	return(SM102_NOT_HOMED_YET_WARNING);

	target	= (int)(SM102_POSITION_6*SM102_PositionRatio);
	speed	= (long)(SM102_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM102_AXIS_MOVE_ACCEL_TIME);

	if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM102_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM102_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM102_initialization(void)//SM102_focus_x_initialization
{
	int ret;

	if((SM102_HOMED_FLAG==ON)||(SM102_LMT_P==ON)){
		if((ret=SM102_move_to_home_position())	!= 0) return(ret);
	}
	else{
		if((ret=SM102_encoder_reset())	!= 0) return(ret);
	}
	return(0x00);
}
