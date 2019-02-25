/*#define _CRT_SECURE_NO_WARNINGS*/
#include "interface.h"
#include "inf_motion_aa.h"

#pragma  warning(disable:4996)

namespace t_flag
{
	int equipment_start = FALSE	;				//设备启动自动模式

	int feed_request_product = FALSE;			//上一站请求放料信息
	int feed_product = FALSE;					//入料信息
	int feed_right_staus =FALSE;				//待夹爪上转盘信息
	int feed_left_staus =FALSE;					//等待新产品上料信息
	int feed_griper_staus = FALSE;				//夹爪工作信息
	int feed_move_start	= FALSE;				//入料气缸位移信息

	int site1_task_product = FALSE;				//一工位产品信息
	int site1_up_product = FALSE;				//一工位上料
	int site1_down_product = FALSE;				//一工位下料信息

	int site2_task_product = FALSE;				//二工位产品信息
	int site2_test_end	= FALSE;
	int site2_test_start = FALSE;

	int site3_task_product = FALSE;
	int site3_test_end	= FALSE;
	int site3_test_start = FALSE;

	int site4_task_product = FALSE;
	int site4_test_end	= FALSE;
	int site4_test_start = FALSE;

	int table_task1_staus = FALSE;				//圆盘旋转一工位信息
	int table_task2_staus = FALSE;				//圆盘旋转二工位信息
	int table_task3_staus = FALSE;				//圆盘旋转三工位信息
	int table_task4_staus = FALSE;				//圆盘旋转四工位信息
	int table_feed_staus = FALSE;				//圆盘旋转入料信息
	int table_gripper_staus = FALSE;			//圆盘旋转夹爪位信息

	int table_staus_safe =	TRUE;				//转盘安全信息

	int carry_product_left_wait =TRUE;			//物料输送左到位发送等待信息
	int carry_product_right_wait =FALSE;		//物料输送右到位发送等待信息	
	int	product_carry_left	= FALSE;			//放至物料输送到位信息
	int	product_carry_right	= FALSE;			//物料输送夹取到位信息
	int pass_product = FALSE;					//产品合格
	int ng_product = FALSE;						//产品NG
	int pass_product_tray = TRUE;				//合格托盘就位
	int ng_product_tray = TRUE;					//NG托盘就位
	int ng_tray_reset = FALSE;					//NG托盘复位

	int product_code[PRODUCT_NUM]  = {1};
	int site2_num = 1;							//二工位站测试编号
	int site3_num = 1;
	int site4_num = 1;
	int carry_num = 1;							//运输物料编号
}

int motion_init_err = 1;
int CARRY_START = FALSE ;
int CARRY_ERR = 0;
char current_dir[MAX_PATH] = {0};

typedef struct _tagPowerSupply_V
{
	ai_comm::RS232 inf;
	int port;
	int baudrate;
	int is_linked;

	float current_limit[3];
	float voltage_val[3];
	float current_val[3];

	HANDLE ps_mutex;
}PowerSupply_V;
PowerSupply_V ps_link;

class MotionDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
	virtual void close(HWND hdlg);
//	virtual void timer(HWND hdlg, unsigned int id);
	virtual LRESULT ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark/* =CTLCOLOR_DLG */);

	int read_shm(char *path, MOTION_SHM *shm);
	int save_shm(char *path, MOTION_SHM *shm);


private:
	int is_init_finished;
	HBRUSH static_brush;
	HBRUSH old_brush;

	char config_path[MAX_PATH];
	char config_path_power[MAX_PATH];
	char status[MAX_PATH];
	HBRUSH dlg_brush, old_dlg_brush;

};
MotionDlg motion_dlg;

BOOL CALLBACK Dlg_Motion_Config(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,	motion_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		motion_dlg.command);
		HANDLE_DLGMSG(hdlg, WM_DRAWITEM,	motion_dlg.draw_item);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		motion_dlg.close);
	}
	return FALSE;

}
void ng_reset_led(void)
{
	Y47_ON;
	Sleep(1000);
	Y47_OFF;
	Sleep(1000);
}
void pass_reset_led(void)
{
	Y47_ON;
	Sleep(1000);
	Y47_OFF;
	Sleep(1000);
}
int new_tray_replace(void)
{
	int err,timeout;
	if (!S37)
	{
		log("no tray err");
		return S37_OFF_ERR;
	}
	if (err=SM102_move_to_stack1_position() != SUCCESS) 
	{
		log("line t move stack1 err = %d",err);
		return err;
	}
	log("line t move stack1");
	if (err = tray_block_cylinder(2) != SUCCESS)
	{
		log("tray block cylinder open err=%d",err);
		return err;
	}
	log("tray block cylinder open");
	Sleep(100);
	if (err=SM102_move_to_stack2_position() != SUCCESS) 
	{
		log("line t move stack2 err = %d",err);
		return err;
	}
	log("line t move stack2");

	if (err = tray_block_cylinder(1) != SUCCESS)
	{
		log("tray block cylinder close err=%d",err);
		return err;
	}
	log("tray block cylinder close");
	if (err=SM102_move_to_stack3_position() != SUCCESS) 
	{
		log("line t move stack3 err = %d",err);
		return err;
	}
	log("line t move stack3");

	timeout = 500;
	while(timeout-- > 0 && (!S35))
	{
		Sleep(30);
	}
	if (!S35)
	{
		log("S35 off err=%d",err);
		return S35_OFF_ERR;
	}
	if (err = pass_tray_cylinder(1) != SUCCESS)
	{
		log("pass tray up err=%d",err);
		return err;
	}
	log("pass tray up");
	Y38_OFF;
	log("line move stop");
	t_flag::pass_product_tray = TRUE;
	return 0;
}
int site2_test(void)
{
	int err;
	if (err = second_cylinder_cfg(1) != SUCCESS)	return err;
	Y51_OFF;
	Y52_OFF;
	Y50_ON;

	if (err = second_cylinder_cfg(2) != SUCCESS)	return err;
	return 0;
}
int site3_test(void)
{
	int err;
	int result;
	
	if (err = third_cylinder_cfg(1) != SUCCESS)	return err;
	Y50_OFF;
	Y52_OFF;
	Y51_ON;
	START_SITE_REG_MARK_CHART_TEST = TRUE;
	Sleep(9000);
	if (t_flag::product_code[t_flag::site3_num] == FALSE)
	{
		SfrResult(3);
		exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
	}
	else if (t_flag::product_code[t_flag::site3_num] == TRUE)
	{
		if (result = SfrResult(3) != TRUE)
		{
			log("reg result = %d",result);
			t_flag::product_code[t_flag::site3_num] = FALSE;
		}
		if (result = exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT) != TRUE)
		{
			log("reg result = %d",result);
			t_flag::product_code[t_flag::site3_num] = FALSE;
		}
	}
	log("product_code_std[%d]=%d",t_flag::site3_num,t_flag::product_code[t_flag::site3_num]);
 	t_flag::site3_num++;
	Sleep(100);
	if (err = third_cylinder_cfg(2) != SUCCESS)	return err;
	START_SITE_REG_MARK_CHART_TEST = FALSE;
	return 0;
}
int site4_test(void)
{
	int err;
	int result;
	if (err = fourth_cylinder_cfg(1) != SUCCESS)	return err;
	Y51_OFF;
	Y50_OFF;
	Y52_ON;
	START_SITE_WHITE_TEST = TRUE;
	Sleep(9000);
	if (t_flag::product_code[t_flag::site4_num] == FALSE)
	{
		log("site4 product FALSE");
		exec_white_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
	}
	else if (t_flag::product_code[t_flag::site4_num] == TRUE)
	{
		if (result = exec_white_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT) != TRUE)
		{
			log("white result = %d",result);
			t_flag::product_code[t_flag::site4_num] = FALSE;
		}
	}
	log("product_code_std[%d]=%d",t_flag::site4_num,t_flag::product_code[t_flag::site4_num]);
	t_flag::site4_num++;
	Sleep(100);
	if (err = fourth_cylinder_cfg(2) != SUCCESS)	return err;
	START_SITE_WHITE_TEST = FALSE;
	return 0;
}

BOOL MotionDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	log("***********************motion init.**********************");
	int errcode = 0;
	sprintf(config_path, "%s\\recipe\\%s\\motion_shm.mem", s_test::dir, s_test::recipe_name);

	int err = read_shm(config_path, &mshm);

	if (err != 0x00){
		log("open shm file error[%d]", err);
	}
//	sprintf(status, "Init Motor %s[%d].",(motion_init_err != 0x00)?"Fail":"OK", motion_init_err);
	SetDlgItemText(hdlg, IDC_STATUS, status);
	sprintf(config_path_power, "%s\\recipe\\%s\\power.ini", s_test::dir, s_test::recipe_name);	
	log("path:%s",config_path_power);
	
	ps_link.is_linked = FALSE;
	ps_link.port	  = ai_win::read_integer_key(config_path_power, "power_supply", "com_port"	);
	ps_link.baudrate  = ai_win::read_integer_key(config_path_power, "power_supply", "com_baudrate");
	ps_link.current_limit[0] = ai_win::read_float_key(config_path_power, "power_supply", "current_limit0");
	ps_link.current_limit[1] = ai_win::read_float_key(config_path_power, "power_supply", "current_limit1");
	ps_link.current_limit[2] = ai_win::read_float_key(config_path_power, "power_supply", "current_limit2");

	ps_link.voltage_val[0] = ai_win::read_float_key(config_path_power, "power_supply", "voltage_val0");
	ps_link.voltage_val[1] = ai_win::read_float_key(config_path_power, "power_supply", "voltage_val1");
	ps_link.voltage_val[2] = ai_win::read_float_key(config_path_power, "power_supply", "voltage_val2");

	ps_link.current_val[0] = ai_win::read_float_key(config_path_power, "power_supply", "current_val0");
	ps_link.current_val[1] = ai_win::read_float_key(config_path_power, "power_supply", "current_val1");
	ps_link.current_val[2] = ai_win::read_float_key(config_path_power, "power_supply", "current_val2");

	ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT1, ps_link.current_limit[0], 3);
	ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT2, ps_link.current_limit[1], 3);
