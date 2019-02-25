//*** AC servo motor number definition
#define Motor_SM101		1
#define Motor_SM102		2
#define Motor_SM103		3
#define Motor_SM104		4
#define Motor_SM105		5
#define Motor_SM202		6
#define Motor_SM203		7
#define Motor_SM204		8


//*** AC servo motor SM101 motion number definition ===================================
#define SM101_HOMING										1
#define SM101_MOVE_TO_HOME_POSITION							2
#define SM101_FOCUS_X_MOVE_TO_VISION_POSITION				3
#define SM101_FOCUS_X_OFFSET_MOVE_FROM_VISION_POSITION		4
#define SM101_FOCUS_X_MOVE_TO_LASER_POSITION				5
#define SM101_FOCUS_X_MOVE_TO_CONE_POSITION					6
#define SM101_FOCUS_X_INITIALIZATION						7
#define SM101_FOCUS_X_OFFSET_MOVE_FROM_CONE_POSITION		8
#define SM101_MOVE_TO_TARGET_DISTANCE						9

#ifdef DECLARE
int (*SM101_fun[])()={	NULL,												// fun no = 0;
						SM101_encoder_reset,								// 1;
						SM101_move_to_home_position,						// 2;
						SM101_move_to_load_position1,						// 3;
						SM101_move_to_load_position2,						// 4;
						SM101_move_to_load_position3,						// 5;
						SM101_move_to_load_position4,						// 6;
						SM101_move_to_load_position5,						// 7;
						SM101_move_to_load_position6,						// 8;

						NULL };										
#else 
extern int (*SM101_fun[])();
#endif

//*** AC servo motor SM102 motion number definition ===================================
#define SM102_HOMING										1
#define SM102_MOVE_TO_HOME_POSITION							2
#define SM102_FOCUS_Y_MOVE_TO_VISION_POSITION				3
#define SM102_FOCUS_Y_OFFSET_MOVE_FROM_VISION_POSITION		4
#define SM102_FOCUS_Y_MOVE_TO_LASER_POSITION				5
#define SM102_FOCUS_Y_MOVE_TO_CONE_POSITION					6
#define SM102_FOCUS_Y_INITIALIZATION						7
#define SM102_FOCUS_Y_OFFSET_MOVE_FROM_CONE_POSITION		8

#ifdef DECLARE
int (*SM102_fun[])()={	NULL,												// fun no = 0;v
						SM102_encoder_reset,								// 1;
						SM102_move_to_home_position,						// 2;
						SM102_move_to_load_position1,						// 3;
						SM102_move_to_load_position2,						// 4;
						SM102_move_to_load_position3,						// 5;
						SM102_move_to_load_position4,						// 6;
						SM102_move_to_load_position5,						// 7;
						SM102_move_to_load_position6,						// 8;

						NULL };										
#else 
extern int (*SM102_fun[])();
#endif

//*** AC servo motor SM103 motion number definition ===================================
#define SM103_HOMING											1
#define SM103_MOVE_TO_HOME_POSITION								2
#define SM103_FOCUS_Z_MOVE_TO_VISION_POSITION					3
#define SM103_FOCUS_Z_MOVE_TO_LASER_POSITION					4
#define SM103_FOCUS_Z_MOVE_TO_CONE_POSITION_WITH_LASER_OFFSET	5
#define SM103_FOCUS_Z_INITIALIZATION							6

#define SM103_MOVE_TO_PASS_POSITION								9
#define SM103_MOVE_TO_NG_POSITION								10

#ifdef DECLARE
int (*SM103_fun[])()={	NULL,													// fun no = 0;
						SM103_encoder_reset,								// 1;
						SM103_move_to_home_position,						// 2;
						SM103_move_to_load_position1,						// 3;
						SM103_move_to_load_position2,						// 4;
						SM103_move_to_load_position3,						// 5;
						SM103_move_to_load_position4,						// 6;
						SM103_move_to_load_position5,						// 7;
						SM103_move_to_load_position6,						// 8;
						SM103_move_to_pass_position,						// 9;
						SM103_move_to_ng_position,							//10;
						NULL };										
#else 
extern int (*SM103_fun[])();
#endif

