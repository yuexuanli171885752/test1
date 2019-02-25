#define _CRT_SECURE_NO_WARNINGS
#define EXTERN
#define DECLARE
#include "interface.h"
#include "inf_motion_aa.h"

#define M_CHART_Z						0
#define M_LINE_T						1
#define M_LINE_Y						2
#define M_LINE_X						3
#define M_TABLE_U						4


#define TIMER_MOTOR_DLG_JOG				1
#define MOTOR_NUM						5

HANDLE	Motors1_hMutex;
HANDLE	Motors2_hMutex;
HANDLE	Motors3_hMutex;
HANDLE	Err_hMutex;

SlantedBW_SM FS;
extern SlantedBW mysbw;

extern DWORD WINAPI home_motors_thread(LPVOID temp);
extern DWORD WINAPI auto_motion_thread(LPVOID temp);

char err_msg[200][MAX_PATH] ={0};

#define		thread_no		21

LPCTSTR		motors1_mutex_name	= "MUTEX_MOTORS1";
LPCTSTR		motors2_mutex_name	= "MUTEX_MOTORS2";
LPCTSTR		motors3_mutex_name	= "MUTEX_MOTORS3";
LPCTSTR		err_mutex_name		= "MUTEX_ERR";

HANDLE		hThread[thread_no];
DWORD		dwThreadID[thread_no];

int motor_init_err = 1;
int select_motor = M_LINE_X;
int motor_flag = 0;

void load_err_msg(void)//motor return errcode
{
	strcpy(err_msg[100], "SM101_NOT_READY_ERR			");	
	strcpy(err_msg[101], "SM101_NOT_IN_POSITION_ERR		");	
	strcpy(err_msg[102], "SM101_ALARM_ERR				");	
	strcpy(err_msg[103], "SM101_P_LIMIT_ERR				");	
	strcpy(err_msg[104], "SM101_N_LIMIT_ERR				");	
	strcpy(err_msg[105], "SM101_HOME_SENSOR_STILL_ON_ERR");	
	strcpy(err_msg[106], "SM101_HOMING_FAILURE_ERR		");	
	strcpy(err_msg[107], "SM101_NOT_HOMED_YET_WARNING	");	
	strcpy(err_msg[108], "SM101_TRIGGER_NOT_READY_ERR	");	
	strcpy(err_msg[109], "SM101_MOTION_COMMAND_ERR		");	

	strcpy(err_msg[110], "SM102_NOT_READY_ERR			");	
	strcpy(err_msg[111], "SM102_NOT_IN_POSITION_ERR		");	
	strcpy(err_msg[112], "SM102_ALARM_ERR				");	
	strcpy(err_msg[113], "SM102_P_LIMIT_ERR				");	
	strcpy(err_msg[114], "SM102_N_LIMIT_ERR				");	
	strcpy(err_msg[115], "SM102_HOME_SENSOR_STILL_ON_ERR");	
	strcpy(err_msg[116], "SM102_HOMING_FAILURE_ERR		");	
	strcpy(err_msg[117], "SM102_NOT_HOMED_YET_WARNING	");	
	strcpy(err_msg[118], "SM102_TRIGGER_NOT_READY_ERR	");	
	strcpy(err_msg[119], "SM102_MOTION_COMMAND_ERR		");	

	strcpy(err_msg[120], "SM103_NOT_READY_ERR			");
	strcpy(err_msg[121], "SM103_NOT_IN_POSITION_ERR		");
	strcpy(err_msg[122], "SM103_ALARM_ERR				");
	strcpy(err_msg[123], "SM103_P_LIMIT_ERR				");
	strcpy(err_msg[124], "SM103_N_LIMIT_ERR				");
	strcpy(err_msg[125], "SM103_HOME_SENSOR_STILL_ON_ERR");
	strcpy(err_msg[126], "SM103_HOMING_FAILURE_ERR		");
	strcpy(err_msg[127], "SM103_NOT_HOMED_YET_WARNING	");
	strcpy(err_msg[128], "SM103_TRIGGER_NOT_READY_ERR	");
	strcpy(err_msg[129], "SM103_MOTION_COMMAND_ERR		");

	strcpy(err_msg[130], "SM104_NOT_READY_ERR			");
	strcpy(err_msg[131], "SM104_NOT_IN_POSITION_ERR		");
	strcpy(err_msg[132], "SM104_ALARM_ERR				");
	strcpy(err_msg[133], "SM104_P_LIMIT_ERR				");
	strcpy(err_msg[134], "SM104_N_LIMIT_ERR				");
	strcpy(err_msg[135], "SM104_HOME_SENSOR_STILL_ON_ERR");
	strcpy(err_msg[136], "SM104_HOMING_FAILURE_ERR		");
	strcpy(err_msg[137], "SM104_NOT_HOMED_YET_WARNING	");
	strcpy(err_msg[138], "SM104_TRIGGER_NOT_READY_ERR	");
	strcpy(err_msg[139], "SM104_MOTION_COMMAND_ERR		");

	strcpy(err_msg[140], "SM105_NOT_READY_ERR			");
	strcpy(err_msg[141], "SM105_NOT_IN_POSITION_ERR		");
	strcpy(err_msg[142], "SM105_ALARM_ERR				");
	strcpy(err_msg[143], "SM105_P_LIMIT_ERR				");
	strcpy(err_msg[144], "SM105_N_LIMIT_ERR				");
	strcpy(err_msg[145], "SM105_HOME_SENSOR_STILL_ON_ERR");
	strcpy(err_msg[146], "SM105_HOMING_FAILURE_ERR		");
	strcpy(err_msg[147], "SM105_NOT_HOMED_YET_WARNING	");
	strcpy(err_msg[148], "SM105_TRIGGER_NOT_READY_ERR	");
	strcpy(err_msg[149], "SM105_MOTION_COMMAND_ERR		");

	strcpy(err_msg[180], "AA_MOVE_Z_ERR"				 );
}

//////////////////////////////////////////////////////////////////////////
//interface function
int motor_init(void)
{
	BYTE	NumLine,NumDevice_motor,NumDevice_IO;

	BUTTON_VALUE = 0X00;
	ai_win::get_current_directory(s_test::dir);	
	sprintf(s_test::ps_config_path,"%s\\recipe\\MN_Config.ini",s_test::dir);

	log("====Motor Init====");
	Err_hMutex		= CreateMutex(NULL, FALSE, err_mutex_name		); log("create Err_hMutex[%d]", GetLastError());
	Motors1_hMutex	= CreateMutex(NULL, FALSE, motors1_mutex_name	); log("create motors1_hMutex[%d]", GetLastError());
	Motors2_hMutex	= CreateMutex(NULL, FALSE, motors2_mutex_name	); log("create motors2_hMutex[%d]", GetLastError());
	Motors3_hMutex	= CreateMutex(NULL, FALSE, motors3_mutex_name	); log("create motors3_hMutex[%d]", GetLastError());     
	hThread[0]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Tscreen,				NULL,0,&dwThreadID[0]);	

	QueryPerformanceFrequency(&Frequency);
    int errcode;	// get hardware performance counts per second
	if((errcode = mn_open_all(&NumLine) ) != SUCCESS){
		log("MN200 Card open failure, and error code=%d\n", errcode);
		return errcode;
	}

	//==== reset card ============================================================================ 
	if((errcode = mn_reset(LINE_NO_0)) != SUCCESS ) return(errcode);
	if((errcode = mn_reset(LINE_NO_1)) != SUCCESS ) return(errcode);

	//==== set communication speed ===============================================================
	if((errcode = mn_set_comm_speed(LINE_NO_0, COMMSPEED_20M)) != SUCCESS ) return(errcode);
	if((errcode = mn_set_comm_speed(LINE_NO_1, COMMSPEED_20M)) != SUCCESS ) return(errcode);

	//==== Motionnet communication start =========================================================
	if((errcode = mn_start_line(LINE_NO_0, &NumDevice_IO)) != SUCCESS ) return(errcode);
	if((errcode = mn_start_line(LINE_NO_1, &NumDevice_motor)) != SUCCESS ) return(errcode);