//	ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT3, ps_link.current_limit[2], 3);

	return TRUE;
}
 void MotionDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{
	switch(id)
	{
	case IDC_FIRST_CYLINDER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?250:251); break;
	case IDC_SECOND_CYLINDER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?252:253); break;
	case IDC_THIRD_CYLINDER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?254:255); break;
	case IDC_FOURTH_CYLINDER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?256:257); break;
	case IDC_DARK_BOX:			BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?258:259); break;
	case IDC_LIGHT_BOX:			BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?260:261); break;
	case IDC_FEED_CYLINDER:		BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?262:263); break;
	case IDC_GRIPPER_MOVE:		BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?264:265); break;
	case IDC_PRODUCT_CYLINDER:	BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?266:267); break;
	case IDC_GRIPPER_SUPPORT:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?268:269); break;
	case IDC_GRIPPER_CYLINDER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?270:271); break;
	case IDC_PRODUCT_GRIPPER_SUPPORT:BUTTON_VALUE = (GET_CHECKER(hdlg,id)?272:273); break;
	case IDC_PRODUCT_GRIPPER:	BUTTON_VALUE = 	(GET_CHECKER(hdlg,id)?274:275); break;

	case IDC_PASS_TRAY:			BUTTON_VALUE =	(GET_CHECKER(hdlg,id)?276:277); break;
	case IDC_STACK_CYLINDER:	BUTTON_VALUE =	(GET_CHECKER(hdlg,id)?278:279); break;
	case IDC_BLOCK_CYLINDER:	BUTTON_VALUE =	(GET_CHECKER(hdlg,id)?280:281); break;
	case IDC_NG_TRAY:			BUTTON_VALUE =	(GET_CHECKER(hdlg,id)?282:283); break;
	case IDC_STACK1:			BUTTON_VALUE = 232; break;
	case IDC_STACK2:			BUTTON_VALUE = 233; break;
	case IDC_STACK3:			BUTTON_VALUE = 234; break;

	case IDC_FORWARD_FLUID:		BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?474:475); break; 
	case IDC_REVERSE_FLUID:		BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?476:477); break; 

	case IDC_CYLINDER_ALL:      BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?446:447);	break;
	case IDC_SECOND_POWER:		BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?498:499);	break;
	case IDC_THIRD_POWER:		BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?500:501);	break;
	case IDC_FOURTH_POWER:		BUTTON_VALUE =  (GET_CHECKER(hdlg,id)?502:503);	break;

	case IDC_START_CARRY:		t_flag::pass_product = TRUE;break;
	case IDC_START_CARRY2:		t_flag::ng_product = TRUE;break;
	case IDC_PASS_PRODUCT:		BUTTON_VALUE = 230;	 break;
	case IDC_NG_PRODUCT:		BUTTON_VALUE = 231;	 break;
	case IDC_HOME_PRODUCT:		BUTTON_VALUE = 227;  break;

//	case IDC_LEFT_GRIPPER:		BUTTON_VALUE = 240;  break;
//	case IDC_ALL_GRIPPER:		BUTTON_VALUE = 241;  break;
//	case IDC_RIGHT_GRIPPER:		BUTTON_VALUE = 242;  break;

	case IDC_TURN90_CW:			BUTTON_VALUE =  228; break;
	case IDC_TURN90_CCW:		BUTTON_VALUE = 	229; break;
	case IDC_HOME_ALL:			BUTTON_VALUE =	221; break;
	case IDC_START_TRAY:		t_flag:: pass_product_tray = FALSE;  break;

	case IDC_START:				BUTTON_VALUE = (GET_CHECKER(hdlg,id)?506:507);		break;
	case IDC_CLEAR_PASS:		SM104_COUNT = 0,	SM103_COUNT = 0;	break;
	case IDC_CLEAR_NG:			SM104_NG_COUNT = 0, SM103_NG_COUNT = 0; break;
	case IDC_START_FEED:		t_flag::feed_product = TRUE;			break;
	case IDC_STOP:				
		HomeMachine_StopFlag = TRUE;
		Sleep(1000);
		SM101_motor_stop();
		SM102_motor_stop();
		SM103_motor_stop();
		SM104_motor_stop();
		SM105_motor_stop();
		EMERGENCY_STOP_FLAG = TRUE;

		SetDlgItemText(HWND_MOTION, IDC_STATUS, "Stop All Motors");
		ITEM_ENABLED(HWND_MOTION, IDC_HOME_ALL);
		break;
	case IDC_RESET2:			EMERGENCY_STOP_FLAG = FALSE;	break;
	}
}
void MotionDlg::close(HWND hdlg)
{
	EndDialog(hdlg, FALSE);
}
LRESULT MotionDlg::ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark)
{
	DeleteObject(static_brush);
	SelectObject(hdc, (HBRUSH)old_brush);
	static_brush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)/*BRUSH_LIGHTGRAY*/);
	old_brush = (HBRUSH)SelectObject(hdc, (HBRUSH)static_brush);

	char text[20] = {0};
	HFONT hfont = CreateFont(20, 0,0,0,20,0,0,0,ANSI_CHARSET,3,2,1, VARIABLE_PITCH | FF_ROMAN, "Arial");
	HFONT old_font = (HFONT)SelectObject(hdc, hfont);

	SetBkMode(hdc, TRANSPARENT);

	switch (GetDlgCtrlID(hcolor))
	{
	case IDC_PS_STATUS:
		DeleteObject(static_brush);
		SelectObject(hdc, (HBRUSH)old_brush);
		if (ps_link.is_linked == TRUE){
			static_brush = CreateSolidBrush(RGB(0x00, 0xff, 0x00));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0x0ff, 0x00, 0x00));
		}
		break;
	}
	return 0;
}
int MotionDlg::read_shm(char *path, MOTION_SHM *shm)
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

