//=========================================================================================  
//	Project:		AI-200T-NEW
//	Program:		define.h
//	date:			2015-2-19
//===========================================================================================
#ifndef EXTERN
#define EXTERN extern
#endif

/* define machine status flags */
#ifndef NULL
#define NULL     0
#endif

// General Define
#define ENABLE					1
#define DISABLE					0
#define	UNCERTAIN				0

#define PI						(acos(-1.0)) //3.1415926535    

#define EXIT_NUMBER				999
#define ENDING_FINISH			888
#define EXIT_CONFIRM			9999
#define OPEN_FINISH				22733
#define ESC_NUMBER				1999

#define UV_FIXED_MODE			0
#define UV_POINTS_MODE			1
#define UV_ROTATE_MODE			2


#define TF_BACK_STAY			0
#define TF_BACK_CENTER_PEAK		1
#define TF_BACK_START_POS		2
#define TF_BACK_FIRST_PEAK		3

/*****************************/
/* M/C STATE definition		 */
/*****************************/
#define INITIAL_STATE				0x0001
#define READY_STATE					0x0002
#define ALARM_STATE					0x0004
#define STOP_STATE					0x0008
#define RUN_STATE					0x0010
#define MANUAL_STATE				0x0020
#define WARNING_STATE				0x0040
#define FINISH_STATE				0x0080

/*****************************/
/* TASK STATE definition	 */
/*****************************/
#define READY_TO_ROTATE			1
#define READY_TO_PROCESS		2
#define UNDER_PROCESSING		3
#define PROCESSED				4	

/*********************************/
/* SITE Test Result Definition	 */
/*********************************/
#define DEVICE_NOT_TESTED		31
#define DEVICE_TEST_OK			32
#define DEVICE_TEST_NG			33

/*********************************/
/* Result Display Trigger Definition	 */
/*********************************/
#define DISPLAY_NOT_CHANGE		0
#define DISPLAY_CLEAR			1
#define DISPLAY_UPDATE_NEW		2
//=== Auto task command list ===================
//=== Focus adjust unit command list ===========
#define AUTO_FOCUS_UNIT_READY_FOR_COMMAND			2

#define AUTO_FOCUS_X_MOVE_HOME						10
#define AUTO_FOCUS_X_MOVE_VISION					11
#define AUTO_FOCUS_X_OFFSET_MOVE_FROM_VISION		12
#define AUTO_FOCUS_X_MOVE_LASER						13
#define AUTO_FOCUS_X_MOVE_CONE						14

#define AUTO_FOCUS_Y_MOVE_HOME						20
#define AUTO_FOCUS_Y_MOVE_VISION					21
#define AUTO_FOCUS_Y_OFFSET_MOVE_FROM_VISION		22
#define AUTO_FOCUS_Y_MOVE_LASER						23
#define AUTO_FOCUS_Y_MOVE_CONE						24

#define AUTO_FOCUS_Z_MOVE_HOME						30
#define AUTO_FOCUS_Z_MOVE_VISION					31
#define AUTO_FOCUS_Z_MOVE_LASER						32
#define AUTO_FOCUS_Z_MOVE_CONE_Z_LASER_OFFSET		33

#define AUTO_FOCUS_R_MOVE_HOME						40

#define AUTO_FOCUS_XY_MOVE_HOME						50
#define AUTO_FOCUS_XY_MOVE_VISION					51
#define AUTO_FOCUS_XY_OFFSET_MOVE_FROM_VISION		52
#define AUTO_FOCUS_XY_MOVE_LASER					53
#define AUTO_FOCUS_XY_MOVE_CONE						54

#define AUTO_FOCUS_XYR_MOVE_HOME					60
#define AUTO_FOCUS_XYR_MOVE_VISION					61
#define AUTO_FOCUS_XY_MOVE_CONE_R_OFFSET			62

#define AUTO_FOCUS_XYR_OFFSET_MOVE_FROM_CONE		63

#define AUTO_FOCUS_R_ROTATE							64

#define AUTO_FOCUS_VISION_LIGHTSOURCE_ON			70
#define AUTO_FOCUS_VISION_LIGHTSOURCE_OFF			71 