/*
	if((errcode = Motion_Motor_Init()) != SUCCESS_NO_ERROR){
		log("Device PISO-PS400 initialization error ! ERROR_CODE2=%d\n",errcode);
		ps400_close_all();
		return (errcode);
	}*/

	if((errcode = mn_load_ezgo_cfg ( LINE_NO_1,s_test::ps_config_path)) != SUCCESS){
		log("Device PISO-MN200 initialization error ! ERROR_CODE2=%d\n",errcode);
		mn_close_all();
		return (errcode);
	}
	log(">Motor Card Init OK.\n");
	SM101_HOMED_FLAG=SM102_HOMED_FLAG=SM103_HOMED_FLAG=SM104_HOMED_FLAG=SM105_HOMED_FLAG=0;
	log("home flag=%d",HOME_FLAG);

	SM101_SERVO_ON;
	SM102_SERVO_ON;
	SM103_SERVO_ON;
	SM104_SERVO_ON;
	SM105_SERVO_ON;

	//---------------------------------------------------------------------------------------------
	hThread[1]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Iokernel,				NULL,0,&dwThreadID[1]);	
	hThread[2]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Readcntr,				NULL,0,&dwThreadID[2]);
/*	hThread[4]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Trans_carrier,		NULL,0,&dwThreadID[4]);*/
/*	hThread[4]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)product_bin,			NULL,0,&dwThreadID[4]);*/
	hThread[3]  = CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)power_supply_link_thread,   NULL, 0, &dwThreadID[3]);
	
//	hThread[3]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Monitor,				NULL,0,&dwThreadID[3]);
//	hThread[4]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Lamp,					NULL,0,&dwThreadID[4]);
//	hThread[5]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ErrProcs,				NULL,0,&dwThreadID[5]);
//	hThread[6]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FocusCheckTask,		NULL,0,&dwThreadID[6]);
//	hThread[7]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AFTestTask,			NULL,0,&dwThreadID[7]);
//	hThread[8]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ImageTestTask,		NULL,0,&dwThreadID[8]);
//	hThread[9]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CentralRotateTask,	NULL,0,&dwThreadID[9]);
//	hThread[10]	= CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ManualCommandServer,	NULL,0,&dwThreadID[10]);	
	hThread[11] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TriggerSM101,			NULL,0,&dwThreadID[11]);	
	hThread[12] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TriggerSM102,			NULL,0,&dwThreadID[12]);	
	hThread[13] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TriggerSM103,			NULL,0,&dwThreadID[13]);	
	hThread[14] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TriggerSM104,			NULL,0,&dwThreadID[14]);	
	hThread[15] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TriggerSM105,			NULL,0,&dwThreadID[15]);	
//	hThread[16] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)power_supply_link_thread,  NULL, 0, &dwThreadID[16]);
	hThread[19] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)home_motors_thread,	NULL,0,&dwThreadID[19]);

	return 0;
}
class MotorDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
	virtual void close(HWND hdlg);
	virtual void timer(HWND hdlg, unsigned int id);
	virtual LRESULT ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark/* =CTLCOLOR_DLG */);

	int read_shm(char *path, MOTION_SHM *shm);
	int save_shm(char *path, MOTION_SHM *shm);


private:
	char config_path[MAX_PATH];
	char status[MAX_PATH];
	HBRUSH dlg_brush, old_dlg_brush;

};

MotorDlg motor_dlg;

BOOL CALLBACK motor_cfg_dlg(HWND hdlg, unsigned int message, 
	WPARAM wParam, LPARAM lParam)
{
	switch (message){
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,  motor_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		motor_dlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		motor_dlg.close);
		HANDLE_DLGMSG(hdlg, WM_TIMER,		motor_dlg.timer);
	}


	return FALSE;
}

BOOL MotorDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	log("***********************motor init.**********************");
	FS = s_test::mysbw_sm;
	char motor_name[MOTOR_NUM][30] = {"Chart Z", "Line T", "Line Y", "Line X", "Table U"};
	int motor_id[MOTOR_NUM] = {IDC_MOTOR, IDC_MOTOR2, IDC_MOTOR3, IDC_MOTOR4, IDC_MOTOR5};
	sprintf(config_path, "%s\\recipe\\%s\\sharemem.mem", s_test::dir, s_test::recipe_name);

	for (int i=0; i<MOTOR_NUM; i++){
		SetDlgItemText(hdlg, motor_id[i], motor_name[i]);
	}
	SET_CHECKER(hdlg, IDC_MOTOR, TRUE);
	SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Chart Z");
	select_motor = M_LINE_X;

	COMBO_ADDSTR(hdlg, IDC_SELECT_ROI, "Bottom"	);
	COMBO_ADDSTR(hdlg, IDC_SELECT_ROI, "Right"	);
	COMBO_ADDSTR(hdlg, IDC_SELECT_ROI, "Top"	);
	COMBO_ADDSTR(hdlg, IDC_SELECT_ROI, "Left"	);

	int err = read_shm(config_path, &mshm);

	if (err != 0x00){
		log("open shm file error[%d]", err);
	}
	else{
//		SET_CHECKER(hdlg, IDC_MOVE_HOME_OFFSET, RUN_HOME_OFFSET);

		ai_win::set_dlg_item_int(hdlg, IDC_PLC_DB,		  PLC_DB_INDEX			);
		ai_win::set_dlg_item_int(hdlg, IDC_DB_START,	  PLC_DB_START			);
		ai_win::set_dlg_item_int(hdlg, IDC_DB_LENS,		  PLC_DB_LENS			);
		ai_win::set_dlg_item_int(hdlg, IDC_PLC_DB2,		  PLC_DB_INDEX_R	    );
		ai_win::set_dlg_item_int(hdlg, IDC_DB_START2,	  PLC_DB_START_R		);
		ai_win::set_dlg_item_int(hdlg, IDC_DB_LENS2,	  PLC_DB_LENS_R			);

		ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	  LINE_X_HOME_SPEED		);
		ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	  LINE_X_JOG_SPEED		);
		ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	  LINE_X_MOVE_SPEED		);
		ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		  LINE_X_HOME_ACCEL_TIME);
		ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		  LINE_X_JOG_ACCEL_TIME	);
		ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		  LINE_X_MOVE_ACCEL_TIME);

		ai_win::set_dlg_item_int(hdlg, IDC_PASS_NUMX,	  LINE_X_PASS_NUM		);
		ai_win::set_dlg_item_int(hdlg, IDC_PASS_NUMY,	  LINE_Y_PASS_NUM		);
		ai_win::set_dlg_item_int(hdlg, IDC_NG_NUMX,		  LINE_Y_NG_NUM			);
		ai_win::set_dlg_item_int(hdlg, IDC_NG_NUMY,		  LINE_X_NG_NUM			);
		ai_win::set_dlg_item_float(hdlg, IDC_PASS_OFFSETX,	  (double)(LINE_X_PASS_OFFSET*1.0/LINE_X_RATIO),	 4);
		ai_win::set_dlg_item_float(hdlg, IDC_PASS_OFFSETY,	  (double)(LINE_Y_PASS_OFFSET*1.0/LINE_Y_RATIO),	 4);
		ai_win::set_dlg_item_float(hdlg, IDC_NG_OFFSETX,	  (double)(LINE_X_NG_OFFSET	*1.0/LINE_X_RATIO),		 4);
		ai_win::set_dlg_item_float(hdlg, IDC_NG_OFFSETY,	  (double)(LINE_Y_NG_OFFSET	*1.0/LINE_Y_RATIO),		 4);
		ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSETX,	  (double)(LINE_X_HOME_OFFSET	*1.0/LINE_X_RATIO),	 4);
		ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSETY,	  (double)(LINE_Y_HOME_OFFSET	*1.0/LINE_Y_RATIO),	 4);
		ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK1,	  (double)(LINE_T_STACK1*1.0/LINE_T_RATIO), 4);
		ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK2,	  (double)(LINE_T_STACK2*1.0/LINE_T_RATIO), 4);
		ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK3,	  (double)(LINE_T_STACK3*1.0/LINE_T_RATIO), 4);