int MotionDlg::save_shm(char *path, MOTION_SHM *shm)
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
//======================一工位顶升=============================
int first_cylinder_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!FIRST_CYLINDER_DOWN_SENSOR && FIRST_CYLINDER_UP_SENSOR)
		{
			log("first cylinder already up");
			return 0;
		}
		FIRST_CYLINDER_UP;
		timeout = 300;
		while(timeout-- >0 && !FIRST_CYLINDER_UP_SENSOR)
			Sleep(10);

		if (!FIRST_CYLINDER_UP_SENSOR) return S08_OFF_ERR;
		break;
	case 2:
		if (FIRST_CYLINDER_DOWN_SENSOR && !FIRST_CYLINDER_UP_SENSOR)
		{
			log("first cylinder already down");
			return 0;
		}
		FIRST_CYLINDER_DOWN;
		timeout = 300;
		while(timeout-- >0 &&!FIRST_CYLINDER_DOWN_SENSOR )
			Sleep(10);
		if (!FIRST_CYLINDER_DOWN_SENSOR) return S07_OFF_ERR;
		break;
	}
	return 0;
}
//======================二工位顶升=============================
int second_cylinder_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;
	switch(i)
	{
	case 1:
		if (!SECOND_CYLINDER_DOWN_SENSOR && SECOND_CYLINDER_UP_SENSOR)
		{
			log("second cylinder already up");
			return 0;
		}
		SECOND_CYLINDER_UP;
 		timeout = 300;
 		while(timeout-- >0 && !SECOND_CYLINDER_UP_SENSOR)
 			Sleep(10);
 		if (!SECOND_CYLINDER_UP_SENSOR) return S02_OFF_ERR;
		break;
	case 2:
		if (SECOND_CYLINDER_DOWN_SENSOR && !SECOND_CYLINDER_UP_SENSOR)
		{
			log("second cylinder already down");
			return 0;
		}
		SECOND_CYLINDER_DOWN;
		timeout = 300;
		while(timeout-- >0 && !SECOND_CYLINDER_DOWN_SENSOR )
			Sleep(10);
 		if (!SECOND_CYLINDER_DOWN_SENSOR) return S01_OFF_ERR;
		break;
	}
	return 0;
}
//======================三工位顶升=============================
int third_cylinder_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;
	switch(i)
	{
	case 1:
		if (!THIRD_CYLINDER_DOWN_SENSOR && THIRD_CYLINDER_UP_SENSOR)
		{
			log("third cylinder already up");
			return 0;
		}
		THIRD_CYLINDER_UP;
		timeout = 300;
 		while(timeout-- >0 && !THIRD_CYLINDER_UP_SENSOR)
 			Sleep(10);
 		if (!THIRD_CYLINDER_UP_SENSOR) return S04_OFF_ERR;
		break;
	case 2:
		if (THIRD_CYLINDER_DOWN_SENSOR && !THIRD_CYLINDER_UP_SENSOR)
		{
			log("third cylinder already down");
			return 0;
		}
		THIRD_CYLINDER_DOWN;
 		timeout = 300;
 		while(timeout-- >0 &&!THIRD_CYLINDER_DOWN_SENSOR )
 			Sleep(10);
 		if (!THIRD_CYLINDER_DOWN_SENSOR) return S03_OFF_ERR;
		break;
	}
	return 0;
}
//======================四工位顶升=============================
int fourth_cylinder_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!FOURTH_CYLINDER_DOWN_SENSOR && FOURTH_CYLINDER_UP_SENSOR)
		{
			log("fourth cylinder already up");
			return 0;
		}
		FOURTH_CYLINDER_UP;
 		timeout = 300;
 		while(timeout-- >0 && !FOURTH_CYLINDER_UP_SENSOR)
 			Sleep(10);
 		if (!FOURTH_CYLINDER_UP_SENSOR) return S06_OFF_ERR;
		break;
	case 2:
		if (FOURTH_CYLINDER_DOWN_SENSOR && !FOURTH_CYLINDER_UP_SENSOR)
		{
			log("fourth cylinder already down");
			return 0;
		}
		FOURTH_CYLINDER_DOWN;
		timeout = 300;
		while(timeout-- >0 && !FOURTH_CYLINDER_DOWN_SENSOR)
			Sleep(10);
		if (!FOURTH_CYLINDER_DOWN_SENSOR) return S05_OFF_ERR;
		break;
	}
	return 0;
}
//======================暗盒测试=============================
int dark_box_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!DARK_BOX_UP_SENSOR && DARK_BOX_DOWN_SENSOR)
		{
			log("dark box already down");
			return 0;
		}
		DARK_BOX_DOWN;
 		timeout = 300;
 		while(timeout-- >0 && !DARK_BOX_DOWN_SENSOR)
 			Sleep(10);
 		if (!DARK_BOX_DOWN_SENSOR) return S14_OFF_ERR;
		break;
	case 2:
		if (DARK_BOX_UP_SENSOR && !DARK_BOX_DOWN_SENSOR)
		{
			log("dark box already up");
			return 0;
		}
		DARK_BOX_UP;
 		timeout = 300;
 		while(timeout-- >0 && !DARK_BOX_UP_SENSOR)
 			Sleep(10);
 		if (!DARK_BOX_UP_SENSOR) return S13_OFF_ERR;
		break;
	}
	return 0;
}
//======================光盒测试=============================
int light_box_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!LIGHT_BOX_UP_SENSOR && LIGHT_BOX_DOWN_SENSOR)
		{
			log("light box already down");
			return 0;
		}
		LIGHT_BOX_DOWN;
 		timeout = 300;
 		while(timeout-- >0 && !LIGHT_BOX_DOWN_SENSOR)
 			Sleep(10);
 		if (!LIGHT_BOX_DOWN_SENSOR) return S14_OFF_ERR;
		break;
	case 2:
		if (LIGHT_BOX_UP_SENSOR && !LIGHT_BOX_DOWN_SENSOR)
		{
			log("light box already up");
			return 0;
		}
		LIGHT_BOX_UP;
 		timeout = 300;
 		while(timeout-- >0 && !LIGHT_BOX_UP_SENSOR)
 			Sleep(10);
 		if (!LIGHT_BOX_UP_SENSOR) return S13_OFF_ERR;
		break;
	}
	return 0;
}
//====================夹爪移动气缸====================
int gripper_move_cfg(int i)
{
	if(SM105_MOVING == ON) return(SM105_MOVING_ERR);
	if(FEED_GRIPPER_DOWN_SENSOR == ON) return(FEED_GRIPPER_DOWN_ERR);
		int timeout;

	switch(i)
	{
	case 1:
		if (!FEED_GRIPPER_CYLINDER_RIGHT_SENSOR && FEED_GRIPPER_CYLINDER_LEFT_SENSOR)
		{
			log("feed gripper already left");
			return 0;
		}
		FEED_GRIPPER_MOVE_RIGHT_OFF;
		FEED_GRIPPER_MOVE_LEFT_ON;
 		timeout = 300;
 		while(timeout-- >0 &&(!FEED_GRIPPER_CYLINDER_LEFT_SENSOR || FEED_GRIPPER_CYLINDER_RIGHT_SENSOR))
 			Sleep(10);
 		if (FEED_GRIPPER_CYLINDER_RIGHT_SENSOR) return S19_STILL_ON_ERR;
 		if (!FEED_GRIPPER_CYLINDER_LEFT_SENSOR) return S20_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!FEED_GRIPPER_CYLINDER_LEFT_SENSOR && FEED_GRIPPER_CYLINDER_RIGHT_SENSOR)
		{
			log("feed gripper already right");
			return 0;
		}
		FEED_GRIPPER_MOVE_LEFT_OFF;
		FEED_GRIPPER_MOVE_RIGHT_ON;
 		timeout = 300;
 		while(timeout-- >0 &&(!FEED_GRIPPER_CYLINDER_RIGHT_SENSOR || FEED_GRIPPER_CYLINDER_LEFT_SENSOR))
 			Sleep(10);
 		if (FEED_GRIPPER_CYLINDER_LEFT_SENSOR) return S20_STILL_ON_ERR;
 		if (!FEED_GRIPPER_CYLINDER_RIGHT_SENSOR) return S19_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//===================入料气缸============================
int feed_cylinder_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);

	if(FEED_GRIPPER_DOWN_SENSOR == ON) return(FEED_GRIPPER_DOWN_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!FEED_CYLINDER_LEFT_SENSOR && FEED_CYLINDER_RIGHT_SENSOR)
		{
			log("product cylinder already right");
			return 0;
		}
		FEED_CYLINDER_LEFT_OFF;
		FEED_CYLINDER_RIGHT_ON;
		timeout = 300;
		while(timeout-- > 0 && (!FEED_CYLINDER_RIGHT_SENSOR || FEED_CYLINDER_LEFT_SENSOR))
			Sleep(10);
		if (FEED_CYLINDER_LEFT_SENSOR) return S17_STILL_ON_ERR;
		if (!FEED_CYLINDER_RIGHT_SENSOR) return S18_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!FEED_CYLINDER_RIGHT_SENSOR && FEED_CYLINDER_LEFT_SENSOR)
		{
			log("product cylinder already left");
			return 0;
		}
		FEED_CYLINDER_RIGHT_OFF;
		FEED_CYLINDER_LEFT_ON;
		timeout = 300;
		while(timeout-- > 0 && (!FEED_CYLINDER_LEFT_SENSOR || FEED_CYLINDER_RIGHT_SENSOR))
			Sleep(10);
		if (FEED_CYLINDER_RIGHT_SENSOR) return S18_STILL_ON_ERR;
		if (!FEED_CYLINDER_LEFT_SENSOR) return S17_OFF_ERR;
		return 0;
		break;
	
	}
	return 0;
}
//===================物料输送气缸============================
int product_cylinder_cfg(int i)
{
	if(FEED_GRIPPER_DOWN_SENSOR == ON) return(FEED_GRIPPER_DOWN_ERR);
	int timeout;

	switch(i)
	{
	case 1:
		if (!PRODUCT_CYLINDER_LEFT_SENSOR && PRODUCT_CYLINDER_RIGHT_SENSOR)
		{
			log("product cylinder already left");
			return 0;
		}
		PRODUCT_CYLINDER_RIGHT;
		timeout = 300;
		while(timeout-- > 0 && !PRODUCT_CYLINDER_RIGHT_SENSOR)
			Sleep(10);
		if (!PRODUCT_CYLINDER_RIGHT_SENSOR) return PRODUCT_CYLINDER_ERR;
		return 0;
		break;
	case 2:
		if (!PRODUCT_CYLINDER_RIGHT_SENSOR && PRODUCT_CYLINDER_LEFT_SENSOR)
		{
			log("product cylinder already right");
			return 0;
		}
		PRODUCT_CYLINDER_LEFT;
		timeout = 300;
		while(timeout-- > 0 && !PRODUCT_CYLINDER_LEFT_SENSOR)
			Sleep(10);
		if (!PRODUCT_CYLINDER_LEFT_SENSOR) return PRODUCT_CYLINDER_ERR;
		return 0;
		break;
	}
	return 0;
}
//==================入料夹爪上下气缸=============================
int gripper_support_cfg(int i)
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout;
	switch(i)
	{
	case 1:
		if (!FEED_GRIPPER_UP_SENSOR && FEED_GRIPPER_DOWN_SENSOR)
		{
			log("feed gripper already down");
			return 0;
		}
		FEED_GRIPPER_UP_OFF;
		FEED_GRIPPER_DOWN_ON;
		timeout = 300;
		while(timeout-- >0 &&(FEED_GRIPPER_UP_SENSOR || !FEED_GRIPPER_DOWN_SENSOR))
			Sleep(10);
		if (FEED_GRIPPER_UP_SENSOR) return S21_STILL_ON_ERR;
		if (!FEED_GRIPPER_DOWN_SENSOR) return S22_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!FEED_GRIPPER_DOWN_SENSOR && FEED_GRIPPER_UP_SENSOR)
		{
			log("feed gripper already up");
			return 0;
		}
		FEED_GRIPPER_DOWN_OFF;
		FEED_GRIPPER_UP_ON;
		timeout = 300;
		while(timeout-- >0 &&(FEED_GRIPPER_DOWN_SENSOR || !FEED_GRIPPER_UP_SENSOR))
			Sleep(10);
		if (FEED_GRIPPER_DOWN_SENSOR) return S22_STILL_ON_ERR;
		if (!FEED_GRIPPER_UP_SENSOR) return S21_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//=======================入料夹爪气缸===================