//*** AC servo motor SM104 motion number definition ===================================
#define SM104_HOMING									1
#define SM104_MOVE_TO_HOME_POSITION						2
#define SM104_FOCUS_R_MOVE_TO_VISION_POSITION			3
#define SM104_FOCUS_R_OFFSET_MOVE_FROM_VISION_POSITION	4
#define SM104_FOCUS_R_INITIALIZATION					5
#define SM104_FOCUS_R_OFFSET_MOVE_FROM_CONE_POSITION	6
#define SM104_MOVE_TO_TARGET_DISTANCE					7
#define SM104_MOVE_TO_PASS_POSITION						9
#define SM104_MOVE_TO_NG_POSITION						10
#ifdef DECLARE
int (*SM104_fun[])()={	NULL,												// fun no = 0;
						SM104_encoder_reset,								// 1;
						SM104_move_to_home_position,						// 2;
						SM104_move_to_load_position1,						// 3;
						SM104_move_to_load_position2,						// 4;
						SM104_move_to_load_position3,						// 5;
						SM104_move_to_load_position4,						// 6;
						SM104_move_to_load_position5,						// 7;
						SM104_move_to_load_position6,						// 8;
						SM104_move_to_pass_position,						// 9;
						SM104_move_to_ng_position,							//10;

						NULL };										
#else 
extern int (*SM104_fun[])();
#endif

//*** AC servo motor SM105 motion number definition ===================================
#define SM105_HOMING											1
#define SM105_MOVE_TO_HOME_POSITION								2
#define SM105_DISPENSE_X_MOVE_TO_VISION_POSITION				3
#define SM105_DISPENSE_X_MOVE_TO_LASER_POSITION					4
#define SM105_DISPENSE_X_MOVE_TO_NEEDLE_POSITION				5
#define SM105_DISPENSE_X_MOVE_TO_CURE_CENTER_POSITION			6
#define SM105_DISPENSE_X_MOVE_TO_CURE_INDEX_POSITION_1			7
#define SM105_DISPENSE_X_MOVE_TO_CURE_INDEX_POSITION_2			8
#define SM105_DISPENSE_X_MOVE_TO_CURE_INDEX_POSITION_3			9
#define SM105_DISPENSE_X_MOVE_TO_CURE_INDEX_POSITION_4			10
#define SM105_DISPENSE_X_MOVE_TO_CURE_INDEX_POSITION_5			11
#define SM105_DISPENSE_X_MOVE_TO_TOUCH_POSITION					12
#define SM105_DISPENSE_X_MOVE_TO_GLASS_POSITION					13
#define SM105_DISPENSE_X_MOVE_TO_CLEAN_POSITION					14
#define SM105_DISPENSE_X_MOVE_TO_INDEX_POSITION_1				15
#define SM105_DISPENSE_X_MOVE_TO_INDEX_POSITION_2				16
#define SM105_DISPENSE_X_MOVE_TO_INDEX_POSITION_3				17
#define SM105_DISPENSE_X_MOVE_TO_INDEX_POSITION_4				18
#define SM105_DISPENSE_X_MOVE_TO_INDEX_POSITION_5				19
#define SM105_DISPENSE_X_MOVE_TO_VISION_ON_GLASS_POSITION		20
#define SM105_DISPENSE_X_MOVE_TO_LASER_ON_GLASS_POSITION		21
#define SM105_DISPENSE_X_OFFSET_MOVE_FROM_VISION_POSITION		22
#define SM105_DISPENSE_X_INITIALIZATION							23
#define SM105_DISPENSE_X_MOVE_VISION_CENTER						24
#define SM105_MOVE_TO_TARGET_DISTANCE							25

#ifdef DECLARE
int (*SM105_fun[])()={	NULL,													// fun no = 0;
						SM105_encoder_reset,									// 1;
						SM105_move_to_home_position,							// 2;
						SM105_move_to_load_position1,							// 3;
						SM105_initialization,									// 23;
						SM105_move_to_load_position2,							// 24;
						
						NULL };										
#else 
extern int (*SM105_fun[])();
#endif

#define  SM105_SITE_TASK_COMMON_WORK							 1
#define  SM105_SITE_TASK_UNNAME1								 2
#define  SM105_SITE_TASK_UNNAME2								 3
#define  SM105_SITE_TASK_UNNAME3								 4

#ifdef DECLARE
int (*SM105_fun_site[])()={	NULL,													// fun no = 0;
	//SM105_pos_cw,									// 1;
	//	SM105_move_to_home_position,							// 2;
	//	SM105_move_to_load_position1,							// 3;
	//	SM105_initialization,									// 4;
	//	SM105_move_to_load_position2,							// 5;
	NULL };										
#else 
extern int (*SM105_fun_site[])();
#endif




		


