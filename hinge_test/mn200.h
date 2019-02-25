#ifndef _MN200DLL_h_
#define _MN200DLL_h_
//================================================================================
// Communication Speed Configuration
#define COMMSPEED_2_5M	0
#define COMMSPEED_5M	1
#define COMMSPEED_10M	2
#define COMMSPEED_20M	3

// Output Pulse Configuration
#define PULSE_MODE_PULSE_LOGIC_LOW_DIR_FORWARD_HIGH		0
#define PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_HIGH	1
#define PULSE_MODE_PULSE_LOGIC_LOW_DIR_FORWARD_LOW		2
#define PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_LOW		3

#define PULSE_MODE_CW_LOGIC_LOW							4
#define PULSE_MODE_CW_LOGIC_HIGH						7

#define PULSE_MODE_A_LEAD_B								5
#define PULSE_MODE_A_LAG_B								6

// Encoder Configuration
#define ENCODER_MODE_AB				0  // Quadrature pulse input
#define ENCODER_MODE_AB_DIVID_2		1
#define ENCODER_MODE_AB_DIVID_4		2
#define ENCODER_MODE_CW_CCW         3  // Up/Down pulse input

// Motion related IO Configuration
typedef enum
{
	PULSE_MODE						=	0,
	EL_PROC							=	3,
	SD_ENA							=	31,
	SD_PROC							=	4,
	SDLTC_ENA						=	5,
	SD_LOGIC						=	6,
	ORG_LOGIC						=	7,
	ALM_PROC						=	8,
	ALM_LOGIC						=	9,
	ERC_ERR_ENA						=	10,
	ERC_ORG_ENA						=	11,
	ERC_LEN							=	12,
	ERC_LOGIC						=	15,
	MANUAL_SD_ENA					=	16,
	MANUAL_TRI_DRV_ELIMINATE_ENA	=	17,
	INP_ENA							=	18,
	INP_LOGIC						=	19,
	RESET_OUTPLS_AFTER_ORG_ENA		=	20,
	RESET_ENC_AFTER_ORG_ENA			=	21,
	RESET_CNT3_AFTER_ORG_ENA		=	22,
	ENC_MODE						=	23,
	ENC_REV_ENA						=	25,
	ENC_Z_LOGIC						=	26,
	OUTPLS_REV_ENA					=	27,
	CNT3_MODE						=	28,
	ALL_DATA						=	32,
}
MotionConfig;

#define	SLOWDOWN		0
#define SUDDEN_STOP		0
#define SLOWDOWN_STOP	1

#define DISABLE_FEATURE	0
#define ENABLE_FEATURE	1

#define LOGIC_ACTIVE_LOW	0
#define LOGIC_ACTIVE_HIGH	1

#define ERC_LEN_12us 			0
#define ERC_LEN_93us 			1
#define ERC_LEN_371us 			2
#define ERC_LEN_1500us 			3
#define ERC_LEN_12ms 			4
#define ERC_LEN_48ms 			5
#define ERC_LEN_95ms 			6
#define ERC_LEN_LEVEL_OUTPUT	7

// Filter Selection
#define FILTER_DATA_EMG_EL_SD_ORG_ALM_INP	0
#define FILTER_DATA_ENCODER					1

// Cmp/Ring Source Selection
#define PULSE_COMMAND		0
#define ENCODER_POSITION	1

// SERVO/ERC/ALM_RESET ON/OFF
#define TURN_OFF	0
#define TURN_ON		1

// Move Direction
#define MOVE_DIRECTION_REVERSE	0
#define MOVE_DIRECTION_FORWARD	1

#define MOVE_DIRECTION_CW		0
#define MOVE_DIRECTION_CCW		1

// Speed Parameter
typedef enum
{
	MAXSPEED_DO_NOT_CHANGE,
	MAXSPEED_10K,				// 0.1 ~ 10,000 PPS
	MAXSPEED_20K,            	// 0.2 ~ 20,000 PPS
	MAXSPEED_50K,            	// 0.5 ~ 50,000 PPS
	MAXSPEED_100K,           	//  1 ~ 100,000 PPS
	MAXSPEED_200K,           	//  2 ~ 200,000 PPS
	MAXSPEED_500K,           	//  5 ~ 200,000 PPS
	MAXSPEED_1M,		        // 10 ~ 1,000,000 PPS
	MAXSPEED_2M,             	// 20 ~ 2,000,000 PPS
	MAXSPEED_5M,             	// 50 ~ 5,000,000 PPS
	MAXSPEED_6M             	// 66 ~ 6,666,666 PPS
}MaxSpeed;