/*
		ai_win::set_dlg_item_float(hdlg, IDC_P1_X,		  SM201_POSITION_1,     3);
		ai_win::set_dlg_item_float(hdlg, IDC_P2_X,		  SM201_POSITION_2,     3);
		ai_win::set_dlg_item_float(hdlg, IDC_P3_X,		  SM201_POSITION_3,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P4_X,		  SM201_POSITION_4,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P5_X,		  SM201_POSITION_5,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P6_X,		  SM201_POSITION_6,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P7_X,		  SM201_POSITION_7,		3);

		ai_win::set_dlg_item_float(hdlg, IDC_P1_Y,		  SM202_POSITION_1,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P2_Y,		  SM202_POSITION_2,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P3_Y,		  SM202_POSITION_3,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P4_Y,		  SM202_POSITION_4,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P5_Y,		  SM202_POSITION_5,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P6_Y,		  SM202_POSITION_6,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P7_Y,		  SM202_POSITION_7,		3);

		ai_win::set_dlg_item_float(hdlg, IDC_P1_Z,		  SM203_POSITION_1,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P2_Z,		  SM203_POSITION_2,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P3_Z,		  SM203_POSITION_3,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P4_Z,		  SM203_POSITION_4,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P5_Z,		  SM203_POSITION_5,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P6_Z,		  SM203_POSITION_6,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_P7_Z,		  SM203_POSITION_7,		3);

		ai_win::set_dlg_item_float(hdlg, IDC_TABLE_X1,	  SM204_POSITION_1,		3);
		ai_win::set_dlg_item_float(hdlg, IDC_TABLE_X2,	  SM204_POSITION_2,		3);*/

		ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(LINE_X_HOME_OFFSET*1.0/LINE_X_RATIO),		 4);

// 		SET_CHECKER		(hdlg, IDC_MIRROR_X,				IMG_MIRROR_X				);
// 		SET_CHECKER		(hdlg, IDC_MIRROR_Y,				IMG_MIRROR_Y				);
	
		SetTimer(hdlg, TIMER_MOTOR_DLG_JOG, 100, NULL);

		DWORD dw;
		HANDLE hmonitor = CreateThread(NULL, 0, position_display_thread, NULL, 0, &dw);
		CloseHandle(hmonitor);

		HANDLE hbutton = CreateThread(NULL, 0, button_process_thread, NULL, 0, &dw);
		CloseHandle(hbutton);
	}

	motor_init_err = motor_init();

	sprintf(status, "Init Motor %s[%d].", (motor_init_err != 0x00)?"Fail":"OK", motor_init_err);
	SetDlgItemText(hdlg, IDC_STATUS2, status);
	load_err_msg();

	return TRUE;
}

void MotorDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{
	float float_val = 0.0f;
	double double_val = 0.0;
	int int_val = 0;
	int err = 0;
	switch (id)
	{
/*
	case IDC_CARRY_START:
		CARRY_START = TRUE;
		break;

	case IDC_CARRY_END:
		CARRY_START = FALSE;
		break;*/


	case IDC_SAVE_CONFIG:
		{
			err = save_shm(config_path, &mshm);
			log("save shm file[%d]", err);
		}
		break;

	case IDC_MOTOR:
		if (GET_CHECKER(hdlg, id)){
			SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Chart z");
			select_motor = M_CHART_Z;
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	CHART_Z_HOME_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	CHART_Z_JOG_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	CHART_Z_MOVE_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		CHART_Z_HOME_ACCEL_TIME);
			ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		CHART_Z_JOG_ACCEL_TIME );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		CHART_Z_MOVE_ACCEL_TIME);

			ai_win::set_dlg_item_float(hdlg, IDC_TARGET_RELATED_DISTANCE, (double)(CHART_Z_SHORT_MOVE_DISTANCE*1.0/CHART_Z_RATIO), 4);
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(CHART_Z_HOME_OFFSET*1.0/CHART_Z_RATIO),		 4);
		}
		break;

	case IDC_MOTOR2:
		if (GET_CHECKER(hdlg, id)){
			SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Line T");
			select_motor = M_LINE_T;
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	LINE_T_HOME_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	LINE_T_JOG_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	LINE_T_MOVE_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		LINE_T_HOME_ACCEL_TIME);
			ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		LINE_T_JOG_ACCEL_TIME );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		LINE_T_MOVE_ACCEL_TIME);

			ai_win::set_dlg_item_float(hdlg, IDC_TARGET_RELATED_DISTANCE, (double)(LINE_T_SHORT_MOVE_DISTANCE*1.0/LINE_T_RATIO), 4);
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(LINE_T_HOME_OFFSET*1.0/LINE_T_RATIO),		 4);
		}
		break;

	case IDC_MOTOR3:
		if (GET_CHECKER(hdlg, id)){
			SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Line Y");
			select_motor = M_LINE_Y;
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	LINE_Y_HOME_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	LINE_Y_JOG_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	LINE_Y_MOVE_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		LINE_Y_HOME_ACCEL_TIME);
			ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		LINE_Y_JOG_ACCEL_TIME );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		LINE_Y_MOVE_ACCEL_TIME);

			ai_win::set_dlg_item_float(hdlg, IDC_TARGET_RELATED_DISTANCE, (double)(LINE_Y_SHORT_MOVE_DISTANCE*1.0/LINE_Y_RATIO), 4);
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(LINE_Y_HOME_OFFSET*1.0/LINE_Y_RATIO),		 4);
		}

		break;

	case IDC_MOTOR4:
		if (GET_CHECKER(hdlg, id)){
			SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Line X");
			select_motor = M_LINE_X;
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	LINE_X_HOME_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	LINE_X_JOG_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	LINE_X_MOVE_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		LINE_X_HOME_ACCEL_TIME);
			ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		LINE_X_JOG_ACCEL_TIME );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		LINE_X_MOVE_ACCEL_TIME);

			ai_win::set_dlg_item_float(hdlg, IDC_TARGET_RELATED_DISTANCE, (double)(LINE_X_SHORT_MOVE_DISTANCE*1.0/LINE_X_RATIO), 4);
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(LINE_X_HOME_OFFSET*1.0/LINE_X_RATIO),		 4);
		}
		break;

	case IDC_MOTOR5:
		if (GET_CHECKER(hdlg, id)){
			SetDlgItemText(hdlg, IDC_MOTOR_SELECT, "Table U");
			select_motor = M_TABLE_U;
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_HOME,	TABLE_U_HOME_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_JOG,	TABLE_U_JOG_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_SPEED_MOVE,	TABLE_U_MOVE_SPEED	  );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_HOME,		TABLE_U_HOME_ACCEL_TIME);
			ai_win::set_dlg_item_int(hdlg, IDC_AD_JOG,		TABLE_U_JOG_ACCEL_TIME );
			ai_win::set_dlg_item_int(hdlg, IDC_AD_MOVE,		TABLE_U_MOVE_ACCEL_TIME);

			ai_win::set_dlg_item_float(hdlg, IDC_TARGET_RELATED_DISTANCE, (double)(TABLE_U_SHORT_MOVE_DISTANCE*1.0/TABLE_U_RATIO), 4);
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET,			  (double)(TABLE_U_HOME_OFFSET*1.0/TABLE_U_RATIO),		 4);
		}
		break;

	case IDC_MOTOR_SERVO_ON:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX	; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX; break;
		}
		break;

	case IDC_MOTOR_SERVO_OFF:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX+1; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX+1; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX	+1; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX+1; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX+1; break;	
		}
		break;

	case IDC_MOTOR_HOME:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX+3; motor_flag=2; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX+3; motor_flag=2; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX+3; motor_flag=2; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX+3; motor_flag=2; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX+3; motor_flag=2; break;	
		}
		break;

	case IDC_MOTOR_STOP:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX+6; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX+6; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX+6; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX+6; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX+6; break;	

		}
		break;

	case IDC_MOVE_JOG_PLUS:  break;
	case IDC_MOVE_JOG_MINUS: break;

	case IDC_MOVE_POS_PLUS:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX+7; motor_flag=2; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX+7; motor_flag=2; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX+7; motor_flag=2; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX+7; motor_flag=2; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX+7; motor_flag=2; break;
		}
		break;

	case IDC_MOVE_POS_MINUS:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = CHART_Z_BUTTON_INDEX+8; motor_flag=2; break;
		case M_LINE_T: BUTTON_VALUE  = LINE_T_BUTTON_INDEX+8; motor_flag=2; break;
		case M_LINE_Y: BUTTON_VALUE  = LINE_Y_BUTTON_INDEX+8; motor_flag=2; break;
		case M_LINE_X: BUTTON_VALUE  = LINE_X_BUTTON_INDEX+8; motor_flag=2; break;
		case M_TABLE_U: BUTTON_VALUE = TABLE_U_BUTTON_INDEX+8; motor_flag=2; break;

		}
		break;

	case IDC_HOME_OFFSET_SET:
		switch (select_motor){
		case M_CHART_Z:
			CHART_Z_HOME_OFFSET = CHART_Z_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET, (double)(CHART_Z_HOME_OFFSET*1.0/CHART_Z_RATIO), 4);
			break;

		case M_LINE_T:
			LINE_T_HOME_OFFSET = LINE_T_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET, (double)(LINE_T_HOME_OFFSET*1.0/LINE_T_RATIO), 4);
			break;

		case M_LINE_Y:
			LINE_Y_HOME_OFFSET = LINE_Y_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET, (double)(LINE_Y_HOME_OFFSET*1.0/LINE_Y_RATIO), 4);
			break;

		case M_LINE_X:
			LINE_X_HOME_OFFSET = LINE_X_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET, (double)(LINE_X_HOME_OFFSET*1.0/LINE_X_RATIO), 4);
			break;

		case M_TABLE_U:
			TABLE_U_HOME_OFFSET = TABLE_U_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSET, (double)(TABLE_U_HOME_OFFSET*1.0/TABLE_U_RATIO), 4);
			break;
		}
		break;
	case IDC_PASS_OFFSET:
		{
			LINE_Y_PASS_OFFSET = LINE_Y_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_PASS_OFFSETY, (double)(LINE_Y_PASS_OFFSET*1.0/LINE_Y_RATIO), 4);

			LINE_X_PASS_OFFSET = LINE_X_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_PASS_OFFSETX, (double)(LINE_X_PASS_OFFSET*1.0/LINE_X_RATIO), 4);

			break;
		}
		break;
	case IDC_NG_OFFSET:
		{

			LINE_Y_NG_OFFSET = LINE_Y_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_NG_OFFSETY, (double)(LINE_Y_NG_OFFSET*1.0/LINE_Y_RATIO), 4);

			LINE_X_NG_OFFSET = LINE_X_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_NG_OFFSETX, (double)(LINE_X_NG_OFFSET*1.0/LINE_X_RATIO), 4);
			break;

		}
		break;
	case IDC_HOME_OFFSET2:
		{

			LINE_Y_PRODUCT_HOME_OFFSET = LINE_Y_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSETY, (double)(LINE_Y_PRODUCT_HOME_OFFSET*1.0/LINE_Y_RATIO), 4);

			LINE_X_PRODUCT_HOME_OFFSET = LINE_X_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_HOME_OFFSETX, (double)(LINE_X_PRODUCT_HOME_OFFSET*1.0/LINE_X_RATIO), 4);

			break;
		}
		break;
	case IDC_STACK_OFFSET1:
		{

			LINE_T_STACK1 = LINE_T_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK1, (double)(LINE_T_STACK1*1.0/LINE_T_RATIO), 4);
			
			break;
		}
		break;
	case IDC_STACK_OFFSET2:
		{

			LINE_T_STACK2 = LINE_T_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK2, (double)(LINE_T_STACK2*1.0/LINE_T_RATIO), 4);
		
			break;
		}
		break;
	case IDC_STACK_OFFSET3:
		{

			LINE_T_STACK3 = LINE_T_POSITION;
			ai_win::set_dlg_item_float(hdlg, IDC_OFFSET_STACK3, (double)(LINE_T_STACK3*1.0/LINE_T_RATIO), 4);
			
			break;
		}
		break;
	case IDC_MOVE_STACK1:
		
		BUTTON_VALUE = 232;
		break;
	case IDC_MOVE_STACK2:
		BUTTON_VALUE = 233;
		break;
	case IDC_MOVE_STACK3:
		BUTTON_VALUE = 234;
		break;
	case IDC_MOVE_PASS:
		SM103_COUNT = ai_win::get_dlg_item_int(HWND_MOTOR_MOTION,IDC_PASS_NUMY);
		SM104_COUNT = ai_win::get_dlg_item_int(HWND_MOTOR_MOTION,IDC_PASS_NUMX);
		BUTTON_VALUE = 225;
		break;

	case IDC_MOVE_NG:
		SM103_NG_COUNT = ai_win::get_dlg_item_int(HWND_MOTOR_MOTION,IDC_NG_NUMY);
		SM104_NG_COUNT = ai_win::get_dlg_item_int(HWND_MOTOR_MOTION,IDC_NG_NUMX);
		BUTTON_VALUE = 226;
		break;

	case IDC_MOVE_HOME:
		BUTTON_VALUE = 227;
		break;

	case IDC_MOVE_TO_HOME_OFFSET:
		switch (select_motor){
		case M_CHART_Z: BUTTON_VALUE = 210; break;
		case M_LINE_T:  BUTTON_VALUE = 211; break;
		case M_LINE_Y:  BUTTON_VALUE = 212; break;
		case M_LINE_X:  BUTTON_VALUE = 213; break;
		case M_TABLE_U: BUTTON_VALUE = 214; break;
		}
		break;