#define AUTO_FOCUS_CL_MOVE_LEFT						80
#define AUTO_FOCUS_CL_MOVE_RIGHT					81

#define AUTO_FOCUS_UNIT_DONE						92

//////////////////////////////////////////////////////////////////////////
//define for auto focus process
#define AUTO_FOCUS_PROGRAM_READY					2
#define AUTO_FOCUS_PROGRAM_PROCESS					3
#define AUTO_FOCUS_PROGRAM_DONE						4 

//=== AF Test unit command list =============
#define AUTO_AF_TSET_UNIT_READY_FOR_COMMAND			3

#define AUTO_AF_TEST_UNIT_LIGHT_MOVE_LEFT			10
#define AUTO_AF_TEST_UNIT_LIGHT_MOVE_RIGHT			11

#define AUTO_AF_TEST_UNIT_DONE						93

//=== Dispenser unit command list ==============
#define AUTO_DISPENSER_UNIT_READY_FOR_COMMAND		4

#define AUTO_DISPENSER_UNIT_X_MOVE_HOME				10
#define AUTO_DISPENSER_UNIT_X_MOVE_VISION			11
#define AUTO_DISPENSER_UNIT_X_MOVE_LASER			12

#define AUTO_DISPENSER_UNIT_Y_MOVE_HOME				20
#define AUTO_DISPENSER_UNIT_Y_MOVE_VISION			21
#define AUTO_DISPENSER_UNIT_Y_MOVE_LASER			22

#define AUTO_DISPENSER_UNIT_Z_MOVE_HOME				30
#define AUTO_DISPENSER_UNIT_Z_MOVE_VISION			31
#define AUTO_DISPENSER_UNIT_Z_MOVE_LASER			32

#define AUTO_DISPENSER_UNIT_R_MOVE_HOME				40

#define AUTO_DISPENSER_UNIT_XY_MOVE_HOME			50
#define AUTO_DISPENSER_UNIT_XY_MOVE_VISION			51
#define AUTO_DISPENSER_UNIT_XY_MOVE_LASER			52

#define AUTO_IMAGE_UNIT_MOVE_LEFT					60
#define AUTO_IMAGE_UNIT_MOVE_RIGHT					61
#define AUTO_IMAGE_UNIT_MOVE_DOWN					62
#define AUTO_IMAGE_UNIT_MOVE_UP						63

#define AUTO_DISPENSER_UNIT_DONE					94

//====================================================
//=== Manual task command list =======================
//=== Focus adjust unit command list =================
#define MANUAL_OPERATION_READY_FOR_COMMAND			200

#define MANUAL_FOCUS_X_MOVE_HOME					210
#define MANUAL_FOCUS_X_MOVE_VISION					211
#define MANUAL_FOCUS_X_OFFSET_MOVE_FROM_VISION		212
#define MANUAL_FOCUS_X_MOVE_LASER					213
#define MANUAL_FOCUS_X_MOVE_CONE					214

#define MANUAL_FOCUS_Y_MOVE_HOME					220
#define MANUAL_FOCUS_Y_MOVE_VISION					221
#define MANUAL_FOCUS_Y_OFFSET_MOVE_FROM_VISION		222
#define MANUAL_FOCUS_Y_MOVE_LASER					223
#define MANUAL_FOCUS_Y_MOVE_CONE					224

#define MANUAL_FOCUS_Z_MOVE_HOME					230
#define MANUAL_FOCUS_Z_MOVE_VISION					231
#define MANUAL_FOCUS_Z_MOVE_LASER					232
#define MANUAL_FOCUS_Z_MOVE_CONE_Z_LASER_OFFSET		233

#define MANUAL_FOCUS_R_MOVE_HOME					240

#define MANUAL_FOCUS_XY_MOVE_HOME					250
#define MANUAL_FOCUS_XY_MOVE_VISION					251
#define MANUAL_FOCUS_XY_OFFSET_MOVE_FROM_VISION		252
#define MANUAL_FOCUS_XY_MOVE_LASER					253
#define MANUAL_FOCUS_XY_MOVE_CONE					254

