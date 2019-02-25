
#include "interface.h"
#include "inf_motion_aa.h"
//下料x轴
#pragma warning(disable:4244)
void read_SM104_position(void)
{
	long position;
	WaitForSingleObject(SM104_mutex,INFINITE);
	mn_get_enccounter(LINE_NO_1, DEVICE_SM104_No,&position);
	SM104_POSITION = (position);
	ReleaseMutex(SM104_mutex);
	return;
}

int SM104_home_search(void)
{
	int		errcode, timeout;
	BYTE	status, HomeMode, EZcount;

	HomeMode=0;
	EZcount =0;

	if(SM104_SERVO_ALARM==ON )	return(SM104_ALARM_ERR);
	if(SM104_SERVO_READY==ON)	return(SM104_NOT_READY_ERR);
	if(SM104_SERVO_CHECK==OFF)	return(SM104_NOT_READY_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);
	if(PRODUCT_GRIPPER_DOWN_SENSOR==ON)return(PRODUCT_GRIPPER_DOWN_ERR);

	SpeedPar104_home.Drive_Speed = (long)(SM104_AXIS_HOME_SPEED); // unit = pulses per second PPS
	SpeedPar104_home.Acc		  = (long)(SM104_AXIS_HOME_ACCEL_TIME);
	SpeedPar104_home.Start_Speed = (int)(sqrt((double)SpeedPar104_home.Drive_Speed));	

	if (SpeedPar104_home.Start_Speed<2000)SpeedPar104_home.Start_Speed=2000;

	//=== Home PHASE-1 =======================================================

	log("before set home");
	timeout = 1000;
	while (!SM104_LMT_N && timeout-- > 0 )
	{
		SM104_jog_ccw();
		Sleep(50);
	}
	if (timeout <= 0)
	{
		SM104_motor_stop();
		return(SM104_HOMING_FAILURE_ERR);
	}	
	Sleep(50);
	if((errcode = mn_home_start(LINE_NO_1,DEVICE_SM104_No, SpeedPar104_home, MOVE_DIRECTION_FORWARD,HomeMode,EZcount)) != SUCCESS)
		return(SM104_MOTION_COMMAND_ERR);
	log("after set home");
	timeout=5000;//50s
	status = OFF;
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if(EmergencyStopFlag==ON){
			SM104_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		if(HomeMachine_StopFlag==ON){ 
			SM104_motor_stop(); 
			return(0x00); 
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM104_No, &status);
		Sleep(10);
	}
	if(timeout <= 0x00){
		SM104_motor_stop();
		return(SM104_HOMING_FAILURE_ERR);
	}
	else{
		SM104_HOMED_FLAG = ON;
		log("sm101 home flag=%d",SM104_HOMED_FLAG);
		return(0);
	}

}
int SM104_motor_stop(void)
{
	WaitForSingleObject(SM104_mutex, INFINITE);
	mn_stop_move(LINE_NO_1, DEVICE_SM104_No, SUDDEN_STOP);
	ReleaseMutex(SM104_mutex);
	return(0);
}
int SM104_encoder_reset()
{
	int  errcode;
	long position;

	if((errcode=SM104_home_search()) != 0x00) return(errcode);
	log("err=%d",errcode);
	if(HomeMachine_StopFlag==ON){ SM104_motor_stop(); return(0x00); }

	Sleep(2000);
	WaitForSingleObject(SM104_mutex,INFINITE);

	mn_set_enccounter(LINE_NO_1, DEVICE_SM104_No, 0x00); 
	mn_get_enccounter(LINE_NO_1, DEVICE_SM104_No, &position);
	SM104_POSITION = (position);
	ReleaseMutex(SM104_mutex);
	return(0x00);
}
int SM104_jog_cw(void)
{
	int  errcode;

	if(SM104_SERVO_ALARM==ON)	return(SM104_ALARM_ERR);
	if(SM104_SERVO_READY==ON)	return(SM104_NOT_READY_ERR);
	if(SM104_SERVO_CHECK==OFF)	return(SM104_NOT_READY_ERR);
	if(PRODUCT_GRIPPER_DOWN_SENSOR==ON)return(PRODUCT_GRIPPER_DOWN_ERR);

	SpeedPar104_jog.Drive_Speed = (long)(SM104_AXIS_JOG_SPEED);
	SpeedPar104_jog.Acc = (long)(SM104_AXIS_JOG_ACCEL_TIME);
	SpeedPar104_jog.Start_Speed = (int)(sqrt((double)SpeedPar104_jog.Drive_Speed ));

	if (SpeedPar104_jog.Start_Speed<2000)  SpeedPar104_jog.Start_Speed = 2000;
	DWORD dwrt = WaitForSingleObject(SM104_mutex, LONG_MAX);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM104_No, 
		SpeedPar104_jog,MOVE_DIRECTION_FORWARD)) != SUCCESS){
			ReleaseMutex(SM104_mutex);
			return(SM104_MOTION_COMMAND_ERR);
	}
	int rt = ReleaseMutex(SM104_mutex);
	return(0x00);
}
int SM104_jog_ccw(void)
{
	
	int  errcode;
	
	if(SM104_SERVO_ALARM==ON )	return(SM104_ALARM_ERR);
	if(SM104_SERVO_READY==ON)	return(SM104_NOT_READY_ERR);
	if(SM104_SERVO_CHECK==OFF)	return(SM104_NOT_READY_ERR);
	if(PRODUCT_GRIPPER_DOWN_SENSOR==ON)return(PRODUCT_GRIPPER_DOWN_ERR);

	SpeedPar104_jog.Drive_Speed = (long)(SM104_AXIS_JOG_SPEED);
	SpeedPar104_jog.Acc		    = (long)(SM104_AXIS_JOG_ACCEL_TIME);
	SpeedPar104_jog.Start_Speed = (int)(sqrt((double)SpeedPar104_jog.Drive_Speed));

	if (SpeedPar104_jog.Start_Speed<2000) SpeedPar104_jog.Start_Speed = 2000;
	WaitForSingleObject(SM104_mutex,INFINITE);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM104_No, 
		SpeedPar104_jog,MOVE_DIRECTION_REVERSE)) != SUCCESS){
			ReleaseMutex(SM104_mutex);
			return(SM104_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM104_mutex);
	return(0x00);
}  
/*int SM104_pos_feed(void)
{
	long	distance;
	int		timeout, errcode;
	BYTE	status;

	if(SM104_SERVO_ALARM==ON )	return(SM104_ALARM_ERR);
	if(SM104_SERVO_READY==ON )	return(SM104_NOT_READY_ERR);
	if(SM104_SERVO_CHECK==OFF)	return(SM104_NOT_READY_ERR);
	if(SM104_LMT_N		==ON )	return(SM104_N_LIMIT_ERR);
	if(SM104_LMT_P		==ON )	return(SM104_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	SpeedPar104_pos.Drive_Speed = (long)(SM104_AXIS_MOVE_SPEED);
	SpeedPar104_pos.Acc		    = (long)(SM104_AXIS_MOVE_ACCEL_TIME);
	SpeedPar104_pos.Dec		    = (long)(SM104_AXIS_MOVE_ACCEL_TIME);
	SpeedPar104_pos.Start_Speed = (int)(sqrt((double)SpeedPar104_pos.Drive_Speed));

	distance= 2000;

	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM104_No, //风险，distance过大,需增加判断distance
		SpeedPar104_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
		return(SM104_MOTION_COMMAND_ERR);
	}
	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM104_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM104_LMT_N==ON)||(SM104_LMT_P==ON)){
			SM104_motor_stop();
			if(SM104_LMT_P==ON) return(SM104_P_LIMIT_ERR);
			return(SM104_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM104_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM104_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕
}*/