// 
// 	case IDC_MOVE_LOAD_POSITION1:
// 		log("recv move to p1");
// 		BUTTON_VALUE = 310; break;
// 		
// 	case IDC_MOVE_LOAD_POSITION2:
// 		log("recv move to p2");
// 		BUTTON_VALUE = 311; break;
// 
// 	case IDC_MOVE_LOAD_POSITION3:
// 		log("recv move to p3");
// 		BUTTON_VALUE = 312; break;
// 
// 	case IDC_MOVE_LOAD_POSITION4:
// 		log("recv move to p4");
// 		BUTTON_VALUE = 313; break;
// 
// 	case IDC_MOVE_LOAD_POSITION5:
// 		log("recv move to p5");
// 		BUTTON_VALUE = 314; break;
// 
// 	case IDC_MOVE_LOAD_POSITION6:
// 		log("recv move to p6");
// 		BUTTON_VALUE = 315; break;

	case IDC_TARGET_RELATED_DISTANCE:
		double_val = ai_win::get_dlg_item_float(hdlg, id);

		switch (select_motor){
		case M_CHART_Z: CHART_Z_SHORT_MOVE_DISTANCE = (int)(double_val*CHART_Z_RATIO+0.5); break;
		case M_LINE_T :  LINE_T_SHORT_MOVE_DISTANCE = (int)(double_val*LINE_T_RATIO+0.5) ; break;
		case M_LINE_Y :  LINE_Y_SHORT_MOVE_DISTANCE = (int)(double_val*LINE_Y_RATIO+0.5) ; break;
		case M_LINE_X :  LINE_X_SHORT_MOVE_DISTANCE  = (int)(double_val*LINE_X_RATIO+0.5) ; break;
		case M_TABLE_U: TABLE_U_SHORT_MOVE_DISTANCE = (int)(double_val*TABLE_U_RATIO+0.5); break;
		}
		break;

	case IDC_CLEAR_MSG:
		SetDlgItemText(hdlg, IDC_STATUS2, "");
		break;

	case IDC_STOP_ALL:
		HomeMachine_StopFlag = TRUE;
		Sleep(1000);
		SM101_motor_stop();
		SM102_motor_stop();
		SM103_motor_stop();
		SM104_motor_stop();
		SM105_motor_stop();
		EMERGENCY_STOP_FLAG = TRUE;

		SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, "Stop All Motors");
		ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_HOME_ALL);
		break;

	case IDC_HOME_ALL:
		BUTTON_VALUE = 221;
		break;

	case IDC_RESET:
		EMERGENCY_STOP_FLAG = FALSE;
		break;

	case IDC_MOVE_HOME_OFFSET:
		RUN_HOME_OFFSET = GET_CHECKER(hdlg, id);
		break;	//////////////////////////////////////////////////////////////////////////

	case IDC_PIXEL_SIZE:
		PIXEL_SIZE = ai_win::get_dlg_item_float(hdlg, id);
		break;

	case IDC_TARGET_CENTER_X:
		AA_TARGET_CENTER_X = ai_win::get_dlg_item_int(hdlg, id);
		break;

	case IDC_TARGET_CENTER_Y:
		AA_TARGET_CENTER_Y = ai_win::get_dlg_item_int(hdlg, id);
		break;

	}

	switch (code_notify){
	case BN_CLICKED:
		switch (id){
		case IDC_MOVE_JOG_PLUS:
		case IDC_MOVE_JOG_MINUS:
			switch (select_motor){
			case M_CHART_Z:  BUTTON_VALUE = CHART_Z_BUTTON_INDEX+6; break;
			case M_LINE_T :  BUTTON_VALUE = LINE_T_BUTTON_INDEX+6; break;
			case M_LINE_Y :  BUTTON_VALUE = LINE_Y_BUTTON_INDEX+6; break;
			case M_LINE_X :  BUTTON_VALUE = LINE_X_BUTTON_INDEX+6; break;
			case M_TABLE_U:  BUTTON_VALUE = TABLE_U_BUTTON_INDEX+6; break;

			}
			break;
		}
		break;

	case EN_CHANGE:
		switch (id){
		case IDC_SPEED_HOME:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_HOME_SPEED = int_val; break;
			case M_LINE_T : LINE_T_HOME_SPEED = int_val ; break;
			case M_LINE_Y : LINE_Y_HOME_SPEED = int_val ; break;
			case M_LINE_X : LINE_X_HOME_SPEED = int_val ; break;
			case M_TABLE_U: TABLE_U_HOME_SPEED = int_val; break;	
			}
			break;

		case IDC_SPEED_JOG:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_JOG_SPEED = int_val; break;
			case M_LINE_T : LINE_T_JOG_SPEED = int_val; break;
			case M_LINE_Y : LINE_Y_JOG_SPEED = int_val; break;
			case M_LINE_X : LINE_X_JOG_SPEED = int_val; break;
			case M_TABLE_U: TABLE_U_JOG_SPEED = int_val; break;
			}
			break;

		case IDC_SPEED_MOVE:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_MOVE_SPEED = int_val; break;
			case M_LINE_T : LINE_T_MOVE_SPEED = int_val; break;
			case M_LINE_Y : LINE_Y_MOVE_SPEED = int_val; break;
			case M_LINE_X : LINE_X_MOVE_SPEED = int_val; break;
			case M_TABLE_U: TABLE_U_MOVE_SPEED = int_val; break;
			}
			break;

		case IDC_AD_HOME:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_HOME_ACCEL_TIME = int_val; break;
			case M_LINE_T : LINE_T_HOME_ACCEL_TIME = int_val; break;
			case M_LINE_Y : LINE_Y_HOME_ACCEL_TIME = int_val; break;
			case M_LINE_X : LINE_X_HOME_ACCEL_TIME = int_val; break;
			case M_TABLE_U: TABLE_U_HOME_ACCEL_TIME = int_val; break;	

			}
			break;

		case IDC_AD_JOG:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_JOG_ACCEL_TIME = int_val; break;
			case M_LINE_T : LINE_T_JOG_ACCEL_TIME = int_val; break;
			case M_LINE_Y : LINE_Y_JOG_ACCEL_TIME = int_val; break;
			case M_LINE_X : LINE_X_JOG_ACCEL_TIME = int_val; break;
			case M_TABLE_U: TABLE_U_JOG_ACCEL_TIME = int_val; break;	

			}
			break;

		case IDC_AD_MOVE:
			int_val = GetDlgItemInt(hdlg, id, NULL, FALSE);
			switch (select_motor){
			case M_CHART_Z: CHART_Z_MOVE_ACCEL_TIME = int_val; break;
			case M_LINE_T : LINE_T_MOVE_ACCEL_TIME = int_val; break;
			case M_LINE_Y : LINE_Y_MOVE_ACCEL_TIME = int_val; break;
			case M_LINE_X : LINE_X_MOVE_ACCEL_TIME = int_val; break;
			case M_TABLE_U: TABLE_U_MOVE_ACCEL_TIME = int_val; break;	

			}
			break;
	
		case IDC_LENS_EFL:				AA_LENS_EFL			   = ai_win::get_dlg_item_float(hdlg, id);  break;
		case IDC_PLC_DB:				PLC_DB_INDEX		   = ai_win::get_dlg_item_int(hdlg, id);	break;
		case IDC_DB_START:				PLC_DB_START		   = ai_win::get_dlg_item_int(hdlg, id);	break;
		case IDC_DB_LENS:				PLC_DB_LENS			   = ai_win::get_dlg_item_int(hdlg, id);	break;
		case IDC_PLC_DB2:				PLC_DB_INDEX_R		   = ai_win::get_dlg_item_int(hdlg, id);	break;
		case IDC_DB_START2:				PLC_DB_START_R		   = ai_win::get_dlg_item_int(hdlg, id);	break;
		case IDC_DB_LENS2:				PLC_DB_LENS_R		   = ai_win::get_dlg_item_int(hdlg, id);	break;

