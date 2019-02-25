#include "inf_motion_aa.h"
#include "interface.h"



DWORD Iokernel(LPVOID lpvThreadParm)
{
	while (1){

		//=== Servo Motor State ====================================================
		if(SM101_HOME==ON)			mshm.SM101_MOTOR_STATUS |= 0x01;
		else						mshm.SM101_MOTOR_STATUS &= 0xfe;
		if(SM101_LMT_P==ON)			mshm.SM101_MOTOR_STATUS |= 0x02;
		else						mshm.SM101_MOTOR_STATUS &= 0xfd;
		if(SM101_LMT_N==ON)			mshm.SM101_MOTOR_STATUS |= 0x04;
		else						mshm.SM101_MOTOR_STATUS &= 0xfb;
		if(SM101_SERVO_ALARM==ON)	mshm.SM101_MOTOR_STATUS |= 0x08;
		else						mshm.SM101_MOTOR_STATUS &= 0xf7;
		if(SM101_SERVO_CHECK==ON)	mshm.SM101_MOTOR_STATUS |= 0x10;
		else						mshm.SM101_MOTOR_STATUS &= 0xef;
		if(SM101_INPOS==ON)			mshm.SM101_MOTOR_STATUS |= 0x20;
		else						mshm.SM101_MOTOR_STATUS &= 0xdf;
		//--------------------------------------------------------------------------
		if(SM102_HOME==ON)			mshm.SM102_MOTOR_STATUS |= 0x01;
		else						mshm.SM102_MOTOR_STATUS &= 0xfe;
		if(SM102_LMT_P==ON)			mshm.SM102_MOTOR_STATUS |= 0x02;
		else						mshm.SM102_MOTOR_STATUS &= 0xfd;
		if(SM102_LMT_N==ON)			mshm.SM102_MOTOR_STATUS |= 0x04;
		else						mshm.SM102_MOTOR_STATUS &= 0xfb;
		if(SM102_SERVO_ALARM==ON)	mshm.SM102_MOTOR_STATUS |= 0x08;
		else						mshm.SM102_MOTOR_STATUS &= 0xf7;
		if(SM102_SERVO_CHECK==ON)	mshm.SM102_MOTOR_STATUS |= 0x10;
		else						mshm.SM102_MOTOR_STATUS &= 0xef;
		if(SM102_INPOS==ON)			mshm.SM102_MOTOR_STATUS |= 0x20;
		else						mshm.SM102_MOTOR_STATUS &= 0xdf;
		//--------------------------------------------------------------------------
		if(SM103_HOME==ON)			mshm.SM103_MOTOR_STATUS |= 0x01;
		else						mshm.SM103_MOTOR_STATUS &= 0xfe;
		if(SM103_LMT_P==ON)			mshm.SM103_MOTOR_STATUS |= 0x02;
		else						mshm.SM103_MOTOR_STATUS &= 0xfd;
		if(SM103_LMT_N==ON)			mshm.SM103_MOTOR_STATUS |= 0x04;
		else						mshm.SM103_MOTOR_STATUS &= 0xfb;
		if(SM103_SERVO_ALARM==ON)	mshm.SM103_MOTOR_STATUS |= 0x08;
		else						mshm.SM103_MOTOR_STATUS &= 0xf7;
		if(SM103_SERVO_CHECK==ON)	mshm.SM103_MOTOR_STATUS |= 0x10;
		else						mshm.SM103_MOTOR_STATUS &= 0xef;
		if(SM103_INPOS==ON)			mshm.SM103_MOTOR_STATUS |= 0x20;
		else						mshm.SM103_MOTOR_STATUS &= 0xdf;
		//--------------------------------------------------------------------------
		if(SM104_HOME==ON)			mshm.SM104_MOTOR_STATUS |= 0x01;
		else						mshm.SM104_MOTOR_STATUS &= 0xfe;
		if(SM104_LMT_P==ON)			mshm.SM104_MOTOR_STATUS |= 0x02;
		else						mshm.SM104_MOTOR_STATUS &= 0xfd;
		if(SM104_LMT_N==ON)			mshm.SM104_MOTOR_STATUS |= 0x04;
		else						mshm.SM104_MOTOR_STATUS &= 0xfb;
		if(SM104_SERVO_ALARM==ON)	mshm.SM104_MOTOR_STATUS |= 0x08;
		else						mshm.SM104_MOTOR_STATUS &= 0xf7;
		if(SM104_SERVO_CHECK==ON)	mshm.SM104_MOTOR_STATUS |= 0x10;
		else						mshm.SM104_MOTOR_STATUS &= 0xef;
		if(SM104_INPOS==ON)			mshm.SM104_MOTOR_STATUS |= 0x20;
		else						mshm.SM104_MOTOR_STATUS &= 0xdf;
		//--------------------------------------------------------------------------
		if(SM105_HOME==ON)			mshm.SM105_MOTOR_STATUS |= 0x01;
		else						mshm.SM105_MOTOR_STATUS &= 0xfe;
		if(SM105_LMT_P==ON)			mshm.SM105_MOTOR_STATUS |= 0x02;
		else						mshm.SM105_MOTOR_STATUS &= 0xfd;
		if(SM105_LMT_N==ON)			mshm.SM105_MOTOR_STATUS |= 0x04;
		else						mshm.SM105_MOTOR_STATUS &= 0xfb;
		if(SM105_SERVO_ALARM==ON)	mshm.SM105_MOTOR_STATUS |= 0x08;
		else						mshm.SM105_MOTOR_STATUS &= 0xf7;
		if(SM105_SERVO_CHECK==ON)	mshm.SM105_MOTOR_STATUS |= 0x10;
		else						mshm.SM105_MOTOR_STATUS &= 0xef;
		if(SM105_INPOS==ON)			mshm.SM105_MOTOR_STATUS |= 0x20;
		else						mshm.SM105_MOTOR_STATUS &= 0xdf;
		Sleep(20);
	}

	return FALSE;
}