int gripper_cylinder_cfg(int i)//
{
	if (SM105_MOVING == ON) return(SM105_MOVING_ERR);
	int timeout = 0;
	switch(i)
	{
	case 1:
// 		if ((FEED_LEFT_GRIPPER_CLOSE_SENSOR && FEED_RIGHT_GRIPPER_CLOSE_SENSOR) &&
// 			(!FEED_LEFT_GRIPPER_OPEN_SENSOR && !FEED_RIGHT_GRIPPER_OPEN_SENSOR))
// 		{
// 			log("feed all gripper close already");
// 			return 0;
// 		}
		FEED_GRIPPER_CYLINDER_OPEN_OFF;
		FEED_GRIPPER_CYLINDER_CLOSE_ON;
		timeout = 100;
		while(timeout-- > 0 && (!FEED_LEFT_GRIPPER_CLOSE_SENSOR || !FEED_RIGHT_GRIPPER_CLOSE_SENSOR ||
			FEED_LEFT_GRIPPER_OPEN_SENSOR || FEED_RIGHT_GRIPPER_OPEN_SENSOR))
			{
				Sleep(10);
		}
		if (!FEED_LEFT_GRIPPER_CLOSE_SENSOR ) return FEED_LEFT_GRIPPER_NOTHING_ERR;
		if (!FEED_RIGHT_GRIPPER_CLOSE_SENSOR ) return  FEED_RIGHT_GRIPPER_NOTHING_ERR;
		
		return 0;
		break;
	case 2:
// 		if ((!FEED_LEFT_GRIPPER_CLOSE_SENSOR && !FEED_RIGHT_GRIPPER_CLOSE_SENSOR) &&
// 			(FEED_LEFT_GRIPPER_OPEN_SENSOR && FEED_RIGHT_GRIPPER_OPEN_SENSOR))
// 		{
// 			log("feed all gripper open already");
// 			return 0;
// 		}
		FEED_GRIPPER_CYLINDER_CLOSE_OFF;
		FEED_GRIPPER_CYLINDER_OPEN_ON;	
		timeout = 100;
		while(timeout-- > 0 && ((FEED_LEFT_GRIPPER_CLOSE_SENSOR || FEED_RIGHT_GRIPPER_CLOSE_SENSOR) ||
			(!FEED_LEFT_GRIPPER_OPEN_SENSOR || !FEED_RIGHT_GRIPPER_OPEN_SENSOR)))
			Sleep(10);
		if (!FEED_LEFT_GRIPPER_OPEN_SENSOR || !FEED_RIGHT_GRIPPER_OPEN_SENSOR) return FEED_GRIPPER_OPEN_ERR;
		return 0;
		break;
	case 3://left no product
// 		if (FEED_RIGHT_GRIPPER_CLOSE_SENSOR && !FEED_RIGHT_GRIPPER_OPEN_SENSOR)
// 		{
// 			log("feed right gripper close already");
// 			return 0;
// 		}
		FEED_GRIPPER_CYLINDER_OPEN_OFF;
		FEED_GRIPPER_CYLINDER_CLOSE_ON;
		timeout = 100;
		while(timeout-- > 0 && (!FEED_RIGHT_GRIPPER_CLOSE_SENSOR || FEED_RIGHT_GRIPPER_OPEN_SENSOR))
			Sleep(10);
		if (!FEED_RIGHT_GRIPPER_CLOSE_SENSOR ) return  FEED_RIGHT_GRIPPER_NOTHING_ERR;
		return 0;
		break;
	case 4://right no product
// 		if (FEED_RIGHT_GRIPPER_CLOSE_SENSOR && !FEED_RIGHT_GRIPPER_OPEN_SENSOR)
// 		{
// 			log("feed left gripper close already");
// 			return 0;
// 		}
		FEED_GRIPPER_CYLINDER_OPEN_OFF;
		FEED_GRIPPER_CYLINDER_CLOSE_ON;
		timeout = 100;
		while(timeout-- > 0 && (!FEED_LEFT_GRIPPER_CLOSE_SENSOR || FEED_LEFT_GRIPPER_OPEN_SENSOR))
			Sleep(10);
		if (!FEED_LEFT_GRIPPER_CLOSE_SENSOR ) return FEED_LEFT_GRIPPER_NOTHING_ERR;
		return 0;
		break;
	}	
	return 0;
}
//======================产品夹爪升降=========================
int product_gripper_support_cfg(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!PRODUCT_GRIPPER_UP_SENSOR && PRODUCT_GRIPPER_DOWN_SENSOR)
		{
			log("product gripper already down");
			return 0;
		}
		PRODUCT_GRIPPER_DOWN;
		timeout = 300;
		while(timeout-- > 0 && !PRODUCT_GRIPPER_DOWN_SENSOR)
			Sleep(10);
		if (!PRODUCT_GRIPPER_DOWN_SENSOR) return S34_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!PRODUCT_GRIPPER_DOWN_SENSOR && PRODUCT_GRIPPER_UP_SENSOR)
		{
			log("product gripper already down");
			return 0;
		}
		PRODUCT_GRIPPER_UP;
		timeout = 300;
		while(timeout-- > 0 && !PRODUCT_GRIPPER_UP_SENSOR)
			Sleep(10);
		if (!PRODUCT_GRIPPER_UP_SENSOR) return S33_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//=======================产品夹爪张闭========================
int product_gripper_cfg(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!PRODUCT_GRIPPER_OPEN_SENSOR && PRODUCT_GRIPPER_CLOSE_SENSOR)
		{
			log("product gripper already close");
			return 0;
		}
		PRODUCT_GRIPPER_OPEN_OFF;
		PRODUCT_GRIPPER_CLOSE_ON;
		Sleep(50);
		timeout = 300;
		while(timeout-- > 0 && (!PRODUCT_GRIPPER_CLOSE_SENSOR || PRODUCT_GRIPPER_OPEN_SENSOR))
			Sleep(10);
		if (PRODUCT_GRIPPER_OPEN_SENSOR) return S25_STILL_ON_ERR;
		if (!PRODUCT_GRIPPER_CLOSE_SENSOR) return S26_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!PRODUCT_GRIPPER_CLOSE_SENSOR && PRODUCT_GRIPPER_OPEN_SENSOR)
		{
			log("product gripper already open");
			return 0;
		}
		PRODUCT_GRIPPER_CLOSE_OFF;
		PRODUCT_GRIPPER_OPEN_ON;
		Sleep(50);
		timeout = 300;
		while(timeout-- > 0 && (!PRODUCT_GRIPPER_OPEN_SENSOR || PRODUCT_GRIPPER_CLOSE_SENSOR))
			Sleep(10);
		if (PRODUCT_GRIPPER_CLOSE_SENSOR) return S26_STILL_ON_ERR;
		if (!PRODUCT_GRIPPER_OPEN_SENSOR) return S25_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//=======================料盒顶升====================
int pass_tray_cylinder(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!PRODUCT_TRAY_DOWN_SENSOR && PRODUCT_TRAY_UP_SENSOR)
		{
			log("product tray already up");
			return 0;
		}
		PRODUCT_TRAY_DOWN_OFF;
		PRODUCT_TRAY_UP_ON;
		timeout = 300;
		while(timeout-- > 0 && (!PRODUCT_TRAY_UP_SENSOR || PRODUCT_TRAY_DOWN_SENSOR))
			Sleep(10);
		if (PRODUCT_TRAY_DOWN_SENSOR) return S72_STILL_ON_ERR;
		if (!PRODUCT_TRAY_UP_SENSOR) return S71_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!PRODUCT_TRAY_UP_SENSOR && PRODUCT_TRAY_DOWN_SENSOR)
		{
			log("product tray already down");
			return 0;
		}
		PRODUCT_TRAY_UP_OFF;
		PRODUCT_TRAY_DOWN_ON;

		timeout = 300;
		while(timeout-- > 0 && (!PRODUCT_TRAY_DOWN_SENSOR || PRODUCT_TRAY_UP_SENSOR))
			Sleep(10);
		if (PRODUCT_TRAY_UP_SENSOR)	   return S71_STILL_ON_ERR;
		if (!PRODUCT_TRAY_DOWN_SENSOR) return S72_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//=======================堆栈顶升====================
int stack_cylinder(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!STACK_CYLINDER_DOWN_SENSOR && STACK_CYLINDER_UP_SENSOR)
		{
			log("stack cylinder already up");
			return 0;
		}
		STACK_CYLINDER_UP;
		timeout = 300;
		while(timeout-- > 0 && !STACK_CYLINDER_UP_SENSOR)
			Sleep(10);
		if (!STACK_CYLINDER_UP_SENSOR) return S46_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!STACK_CYLINDER_UP_SENSOR && STACK_CYLINDER_DOWN_SENSOR)
		{
			log("stack cylinder already down");
			return 0;
		}
		STACK_CYLINDER_DOWN;
		timeout = 300;
		while(timeout-- > 0 && !STACK_CYLINDER_DOWN_SENSOR)
			Sleep(10);
		if (!STACK_CYLINDER_DOWN_SENSOR) return S45_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
