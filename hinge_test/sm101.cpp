#include "interface.h"
#include "inf_motion_aa.h"
//图卡升降


void read_SM101_position(void)
{
	long position;
	WaitForSingleObject(SM101_mutex,INFINITE);
	mn_get_enccounter(LINE_NO_1, DEVICE_SM101_No,&position);
	SM101_POSITION = (position);
	ReleaseMutex(SM101_mutex);
	return;
}

int SM101_home_search(void)
{
	int		errcode, timeout;
	BYTE	status, HomeMode, EZcount;

	HomeMode=0;
	EZcount =0;

	if(SM101_SERVO_ALARM==ON )	return(SM101_ALARM_ERR);
	if(SM101_SERVO_READY==ON)	return(SM101_NOT_READY_ERR);
	if(SM101_SERVO_CHECK==OFF)	return(SM101_NOT_READY_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	SpeedPar101_home.Drive_Speed = (long)(SM101_AXIS_HOME_SPEED); // unit = pulses per second PPS
	SpeedPar101_home.Acc		  = (long)(SM101_AXIS_HOME_ACCEL_TIME);
	SpeedPar101_home.Start_Speed = (int)(sqrt((double)SpeedPar101_home.Drive_Speed));	

	if (SpeedPar101_home.Start_Speed<2000)SpeedPar101_home.Start_Speed=2000;

	//=== Home PHASE-1 =======================================================

	log("before set home");
	if(!SM101_LMT_P)
	{
		SM101_jog_cw();
		Sleep(50);
	}
	if((errcode = mn_home_start(LINE_NO_1,DEVICE_SM101_No, SpeedPar101_home, MOVE_DIRECTION_REVERSE,HomeMode,EZcount)) != SUCCESS)
	return(SM101_MOTION_COMMAND_ERR);
	log("after set home");
	timeout=5000;//50s
	status = OFF;
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if(EmergencyStopFlag==ON){
			SM101_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		if(HomeMachine_StopFlag==ON){ 
			SM101_motor_stop(); 
			return(0x00); 
		}
		 mn_motion_done(LINE_NO_1, DEVICE_SM101_No, &status);
		Sleep(10);
	}
	if(timeout <= 0x00){
		SM101_motor_stop();
		return(SM101_HOMING_FAILURE_ERR);
	}
	else{
		SM101_HOMED_FLAG = ON;
		log("sm101 home flag=%d",SM101_HOMED_FLAG);
		return(0);
	}
	
}
int SM101_motor_stop(void)
{
	WaitForSingleObject(SM101_mutex, INFINITE);
	mn_stop_move(LINE_NO_1, DEVICE_SM101_No, SUDDEN_STOP);
	ReleaseMutex(SM101_mutex);
	return(0);
}
int SM101_encoder_reset()
{
	int  errcode;
	long position;

	if((errcode=SM101_home_search()) != 0x00) return(errcode);
	log("err=%d",errcode);
	if(HomeMachine_StopFlag==ON){ SM101_motor_stop(); return(0x00); }

	Sleep(1500);
	WaitForSingleObject(SM101_mutex,INFINITE);

	mn_set_enccounter(LINE_NO_1, DEVICE_SM101_No, 0x00); 
	mn_get_enccounter(LINE_NO_1, DEVICE_SM101_No, &position);
	SM101_POSITION = (position);
	ReleaseMutex(SM101_mutex);
	return(0x00);
}
int SM101_jog_cw(void)
{
	int  errcode;

	if(SM101_SERVO_ALARM==ON)	return(SM101_ALARM_ERR);
	if(SM101_SERVO_READY==ON)	return(SM101_NOT_READY_ERR);
	if(SM101_SERVO_CHECK==OFF)	return(SM101_NOT_READY_ERR);

	SpeedPar101_jog.Drive_Speed = (long)(SM101_AXIS_JOG_SPEED);
	SpeedPar101_jog.Acc = (long)(SM101_AXIS_JOG_ACCEL_TIME);
	SpeedPar101_jog.Start_Speed = (int)(sqrt((double)SpeedPar101_jog.Drive_Speed ));

	if (SpeedPar101_jog.Start_Speed<2000)  SpeedPar101_jog.Start_Speed = 2000;
	DWORD dwrt = WaitForSingleObject(SM101_mutex, LONG_MAX);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM101_No, 
		SpeedPar101_jog,MOVE_DIRECTION_FORWARD)) != SUCCESS){
			ReleaseMutex(SM101_mutex);
			return(SM101_MOTION_COMMAND_ERR);
	}
	int rt = ReleaseMutex(SM101_mutex);
	return(0x00);
}
int SM101_jog_ccw(void)
{
	
	int  errcode;

	if(SM101_SERVO_ALARM==ON )	return(SM101_ALARM_ERR);
	if(SM101_SERVO_READY==ON)	return(SM101_NOT_READY_ERR);
	if(SM101_SERVO_CHECK==OFF)	return(SM101_NOT_READY_ERR);

	SpeedPar101_jog.Drive_Speed = (long)(SM101_AXIS_JOG_SPEED);
	SpeedPar101_jog.Acc		 = (long)(SM101_AXIS_JOG_ACCEL_TIME);
	SpeedPar101_jog.Start_Speed = (int)(sqrt((double)SpeedPar101_jog.Start_Speed));

	if (SpeedPar101_jog.Start_Speed<2000) SpeedPar101_jog.Start_Speed = 2000;
	WaitForSingleObject(SM101_mutex,INFINITE);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM101_No, 
		SpeedPar101_jog,MOVE_DIRECTION_REVERSE)) != SUCCESS){
			ReleaseMutex(SM101_mutex);
			return(SM101_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM101_mutex);
	return(0x00);
}  
int SM101_relative_movement(int target, int speed, int acceleration, int deceleration)//diandong
{

	long	distance;
	int		timeout, errcode;
	BYTE	status;
	

	if(SM101_SERVO_ALARM==ON )	return(SM101_ALARM_ERR);
	if(SM101_SERVO_READY==ON)	return(SM101_NOT_READY_ERR);
	if(SM101_SERVO_CHECK==OFF)	return(SM101_NOT_READY_ERR);
	if(SM101_LMT_N		==ON )	return(SM101_N_LIMIT_ERR);
	if(SM101_LMT_P		==ON )	return(SM101_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);

	read_SM101_position();//获取当前位
	distance = (target - SM101_POSITION); //target 目标位
	if(abs(distance) < 1) return(0);//ok

	SpeedPar101_pos.Drive_Speed = speed;
	SpeedPar101_pos.Acc		 = acceleration;
	SpeedPar101_pos.Dec		 = deceleration;
	SpeedPar101_pos.Start_Speed = (int)(sqrt((double)speed));

	if (SpeedPar101_pos.Start_Speed<2000) SpeedPar101_pos.Start_Speed = 2000;
	WaitForSingleObject(SM101_mutex,INFINITE);
	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM101_No, //风险，distance过大,需增加判断distance
		SpeedPar101_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
			ReleaseMutex(SM101_mutex);
			return(SM101_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM101_mutex);

	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM101_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM101_LMT_N==ON)||(SM101_LMT_P==ON)){
			SM101_motor_stop();
			if(SM101_LMT_P==ON) return(SM101_P_LIMIT_ERR);
			return(SM101_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM101_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM101_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕

	timeout = 1000; //10s
	while((abs(target-SM101_POSITION) > 4)&&(timeout-- >= 0x00)){
		if((SM101_LMT_N==ON)||(SM101_LMT_P==ON)){
			SM101_motor_stop();
			if(SM101_LMT_P==ON) return(SM101_P_LIMIT_ERR);
			return(SM101_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM101_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		read_SM101_position();
		Sleep(10);
	}//判断是否运动到目标值
	if(timeout < 0x00) return(SM101_NOT_IN_POSITION_ERR);
	return(0);
}
int SM101_move_to_home_position(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= SM101_HOME_OFFSET;
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_lens_loading_position(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(AA_LENS_Y_LOADING_POS*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_relative_short_move(int MoveDistance)
{
	int accel, speed, target, y;

	read_SM101_position();
	target	= SM101_POSITION + MoveDistance; 
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM101_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}

int SM101_move_to_load_position1(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(SM101_POSITION_1*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_load_position2(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(SM101_POSITION_2*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_load_position3(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(SM101_POSITION_3*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_load_position4(void)//SM101_focus_x_offset_move_from_cone_position(void)
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	read_SM101_position();
	target	= (int)(SM101_POSITION_4*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_load_position5(void)//SM101_focus_x_move_to_laser_position
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(SM101_POSITION_5*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_move_to_load_position6(void)//SM101_focus_x_move_to_cone_position
{
	int accel, speed, target, y;

	if(SM101_HOMED_FLAG	==OFF)	return(SM101_NOT_HOMED_YET_WARNING);

	target	= (int)(SM101_POSITION_6*SM101_PositionRatio);
	speed	= (long)(SM101_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM101_AXIS_MOVE_ACCEL_TIME);

	if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM101_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM101_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM101_initialization(void)//SM101_focus_x_initialization
{
	int ret;

	if((SM101_HOMED_FLAG==ON)||(SM101_LMT_P==ON)){
		if((ret=SM101_move_to_home_position())	!= 0) return(ret);
	}
	else{
		if((ret=SM101_encoder_reset())	!= 0) return(ret);
	}
	return(0x00);
}