typedef struct _SPEED_PAR
{
	double		Start_Speed;			// Initial & Stop Speed (PPS)
	double		Drive_Speed;			// Operation Speed (PPS)
	double		Correction_Speed;		// Correction Speed (PPS)
	double		Acc;					// Acceleration data (s/ PPS/S)
	double		Dec;					// Deceleration data (s/ PPS/S)
	BYTE		AccDec_Mode;			// ADC_MODE_RATE / ADC_MODE_TIME
	BYTE		SCurve_Enable;			// DISABLE / Enable
	double		SCurveAcc_Sect;			// Acceleration S-curve range (PPS)
	double		SCurveDec_Sect;			// Deceleration S-curve range (PPS)
	MaxSpeed	Max_Speed;				// selecting MaxSpeed to set the speed magnification rate
}SPEED_PAR, *PSPEED_PAR;

#define ADC_MODE_RATE		0	// Specify the rate (number of pulses per second)
#define ADC_MODE_TIME		1	// Specify the time (seconds)

// Fix Move Mode
#define FIX_MOVE_MODE_REL					0x41
#define FIX_MOVE_MODE_ABS_BY_OUTPLS			0x42
#define FIX_MOVE_MODE_ABS_BY_ENC			0x43
#define FIX_MOVE_MODE_ZERO_RETURN_BY_OUTPLS	0x44
#define FIX_MOVE_MODE_ZERO_RETURN_BY_ENC	0x45

// Compare Trigger Direction
#define CMPTRIG_DIRECTION_BOTH				8
#define CMPTRIG_DIRECTION_FORWARD			9
#define CMPTRIG_DIRECTION_REVERSE			10

// Device Information
#define DEV_INF_NO_DEV				0x00
#define DEV_INF_MOTION_DEV			0x8b
#define DEV_INF_IO_32OUT_DEV		0x80
#define DEV_INF_IO_16IN_16OUT_DEV	0x82
#define DEV_INF_IO_32IN_DEV			0x84

// Status
#define MOTION_NOT_DONE 0
#define MOTION_DONE		1

#define CONTINUE_INTERPOLATION_NEXT_NOT_READY 	0
#define CONTINUE_INTERPOLATION_NEXT_READY 		1

#define ERR_STATUS_SW_PEL_STOP			0x00000001
#define ERR_STATUS_SW_MEL_STOP			0x00000002
#define ERR_STATUS_PEL_STOP				0x00000008
#define ERR_STATUS_MEL_STOP				0x00000010
#define ERR_STATUS_ALM_STOP				0x00000020
#define ERR_STATUS_EMG_STOP				0x00000080
#define ERR_STATUS_SD_STOP				0x00000100

typedef struct _MOTION_DEV_IO
{
	BYTE	SVON;
	BYTE	RESET_ALM;
	BYTE	RDY;
	BYTE	ALM;
	BYTE	PEL;
	BYTE	MEL;
	BYTE	ORG;
	BYTE	SDLTC;
	BYTE	SDIN;
	BYTE	INP;
	BYTE	EMG;
	BYTE	EZ;
	BYTE	ERC;
}MOTION_IO, *PMOTION_IO;

#ifdef __cplusplus
extern "C"
{
#endif
//============================================================================
//====================== MN200 DLL (V1.0) Release Functions===================
//============================================================================

// OS Configuration Routines
short __stdcall mn_open_all(BYTE* pNumLine);
short __stdcall mn200_get_lineinfo(BYTE bScannedIndex, BYTE* pLineNo);
short __stdcall mn200_get_cardinfo(BYTE bScannedIndex, BYTE* pCardID);
short __stdcall mn_close_all();
short __stdcall mn_reset(BYTE bLineNo);
short __stdcall mn_set_comm_speed(BYTE bLineNo, BYTE bCommSpeed);
short __stdcall mn_start_line(BYTE bLineNo, BYTE* pNumDev);
short __stdcall mn_stop_line(BYTE bLineNo);

// Hardware Configuration Routines (Optional)
short __stdcall mn_set_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD dwData);
short __stdcall mn_get_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD* pData);
short __stdcall mn_set_filter(BYTE bLineNo, BYTE bDevNo, BYTE bFilterSelection, BYTE bFilterEnable);
short __stdcall mn_set_softlimit(BYTE bLineNo, BYTE bDevNo, BYTE bSWLimitEnable, BYTE bCmpSource, BYTE bStopMode, long LimitPositive, long LimitNegaitive);
short __stdcall mn_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bServoOn);
short __stdcall mn_set_erc(BYTE bLineNo, BYTE bDevNo, BYTE bErcOn);
short __stdcall mn_alarm_reset(BYTE bLineNo, BYTE bDevNo, BYTE bAlmRstOn);
short __stdcall mn_load_ezgo_cfg(BYTE bLineNo, char* FileName);

// AUTOMATIC HOME SEARCH
short __stdcall mn_home_start(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, BYTE bHomeMode, BYTE bEZcount);