//=====================新托盘阻断====================
int tray_block_cylinder(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!STACK_BLOCK_LEFT_OPEN_SENSOR && !STACK_BLOCK_RIGHT_OPEN_SENSOR && STACK_BLOCK_LEFT_CLOSE_SENSOR && STACK_BLOCK_RIGHT_CLOSE_SENSOR)
		{
			log("stack block already close");
			return 0;
		}
		STACK_BLOCK_CLOSE;
		timeout = 300;
		while(timeout-- > 0 && (STACK_BLOCK_LEFT_OPEN_SENSOR || STACK_BLOCK_RIGHT_OPEN_SENSOR || !STACK_BLOCK_LEFT_CLOSE_SENSOR || !STACK_BLOCK_RIGHT_CLOSE_SENSOR))
			Sleep(10);
		if (STACK_BLOCK_LEFT_OPEN_SENSOR)	 return S41_STILL_ON_ERR;
		if (STACK_BLOCK_RIGHT_OPEN_SENSOR)	 return S43_STILL_ON_ERR;
		if (!STACK_BLOCK_LEFT_CLOSE_SENSOR)  return S42_OFF_ERR;
		if (!STACK_BLOCK_RIGHT_CLOSE_SENSOR) return S44_OFF_ERR;

		return 0;
		break;
	case 2:
		if (STACK_BLOCK_LEFT_OPEN_SENSOR && STACK_BLOCK_RIGHT_OPEN_SENSOR && !STACK_BLOCK_LEFT_CLOSE_SENSOR && !STACK_BLOCK_RIGHT_CLOSE_SENSOR)
		{
			log("stack block already open");
			return 0;
		}
		STACK_BLOCK_OPEN;
		timeout = 300;
		while(timeout-- > 0 && (!STACK_BLOCK_LEFT_OPEN_SENSOR || !STACK_BLOCK_RIGHT_OPEN_SENSOR || STACK_BLOCK_LEFT_CLOSE_SENSOR || STACK_BLOCK_RIGHT_CLOSE_SENSOR))
			Sleep(10);
		if (!STACK_BLOCK_LEFT_OPEN_SENSOR)  return S41_OFF_ERR;
		if (!STACK_BLOCK_RIGHT_OPEN_SENSOR) return S43_OFF_ERR;
		if (STACK_BLOCK_LEFT_CLOSE_SENSOR)  return S42_STILL_ON_ERR;
		if (STACK_BLOCK_RIGHT_CLOSE_SENSOR) return S44_STILL_ON_ERR;
		return 0;
		break;
	}
	return 0;
}
//=====================NG料盒========================
int ng_tray_cylinder(int i)
{
	int timeout;
	switch(i)
	{
	case 1:
		if (!NG_TRAY_CYLINDER_FORWARD_SENSOR && NG_TRAY_CYLINDER_REVERSE_SENSOR)
		{
			log("ng tray already reverse");
			return 0;
		}
		NG_TRAY_FORWARD_OFF;
		NG_TRAY_REVERSE_ON;
		timeout = 300;
		while(timeout-- > 0 && (!NG_TRAY_CYLINDER_REVERSE_SENSOR || NG_TRAY_CYLINDER_FORWARD_SENSOR))
			Sleep(10);
		if (NG_TRAY_CYLINDER_FORWARD_SENSOR) return S39_STILL_ON_ERR;
		if (!NG_TRAY_CYLINDER_REVERSE_SENSOR) return S40_OFF_ERR;
		return 0;
		break;
	case 2:
		if (!NG_TRAY_CYLINDER_REVERSE_SENSOR && NG_TRAY_CYLINDER_FORWARD_SENSOR)
		{
			log("ng tray already forward");
			return 0;
		}
		
		NG_TRAY_REVERSE_OFF;
		NG_TRAY_FORWARD_ON;
		timeout = 300;
		while(timeout-- > 0 && (!NG_TRAY_CYLINDER_FORWARD_SENSOR || NG_TRAY_CYLINDER_REVERSE_SENSOR))
			Sleep(10);
		if (NG_TRAY_CYLINDER_REVERSE_SENSOR)  return S40_STILL_ON_ERR;
		if (!NG_TRAY_CYLINDER_FORWARD_SENSOR) return S39_OFF_ERR;
		return 0;
		break;
	}
	return 0;
}
int left_gripper_motion(void)
{	
	int errcode;
	if (errcode = gripper_move_cfg(1) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}

	if (errcode = first_cylinder_cfg(1) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = gripper_support_cfg(1) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = gripper_cylinder_cfg(4) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	Sleep(500);
	if (errcode = gripper_support_cfg(2) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = gripper_move_cfg(2) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = gripper_support_cfg(1) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if(errcode = gripper_cylinder_cfg(2) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = gripper_support_cfg(2) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	if (errcode = first_cylinder_cfg(2) != SUCCESS)
	{
		log("left_gripper_motion_err=%d",errcode);
	}
	return 0;
}
int all_gripper_motion(void)
{	
	int errcode;
	if (errcode = gripper_move_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = first_cylinder_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = gripper_cylinder_cfg(1) != SUCCESS)
		return errcode;
	log("gripper_cylinder ok");
	Sleep(500);
	if (errcode = gripper_support_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_move_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(1) != SUCCESS)
		return errcode;
	if(errcode = gripper_cylinder_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = first_cylinder_cfg(2) != SUCCESS)
		return errcode;
	return 0;
}
int right_gripper_motion(void)
{	
	int errcode;
	
	if (errcode = gripper_move_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = first_cylinder_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(1) != SUCCESS)
		return errcode;
	if (errcode = gripper_cylinder_cfg(3) != SUCCESS)
		return errcode;
	Sleep(500);
	if (errcode = gripper_support_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_move_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(1) != SUCCESS)
		return errcode;
	if(errcode = gripper_cylinder_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = gripper_support_cfg(2) != SUCCESS)
		return errcode;
	if (errcode = first_cylinder_cfg(2) != SUCCESS)
		return errcode;
	return 0;
}
//=======================电源=======================
DWORD WINAPI power_supply_link_thread(LPVOID temp)
{
	char cmd[20] = {0};
	char feedback[100] = {0};
	float current = 0.0f;
	while (1){
		if (ps_link.is_linked != TRUE){
			ps_link.is_linked = ps_link.inf.com_open(ps_link.port, ps_link.baudrate);
			log("step 1");
			if (ps_link.is_linked == TRUE){
				InvalidateRect(GetDlgItem(HWND_MOTION, IDC_PS_STATUS), NULL, TRUE);
				UpdateWindow(GetDlgItem(HWND_MOTION, IDC_PS_STATUS));

				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_VOL_CH1, ps_link.voltage_val[0], 3);
				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_VOL_CH2, ps_link.voltage_val[1], 3);
				//ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_VOL_CH3, ps_link.voltage_val[2], 3);

				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_CH1, ps_link.current_val[0], 3);
				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_CH2, ps_link.current_val[1], 3);
				//ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_CH3, ps_link.current_val[2], 3);

				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT1, ps_link.current_limit[0], 3);
				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT2, ps_link.current_limit[1], 3);
				//ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_LMT3, ps_link.current_limit[2], 3);

				ps_link.inf.send_data((BYTE *)"SYST:REM\n", 9); //Get control of ps

				ps_link.inf.send_data((BYTE *)"INST CH1\n", 9); //select ch1
				sprintf(cmd, "VOLT %.2f\n", ps_link.voltage_val[0]);
				ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				sprintf(cmd, "CURR %.3f\n", ps_link.current_limit[0]);
				ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				ps_link.inf.send_data((BYTE *)"CHAN:OUTP 1\n", 12);

				ps_link.inf.send_data((BYTE *)"INST CH2\n", 9); //select ch2
				sprintf(cmd, "VOLT %.2f\n", ps_link.voltage_val[1]);
				ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				sprintf(cmd, "CURR %.3f\n", ps_link.current_limit[1]);
				ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				ps_link.inf.send_data((BYTE *)"CHAN:OUTP 1\n", 12);

				//ps_link.inf.send_data((BYTE *)"INST CH3\n", 9); //select ch3
				//sprintf(cmd, "VOLT %.2f\n", ps_link.voltage_val[2]);
				//ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				//sprintf(cmd, "CURR %.3f\n", ps_link.current_limit[2]);
				//ps_link.inf.send_data((BYTE *)cmd, strlen(cmd));
				//ps_link.inf.send_data((BYTE *)"CHAN:OUTP 1\n", 12);
			}
		}
		else{
			if (COMM_DISLPAY == ON ){
				ps_link.inf.send_data((BYTE *)"INST CH1\n", 9); //select ch1
				ps_link.inf.send_data_feedback((BYTE *)"MEAS:CURR?\n", 11, (BYTE *)feedback, sizeof(feedback));
				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_CH1, atof(feedback), 3);

				ps_link.inf.send_data((BYTE *)"INST CH2\n", 9); //select ch2
				ps_link.inf.send_data_feedback((BYTE *)"MEAS:CURR?\n", 11, (BYTE *)feedback, sizeof(feedback));
				ai_win::set_dlg_item_float(HWND_MOTION, IDC_PS_CUR_CH2, atof(feedback), 3);
			}
		}
		Sleep(100);
	}
	return 0;
}
//=================转盘运动=====================
DWORD CALLBACK Table_task_position(LPVOID temp){
	HWND hdlg = (HWND) temp;
	int step = 0;
	int err = 0;
	while(1){
		switch(step)
		{
			case 0:
				if (((t_flag::site1_task_product == FALSE)||(t_flag::site1_up_product == TRUE)) && ((t_flag::site2_task_product == FALSE)||(t_flag::site2_test_end == TRUE))
				   && ((t_flag::site3_task_product == FALSE)||(t_flag::site3_test_end == TRUE)) && ((t_flag::site4_task_product == FALSE)||(t_flag::site4_test_end == TRUE)))
				{
					step = 1;
					break;
				}
				Sleep(50);
				break;
			case 1:
				if ((t_flag::site1_task_product == FALSE)&&(t_flag::site2_task_product == FALSE)&&(t_flag::site3_task_product == FALSE)&&(t_flag::site4_task_product == FALSE))
				{
					step = 0;
					Sleep(500);
					//log("no product table");
					break;
				}
				if(t_flag::feed_griper_staus == TRUE)
				{
					step = 0;
					Sleep(500);
					//log("feed grriper motion table");
					break;
				}
				step = 2;
				break;	
			case 2:
				SM105_MOVING = ON;
				t_flag::table_staus_safe = FALSE;
				if (err= SM105_relative_short_move(90*SM105_PositionRatio)!= SUCCESS)
				{				
					log("table turn 90 false err=%d",err);
					Sleep(500);
					break;
				}
				SM105_MOVING = OFF;
				log("table turn 90");
				if (t_flag::site1_task_product == FALSE)
				{
					t_flag::site2_task_product = FALSE;
				}
				t_flag::table_task2_staus = TRUE;	
				t_flag::table_task3_staus = TRUE;	
				t_flag::table_task4_staus = TRUE;		
				t_flag::table_gripper_staus = TRUE;
				t_flag::table_staus_safe = TRUE;
				Sleep(100);
				step = 0;
			break;	
		}
	}
	return 0;
}
//=================入料口=======================
DWORD CALLBACK feed_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	HWND hdlg = (HWND) temp;
	while(1)
	{
		switch(step)
		{
		case 0:
			if(t_flag::feed_product == TRUE) 
			{
				step = 1;
				t_flag::feed_product = FALSE;
				t_flag::feed_left_staus = FALSE;
				t_flag::feed_move_start = TRUE;
				break;
			}
			Sleep(50);
			break;
		case 1:		
			if ((t_flag::table_staus_safe == TRUE)&& (t_flag::feed_griper_staus = FALSE))
			{
				step = 2;
				break;
			}
			Sleep(10);
			break;
		case 2:
			Sleep(500);
			if (err = feed_cylinder_cfg(1) != SUCCESS)
			{
				log("feed cylinder right err=%d",err);
				step = 2;
				Sleep(5000);
				break;
			}
			log("feed cylinder right");
			t_flag::feed_right_staus = TRUE;
			t_flag::table_staus_safe = FALSE;
			t_flag::feed_move_start = FALSE;
			step = 3;
			break;
		case 3:
			if (t_flag::site1_up_product == TRUE)
			{	
				t_flag::feed_product = FALSE;
				t_flag::feed_right_staus = FALSE;
				step = 4;
				break;
			}
			Sleep(50);
			break;
		case 4:
			if ((t_flag::table_staus_safe == TRUE) && (t_flag::feed_request_product == TRUE))
			{
				step = 5;
				break;
			}
			Sleep(50);
			break;
		case 5:
			Sleep(500);
			if (err = feed_cylinder_cfg(2) != SUCCESS)
			{
				log("feed cylinder left err=%d",err);
				Sleep(5000);
				step = 4;
				break;
			}
			t_flag::feed_left_staus = TRUE;
			log("feed cylinder left");
// 			log("t_flag::site1_task_product=%d,t_flag::site1_up_product=%d,t_flag::site2_task_product=%d,t_flag::site2_test_end=%d",t_flag::site1_task_product,t_flag::site1_up_product,t_flag::site2_task_product,t_flag::site2_test_end);
// 			log("t_flag::site3_task_product=%d,t_flag::site3_test_end=%d,t_flag::site4_task_product=%d,t_flag::site4_test_end=%d",t_flag::site3_task_product,t_flag::site3_test_end,t_flag::site4_task_product,t_flag::site4_test_end);
			step = 0;
			break;
		}
	}
}
//============二工位站========================
DWORD CALLBACK site2_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	HWND hdlg = (HWND) temp;
	while(1)
	{
		switch(step)
		{
		case 0:
			if ((t_flag::site2_task_product == TRUE) && (t_flag::site2_test_start == TRUE))
			{
				t_flag::site2_test_end=FALSE;
				t_flag::site2_test_start = FALSE;
				step = 1;
				log("site 2 step 1");
				break;
			}
			else if (t_flag::site2_task_product == FALSE)
			{
				if (t_flag::table_task2_staus == TRUE)
				{
					t_flag::table_task2_staus = FALSE;
					step=5;
					log("site 2 step 5");
					break;
				}
			}
			Sleep(50);
			break;
		case 1:
			Sleep(5000);						//二工位检测
			log("site2 test");
			step = 2;
			break;
		case 2:
			t_flag::site2_test_end=TRUE;
			t_flag::table_task2_staus = FALSE;
			step = 3;
			break;
		case 3:
			if (t_flag::table_task2_staus == TRUE)
			{
				t_flag::table_task2_staus = FALSE;
				t_flag::site2_task_product = FALSE;
				step = 4;
			}
			Sleep(50);
			break;
		case 4:		
			t_flag::site2_test_end= FALSE;
			t_flag::site3_task_product = TRUE;
			t_flag::site3_test_start = TRUE;
			step = 0;
			break;
		case 5:	
			t_flag::site3_task_product = FALSE;
			step = 0;
			break;
		}
	}
}
//============三工位站========================
DWORD CALLBACK site3_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	HWND hdlg = (HWND) temp;
	while(1)
	{
		switch(step)
		{
		case 0:
			if ((t_flag::site3_task_product == TRUE) && (t_flag::site3_test_start == TRUE))
			{
				t_flag::site3_test_end = FALSE;
				t_flag::site3_test_start = FALSE;
				step = 1;
				log("site 3 step 1");
				break;
			}
			else if (t_flag::site3_task_product == FALSE)
			{
				if (t_flag::table_task3_staus == TRUE)
				{
					t_flag::table_task3_staus = FALSE;
					step = 6;
					log("site 3 step 6");
					break;
				}
			}
			Sleep(50);
			break;
		case 1:
			if(t_flag::site2_task_product == FALSE)
			{
				step=2;
				log("site2 no product");
				break;
			}
			if (t_flag::site2_test_end == TRUE)
			step = 2;
			Sleep(50);
			break;
		case 2:
			if (err = site3_test() != SUCCESS)		//三工位检测
			{
				Sleep(5000);
				log("site3 test err= %d",err);
				break;
			}									
			log("site3 test");
			step = 3;
			break;
		case 3:
			t_flag::site3_test_end=TRUE;
			t_flag::table_task3_staus = FALSE;
			Sleep(20);
			step = 4;
			break;
		case 4:
			if (t_flag::table_task3_staus == TRUE)
			{
				t_flag::table_task3_staus = FALSE;
				t_flag::site3_task_product = FALSE;
				step = 5;
				break;
			}
			Sleep(50);
			break;
		case 5:		
			t_flag::site3_test_end = FALSE;
			t_flag::site4_task_product = TRUE;
			t_flag::site4_test_start = TRUE;
			step = 0;
			break;
		case 6:
			t_flag::site4_task_product = FALSE;
			step = 0;
		}
	}
}
//============四工位站========================
DWORD CALLBACK site4_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	HWND hdlg = (HWND) temp;
	while(1)
	{
		switch(step)
		{
		case 0:
			if ((t_flag::site4_task_product == TRUE) && (t_flag::site4_test_start == TRUE))
			{
				t_flag::site4_test_start = FALSE;
				t_flag::site4_test_end=FALSE;
				step = 1;
				log("site 4 step 1");
				break;
			}
			else if (t_flag::site4_task_product == FALSE)
			{
				if (t_flag::table_task4_staus == TRUE)
				{
					t_flag::table_task4_staus = FALSE;
					step=6;
					log("site 4 step 6");
					break;
				}
			}
			Sleep(50);
			break;
		case 1:
			if(t_flag::site2_task_product == TRUE)
			{
				if (t_flag::site2_test_end == TRUE)
				{
					if(t_flag::site3_task_product == TRUE)
					{
						if (t_flag::site3_test_end == TRUE)
						{
							step=2;
							break;
						}
					}
					if(t_flag::site3_task_product == FALSE)
					{		
						step=2;
						break;
					}
				}
			}
			
			else if((t_flag::site2_task_product == FALSE) && (t_flag::site3_task_product == FALSE))
			{
				step = 2;
				break;
			}
			else if((t_flag::site2_task_product == FALSE) && (t_flag::site3_task_product == TRUE))
			{
				if (t_flag::site3_test_end == TRUE)
				{
					step=2;
					break;
				}
			}
			Sleep(50);
			break;
		case 2:
			log("site4 test");
			Sleep(5000);						//四工位检测
			step = 3;
			break;
		case 3:
			t_flag::site4_test_end=TRUE;
			t_flag::table_task4_staus = FALSE;			
			step = 4;
			break;
		case 4:
			if (t_flag::table_task4_staus == TRUE)
			{
				t_flag::table_task4_staus = FALSE;
				t_flag::site4_task_product = FALSE;
				step = 5;
			}
			Sleep(10);
			break;
		case 5:
			Sleep(50);
			t_flag::site4_test_end = FALSE;
			t_flag::site1_task_product = TRUE;
			t_flag::site1_down_product = TRUE;
			step = 0;
			break;
		case 6:
			t_flag::site1_task_product = FALSE;
			t_flag::site1_down_product = FALSE;
			step = 0;
			break;
		}
	}
}
//=============夹爪相关==========================
DWORD CALLBACK feed_gripper_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	while(1)
	{
		switch(step)
		{
		case 0:
					
			if (t_flag::site1_down_product == TRUE)
			{
				if(t_flag::carry_product_left_wait == TRUE)
					if (t_flag::feed_move_start == TRUE)
					{
						while (t_flag::feed_right_staus == FALSE)
						{
							Sleep(10);
						}
						step = 1;
						t_flag::feed_right_staus = FALSE;
						t_flag::site1_down_product = FALSE;
						t_flag::feed_griper_staus = TRUE;
						log("all gripper step1");
						break;
					}
			}

			if (t_flag::feed_right_staus == TRUE)
			{
				Sleep(100);
				if (t_flag::site1_down_product == FALSE)
				{
					t_flag::feed_right_staus = FALSE;				
					t_flag::feed_griper_staus = TRUE;
					step = 14;
					log("left gripper step14");
					break;
				}
			}
			
			if (t_flag::feed_move_start == FALSE)
			{
				if (t_flag::site1_down_product == TRUE)
				{
					if (t_flag::carry_product_left_wait == TRUE)
					{
						t_flag::carry_product_left_wait = FALSE;
						t_flag::site1_down_product = FALSE;
						t_flag::feed_griper_staus = TRUE;
						step = 27;
						log("right gripper step 27");
						break;
					}
				}
			}
			Sleep(50);
			break;
		case 1:
			if (err = gripper_move_cfg(1) != SUCCESS)
			{
				log("gripper move left err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper move left 1");
			step= 2;
			break;
		case 2:
			if (err = first_cylinder_cfg(1) != SUCCESS)
			{
				log("first cylinder up err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder up 2");
			step = 3;
			break;
		case 3:
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 3");
			step = 4;
			break;
		case 4:
			if (err = gripper_cylinder_cfg(1) != SUCCESS)
			{
				log("all gripper cylinder close err=%d",err);
				Sleep(5000);
				break;
			}
			Sleep(500);
			log("all gripper cylinder close 4");
			step = 5;
			break;
		case 5:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 5");
			step = 6;
			break;
		case 6:
			if (err = gripper_move_cfg(2) != SUCCESS)
			{
				log("gripper move right err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper move right 6");
			step = 7;
			break;
		case 7:
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 7");
			step = 8;
			break;
		case 8:
			if(err = gripper_cylinder_cfg(2) != SUCCESS)
			{
				log("gripper cylinder open err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper cylinder open 8");
			step = 9;
			break;
		case 9:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 9");
			step = 10;
			break;
		case 10:
			if (err = first_cylinder_cfg(2) != SUCCESS)
			{
				log("first cylinder down err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder down 10");
			step = 11;
			break;
		case 11:
			t_flag::feed_griper_staus = FALSE;
			t_flag::site1_up_product = TRUE;
			t_flag::site1_task_product = TRUE;
			t_flag::product_carry_left = TRUE;
			t_flag::table_gripper_staus = FALSE;
			Sleep(100);
			step = 12;
			break;
		case 12:
			if (t_flag::table_gripper_staus == TRUE)
			{
				t_flag::site1_task_product = FALSE;
				t_flag::site1_up_product = FALSE;
				t_flag::table_gripper_staus = FALSE;
				t_flag::site2_task_product = TRUE;
				t_flag::site2_test_start = TRUE;
				step = 0;
				
			}
			Sleep(50);
			break;
		case 14:
			if (err = gripper_move_cfg(1) != SUCCESS)
			{
				log("left gripper move left err=%d",err);
				Sleep(5000);
				break;
			}
			log("left gripper move left 14");
			step = 15;
			break;
		case 15:
			if (err = first_cylinder_cfg(1) != SUCCESS)
			{
				log("first cylinder up err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder up 15");
			step = 16;
			break;
		case 16:
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 16");
			step = 17;
			break;
		case 17:
			if (err = gripper_cylinder_cfg(4) != SUCCESS)
			{
				log("left gripper close err=%d",err);
				Sleep(5000);
				break;
			}
			Sleep(300);
			log("left gripper close 17");
			step = 19;
			break;
		case 19:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 19");
			step = 20;
			break;
		case 20:
			if (err = gripper_move_cfg(2) != SUCCESS)
			{
				log("left gripper move right err=%d",err);
				Sleep(5000);
				break;
			}
			log("left gripper move right 20");
			step = 21;
			break;
		case 21:	
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 21");
			step = 22;
			break;
		case 22:
			if(err = gripper_cylinder_cfg(2) != SUCCESS)
			{
				log("left gripper open err=%d",err);
				Sleep(5000);
				break;
			}
			log("left gripper open 22");
			step = 23;
			break;
		case 23:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 23");
			step = 24;
			break;
		case 24:
			if (err = first_cylinder_cfg(2) != SUCCESS)
			{
				log("first cylinder down err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder down 24");
			step = 25;
			break;
		case 25:
			t_flag::feed_griper_staus = FALSE;
			t_flag::site1_up_product = TRUE;
			t_flag::site1_task_product = TRUE;
			t_flag::table_gripper_staus = FALSE;
			Sleep(100);
			step = 26;
			break;
		case 26:
			if (t_flag::table_gripper_staus == TRUE)
			{
				t_flag::site1_up_product = FALSE;
				t_flag::site1_task_product = FALSE;
				t_flag::table_gripper_staus = FALSE;
				t_flag::site2_task_product = TRUE;
				t_flag::site2_test_start = TRUE;
				step = 0;
			}
			Sleep(50);
			break;
		case 27:
			if (err = gripper_move_cfg(1) != SUCCESS)
			{
				log("right gripper move left err=%d",err);
				Sleep(5000);
				break;
			}
			log("right gripper move left 28");
			step = 29;
			break;
		case 29:
			if (err = first_cylinder_cfg(1) != SUCCESS)
			{
				log("first cylinder up err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder up 29");
			step = 30;
			break;
		case 30:
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 30");
			step = 31;
			break;
		case 31:
			if (err = gripper_cylinder_cfg(3) != SUCCESS)
			{
				log("right gripper close err=%d",err);
				Sleep(5000);
				break;
			}
			Sleep(500);
			log("right gripper close 31");
			step = 32;
			break;
		case 32:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 32");
			step = 33;
			break;
		case 33:
			if (err = gripper_move_cfg(2) != SUCCESS)
			{
				log("right gripper move right err=%d",err);
				Sleep(5000);
				break;
			}
			log("right gripper move right 33");
			step = 34;
			break;
		case 34:
			if (err = gripper_support_cfg(1) != SUCCESS)
			{
				log("gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support down 34");
			step = 35;
			break;
		case 35:
			if(err = gripper_cylinder_cfg(2) != SUCCESS)
			{
				log("right gripper open err=%d",err);
				Sleep(5000);
				break;
			}
			log("right gripper open");
			step = 36;
			break;
		case 36:
			if (err = gripper_support_cfg(2) != SUCCESS)
			{
				log("gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("gripper support up 36");
			step = 37;
			break;
		case 37:
			if (err = first_cylinder_cfg(2) != SUCCESS)
			{
				log("first cylinder down err=%d",err);
				Sleep(5000);
				break;
			}
			log("first cylinder down");
			step = 38;
			break;
		case 38:
			t_flag::feed_griper_staus = FALSE;
			t_flag::product_carry_left = TRUE;
			t_flag::site1_task_product = FALSE;
			step = 0;
			break;
		}
	}
}
//==========================合格产品下料=================
DWORD CALLBACK product_pass_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	while(1)
	{
		switch(step)
		{
		case 0:
			if (t_flag::carry_product_right_wait == TRUE)
			{
				step = 1;
				break;
			}
			Sleep(50);
			break;
		case 1:
			if (t_flag::pass_product == TRUE)
			{
				if (t_flag::pass_product_tray == TRUE)
				{
					step = 2;
					break;
				}
				log("pass product tray empty");
				Sleep(50);
			}
			break;
		case 2:
			t_flag::carry_product_right_wait = FALSE;
			t_flag::pass_product =FALSE;
			step = 3;
			break;
		case 3:
			if (err = product_gripper_support_cfg(1) != SUCCESS)
			{
				log("product gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support down");
			step = 4;
			break;
		case 4:
			if (err = product_gripper_cfg(1) != SUCCESS)
			{
				log("product gripper close err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper close");
			step = 5;
			break;
		case 5:
			if (err = product_gripper_support_cfg(2) != SUCCESS)
			{
				log("product gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support up");
			step = 6;
			break;
		case 6:
			t_flag::product_carry_right = TRUE;
			if(err=motion_product() != SUCCESS)
			{
				log("motion pass product err=%d",err);
				Sleep(5000);
				break;
			}
			log("motion pass product");
			if (SM104_COUNT == 7)
			{
				SM104_COUNT = 0;
				t_flag::pass_product_tray = FALSE;
				log("pass product full");
			}
			step = 7;
			break;
		case 7:
			if (err = product_gripper_support_cfg(1) != SUCCESS)
			{
				log("product gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support down");
			step = 8;
			break;
		case 8:
			if (err = product_gripper_cfg(2) != SUCCESS)
			{
				log("product gripper open err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper open");
			step = 9;
			break;
		case 9:
			if (err = product_gripper_support_cfg(2) != SUCCESS)
			{
				log("product gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support up");
			step = 10;
			break;
		case 10:
			if(err=motor_home_product() != SUCCESS) 
			{
				log("motion home product err=%d",err);
				Sleep(5000);
				break;
			}
			log("motion home product");
			step = 0;
			break;
		}
	}
}
//=========================NG产品下料======================
DWORD CALLBACK product_ng_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	while(1)
	{
		switch(step)
		{
		case 0:
			if (t_flag::carry_product_right_wait == TRUE)
			{
				step = 1;
				break;
			}
			Sleep(50);
			break;
		case 1:
			if (t_flag::ng_product == TRUE)
			{
				if (t_flag::ng_product_tray == TRUE)
				{
					step = 2;
					break;
				}
				log("ng product tray empty");
				Sleep(50);
			}
			break;
		case 2:
			t_flag::carry_product_right_wait = FALSE;
			t_flag::ng_product =FALSE;
			step = 3;
			break;
		case 3:
			if (err = product_gripper_support_cfg(1) != SUCCESS)
			{
				log("product gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support down");
			step = 4;
			break;
		case 4:
			if (err = product_gripper_cfg(1) != SUCCESS)
			{
				log("product gripper close err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper close");
			step = 5;
			break;
		case 5:
			if (err = product_gripper_support_cfg(2) != SUCCESS)
			{
				log("product gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support up");
			step = 6;
			break;
		case 6:
			t_flag::product_carry_right = TRUE;
			if(err=motion_ng_product() != SUCCESS)
			{
				log("motion ng product err=%d",err);
				Sleep(5000);
				break;
			}
			log("motion ng product");
			if (SM104_COUNT == 7)
			{
				SM104_COUNT = 0;
				t_flag::ng_product_tray = FALSE;
			}
			step = 7;
			break;
		case 7:
			if (err = product_gripper_support_cfg(1) != SUCCESS)
			{
				log("product gripper support down err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support down");
			step = 8;
			break;
		case 8:
			if (err = product_gripper_cfg(2) != SUCCESS)
			{
				log("product gripper open err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper open");
			step = 9;
			break;
		case 9:
			if (err = product_gripper_support_cfg(2) != SUCCESS)
			{
				log("product gripper support up err=%d",err);
				Sleep(5000);
				break;
			}
			log("product gripper support up");
			step = 10;
			break;
		case 10:
			if(err=motor_home_product() != SUCCESS) 
			{
				log("motion home product err=%d",err);
				Sleep(5000);
				break;
			}
			log("motion home product");
			step = 0;
			break;
		}
	}
}
//=========================物料运输=====================
DWORD CALLBACK carry_product_task(LPVOID temp)
{
	int err = 0;
	int step = 0;
	while(1)
	{
		switch(step)
		{
		case 0:
			if (t_flag::product_carry_left == TRUE)
			{
				t_flag::carry_product_left_wait = FALSE;
				t_flag::product_carry_left = FALSE;
				step = 1;
				break;
			}
			Sleep(50);
			break;
		case 1:
			if (err = product_cylinder_cfg(1) != SUCCESS)
			{
				log("product cylinder right err=%d",err);
				Sleep(5000);
				break;
			}
			if(t_flag::product_code[t_flag::carry_num] == TRUE)
				t_flag::pass_product = TRUE;
			else if ((t_flag::product_code[t_flag::carry_num] != TRUE))
				t_flag::ng_product = TRUE;
			t_flag::carry_num++;
			log("product cylinder right");
			t_flag::carry_product_right_wait = TRUE;
			step = 2;
			break;
		case 2:
			if (t_flag::product_carry_right == TRUE)
			{

				t_flag::carry_product_right_wait = FALSE;
				t_flag::product_carry_right = FALSE;
				step = 3;
				break;
			}
			break;
		case 3:
			if (err = product_cylinder_cfg(2) != SUCCESS)
			{
				log("product cylinder left err=%d",err);
				Sleep(5000);
				break;
			}
			log("product cylinder left");
			t_flag::carry_product_left_wait = TRUE;
			step = 0;
			break;
		}
	}
}
//======================更换托盘======================
DWORD CALLBACK change_tray_task(LPVOID temp)
{
	int	err = 0;
	int step = 0;
	int timeout = 0;
	
	while(1)
	{
		switch(step)
		{
		case 0: 
			if (t_flag::pass_product_tray == FALSE)
			{				
				step = 1;
				log("pass product no tray");
				break;
			}
			if (t_flag::ng_product_tray == FALSE)
			{
				step = 11;
				log("ng product no tray");
				break;
			}
			Sleep(50);
			break;
		case 1:
			if (err = pass_tray_cylinder(2) != SUCCESS)
			{
				log("pass tray down err=%d",err);
				Sleep(5000);
				break;
			}
			log("pass tray down");
			step = 2;
			break;
		case 2:
			Y39_ON;
			log("line move forward");
			step = 3;
			break;
		case 3:
			timeout = 500;
			while(timeout-- > 0 && (!S47 || !S48))
			{
				Sleep(30);
			}
			if (!S47)
			{
				log("S47 off err");
				Sleep(5000);
				break;
			}
			if (!S48)
			{
				log("S48 off err");
				Sleep(5000);
				break;
			}
			if(err = stack_cylinder(1) != SUCCESS)
			{
				log("stack cylinder up err=%d",err);
				Sleep(5000);
				break;
			}
			log("stack cylinder up");
			Sleep(1000);
			Y39_OFF;
			log("line move stop");
			step = 4;
			break;
		case 4:
			if(err = stack_cylinder(2) != SUCCESS)
			{
				log("stack cylinder down err=%d",err);
				Sleep(5000);
				break;
			}
			log("stack cylinder down");
			Y38_ON;
			log("line move reverse");
			step = 5;
			break;
		case 5:
			if (!S37)
			{
				log("no tray err");
				Sleep(5000);
				break;
			}
			step = 6;
			break;
		case 6:
			if (err=SM102_move_to_stack1_position() != SUCCESS) 
			{
				log("line t move stack1 err = %d",err);
				Sleep(5000);
				break;
			}
			log("line t move stack1");
			if (err = tray_block_cylinder(2) != SUCCESS)
			{
				log("tray block cylinder open err=%d",err);
				Sleep(5000);
				break;
			}
			log("tray block cylinder open");
			step = 7;
			break;
		case 7:
			if (err=SM102_move_to_stack2_position() != SUCCESS) 
			{
				log("line t move stack2 err = %d",err);
				Sleep(5000);
				break;
			}
			log("line t move stack2");
			step = 8;
			break;
		case 8:
			if (err = tray_block_cylinder(1) != SUCCESS)
			{
				log("tray block cylinder close err=%d",err);
				Sleep(5000);
				break;
			}
			log("tray block cylinder close");
			step = 9;
			break;
		case 9:
			if (err=SM102_move_to_stack3_position() != SUCCESS) 
			{
				log("line t move stack3 err = %d",err);
				Sleep(5000);
				break;
			}
			log("line t move stack3");
			step = 10;
			break;
		case 10:
			timeout = 500;
			while(timeout-- > 0 && (!S35))
			{
				Sleep(30);
			}
			if (!S35)
			{
				log("S35 off err=%d");
				Sleep(5000);
				break;
			}
			if (err = pass_tray_cylinder(1) != SUCCESS)
			{
				log("pass tray up err=%d",err);
				Sleep(5000);
				break;
			}
			log("pass tray up");
			Y38_OFF;
			log("line move stop");
			t_flag::pass_product_tray = TRUE;
			step = 0;
			break;
		case 11:
			if(err = ng_tray_cylinder(1) != SUCCESS)
			{
				log("ng tray reverse err=%d",err);
				Sleep(5000);
				break;
			}
			step = 12;
			log("ng tray reverse");
			break;
		case 12:
			if (t_flag::ng_tray_reset == TRUE)
			{
				t_flag::ng_tray_reset = FALSE;
				step = 13;
				break;
			}
			Sleep(100);
			break;
		case 13:
			if(err = ng_tray_cylinder(2) != SUCCESS)
			{
				log("ng tray forward err=%d",err);
				Sleep(5000);
				break;
			}
			step = 12;
			t_flag::ng_product_tray = TRUE;
			log("ng tray forward");
			step = 0;
			break;
		}
	}
}