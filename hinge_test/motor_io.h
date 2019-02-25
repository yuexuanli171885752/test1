//==== define PISO-MN200 board number ============
//==================================================================================  
#define ON			0x01
#define OFF			0x00
#define Normal		==0x00)?ON:OFF) 
#define Inverse		==0x00)?OFF:ON)

//======================================================================
// definition for motion control card-1
//======================================================================
#define LINE_NO_1			1
#define LINE_NO_0			0	


#define DEVICE_SM101_No						1
#define DEVICE_SM102_No						2
#define DEVICE_SM103_No						3
#define DEVICE_SM104_No						4
#define DEVICE_SM105_No						5

#define Card1_Z_Position	SM101_POSITION		
#define Card1_T_Position	SM102_POSITION
#define Card1_Y_Position	SM103_POSITION
#define Card1_X_Position	SM104_POSITION
#define Card1_U_Position	SM105_POSITION

#define DEVICE_IO_ID1						1
#define DEVICE_IO_ID2						2
#define DEVICE_IO_ID3						3
#define DEVICE_IO_ID4						4
#define DEVICE_IO_ID5						5
//========================================================================================
// Card-1, X-axis, SM101 -- 
#define SM101_LMT_N			(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0002)Inverse
#define SM101_LMT_P			(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0004)Inverse
#define SM101_SERVO_ALARM	(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0008)Inverse
#define SM101_HOME			(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0020)Inverse
#define SM101_INPOS			(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0040)Inverse
#define SM101_SERVO_READY	(((mn200_get_status(LINE_NO_1,DEVICE_SM101_No) & 0x0010)Inverse
#define SM101_SERVO_CHECK	((((mshm.CARD2_WR4_STATUS) & DEVICE_SM101_No)Inverse	// X-servo-on check

#define SM101_SERVO_ON		mn_servo_motor_servo_on(LINE_NO_1, DEVICE_SM101_No)
#define SM101_SERVO_OFF		mn_servo_motor_servo_off(LINE_NO_1, DEVICE_SM101_No)

//========================================================================================
// Card-1, Y-axis, SM102 --  
#define SM102_LMT_N			(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0002)Inverse
#define SM102_LMT_P			(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0004)Inverse
#define SM102_SERVO_ALARM	(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0008)Inverse
#define SM102_HOME			(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0020)Inverse
#define SM102_INPOS			(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0040)Inverse
#define SM102_SERVO_READY	(((mn200_get_status(LINE_NO_1,DEVICE_SM102_No) & 0x0010)Inverse
#define SM102_SERVO_CHECK	((((mshm.CARD2_WR4_STATUS) & DEVICE_SM102_No)Inverse	// Y-servo-on check

#define SM102_SERVO_ON		mn_servo_motor_servo_on(LINE_NO_1, DEVICE_SM102_No)
#define SM102_SERVO_OFF		mn_servo_motor_servo_off(LINE_NO_1, DEVICE_SM102_No)

//========================================================================================
// Card-1, Z-axis, SM103 --  
#define SM103_LMT_N			(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0002)Inverse
#define SM103_LMT_P			(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0004)Inverse
#define SM103_SERVO_ALARM	(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0008)Inverse
#define SM103_HOME			(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0020)Inverse
#define SM103_INPOS			(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0040)Inverse
#define SM103_SERVO_READY	(((mn200_get_status(LINE_NO_1,DEVICE_SM103_No) & 0x0010)Inverse
#define SM103_SERVO_CHECK	((((mshm.CARD2_WR4_STATUS) & DEVICE_SM103_No)Inverse	// Z-servo-on check

#define SM103_SERVO_ON		mn_servo_motor_servo_on(LINE_NO_1, DEVICE_SM103_No)
#define SM103_SERVO_OFF		mn_servo_motor_servo_off(LINE_NO_1, DEVICE_SM103_No)

//========================================================================================
// Card-1, U-axis, SM104 --  
#define SM104_LMT_N			(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0002)Inverse
#define SM104_LMT_P			(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0004)Inverse
#define SM104_SERVO_ALARM	(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0008)Inverse
#define SM104_HOME			(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0020)Inverse
#define SM104_INPOS			(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0040)Inverse
#define SM104_SERVO_READY	(((mn200_get_status(LINE_NO_1,DEVICE_SM104_No) & 0x0010)Inverse
#define SM104_SERVO_CHECK	((((mshm.CARD2_WR4_STATUS) & DEVICE_SM104_No)Inverse	// U-servo-on check

#define SM104_SERVO_ON	   mn_servo_motor_servo_on(LINE_NO_1, DEVICE_SM104_No)
#define SM104_SERVO_OFF	   mn_servo_motor_servo_off(LINE_NO_1, DEVICE_SM104_No)

//========================================================================================
// Card-2, X-axis, SM201 -- 
#define SM105_LMT_N			(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0002)Inverse
#define SM105_LMT_P			(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0004)Inverse
#define SM105_SERVO_ALARM	(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0008)Inverse
#define SM105_HOME			(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0020)Inverse
#define SM105_INPOS			(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0040)Inverse
#define SM105_SERVO_READY	(((mn200_get_status(LINE_NO_1,DEVICE_SM105_No) & 0x0210)Inverse
#define SM105_SERVO_CHECK	((((mshm.CARD2_WR4_STATUS) & DEVICE_SM105_No)Inverse	// X-servo-on check

#define SM105_SERVO_ON		mn_servo_motor_servo_on(LINE_NO_1, DEVICE_SM105_No)
#define SM105_SERVO_OFF		mn_servo_motor_servo_off(LINE_NO_1, DEVICE_SM105_No)

//=======================================================================================
// motor-mutex definition
//=======================================================================================
// Card-1, X,Y,Z,U axes use Motors1_hMutex 
#define SM101_mutex			Motors1_hMutex
#define SM102_mutex			Motors1_hMutex
#define SM103_mutex			Motors1_hMutex
#define SM104_mutex			Motors1_hMutex
#define SM105_mutex			Motors1_hMutex
// Card-2, X,Y,Z,U axes use Motors2_hMutex 