// INDEPENDENT MOVING FUNCTIONS
short __stdcall mn_velocity_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection);
short __stdcall mn_fix_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, long Position, BYTE bMoveType);

// INTERPOLATION MOVING FUNCTIONS
short __stdcall mn_set_group(BYTE bLineNo, BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]);
short __stdcall mn_get_group(BYTE bLineNo, BYTE bGrpNo, BYTE* pNumDev, BYTE bDevNo[]);
short __stdcall mn_line2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, BYTE bCnstSpdEnable);
short __stdcall mn_arc2_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, SPEED_PAR SpeedPar, BYTE bDirection, long Dev1CenterPos, long Dev2CenterPos, long Dev1FinishPos, long Dev2FinishPos, DWORD Low32BitDummyDevNo, DWORD High32BitDummyDevNo, BYTE bCnstSpdEnable);
short __stdcall mn_line3_move(BYTE bLineNo, BYTE bDev1No, BYTE bDev2No, BYTE bDev3No, SPEED_PAR SpeedPar, long Dev1Pos, long Dev2Pos, long Dev3Pos);
short __stdcall mn_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR SpeedPar, long DevPos[], BYTE bNumDev);

// CONTINUOUS INTERPOLATION MOVING
short __stdcall mn_conti_interp_next_ready(BYTE bLineNo, BYTE bDevNo, BYTE *pReady);

// OTHER MOTION FUNCTIONS
short __stdcall mn_stop_move(BYTE bLineNo, BYTE bDevNo, BYTE bStopMode);
short __stdcall mn_group_stop_move(BYTE bLineNo, BYTE bGrpNo, BYTE bStopMode);
short __stdcall mn_group_hold_move(BYTE bLineNo, BYTE bGrpNo);
short __stdcall mn_group_start_move(BYTE bLineNo, BYTE bGrpNo);

// ADVANCED MOTION CONFIGURATIONS
short __stdcall mn_set_vring(BYTE bLineNo, BYTE bDevNo, BYTE bVRingEnable, BYTE bVRingSource, DWORD dwVRingValue);
short __stdcall mn_cnst_cmptrig_config(BYTE bLineNo, BYTE bDevNo, BYTE bCmpTrigEnable, BYTE bCmpSource, BYTE bDirection, WORD wCnstPitch);

// MISCELLANEOUS FUNCTIONS
short __stdcall mn_change_v(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bWaitCmpEnable);
short __stdcall mn_change_p(BYTE bLineNo, BYTE bDevNo, long Position);
short __stdcall mn_set_cmdcounter(BYTE bLineNo, BYTE bDevNo, long Data);
short __stdcall mn_set_enccounter(BYTE bLineNo, BYTE bDevNo, long Data);

// STATUS
short __stdcall mn_get_dev_info(BYTE bLineNo, BYTE bDevNo, BYTE* pData);
short __stdcall mn_motion_done(BYTE bLineNo, BYTE bDevNo, BYTE* pDone);
short __stdcall mn_get_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData);
short __stdcall mn_get_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData);
short __stdcall mn_get_speed(BYTE bLineNo, BYTE bDevNo, double *pData);
short __stdcall mn_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO);
short __stdcall mn_get_error_status(BYTE bLineNo, BYTE bDevNo, DWORD* pData);
//2012.12.18
short __stdcall mn_get_comm_err_dev_list(BYTE bLineNo, DWORD *ErrorTable);

//2012.12.12
short __stdcall mn_get_line_status(BYTE bLineNo, WORD* pData);
short __stdcall mn_get_slave_error_table(BYTE bLineNo, DWORD *ErrorTable);
short __stdcall mn_clear_slave_error_flag(BYTE bLineNo, DWORD *ErrorTable);

// Parallel I/O
short __stdcall mn200_get_di(BYTE bCardID, BYTE* pData);
short __stdcall mn200_set_do(BYTE bCardID, BYTE bData);
short __stdcall mn200_get_do(BYTE bCardID, BYTE* pData);

// Serial I/O
short __stdcall	mn_get_di_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);
short __stdcall	mn_set_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE bData);
short __stdcall	mn_get_do_bit(BYTE bLineNo, BYTE bDevNo, BYTE bBitNo, BYTE* pData);

short __stdcall	mn_get_di_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);
short __stdcall	mn_set_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE bData);
short __stdcall	mn_get_do_byte(BYTE bLineNo, BYTE bDevNo, BYTE bByteNo, BYTE* pData);

short __stdcall	mn_get_di_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
short __stdcall	mn_set_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD bData);
short __stdcall	mn_get_do_word(BYTE bLineNo, BYTE bDevNo, BYTE bWordNo, WORD* pData);
//2012.12.12
short __stdcall	mn_get_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE* pData);
short __stdcall	mn_set_port_bit(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bBitNo, BYTE bData);
short __stdcall	mn_get_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE* pData);
short __stdcall	mn_set_port_byte(BYTE bLineNo, BYTE bDevNo, BYTE bPortNo, BYTE bData);

