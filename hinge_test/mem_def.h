#ifndef EXTERN
#define EXTERN extern 
#endif

#define MOTORS_PARA_NUM		300  //	300 = 30 x 10  parameters, assign 30 parameters for each motor
#define INT_RAM_NUM			300	 // General purpose integer type shared memory assignment
#define DOUBLE_RAM_NUM		200	 // General purpose floating type shared memory assignment 
#define TIMER_RAM_NUM		120	 // General purpose floating type shared memory assignment 

typedef struct _shared_memory
{
	long	SM101_ENCODER;	// Focus X -- Card1 Z
	long	SM102_ENCODER;	// Focus Y -- Card1 T
	long	SM103_ENCODER;	// Focus Z -- Card1 Y
	long	SM104_ENCODER;	// Focus R -- Card1 X
	long	SM105_ENCODER;	// Focus U -- Card1 U

	int		SERVO_MOTOR_PARAMETERS[MOTORS_PARA_NUM];

	int		RAM_INT[INT_RAM_NUM];

	double  RAM_DOUBLE[INT_RAM_NUM];

	int		TIMER[TIMER_RAM_NUM];

	WORD   INPUT0_STATUS;		//I/O card #1 Port 0 (S00 ~ S15)
	WORD   INPUT1_STATUS;		//I/O card #1 Port 1 (S16 ~ S31)
	WORD   INPUT2_STATUS;		//I/O card #1 Port 2 (S32 ~ S47)
	WORD   INPUT3_STATUS;		//I/O card #1 Port 3 (S48 ~ S63)
	WORD   INPUT4_STATUS;		//I/O card #1 Port 3 (S64 ~ S80)

	WORD   OUTPUT0_STATUS;		//I/O card #1 Port 0 (Y00 ~ Y07)
	WORD   OUTPUT1_STATUS;		//I/O card #1 Port 1 (Y08 ~ Y15)
	WORD   OUTPUT2_STATUS;		//I/O card #1 Port 2 (Y16 ~ Y23)
	WORD   OUTPUT3_STATUS;		//I/O card #1 Port 3 (Y24 ~ Y31)

	unsigned long	CARD1_WR4_STATUS;
	unsigned long	CARD2_WR4_STATUS;
	unsigned long	CARD3_WR4_STATUS;

	unsigned char	SM101_MOTOR_STATUS;
	unsigned char	SM102_MOTOR_STATUS;
	unsigned char	SM103_MOTOR_STATUS;
	unsigned char	SM104_MOTOR_STATUS;
	unsigned char	SM105_MOTOR_STATUS;

	//-----------------------------------------
	unsigned long	Ps400_AddrBase[4];

	//-----------------------------------------
	unsigned long	P32C32U_AddrBase[4];

} MOTION_SHM;



