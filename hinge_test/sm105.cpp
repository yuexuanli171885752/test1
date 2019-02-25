#include "interface.h"
#include "inf_motion_aa.h"
//转盘

void read_SM105_position(void)
{
	long position;
	WaitForSingleObject(SM105_mutex,INFINITE);
	mn_get_enccounter(LINE_NO_1, DEVICE_SM105_No,&position);
	SM105_POSITION = (position);
	ReleaseMutex(SM105_mutex);
	return;
}

int SM105_home_search(void)
{
	int		errcode, timeout;
	BYTE	status, HomeMode, EZcount;

	timeout=1000;//50s
	HomeMode=0;
	EZcount =0;

	if(SM105_SERVO_ALARM==ON )	return(SM105_ALARM_ERR);
	if(SM105_SERVO_READY==ON)	return(SM105_NOT_READY_ERR);
	if(SM105_SERVO_CHECK==OFF)	return(SM105_NOT_READY_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);
	if (FIRST_CYLINDER_UP_SENSOR || SECOND_CYLINDER_UP_SENSOR || THIRD_CYLINDER_UP_SENSOR 
		|| FOURTH_CYLINDER_UP_SENSOR || DARK_BOX_DOWN_SENSOR || LIGHT_BOX_DOWN_SENSOR||FEED_GRIPPER_DOWN_SENSOR||
		(!FEED_CYLINDER_RIGHT_SENSOR && !FEED_CYLINDER_LEFT_SENSOR))
		return (SM105_CYLINDER_UP_ERR);

	SM105_MOVING = ON;
	SpeedPar105_home.Drive_Speed	 = (long)(SM105_AXIS_HOME_SPEED);						// unit = pulses per second PPS
	SpeedPar105_home.Acc			 = (long)(SM105_AXIS_HOME_ACCEL_TIME);
	SpeedPar105_home.Start_Speed	 = (int)(sqrt((double)SpeedPar105_home.Drive_Speed));
	SpeedPar105_home.AccDec_Mode	 = ADC_MODE_RATE;
	if (SpeedPar105_home.Start_Speed<2000) SpeedPar105_home.Start_Speed=2000;
	SM105_jog_ccw();
	Sleep(2000);
	timeout=10000;//50s
	while(!SM105_HOME && timeout-- > 0)
	{	
		Sleep(10);
	}
	if (errcode=mn_stop_move(LINE_NO_1,DEVICE_SM105_No,SUDDEN_STOP)!=SUCCESS) return errcode;
		//=== Home PHASE-1 =======================================================
	Sleep(200);

	log("before set home");
//	log("SpeedPar105_home.Drive_Speed=%d,SpeedPar105_home.Acc=%d,SpeedPar105_home.Start_Speed=%d", SpeedPar105_home.Drive_Speed,SpeedPar105_home.Acc,SpeedPar105_home.Start_Speed);
	if((errcode = mn_home_start(LINE_NO_1,DEVICE_SM105_No, SpeedPar105_home, MOVE_DIRECTION_REVERSE,HomeMode,EZcount)) != SUCCESS)
		return(SM105_MOTION_COMMAND_ERR);
	log("after set home,errcode=%d",errcode);
	timeout=5000;//50s
	status = OFF;
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if(EmergencyStopFlag==ON){
			SM105_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		if(HomeMachine_StopFlag==ON){ 
			SM105_motor_stop(); 
			return(0x00); 
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM105_No, &status);
		Sleep(10);
	}
	if(timeout <= 0x00){
		SM105_motor_stop();
		return(SM105_HOMING_FAILURE_ERR);
	}
	else{
		SM105_HOMED_FLAG = ON;
		log("sm101 home flag=%d",SM105_HOMED_FLAG);
		return(0);
	}
	SM105_MOVING = OFF;
}
int SM105_motor_stop(void)
{
	SM105_MOVING = OFF;
	WaitForSingleObject(SM105_mutex, INFINITE);
	mn_stop_move(LINE_NO_1, DEVICE_SM105_No, SUDDEN_STOP);
	ReleaseMutex(SM105_mutex);
	return(0);
}
int SM105_encoder_reset()
{
	int  errcode;
	long position;

	if((errcode=SM105_home_search()) != 0x00) return(errcode);
	log("err=%d",errcode);
	if(HomeMachine_StopFlag==ON){ SM105_motor_stop(); return(0x00); }

	Sleep(2000);
	WaitForSingleObject(SM105_mutex,INFINITE);

	mn_set_enccounter(LINE_NO_1, DEVICE_SM105_No, 0x00); 
	mn_get_enccounter(LINE_NO_1, DEVICE_SM105_No, &position);
	SM105_POSITION = (position);
	ReleaseMutex(SM105_mutex);
	return(0x00);
}
int SM105_jog_cw(void)
{
	int  errcode;

	if(SM105_SERVO_ALARM==ON)	return(SM105_ALARM_ERR);
	if(SM105_SERVO_READY==ON)	return(SM105_NOT_READY_ERR);
	if(SM105_SERVO_CHECK==OFF)	return(SM105_NOT_READY_ERR);
	if (FIRST_CYLINDER_UP_SENSOR || SECOND_CYLINDER_UP_SENSOR || THIRD_CYLINDER_UP_SENSOR 
		|| FOURTH_CYLINDER_UP_SENSOR || DARK_BOX_DOWN_SENSOR || LIGHT_BOX_DOWN_SENSOR||FEED_GRIPPER_DOWN_SENSOR||
		(!FEED_CYLINDER_RIGHT_SENSOR && !FEED_CYLINDER_LEFT_SENSOR))
		return (SM105_CYLINDER_UP_ERR);
	SM105_MOVING = ON;
	SpeedPar105_jog.Drive_Speed = (long)(SM105_AXIS_JOG_SPEED);
	SpeedPar105_jog.Acc = (long)(SM105_AXIS_JOG_ACCEL_TIME);
	SpeedPar105_jog.Start_Speed = (int)(sqrt((double)SpeedPar105_jog.Drive_Speed ));

	if (SpeedPar105_jog.Start_Speed<2000)  SpeedPar105_jog.Start_Speed = 2000;
	DWORD dwrt = WaitForSingleObject(SM105_mutex, LONG_MAX);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM105_No, 
		SpeedPar105_jog,MOVE_DIRECTION_FORWARD)) != SUCCESS){
			ReleaseMutex(SM105_mutex);
			return(SM105_MOTION_COMMAND_ERR);
	}
	//log("SpeedPar105_jog.Drive_Speed =%d,SpeedPar105_jog.Acc=%d,SpeedPar105_jog.Start_Speed=%d",SpeedPar105_jog.Drive_Speed ,SpeedPar105_jog.Acc,SpeedPar105_jog.Start_Speed);
	int rt = ReleaseMutex(SM105_mutex);
	SM105_MOVING = OFF;
	return(0x00);
}
int SM105_jog_ccw(void)
{
	int  errcode;

	if(SM105_SERVO_ALARM==ON )	return(SM105_ALARM_ERR);
	if(SM105_SERVO_READY==ON)	return(SM105_NOT_READY_ERR);
	if(SM105_SERVO_CHECK==OFF)	return(SM105_NOT_READY_ERR);
	if (FIRST_CYLINDER_UP_SENSOR || SECOND_CYLINDER_UP_SENSOR || THIRD_CYLINDER_UP_SENSOR 
		|| FOURTH_CYLINDER_UP_SENSOR || DARK_BOX_DOWN_SENSOR || LIGHT_BOX_DOWN_SENSOR||FEED_GRIPPER_DOWN_SENSOR ||
		(!FEED_CYLINDER_RIGHT_SENSOR && !FEED_CYLINDER_LEFT_SENSOR))
		return (SM105_CYLINDER_UP_ERR);
	SM105_MOVING = ON;
	SpeedPar105_jog.Drive_Speed = (long)(SM105_AXIS_JOG_SPEED);
	SpeedPar105_jog.Acc		    = (long)(SM105_AXIS_JOG_ACCEL_TIME);
	SpeedPar105_jog.Start_Speed = (int)(sqrt((double)SpeedPar105_jog.Drive_Speed));

	if (SpeedPar105_jog.Start_Speed<2000) SpeedPar105_jog.Start_Speed = 2000;
	WaitForSingleObject(SM105_mutex,INFINITE);
	if((errcode=mn_velocity_move(LINE_NO_1, DEVICE_SM105_No, 
		SpeedPar105_jog,MOVE_DIRECTION_REVERSE)) != SUCCESS){
			ReleaseMutex(SM105_mutex);
			return(SM105_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM105_mutex);
	SM105_MOVING = OFF;
	return(0x00);
}

int SM105_turn90_cw(void)
{
	SM105_relative_short_move(90*SM105_PositionRatio);
	return 0;
}
int SM105_turn90_ccw(void)
{
	SM105_relative_short_move(-90*SM105_PositionRatio);
	return 0;
}
int SM105_relative_movement(int target, int speed, int acceleration, int deceleration)//diandong
{

	long	distance;
	int		timeout, errcode;
	BYTE	status;

	if(SM105_SERVO_ALARM==ON )	return(SM105_ALARM_ERR);
	if(SM105_SERVO_READY==ON)	return(SM105_NOT_READY_ERR);
	if(SM105_SERVO_CHECK==OFF)	return(SM105_NOT_READY_ERR);
	if(SM105_LMT_N		==ON )	return(SM105_N_LIMIT_ERR);
	if(SM105_LMT_P		==ON )	return(SM105_P_LIMIT_ERR);
	if(EMERGENCY_STOP_FLAG == TRUE) return(MACHINE_EMERGENCY_STOP_WARNING);
	if (FIRST_CYLINDER_UP_SENSOR || SECOND_CYLINDER_UP_SENSOR || THIRD_CYLINDER_UP_SENSOR 
		|| FOURTH_CYLINDER_UP_SENSOR || DARK_BOX_DOWN_SENSOR || LIGHT_BOX_DOWN_SENSOR||FEED_GRIPPER_DOWN_SENSOR||
		(!FEED_CYLINDER_RIGHT_SENSOR && !FEED_CYLINDER_LEFT_SENSOR))
		return (SM105_CYLINDER_UP_ERR);

	read_SM105_position();//获取当前位
	distance = (target - SM105_POSITION); //target 目标位
	if(abs(distance) < 1) return(0);//ok

	SpeedPar105_pos.Drive_Speed = speed;
	SpeedPar105_pos.Acc		    = acceleration;
	SpeedPar105_pos.Dec		    = deceleration;
	SpeedPar105_pos.Start_Speed = (int)(sqrt((double)speed));

	if (SpeedPar105_pos.Start_Speed<2000) SpeedPar105_pos.Start_Speed = 2000;
	WaitForSingleObject(SM105_mutex,INFINITE);
	if((errcode= mn_fix_move(LINE_NO_1, DEVICE_SM105_No, //风险，distance过大,需增加判断distance
		SpeedPar105_pos, distance, FIX_MOVE_MODE_REL)) != SUCCESS){
			ReleaseMutex(SM105_mutex);
			return(SM105_MOTION_COMMAND_ERR);
	}
	ReleaseMutex(SM105_mutex);

	timeout=2000;//20s
	mn_motion_done(LINE_NO_1, DEVICE_SM105_No, &status);
	while((status != MOTION_DONE)&&(timeout-- >= 0x00)){
		if((SM105_LMT_N==ON)||(SM105_LMT_P==ON)){
			SM105_motor_stop();
			if(SM105_LMT_P==ON) return(SM105_P_LIMIT_ERR);
			return(SM105_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM105_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		mn_motion_done(LINE_NO_1, DEVICE_SM105_No, &status);
		Sleep(10);
	}//急停,判断极限,过程从开始运动到脉冲发送完毕

	timeout = 1000; //10s
	while((abs(target-SM105_POSITION) > 4)&&(timeout-- >= 0x00)){
		if((SM105_LMT_N==ON)||(SM105_LMT_P==ON)){
			SM105_motor_stop();
			if(SM105_LMT_P==ON) return(SM105_P_LIMIT_ERR);
			return(SM105_N_LIMIT_ERR);
		}
		if(EmergencyStopFlag==ON){
			SM105_motor_stop();
			return(MACHINE_EMERGENCY_STOP_WARNING);
		}
		read_SM105_position();
		Sleep(10);
	}//判断是否运动到目标值
	SM105_MOVING = OFF;
	if(timeout < 0x00) return(SM105_NOT_IN_POSITION_ERR);
	return(0);
}
int SM105_move_to_home_position(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= SM105_HOME_OFFSET;
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_lens_loading_position(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(AA_LENS_Y_LOADING_POS*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_relative_short_move(int MoveDistance)
{
	int accel, speed, target, y;
	SM105_MOVING = ON;
	read_SM105_position();
	target	= SM105_POSITION + MoveDistance; 
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if(abs(SM105_SHORT_MOVE_DISTANCE) < 1) return(0);
	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	return(0x00);
}

int SM105_move_to_load_position1(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(SM105_POSITION_1*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_load_position2(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(SM105_POSITION_2*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_load_position3(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(SM105_POSITION_3*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_load_position4(void)//SM105_focus_x_offset_move_from_cone_position(void)
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	read_SM105_position();
	target	= (int)(SM105_POSITION_4*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_load_position5(void)//SM105_focus_x_move_to_laser_position
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(SM105_POSITION_5*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_move_to_load_position6(void)//SM105_focus_x_move_to_cone_position
{
	int accel, speed, target, y;

	if(SM105_HOMED_FLAG	==OFF)	return(SM105_NOT_HOMED_YET_WARNING);

	target	= (int)(SM105_POSITION_6*SM105_PositionRatio);
	speed	= (long)(SM105_AXIS_MOVE_SPEED); // unit = pulses per second PPS
	accel	= (long)(SM105_AXIS_MOVE_ACCEL_TIME);

	if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00){
		if(y==MACHINE_EMERGENCY_STOP_WARNING){ SM105_motor_stop(); return(MACHINE_EMERGENCY_STOP_WARNING); }
		if((y=SM105_relative_movement(target, speed, accel, accel)) != 0x00) return(y);
	}
	return(0x00);
}

int SM105_initialization(void)//SM105_focus_x_initialization
{
	int ret;

	if((SM105_HOMED_FLAG==ON)||(SM105_LMT_P==ON)){
		if((ret=SM105_move_to_home_position())	!= 0) return(ret);
	}
	else{
		if((ret=SM105_encoder_reset())	!= 0) return(ret);
	}
	return(0x00);
}