#ifdef __cplusplus
}
#endif

// Return error code
#define SUCCESS									0       	//No error
#define	ERROR_NO_CARD_FOUND						-100       	//Board not connected.The board is not identified.
#define	ERROR_IOCTL_FAILED						-101       	//DeviceIoControl fail!
#define	ERROR_INVALID_LINE_NO					-102       	//Line number assignment error
#define ERROR_COMM_NOT_START					-103    	//I/O communication failed to start error
#define	ERROR_INVALID_DEV_NO					-104       	//Device number assignment error
#define	ERROR_NO_DEV_FOUND						-105       	//Device not connected.The device is not identified.
#define ERROR_SET_IO_DEV						-106
#define ERROR_SET_MOTION_DEV					-107
#define ERROR_START_SPEED_EXCEED_DRIVING_SPEED	-108
#define ERROR_INVALID_MAX_SPEED_SELECTION		-109
#define ERROR_SET_START_SPEED_OUT_RANGE         -110
#define ERROR_SET_DRIVING_SPEED_OUT_RANGE       -111
#define ERROR_INVALID_SCURVE_ENABLE             -112
#define ERROR_INVALID_ADC_MODE					-113
#define ERROR_INVALID_ACC_DATA					-114
#define ERROR_SET_ACC_DOUBLE_DEC				-115
#define ERROR_SET_ACC_OUT_RANGE					-116
#define ERROR_SET_DEC_OUT_RANGE					-117
#define ERROR_SET_ACC_SECT_OUT_RANGE			-118
#define ERROR_SET_DEC_SECT_OUT_RANGE			-119
#define ERROR_SET_CORRECTION_SPD_OUT_RANGE		-120
#define ERROR_SET_SCAN_INDEX_OUT_RANGE			-121
#define ERROR_INVALID_COMM_SPEED				-122
#define ERROR_COMM_NOT_STOP						-123
#define ERROR_SET_DATA							-124
#define ERROR_INVALID_CONFIG_ITEM				-125
#define ERROR_INVALID_FILTER_ITEM				-126
#define ERROR_INVALID_SOFTWARE_LIMIT_SOURCE		-127
#define ERROR_INVALID_STOP_MODE					-128
#define ERROR_CONFLICT_WITH_VRING				-129
#define ERROR_INVALID_MOVE_DIRECTION			-130
#define ERROR_INVALID_HOME_MODE					-131
#define ERROR_INVALID_EZ_COUNT					-132
#define ERROR_MOVE_HOLD							-133
#define ERROR_EMG_SIGNAL_ON						-134
#define ERROR_ALM_SIGNAL_ON						-135
#define ERROR_MEL_SIGNAL_ON						-136
#define ERROR_PEL_SIGNAL_ON						-137
#define ERROR_WAIT_INP							-138
#define ERROR_WAIT_ERC							-139
#define ERROR_WAIT_BACKLASH_CORRECT				-140
#define ERROR_WAIT_PULSE_IN						-141
#define ERROR_MOTION_NOT_COMPLETE				-142
#define ERROR_INVALID_FIX_MOVE_MODE				-143
#define ERROR_REGISTER_FULL						-144
#define ERROR_INVALID_POSITION					-145
#define ERROR_INVALID_GROUPNO					-146
#define ERROR_INVALID_NUM_DEV					-147
#define ERROR_GROUP_ALREADY_HOLD				-148
#define ERROR_SET_ARC_FINISH_POS				-149
#define ERROR_SET_BIT_DUMMY_DEV					-150
#define ERROR_INVALID_DEV_HOLD					-151
#define ERROR_GROUP_NOT_HOLD					-152
#define ERROR_INVALID_VRING_SOURCE				-153
#define ERROR_INVALID_VRING_VALUE				-154
#define ERROR_INVALID_CMPTRIG_SOURCE			-155
#define ERROR_INVALID_CMPTRIG_DIRECTION			-156
#define ERROR_INVALID_CMPTRIG_PITCH				-157
#define ERROR_MOTION_IS_COMPLETED 				-158
#define	ERROR_INVALID_CARD_ID					-159
#define	ERROR_INVALID_OUTPUT_DATA				-160
#define	ERROR_INVALID_BITNO						-161
#define	ERROR_SET_BITNO							-162
#define ERROR_INVALID_PORTNO					-163
#define ERROR_SET_PORTNO						-164
#define ERROR_CARD_ID_DUPLICATED				-165
#define ERROR_CONFIG_FILE_LOAD					-166
#define ERROR_CONFIG_FILE_MATCH					-167
#define ERROR_COMM_DISCONNECT					-168

#endif