/*
		case IDC_P1_X:					SM201_POSITION_1	   = ai_win::get_dlg_item_float(hdlg,id);	break;			
		case IDC_P2_X:					SM201_POSITION_2	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P3_X:					SM201_POSITION_3	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P4_X:					SM201_POSITION_4	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P5_X:					SM201_POSITION_5	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P6_X:					SM201_POSITION_6	   = ai_win::get_dlg_item_float(hdlg,id);	break;

		case IDC_P1_Y:					SM202_POSITION_1	   = ai_win::get_dlg_item_float(hdlg,id);	break;	
		case IDC_P2_Y:					SM202_POSITION_2	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P3_Y:					SM202_POSITION_3	   = ai_win::get_dlg_item_float(hdlg,id);	break;	
		case IDC_P4_Y:					SM202_POSITION_4	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P5_Y:					SM202_POSITION_5	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P6_Y:					SM202_POSITION_6	   = ai_win::get_dlg_item_float(hdlg,id);	break;

		case IDC_P1_Z:					SM203_POSITION_1	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P2_Z:					SM203_POSITION_2	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P3_Z:					SM203_POSITION_3	   = ai_win::get_dlg_item_float(hdlg,id);	break;	
		case IDC_P4_Z:					SM203_POSITION_4	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P5_Z:					SM203_POSITION_5	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_P6_Z:					SM203_POSITION_6	   = ai_win::get_dlg_item_float(hdlg,id);	break;

		case IDC_TABLE_X1:				SM204_POSITION_1	   = ai_win::get_dlg_item_float(hdlg,id);	break;
		case IDC_TABLE_X2:				SM204_POSITION_2	   = ai_win::get_dlg_item_float(hdlg,id);	break;*/
	//	case IDC_P1_X:					SM201_POSITION_1	   = ai_win::get_dlg_item_float(hdlg,id);	break;

		}

		break;
	}
}

void MotorDlg::timer(HWND hdlg, unsigned int id)
{
	int err = 0;
	switch (id){
	case TIMER_MOTOR_DLG_JOG:
		{
			int jog_up_button   = ::SendMessage(GetDlgItem(hdlg, IDC_MOVE_JOG_PLUS),  BM_GETSTATE, 0, 0);
			int jog_down_button = ::SendMessage(GetDlgItem(hdlg, IDC_MOVE_JOG_MINUS), BM_GETSTATE, 0, 0);

			if (jog_up_button == 0x26c){
				switch (select_motor){
				case M_CHART_Z:	BUTTON_VALUE = CHART_Z_BUTTON_INDEX+4; break;
				case M_LINE_T:	BUTTON_VALUE = LINE_T_BUTTON_INDEX+4; break;
				case M_LINE_Y:	BUTTON_VALUE = LINE_Y_BUTTON_INDEX+4; break;
				case M_LINE_X:	BUTTON_VALUE = LINE_X_BUTTON_INDEX+4; break;
				case M_TABLE_U:	BUTTON_VALUE = TABLE_U_BUTTON_INDEX+4; break;

				}

				log("jog cw active!");
			}

			if (jog_down_button == 0x26c ){
				switch (select_motor){
				case M_CHART_Z:	BUTTON_VALUE = CHART_Z_BUTTON_INDEX+5; break;
				case M_LINE_T:	BUTTON_VALUE = LINE_T_BUTTON_INDEX+5; break;
				case M_LINE_Y:	BUTTON_VALUE = LINE_Y_BUTTON_INDEX+5; break;
				case M_LINE_X:	BUTTON_VALUE = LINE_X_BUTTON_INDEX+5; break;
				case M_TABLE_U:	BUTTON_VALUE = TABLE_U_BUTTON_INDEX+5; break;

				}
				log("jog ccw active!");
			}
		}
		break;
	}
}

void MotorDlg::close(HWND hdlg)
{
	EndDialog(hdlg, FALSE);
}

LRESULT MotorDlg::ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark)
{
	DeleteObject(dlg_brush);
	SetBkMode(hdc, TRANSPARENT);
	dlg_brush = CreateSolidBrush(RGB(0x66, 0x66, 0x66));
	old_dlg_brush = (HBRUSH)SelectObject(hdc, (HBRUSH)dlg_brush);

	return (LRESULT)dlg_brush;
}