int SM104_relative_movement(int target, int speed, int acceleration, int deceleration)//diandong
{

	long	distance;
	int		timeout, errcode;
	BYTE	status;

	if(SM104_SERVO_ALARM==ON )	return(SM104_ALARM_ERR);
	if(SM104_SERVO_READY==ON)	return(SM104_NOT_READY_ERR);
	if(SM104_SERVO_CHECK==OFF)	return(SM104_NOT_READY_ERR);
	if(SM104_LMT_N		==ON )	return(SM104_N_LIMIT_ERR);
	if(SM104_LMT_P		==ON )	return(SM104_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);
	if(PRODUCT_GRIPPER_DOWN_SENSOR==ON)return(PRODUCT_GRIPPER_DOWN_ERR);

	read_SM104_position();//获取当前位
	distance = (target - SM104_POSITION); //target 目标位
	if(abs(distance) < 1) return(0);//ok

	SpeedPar104_pos.Drive_Speed = speed;
	SpeedPar104_pos.Acc		 = acceleration;
	SpeedPar104_pos.Dec		 = deceleration;
	SpeedPar104_pos.Start_Speed = (int)(sqrt((double)speed));

	if (SpeedPar104_pos.Start_Speed<2000) SpeedPar104_pos.Start_Speed = 2000;
	WaitForSingleObject(SM104_mutex,INFINITE);
	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM104_No, //风险，distance过大,需增加判断distance
		SpeedPar104_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
			ReleaseMutex(SM104_mutex);
			return(SM104_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM104_mutex);

	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM104_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM104_LMT_N==ON)||(SM104_LMT_P==ON)){
			SM104_motor_stop();
			if(SM104_LMT_P==ON) return(SM104_P_LIMIT_ERR);
			return(SM104_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM104_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM104_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕

	timeout = 1000; //10s
	while((abs(target-SM104_POSITION) > 4)&&(timeout-- >= 0x00)){
		if((SM104_LMT_N==ON)||(SM104_LMT_P==ON)){
			SM104_motor_stop();
			if(SM104_LMT_P==ON) return(SM104_P_LIMIT_ERR);
			return(SM104_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM104_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		read_SM104_position();
		Sleep(10);
	}//判断是否运动到目标值
	if(timeout < 0x00) return(SM104_NOT_IN_POSITION_ERR);
	return(0);
}
int SM104_move_to_home_position(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= SM104_HOME_OFFSET;
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM104_move_to_pass_position(void)
{
	int accel, speed, y;
	double target;
	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= SM104_PASS_OFFSET-(LINE_X_PASS_NUM*(42.0*LINE_X_RATIO));
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM104_move_to_ng_position(void)
{
	int accel, speed, y;
	double target;
	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= SM104_NG_OFFSET-(LINE_X_NG_NUM*(42.0*LINE_X_RATIO));
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM104_move_product_home_position(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= SM104_PRODUCT_HOME_OFFSET;
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}
int SM104_move_to_lens_loading_position(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(AA_LENS_Y_LOADING_POS*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_relative_short_move(int MoveDistance)
{
	int accel, speed, target, y;

	read_SM104_position();
	target	= SM104_POSITION + MoveDistance; 
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM104_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}
/*
int SM104_feed_move(int MoveDistance)
{
	int accel, speed, target, y;

	read_SM104_position();
	target	= SM104_POSITION + MoveDistance; 
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM104_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}*/

int SM104_move_to_load_position1(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(SM104_POSITION_1*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_move_to_load_position2(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(SM104_POSITION_2*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_move_to_load_position3(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(SM104_POSITION_3*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_move_to_load_position4(void)//SM104_focus_x_offset_move_from_cone_position(void)
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	read_SM104_position();
	target	= (int)(SM104_POSITION_4*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_move_to_load_position5(void)//SM104_focus_x_move_to_laser_position
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(SM104_POSITION_5*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_move_to_load_position6(void)//SM104_focus_x_move_to_cone_position
{
	int accel, speed, target, y;

	if(SM104_HOMED_FLAG	==OFF)	return(SM104_NOT_HOMED_YET_WARNING);

	target	= (int)(SM104_POSITION_6*SM104_PositionRatio);
	speed	= (long)(SM104_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM104_AXIS_MOVE_ACCEL_TIME);

	if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM104_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM104_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM104_initialization(void)//SM104_focus_x_initialization
{
	int ret;

	if((SM104_HOMED_FLAG==ON)||(SM104_LMT_P==ON)){
		if((ret=SM104_move_to_home_position())	!= 0) return(ret);
	}
	else{
		if((ret=SM104_encoder_reset())	!= 0) return(ret);
	}
	return(0x00);
}