#define MANUAL_FOCUS_XYR_MOVE_HOME					260
#define MANUAL_FOCUS_XYR_MOVE_VISION				261
#define MANUAL_FOCUS_XY_MOVE_CONE_R_OFFSET			262

#define MANUAL_FOCUS_VISION_LIGHTSOURCE_ON			263
#define MANUAL_FOCUS_VISION_LIGHTSOURCE_OFF			264

//=== AF Test unit command list =============
#define MANUAL_AF_TEST_UNIT_LIGHT_MOVE_LEFT			270
#define MANUAL_AF_TEST_UNIT_LIGHT_MOVE_RIGHT		280

//=== Dispenser unit command list ==============
#define MANUAL_DISPENSER_UNIT_X_MOVE_HOME							290
#define MANUAL_DISPENSER_UNIT_X_MOVE_VISION							291
#define MANUAL_DISPENSER_UNIT_X_MOVE_LASER							292
#define MANUAL_DISPENSER_UNIT_X_MOVE_GLASS							293
#define MANUAL_DISPENSER_UNIT_X_MOVE_VISION_ON_GLASS				294
#define MANUAL_DISPENSER_UNIT_X_MOVE_LASER_ON_GLASS					295
#define MANUAL_DISPENSER_UNIT_X_MOVE_VISION_CENTER					296

#define MANUAL_DISPENSER_UNIT_Y_MOVE_HOME							300
#define MANUAL_DISPENSER_UNIT_Y_MOVE_VISION							301
#define MANUAL_DISPENSER_UNIT_Y_MOVE_LASER							302
#define MANUAL_DISPENSER_UNIT_Y_MOVE_GLASS							303
#define MANUAL_DISPENSER_UNIT_Y_MOVE_VISION_ON_GLASS				304
#define MANUAL_DISPENSER_UNIT_Y_MOVE_LASER_ON_GLASS					305
#define MANUAL_DISPENSER_UNIT_Y_MOVE_VISION_CENTER					306

#define MANUAL_DISPENSER_UNIT_Z_MOVE_HOME							310
#define MANUAL_DISPENSER_UNIT_Z_MOVE_VISION							311
#define MANUAL_DISPENSER_UNIT_Z_MOVE_LASER							312
#define MANUAL_DISPENSER_UNIT_Z_MOVE_GLASS							313
#define MANUAL_DISPENSER_UNIT_Z_MOVE_VISION_ON_GLASS				314
#define MANUAL_DISPENSER_UNIT_Z_MOVE_LASER_ON_GLASS					315

#define MANUAL_DISPENSER_UNIT_R_MOVE_HOME							320

#define MANUAL_DISPENSER_UNIT_XY_MOVE_HOME							331
#define MANUAL_DISPENSER_UNIT_XY_MOVE_VISION						332
#define MANUAL_DISPENSER_UNIT_XY_MOVE_LASER							333
#define MANUAL_DISPENSER_UNIT_XY_MOVE_GLASS							334
#define MANUAL_DISPENSER_UNIT_XY_MOVE_VISION_ON_GLASS				335
#define MANUAL_DISPENSER_UNIT_XY_MOVE_LASER_ON_GLASS				336
#define MANUAL_DISPENSER_UNIT_XY_MOVE_VISION_CENTER_ON_GLASS		337
#define MANUAL_DISPENSER_UNIT_XY_MOVE_VISION_CENTER					338

#define MANUAL_DISPENSER_UNIT_DISPENSE_ONE_CYCLE					340 //  with dispensing time at OnePointDispensingTime, shm->RAM_INT[172]	// unit = 1ms
#define MANUAL_DISPENSER_UNIT_DISPENSE_VALVE_ON						341 
#define MANUAL_DISPENSER_UNIT_DISPENSE_VALVE_OFF					342 

#define MANUAL_DISPENSER_UNIT_VISION_LIGHTSOURCE_ON					343
#define MANUAL_DISPENSER_UNIT_VISION_LIGHTSOURCE_OFF				344