int MotorDlg::read_shm(char *path, MOTION_SHM *shm)
{
	FILE *file_ptr;
	int counter = 0x00;
	char buf1[40];
	int i;
	float float_val;

	if ((file_ptr=fopen(path, "rt")) == NULL){
		log("read shm file fail.");
		return 0x01;
	}


	//=========================================================================================
	// discard the first line which is title for this data ====================================
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x02);}

	// read SERVO MOTOR parameters 
	for(i=0;i<MOTORS_PARA_NUM;i++)
	{
		fscanf(file_ptr,"%d",&(shm->SERVO_MOTOR_PARAMETERS[i]));     
		counter++;
	}
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x02);}   // report file reading error

	//=========================================================================================
	// discard the first line which is title for this data ====================================
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x05);}
	// read general RAM parameter 
	for(i=0;i<INT_RAM_NUM;i++){
		fscanf(file_ptr,"%d",&(shm->RAM_INT[i]));  
		counter++;
	}
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x05);}   // report file reading error

	//=========================================================================================
	// discard the first line which is title for this data ====================================
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x05);}
	// read general RAM parameter 
	for(i=0;i<DOUBLE_RAM_NUM;i++){
		fscanf(file_ptr,"%f",&float_val); 
		shm->RAM_DOUBLE[i] = (double)float_val;
		counter++;
	}
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x05);}   // report file reading error

	//=========================================================================================
	// discard the first line which is title for this data ====================================
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x04);}
	// read Timer 
	for(i=0;i<TIMER_RAM_NUM;i++){
		fscanf(file_ptr,"%d",&(shm->TIMER[i])); 
		counter++;
	}
	if(fgets(buf1,sizeof(buf1)-1,file_ptr)==NULL){ fclose(file_ptr); return(0x04);}   // report file reading error

	fclose(file_ptr);

	return(0x00);
}

int MotorDlg::save_shm(char *path, MOTION_SHM *shm)
{
	FILE *file_ptr;
	int i, counter;

	if (strlen(path)<=0){
		//		MessageBox(NULL, "Model name not exist.", "Error", 0);
		return -1;
	}

	counter=0x00;
	if((file_ptr=fopen(path,"wt"))==NULL){ 
		//		MessageBox(NULL, "Open Sharing Memory File Error.", "Error", 0);
		return -2; 
	}

	fprintf(file_ptr,"SERVO MOTOR PARAMETRS:%s","\x0D\x0A\x00");   
	for(i=0;i<MOTORS_PARA_NUM;i++){
		fprintf(file_ptr," %d",shm->SERVO_MOTOR_PARAMETERS[i]);  // save servo motor parameters 
		counter++;
	}
	fprintf(file_ptr,"%s","\x0D\x0A\x00"); 

	fprintf(file_ptr,"INT RAM:%s","\x0D\x0A\x00");   
	for(i=0;i<INT_RAM_NUM;i++){
		fprintf(file_ptr," %d",shm->RAM_INT[i]);  // save servo motor parameters 
		counter++;
	}
	fprintf(file_ptr,"%s","\x0D\x0A\x00"); 

	fprintf(file_ptr,"DOUBLE RAM:%s","\x0D\x0A\x00");   
	for(i=0;i<DOUBLE_RAM_NUM;i++){
		fprintf(file_ptr," %f",shm->RAM_DOUBLE[i]);  // save servo motor parameters 
		counter++;
	}
	fprintf(file_ptr,"%s","\x0D\x0A\x00"); 

	fprintf(file_ptr,"TIMER:%s","\x0D\x0A\x00");   
	for(i=0;i<TIMER_RAM_NUM;i++){
		fprintf(file_ptr," %d",shm->TIMER[i]);  // save servo motor parameters 
		counter++;
	}
	fprintf(file_ptr,"%s","\x0D\x0A\x00"); 

	fclose(file_ptr);

	return counter;
}

DWORD WINAPI position_display_thread(LPVOID temp)
{
	double motor_pos[MOTOR_NUM] = {0.0};

	while (1){
		motor_pos[0] = ((double)(CHART_Z_POSITION * 1.0)/(CHART_Z_RATIO	));
		motor_pos[1] = ((double)(LINE_T_POSITION * 1.0)/ (LINE_T_RATIO	));
		motor_pos[2] = ((double)(LINE_Y_POSITION * 1.0)/ (LINE_Y_RATIO	));
		motor_pos[3] = ((double)(LINE_X_POSITION * 1.0)/ (LINE_X_RATIO	));
		motor_pos[4] = ((double)(TABLE_U_POSITION * 1.0)/(TABLE_U_RATIO	));

		switch (select_motor){
		case M_CHART_Z:	
			ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION, motor_pos[0], 4); 
			ai_win::set_dlg_item_int(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION2, CHART_Z_POSITION);
				//SM201 STATUS DISPLAY ============================================================

			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_HOME,			(((CHART_Z_STATUS & 0x01)==0x01)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_PLU,		(((CHART_Z_STATUS & 0x02)==0x02)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_MIN,		(((CHART_Z_STATUS & 0x04)==0x04)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_ALARM,			(((CHART_Z_STATUS & 0x08)==0x08)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_SERVO_ON,		(((CHART_Z_STATUS & 0x10)==0x10)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_INPOS,			(((CHART_Z_STATUS & 0x20)==0x20)?TRUE:FALSE));
			break;

		case M_LINE_T:	
			ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION, motor_pos[1], 4); 
			ai_win::set_dlg_item_int(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION2, LINE_T_POSITION);

			//===== SM203 STATUS DISPLAY ============================================================

			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_HOME,			(((LINE_T_STATUS & 0x01)==0x01)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_PLU,		(((LINE_T_STATUS & 0x02)==0x02)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_MIN,		(((LINE_T_STATUS & 0x04)==0x04)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_ALARM,			(((LINE_T_STATUS & 0x08)==0x08)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_SERVO_ON,		(((LINE_T_STATUS & 0x10)==0x10)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_INPOS,			(((LINE_T_STATUS & 0x20)==0x20)?TRUE:FALSE));
			break;

		case M_LINE_Y:	
			ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION, motor_pos[2], 4); 
			ai_win::set_dlg_item_int(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION2, LINE_Y_POSITION);
			//===== SM102 STATUS DISPLAY ============================================================
	//		log("tilt_x position:%f",motor_pos[2]);
//			log("status:%d,103home:%d£¬limit:%d",LINE_Y_STATUS,SM103_HOME,SM103_LMT_N);
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_HOME,			(((LINE_Y_STATUS & 0x01)==0x01)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_PLU,		(((LINE_Y_STATUS & 0x02)==0x02)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_MIN,		(((LINE_Y_STATUS & 0x04)==0x04)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_ALARM,			(((LINE_Y_STATUS & 0x08)==0x08)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_SERVO_ON,		(((LINE_Y_STATUS & 0x10)==0x10)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_INPOS,			(((LINE_Y_STATUS & 0x20)==0x20)?TRUE:FALSE));

			break;

		case M_LINE_X:	
			ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION, motor_pos[3], 4); 
			ai_win::set_dlg_item_int(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION2, LINE_X_POSITION);
//			mysbw.tilt_x_pos = LINE_Y_POSITION;
			//===== SM101 STATUS DISPLAY ============================================================
		//	log("tilt_y position:%f",motor_pos[3]);
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_HOME,			(((LINE_X_STATUS & 0x01)==0x01)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_PLU,		(((LINE_X_STATUS & 0x02)==0x02)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_MIN,		(((LINE_X_STATUS & 0x04)==0x04)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_ALARM,			(((LINE_X_STATUS & 0x08)==0x08)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_SERVO_ON,		(((LINE_X_STATUS & 0x10)==0x10)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_INPOS,			(((LINE_X_STATUS & 0x20)==0x20)?TRUE:FALSE));
			break;

		case M_TABLE_U:	
			ai_win::set_dlg_item_float(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION, motor_pos[4], 4); 
			ai_win::set_dlg_item_int(HWND_MOTOR_MOTION, IDC_SIGNAL_POSITION2, TABLE_U_POSITION);
//			mysbw.tilt_y_pos = TILT_Y_POSITION;
			//===== SM104 STATUS DISPLAY ============================================================

			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_HOME,			(((TABLE_U_STATUS & 0x01)==0x01)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_PLU,		(((TABLE_U_STATUS & 0x02)==0x02)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_LIMIT_MIN,		(((TABLE_U_STATUS & 0x04)==0x04)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_ALARM,			(((TABLE_U_STATUS & 0x08)==0x08)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_SERVO_ON,		(((TABLE_U_STATUS & 0x10)==0x10)?TRUE:FALSE));
			SET_CHECKER(HWND_MOTOR_MOTION, IDC_CHECK_INPOS,			(((TABLE_U_STATUS & 0x20)==0x20)?TRUE:FALSE));
			break;
		}
		
		Sleep(100);
	}

	return 0;
}

void motor_button_waiting(int wait_time)
{
	Sleep(50);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOTOR_SERVO_ON);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOTOR_SERVO_OFF);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOTOR_HOME);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOTOR_RESET);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOTOR_STOP);

	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_SPEED_HOME);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_SPEED_MOVE);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_SPEED_JOG);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_AD_HOME);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_AD_JOG);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_AD_MOVE);

	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_MINUS);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_PLUS);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_MINUS);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_PLUS);

	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_HOME_ALL);
	//	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_STOP_ALL);
	ITEM_GRAYED(HWND_MOTOR_MOTION, IDC_SAVE);

	int timeout = 10*wait_time;
	while ((MANUAL_MOVING == ON) &&(timeout-- >= 0x00)) Sleep(100);
	Sleep(100);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOTOR_SERVO_ON);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOTOR_SERVO_OFF);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOTOR_HOME);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOTOR_RESET);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOTOR_STOP);

	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_SPEED_HOME);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_SPEED_MOVE);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_SPEED_JOG);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_AD_HOME);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_AD_JOG);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_AD_MOVE);

	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_MINUS);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_PLUS);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_MINUS);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_MOVE_JOG_PLUS);

	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_HOME_ALL);
//	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_STOP_ALL);
	ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_SAVE);

	if (MANUAL_ERROR != 0x00){
		char buf[100] = {0};
		sprintf(buf, "Err[%d]:%s", MANUAL_ERROR, err_msg[MANUAL_ERROR]);
		SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, buf);
		SetDlgItemText(HWND_MOTION, IDC_STATUS, buf);
	}
}

DWORD WINAPI button_process_thread(LPVOID temp)
{
	while (1){
		if (motor_flag){
			motor_button_waiting(motor_flag);
			motor_flag = 0;
		}

		Sleep(50);
	}
	return 0x00;
}
int home_all_motor(void)
{
	int errcode;
	SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, "Motor auto moving...");	
	if(errcode = first_cylinder_cfg(2) != 0)   return errcode;
	if(errcode = second_cylinder_cfg(2) != 0)  return errcode;
	if(errcode = third_cylinder_cfg(2) != 0)   return errcode;
	if(errcode = fourth_cylinder_cfg(2) != 0)  return errcode;
	if(errcode = dark_box_cfg(2) != 0)		   return errcode;
	if(errcode = light_box_cfg(2) != 0)		   return errcode;
	if(errcode = feed_cylinder_cfg(2) != 0)	   return errcode;
	if(errcode = gripper_cylinder_cfg(2) != 0) return errcode;
	if(errcode = gripper_move_cfg(2) != 0)	   return errcode;
	if(errcode = gripper_support_cfg(2) != 0)  return errcode;
	if(errcode = product_gripper_support_cfg(2) != 0) return errcode;
	if(errcode = product_gripper_cfg(2) != 0)  return errcode;
	if(errcode = pass_tray_cylinder(2) != 0)   return errcode;
	if(errcode = stack_cylinder(2) != 0)       return errcode;
	if(errcode = tray_block_cylinder(1) != 0)  return errcode;
	if(errcode = ng_tray_cylinder(2) != 0)	   return errcode;

	SM105_TRIGGER = SM105_HOMING;
	SM103_TRIGGER = SM103_HOMING;
	SM104_TRIGGER = SM104_HOMING;
	SM102_TRIGGER = SM102_HOMING;
	int timeout = 1000;
	while ((SM105_TRIGGER != OFF) || (SM103_TRIGGER != OFF) || (SM104_TRIGGER != OFF) || (SM102_TRIGGER != OFF) && timeout-->=0)
	{
		Sleep(100);
	}
	SM102_TRIGGER = SM102_MOVE_TO_HOME_POSITION;
	SM103_TRIGGER = SM103_MOVE_TO_HOME_POSITION;
	SM104_TRIGGER = SM104_MOVE_TO_HOME_POSITION;
	SM105_TRIGGER = SM105_MOVE_TO_HOME_POSITION;
	if (timeout<=0x00)
	{
		if (SM102_TRIGGER != OFF)	   return SM102_TRIGGER_NOT_READY_ERR;
		if (SM103_TRIGGER != OFF)	   return SM103_TRIGGER_NOT_READY_ERR;
		if (SM104_TRIGGER != OFF)	   return SM104_TRIGGER_NOT_READY_ERR;
		if (SM105_TRIGGER != OFF)	   return SM105_TRIGGER_NOT_READY_ERR;
	}
	HOME_FLAG = TRUE;
	SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, "Motor Home Done.");
	return 0x00;
}

DWORD WINAPI home_motors_thread(LPVOID temp)
{
	int home_result = FALSE;
	char buf[100] = {0};
	HOME_FLAG=FALSE;
	while (1){
		while (EXECUTE_HOME == FALSE) {
			Sleep(10);
			if(SM101_HOMED_FLAG&&SM102_HOMED_FLAG&&SM103_HOMED_FLAG&&SM104_HOMED_FLAG&&SM105_HOMED_FLAG){
				HOME_FLAG=TRUE;

			}
			//		log("home flag:%d",HOME_FLAG);
		}
		if ((home_result = home_all_motor()) != 0x00){
			sprintf(buf, "Err[%d]:%s", home_result, err_msg[home_result]);
			SetDlgItemText(HWND_MOTOR_MOTION, IDC_STATUS2, buf);
			EXECUTE_ERROR = TRUE;
			log("***Home Error***");
		}
		EXECUTE_CMD = FALSE;
		EXECUTE_HOME = FALSE;
		log("execute home = %d",EXECUTE_HOME);
		ITEM_ENABLED(HWND_MOTOR_MOTION, IDC_HOME_ALL);

		Sleep(10);
	}

	return FALSE;
}
/*
DWORD WINAPI product_move_thread(LPVOID temp)
{

}

*/

