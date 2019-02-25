#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "XTrace.h"
#include "s_win.h"
#include "inf_motion_aa.h"
#include "cts.h"

using namespace t_flag;
namespace led_flag
{
	int led_alarm_flag =FALSE;			//告警指示灯
	int yellow_falsh_flag = FALSE;		//黄灯闪烁
}
/*各窗口功能线程及主界面显示*/
extern HINSTANCE app_inst;
extern INIEdit config_dlg[];
extern INIEdit gl_conf[];
extern StdChart st;
extern void	MakeSpaceTableITU(void);
extern DWORD CALLBACK showtimeThread(LPVOID temp);
extern DWORD CALLBACK mouse_operation_thread(LPVOID temp);
extern DWORD CALLBACK test_thread(LPVOID temp);
extern DWORD CALLBACK error_monitor_thread(LPVOID temp);

extern DWORD CALLBACK get_image_thread(LPVOID temp);
extern DWORD CALLBACK display_image_thread(LPVOID temp);
extern DWORD CALLBACK std_chart_test_thread(LPVOID temp);
extern DWORD CALLBACK reg_mark_chart_test_thread(LPVOID temp);
extern DWORD CALLBACK std_test_thread(LPVOID temp);

extern DWORD CALLBACK extern_in_handshake_thread(LPVOID temp);
extern DWORD CALLBACK image_info_thread(LPVOID temp);

extern DWORD CALLBACK vision_link_thread(LPVOID temp);
extern DWORD CALLBACK plc_link_thread(LPVOID temp);
extern DWORD CALLBACK vision_recv_thread(LPVOID temp);
extern DWORD CALLBACK vision_cmd_thread(LPVOID temp);
extern DWORD CALLBACK plc_online_check_thread(LPVOID temp);
//extern DWORD CALLBACK plc_online_counting_thread(LPVOID temp);
extern DWORD CALLBACK plc_read_thread(LPVOID temp);
//extern DWORD CALLBACK web_cam_thread(LPVOID temp);
extern DWORD CALLBACK check_img_exist_thread(LPVOID temp);
extern DWORD CALLBACK white_test_thread(LPVOID temp);
extern DWORD CALLBACK ioStatusThread(LPVOID temp);




extern DWORD CALLBACK feed_gripper_task(LPVOID temp);
extern DWORD CALLBACK site1_task(LPVOID temp);
extern DWORD CALLBACK site2_task(LPVOID temp);
extern DWORD CALLBACK site3_task(LPVOID temp);
extern DWORD CALLBACK site4_task(LPVOID temp);
extern DWORD CALLBACK Table_task_position(LPVOID temp);
extern DWORD CALLBACK feed_task(LPVOID temp);
extern DWORD CALLBACK product_pass_task(LPVOID temp);
extern DWORD CALLBACK product_ng_task(LPVOID temp);
extern DWORD CALLBACK carry_product_task(LPVOID temp);
extern DWORD CALLBACK led_display(LPVOID temp);
extern DWORD CALLBACK change_tray_task(LPVOID temp);
extern DWORD CALLBACK site_com(LPVOID temp);

extern BOOL CALLBACK guide_line_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK histogram_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK measure_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_img_info(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);

extern BOOL CALLBACK dlg_log(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_recipe(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_hw_config(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_test_config(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK dlg_img_info(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);

extern BOOL CALLBACK stdchart_setting_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK std_color_chart_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK white_test_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK white_test_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK reg_mark_chart_setting_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK light_test_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK motor_cfg_dlg(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK Dlg_Motion_Config(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK reg_mark_tf_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK menuDlg_TCP_IP(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);


extern BOOL CALLBACK ioStatus_output_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK ioStatus_input_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);

extern void set_guide_line_color(void);

char connect_status[2][20] = {"External Connect>>", "External Connected"};
extern int item_id[];
extern int order_id[];
extern int loop_id[];
extern int check_id[];
extern int result_id[];
extern int time_id[];
extern SlantedBW mysbw;
class MDlg:public ai_win::Dlg
{
public:
	MDlg();
	virtual BOOL	init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void	command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual LRESULT ctlcolor_static(HWND hdlg, HDC hdc, HWND hcolor, int mark = CTLCOLOR_STATIC );
	virtual LRESULT ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark =CTLCOLOR_DLG);
	virtual void	draw_item(HWND hdlg, const DRAWITEMSTRUCT *pdis);
	virtual void	size(HWND hdlg, unsigned int state, int width, int height);
	virtual void	close(HWND hdlg);
	virtual LRESULT ctlcolor_listbox(HWND hdlg, HDC hdc, HWND hcolor, int mark/* =CTLCOLOR_EDIT */);

private:
	HBRUSH static_brush;
	HMENU menu_handle;
	HBRUSH dlg_brush, old_dlg_brush;

//	BOOL open_single_img_file(HWND hdlg);
};

MDlg mdlg;
BOOL CALLBACK mdlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,	    mdlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		    mdlg.command);
		HANDLE_DLGMSG(hdlg, WM_CTLCOLORSTATIC,  mdlg.ctlcolor_static);
		HANDLE_DLGMSG(hdlg, WM_CTLCOLORDLG,     mdlg.ctlcolor_dlg);
		HANDLE_DLGMSG(hdlg, WM_CTLCOLORLISTBOX, mdlg.ctlcolor_listbox);
		HANDLE_DLGMSG(hdlg, WM_DRAWITEM,	    mdlg.draw_item);
		//		HANDLE_DLGMSG(hdlg, WM_SIZE,		    mdlg.size);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		    mdlg.close);

	case WM_LBUTTONDBLCLK:
		MOUSE_LDBCLICK = TRUE;
		break;

	case WM_LBUTTONDOWN:
		{
			//SetWindowPos(HWND_LOG, HWND_TOP, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			MEASURE_DRAW_RECT_DONE = FALSE;
			RECT rt;
			GetClientRect(GetDlgItem(hdlg, IDC_DISPLAY), &rt);
			IMG_RATIO_X = (float)DISPLAY_IMG_WIDTH/(rt.right-rt.left);
		//	log("img_w=%d",DISPLAY_IMG_WIDTH);
			IMG_RATIO_Y = (float)DISPLAY_IMG_HEIGHT/(rt.bottom-rt.top);
			MOUSE_X = LOWORD(lParam);
			MOUSE_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;
		//	log("mouseX=%d,mouseY=%d",MOUSE_X,MOUSE_Y);
			MOUSE_LDOWN = TRUE;
		}

		break;

	case WM_MOUSEMOVE:
		if ((wParam&MK_LBUTTON)){
			MOUSE_LDOWN_MOVE = TRUE;
			MOUSE_X = LOWORD(lParam);
			MOUSE_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;
		}


		if ((wParam&MK_LBUTTON) && ENABLE_IMG_ZOOM_IN && DRAW_ZOOM_IN_AREA_DONE == FALSE){
			MOUSE_END_X = LOWORD(lParam);
			MOUSE_END_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;
			DRAW_MOVING_MOUSE = TRUE;
		}

		if ((wParam&MK_LBUTTON) && ENABLE_MEASURE == TRUE){
			MEASURE_END_X = LOWORD(lParam);
			MEASURE_END_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;
		}
		break;

	case WM_LBUTTONUP:
		MOUSE_X = LOWORD(lParam);
		MOUSE_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;
		MOUSE_LUP = TRUE;

		if (ENABLE_MEASURE == TRUE){
			MEASURE_END_X = LOWORD(lParam);
			MEASURE_END_Y = HIWORD(lParam)-MYTOOLBAR_HEIGHT;

			if (MEASURE_END_X>MEASURE_START_X && MEASURE_END_Y>MEASURE_START_Y){
				MEASURE_DRAW_RECT_DONE = TRUE;
			}
		}
		break;
	}

	return FALSE;
}

MDlg::MDlg()
{
	static_brush = NULL;
}

LRESULT MDlg::ctlcolor_dlg(HWND hdlg, HDC hdc, HWND hcolor, int mark /* =CTLCOLOR_DLG */)
{
	if (dlg_brush != NULL){
		DeleteObject(dlg_brush);
	}

	dlg_brush = CreateSolidBrush(RGB(0xbd, 0xc7, 0xd7));//主窗口背景色
	old_dlg_brush = (HBRUSH)SelectObject(hdc, (HBRUSH)dlg_brush);

	return (LRESULT)dlg_brush;
}

LRESULT MDlg::ctlcolor_listbox(HWND hdlg, HDC hdc, HWND hcolor, int mark)//主界面log窗口背景色
{
	DeleteObject(dlg_brush);
	SetBkMode(hdc, TRANSPARENT);
	dlg_brush = CreateSolidBrush(RGB(156,170,193));
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, dlg_brush);

	HFONT font     = CreateFont(15, 0, 0, 0, FW_BLACK, 0,0,0, ANSI_CHARSET, 0,0,0,0, "黑体");
	HFONT old_font = (HFONT)SelectObject(hdc, font);
	LISTBOX_SETLHEIGHT(hdlg, IDC_LOG, 17);
	SetTextColor(hdc, COLOR_WHITE);

	DeleteObject(font);

	return (LRESULT)dlg_brush;
}

int load_dll_module(LPVOID temp)
{
	HWND hdlg = (HWND)temp;

	int result = s_test::cimg.init_lib();
	log("Lib:Image convert function init:%s", result?"OK":"Fail");

	//////////////////////////////////////////////////////////////////////////
	//hardware
	char hw_list[HW_CNT][FOLDER_NAME_LENGTH] = {0};
	char hw_path[MAX_PATH] = {0};
	sprintf(hw_path, "%s\\hw\\", s_test::dir);
	int n = ai_win::get_sub_files_name(hw_path, hw_list, HW_CNT, "hw_*.dll");
	HW_NUM = 0;
	if (n>0){
		for (int i=0; i<n; i++){
			result = s_test::imhw[i].load_module(hw_list[i]);
			(RESULT_LOAD_HW(i)) = result;
			log("Lib::Load HW [ %s ] :%s", hw_list[i], result?"OK":"Fail");
			if (result == TRUE){
				HW_NUM++;
				s_test::imhw[i].add_menu(s_test::hmenu, ID_MENU_HW(i));
				char mystr[TEST_CNT][MAX_PATH] = {0};
				strcpy(mystr[0], s_test::dir);
				strcpy(mystr[1], s_test::recipe_name);
				s_test::imhw[i].init(HWND_MAIN, s_test::ram_int, mystr, log);
			}
		}
	}

	char hw_str[100] = {0};

	if (HW_NUM>=1){
		for (int i=0; i<HW_NUM; i++){
			memset(hw_str, 0, sizeof(hw_str));
			result = s_test::imhw[i].get_hw_str(hw_str);
			COMBO_ADDSTR(HWND_HW_CONFIG, IDC_SELECT_INTERNAL_DEVICE, hw_str);
		}
		SELECT_INTERNAL_DEVICE = ai_win::read_integer_key(s_test::config_path, "img", "select_internal_device");
		COMBO_SETCURSEL(HWND_HW_CONFIG, IDC_SELECT_INTERNAL_DEVICE, SELECT_INTERNAL_DEVICE);
		if (SELECT_INTERNAL_DEVICE > HW_NUM-1){
			SELECT_INTERNAL_DEVICE = HW_NUM-1;
		}

		s_test::pimhw = &s_test::imhw[SELECT_INTERNAL_DEVICE];
		DEVICE_START_INIT      = ai_win::read_integer_key(s_test::config_path, "img", "device_start_init");
		if (DEVICE_START_INIT == TRUE){
			result = s_test::pimhw->init_device();
			if (result == TRUE) IMG_DEVICE_INITED = TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//img tools
	char tool_list[TOOL_CNT][FOLDER_NAME_LENGTH] = {0};
	char tool_path[MAX_PATH] = {0};
	sprintf(tool_path, "%s\\tools\\", s_test::dir);
	n = ai_win::get_sub_files_name(tool_path, tool_list, TOOL_CNT, "tool_*.dll");
	if (n>0){
		for (int i=0; i<n; i++){
			result = s_test::imtool[i].load_module(tool_list[i]);
			(RESULT_LOAD_TOOL(i)) = result;
			log("Lib::Load TOOL [ %s ] :%s", tool_list[i], result?"OK":"Fail");
			if (result == TRUE){
				s_test::imtool[i].add_menu(s_test::hmenu, ID_MENU_TOOL(i));
			}
		}
	}


	return FALSE;
}

BOOL MDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{

	ai_win::set_icon(hdlg, IDI_LOGO, IDI_LOGO);
	//////////////////////////////////////////////////////////////////////////
	//Menu
	MENUINFO mi;
	s_test::hmenu = ai_win::create_menu(hdlg, IDR_MENU);
	mi.fMask   = MIM_BACKGROUND;
	mi.hbrBack = CreateSolidBrush(RGB(255, 0, 0));
	SetMenuInfo(s_test::hmenu, &mi);
	DrawMenuBar(hdlg);

	MENU_BMP(hdlg, ID_MULTI_SETTING,		 IDB_T_MSFR		);
	MENU_BMP(hdlg, ID_TOOLS_SINGLESFR,		 IDB_T_SFR		);
	MENU_BMP(hdlg, ID_FOCUS_SETTING,		 IDB_T_ST		);
	MENU_BMP(hdlg, ID_FILE_LOG,				 IDB_T_LOG		);
	MENU_BMP(hdlg, ID_MULTI_SFR_CURVE1,		 IDB_T_CURVE	);
	MENU_BMP(hdlg, ID_MULTI_SFR_CURVE2,		 IDB_T_CURVE	);
	MENU_BMP(hdlg, ID_FOCUS_FOCUSCURVE,		 IDB_T_CURVE	);
	MENU_BMP(hdlg, ID_TEST_CONFIG,			 IDB_T_CONFIG	);
	MENU_BMP(hdlg, ID_SETTING_RECIPE,		 IDB_T_RECIPE	);
	MENU_BMP(hdlg, ID_FILE_LOGIN,			 IDB_T_LOGIN	);
	MENU_BMP(hdlg, ID_EDIT_IMAGEINFO,		 IDB_T_IMG_INFO	);
	MENU_BMP(hdlg, ID_TOOL_HISTOGRAM,		 IDB_T_HISTOGRAM);
	MENU_BMP(hdlg, ID_EDIT_GUIDELINE,		 IDB_T_GUIDE	);
	MENU_BMP(hdlg, ID_TOOL_MEASURE,			 IDB_T_MEASURE	);
	MENU_BMP(hdlg, ID_FILE_SAVEINFOIMAGE,    IDB_T_SAVE		);

	///////////////////////////////////////////////////////////////////////////
	//status bar
	int sub_width[6] = {90, 150, 270, 390, 470, -1};
	HWND_STATUS = ai_win::create_statusbar(hdlg, sub_width, sizeof(sub_width)/sizeof(int), 0);
	char version[20] = {0};
	ai_win::get_version(NULL, TRUE, version);

	char status_string1[100] = {0};
	sprintf(status_string1, " %s", version);

	SendMessage(HWND_STATUS, SB_SETTEXT, 0, (LPARAM)" @2018");
	SendMessage(HWND_STATUS, SB_SETTEXT, 1, (LPARAM)status_string1);
	SendMessage(HWND_STATUS, SB_SETTEXT, 2, (LPARAM)"Image Files");
	SendMessage(HWND_STATUS, SB_SETTEXT, 3, (LPARAM)"");
	SendMessage(HWND_STATUS, SB_SETTEXT, 4, (LPARAM)"");
	SendMessage(HWND_STATUS, SB_SETTEXT, 5, (LPARAM)"");

	//main dlg layout
	RECT rt, rt_m;
	GetClientRect(hdlg, &rt_m);
	GetClientRect(HWND_STATUS, &rt);
// 	MoveWindow(GetDlgItem(hdlg, IDC_DISPLAY), 640, 
// 											  MYTOOLBAR_HEIGHT, 
// 											  640/*rt_m.right-3*/, 
// 											  480/*rt_m.bottom-MYTOOLBAR_HEIGHT-rt.bottom-1*/, FALSE);
	//MoveWindow(GetDlgItem(hdlg, IDC_STATUS), 1, 1, 150, MYTOOLBAR_HEIGHT-2, FALSE);

	//////////////////////////////////////////////////////////////////////////
	//create sub dlg

	HWND_LOG		 = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_LOG),		    hdlg, (DLGPROC)dlg_log);
	MoveWindow(HWND_LOG, s_test::dinfo.rt_log.left,	
						 s_test::dinfo.rt_log.top,	
						 s_test::dinfo.rt_log.right-s_test::dinfo.rt_log.left,	
						 s_test::dinfo.rt_log.bottom-s_test::dinfo.rt_log.top, FALSE);
//	CheckMenuItem(s_test::hmenu, ID_FILE_LOG,	  s_test::dinfo.show_log?MF_CHECKED:MF_UNCHECKED);
//	ShowWindow(HWND_LOG, s_test::dinfo.show_log?SW_NORMAL:SW_HIDE);

	COMBO_ADDSTR(hdlg, IDC_SELECT_IMG_DEVICE, "Image Files");//**********img source select
	COMBO_ADDSTR(hdlg, IDC_SELECT_IMG_DEVICE, "Hinge Cam");
	SELECT_IMG_SOURCE = ai_win::read_integer_key(s_test::config_path, "img", "select_img_source");

	COMBO_SETCURSEL(hdlg, IDC_SELECT_IMG_DEVICE, SELECT_IMG_SOURCE);
	if (SELECT_IMG_SOURCE == IMG_DEV_FILE){
		s_test::cap_dev = (ImgCapDev *)&s_test::img_file;
		SendMessage(HWND_STATUS, SB_SETTEXT, 2, (LPARAM)"Image Files");
	}
	else if (SELECT_IMG_SOURCE == IMG_DEV_HINGE){
		s_test::cap_dev = (ImgCapDev *)&s_test::hinge;
		BOOL bRes = 1;
		SendMessage(HWND_STATUS, SB_SETTEXT, 2, (LPARAM)"HINGE_CAM");
	}

	HWND_RECIPE		   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_RECIPE),	    hdlg, (DLGPROC)dlg_recipe);
	HWND_TEST_CONFIG   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_TEST_CONFIG),	hdlg, (DLGPROC)dlg_test_config);
	HWND_MOTOR_MOTION  = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_MOTOR_CONFIG),  hdlg, (DLGPROC)motor_cfg_dlg);
	HWND_IO_OUTPUT	   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_IO_CONFIG),		hdlg, (DLGPROC)ioStatus_output_process);
	HWND_IO_INPUT	   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_IO_CONFIG1),	hdlg, (DLGPROC)ioStatus_input_process);
	HWND_TCPIP		   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_TCPIP),			hdlg, (DLGPROC)menuDlg_TCP_IP);
	HWND_MOTION		   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_MOTION),		hdlg, (DLGPROC)Dlg_Motion_Config);
	HWND_STD_COLOR_CHART = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_COLOR_SCAL_TEST),hdlg, (DLGPROC)std_color_chart_dlg_process);
	HWND_WHITE		   = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_WHITE),hdlg, (DLGPROC)white_test_dlg_process);
	RECT rt_tc;	
	GetClientRect(HWND_TEST_CONFIG, &rt_tc);
	MoveWindow(HWND_TEST_CONFIG, s_test::dinfo.tcdlg_x, 
								 s_test::dinfo.tcdlg_y, 
								 rt_tc.right-rt_tc.left+4, 
								 rt_tc.bottom-rt_tc.top+20, FALSE);
	CheckMenuItem(s_test::hmenu, ID_TEST_CONFIG, s_test::dinfo.show_tc?MF_CHECKED:MF_UNCHECKED);
	ShowWindow(HWND_TEST_CONFIG, s_test::dinfo.show_tc?SW_NORMAL:SW_HIDE);	

	GetClientRect(HWND_MOTOR_MOTION, &rt_tc);
	MoveWindow(HWND_MOTOR_MOTION, s_test::dinfo.aadlg_x, 
		s_test::dinfo.aadlg_y, 
		rt_tc.right-rt_tc.left+4, 
		rt_tc.bottom-rt_tc.top+20, FALSE);

	HWND_IMG_INFO    = CreateDialog(app_inst, MAKEINTRESOURCE(IDD_IMAGE_INFO),	hdlg, (DLGPROC)dlg_img_info);
	update_mdlg_title(hdlg);

	s_test::read_io_display_string_output(HWND_IO_OUTPUT);
	s_test::read_io_display_string_input(HWND_IO_INPUT);

	//////////////////////////////////////////////////////////////5////////////
	//Create thread
#define PLC_LINK_ON
#define THREAD_NUM	30
	DWORD my_id[THREAD_NUM] = {0};
	HANDLE my_handle[THREAD_NUM] = {NULL};

	my_handle[0]  = CreateThread(NULL, 0, test_thread,				  (LPVOID)hdlg, 0, &my_id[0]);
	my_handle[1]  = CreateThread(NULL, 0, showtimeThread,			  (LPVOID)hdlg, 0, &my_id[1]);
	my_handle[2]  = CreateThread(NULL, 0, mouse_operation_thread,	  (LPVOID)hdlg, 0, &my_id[2]);
	my_handle[3]  = CreateThread(NULL, 0, error_monitor_thread,		  (LPVOID)hdlg, 0, &my_id[3]);
	my_handle[4]  = CreateThread(NULL, 0, get_image_thread,			  (LPVOID)hdlg, 0, &my_id[4]);
	my_handle[5]  = CreateThread(NULL, 0, display_image_thread,		  (LPVOID)hdlg, 0, &my_id[5]);
	my_handle[6]  = CreateThread(NULL, 0, reg_mark_chart_test_thread, (LPVOID)hdlg, 0, &my_id[6]);
	my_handle[7]  = CreateThread(NULL, 0, check_img_exist_thread,	  (LPVOID)hdlg, 0, &my_id[7]);
//	my_handle[8]  = CreateThread(NULL, 0, std_test_thread,			  (LPVOID)hdlg, 0, &my_id[8]);
	my_handle[9]  = CreateThread(NULL, 0, white_test_thread,		  (LPVOID)hdlg, 0, &my_id[9]);
#ifdef PLC_LINK_ON
	my_handle[11] = CreateThread(NULL, 0, plc_link_thread,			  (LPVOID)hdlg, 0, &my_id[9]);
	my_handle[12] = CreateThread(NULL, 0, plc_online_check_thread,	  (LPVOID)hdlg, 0, &my_id[12]);
//	my_handle[13] = CreateThread(NULL, 0, plc_online_counting_thread, (LPVOID)hdlg, 0, &my_id[13]);
	my_handle[14] = CreateThread(NULL, 0, plc_read_thread,			  (LPVOID)hdlg, 0, &my_id[14]);
#endif

	my_handle[15] = CreateThread(NULL, 0, feed_gripper_task,		  (LPVOID)hdlg, 0, &my_id[15]);
	my_handle[16] = CreateThread(NULL, 0, site2_task,				  (LPVOID)hdlg, 0, &my_id[16]);
	my_handle[17] = CreateThread(NULL, 0, site3_task,				  (LPVOID)hdlg, 0, &my_id[17]);
	my_handle[18] = CreateThread(NULL, 0, site4_task,				  (LPVOID)hdlg, 0, &my_id[18]);
	my_handle[19] = CreateThread(NULL, 0, Table_task_position,		  (LPVOID)hdlg, 0, &my_id[19]);
	my_handle[20] = CreateThread(NULL, 0, ioStatusThread,			  (LPVOID)hdlg, 0, &my_id[20]);
	my_handle[21] = CreateThread(NULL, 0, feed_task,				  (LPVOID)hdlg, 0, &my_id[21]);
	my_handle[22] = CreateThread(NULL, 0, product_pass_task,		  (LPVOID)hdlg, 0, &my_id[22]);
	my_handle[23] = CreateThread(NULL, 0, product_ng_task,			  (LPVOID)hdlg, 0, &my_id[23]);
	my_handle[24] = CreateThread(NULL, 0, carry_product_task,		  (LPVOID)hdlg, 0, &my_id[24]);
	my_handle[25] = CreateThread(NULL, 0, led_display,				  (LPVOID)hdlg, 0, &my_id[25]);
	my_handle[26] = CreateThread(NULL, 0, site_com,					  (LPVOID)hdlg, 0, &my_id[26]);	
	my_handle[27] = CreateThread(NULL, 0, change_tray_task,			  (LPVOID)hdlg, 0, &my_id[27]);

	for (int i=0; i<THREAD_NUM; i++){
		CloseHandle(my_handle[i]);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//Load hw, test, tool module
	load_dll_module(hdlg);

	//////////////////////////////////////////////////////////////////////////
	//other initial
	//guide line
	ai_win::load_keys(s_test::config_path, "guide_line", hdlg, gl_conf, 100);
	set_guide_line_color();

	create_report_folder();
	log("Dlg: Main UI init OK.");
	TESTER_ID = ai_win::read_integer_key(".\\recipe\\config.ini", "system", "tester_id");
	log("tester_id=%d",TESTER_ID);
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\%s\\base.ini", s_test::dir, s_test::recipe_name);
	log("base:%s", path);
	IMG_WIDTH	   = ai_win::read_integer_key(path, "image", "img_width");
	IMG_HEIGHT	   = ai_win::read_integer_key(path, "image", "img_height");
	WEB_CAM_WIDTH  = ai_win::read_integer_key(path, "image", "web_cam_img_width");
	WEB_CAM_HEIGHT = ai_win::read_integer_key(path, "image", "web_cam_img_height");
	WEB_CAM_NUM	   = ai_win::read_integer_key(path, "image", "web_cam_num");

	log("read w:%d, h:%d", IMG_WIDTH, IMG_HEIGHT);
	MakeSpaceTableITU();



	if (ERROR_ON != TRUE){
		set_status(APP_READY);//主窗口状态
	}
//	ShowWindow(HWND_LOG, s_test::dinfo.show_log?SW_NORMAL:SW_HIDE);
	return TRUE;
}

void MDlg::size(HWND hdlg, unsigned int state, int width, int height)
{
	RECT rt;
	GetClientRect(HWND_STATUS, &rt);
}

void MDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (code_notify){
	case CBN_SELCHANGE:
		{
			if(TEST_START == true)
				break;
			int index = COMBO_GETCURSEL(hdlg, IDC_SELECT_IMG_DEVICE);
			SELECT_IMG_SOURCE = index;
			log("select img source = %d\n path:%s",index,s_test::config_path);
			ai_win::write_integer_key(s_test::config_path, "img", "select_img_source",index);
			if (SELECT_IMG_SOURCE == IMG_DEV_FILE){
				s_test::cap_dev = (ImgCapDev *)&s_test::img_file;
				SendMessage(HWND_STATUS, SB_SETTEXT, 2, (LPARAM)"Image Files");
			}
			else if (SELECT_IMG_SOURCE == IMG_DEV_HINGE){
				s_test::cap_dev = (ImgCapDev *)&s_test::hinge;
				SendMessage(HWND_STATUS, SB_SETTEXT, 2, (LPARAM)"HingeCam");
			}
		}
		break;
	}
	switch (id)
	{
	//////////////////////////////////////////////////////////////////////////
	//menu

	case ID_FILE_SAVEINFOIMAGE:
		save_info_image(hdlg);
		break;

	case ID_FILE_LOG:
		s_test::dinfo.show_log ^= TRUE;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_log?MF_CHECKED:MF_UNCHECKED);
		ShowWindow(HWND_LOG, s_test::dinfo.show_log?SW_NORMAL:SW_HIDE);
		break;

	case ID_TEST_CONFIG:
		s_test::dinfo.show_tc ^= TRUE;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_tc?MF_CHECKED:MF_UNCHECKED);
		ShowWindow(HWND_TEST_CONFIG, s_test::dinfo.show_tc?SW_NORMAL:SW_HIDE);
		break;

	case ID_SETTING_RECIPE:
		s_test::dinfo.show_recipe ^= TRUE;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_recipe?MF_CHECKED:MF_UNCHECKED);
		ShowWindow(HWND_RECIPE, s_test::dinfo.show_recipe?SW_NORMAL:SW_HIDE);
		break;

	case ID_EDIT_GUIDELINE:
		if (HWND_GUIDELINE == NULL){
			HWND_GUIDELINE = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE), 
				MAKEINTRESOURCE(IDD_GUIDE_LINE), hdlg, (DLGPROC)guide_line_dlg_process);
		}
		ShowWindow(HWND_GUIDELINE, SW_NORMAL);
		break;

	case ID_EDIT_IMAGEINFO:
		s_test::dinfo.show_img_info ^= TRUE;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_img_info?MF_CHECKED:MF_UNCHECKED);
		ShowWindow(HWND_IMG_INFO, s_test::dinfo.show_img_info?SW_NORMAL:SW_HIDE);
		break;
	//////////////////////////////////////////////////////////////////////////
	//test
	case ID_MOTOR_CONFIG:
		if (HWND_MOTOR_MOTION == NULL){
			HWND_MOTOR_MOTION = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_AA_CONFIG), hdlg, (DLGPROC)motor_cfg_dlg);
		}
		ShowWindow(HWND_MOTOR_MOTION, SW_NORMAL);
		break;
	case ID_MOTION:
		if (HWND_MOTION == NULL){
			HWND_MOTION = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_MOTION), hdlg, (DLGPROC)Dlg_Motion_Config);
		}
		ShowWindow(HWND_MOTION, SW_NORMAL);
		break;

	case ID_GRAY_COLOR_TEST:
		if (HWND_STD_COLOR_CHART == NULL){
			HWND_STD_COLOR_CHART = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_COLOR_SCAL_TEST), hdlg, (DLGPROC)std_color_chart_dlg_process);
		}
		ShowWindow(HWND_STD_COLOR_CHART, SW_NORMAL);
		break;

	case ID_WHITE:
		if (HWND_WHITE == NULL){
			HWND_WHITE = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_WHITE), hdlg, (DLGPROC)white_test_dlg_process);
		}
		ShowWindow(HWND_WHITE, SW_NORMAL);
		break;

	case ID_REGMARKCHART_CONFIGURE:
		if (HWND_REG_MARK_CHART == NULL){
			HWND_REG_MARK_CHART = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_REG_MARK_CHART), hdlg, (DLGPROC)reg_mark_chart_setting_dlg_process);
		}
		ShowWindow(HWND_REG_MARK_CHART, SW_NORMAL);
		break;
	case ID_LIGHT:
		if (HWND_LIGHT == NULL){
			HWND_LIGHT = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_LIGHT), hdlg, (DLGPROC)light_test_dlg_process);
		}
		ShowWindow(HWND_LIGHT, SW_NORMAL);
		break;

	//tool
	case ID_TOOL_HISTOGRAM:
		if (HWND_HISTOGRAM == NULL){
			HWND_HISTOGRAM = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE), 
				MAKEINTRESOURCE(IDD_HISTOGRAM), hdlg, (DLGPROC)histogram_dlg_process);
		}
		ShowWindow(HWND_HISTOGRAM, SW_NORMAL);
		IS_HISTOGRAM_DLG_SHOW = TRUE;
		break;

	case ID_TOOL_MEASURE:
		if (HWND_MEASURE == NULL){
			HWND_MEASURE = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE), 
				MAKEINTRESOURCE(IDD_MEASURE), hdlg, (DLGPROC)measure_dlg_process);
		}

		if (IS_IMAGE_SCENERY_OCCUPIED){
			MessageBox(hdlg, "Image scenery is occupied!", "Error", 0);
		}
		else{
			ENABLE_MEASURE = TRUE;
			ShowWindow(HWND_MEASURE, SW_NORMAL);
			IS_IMAGE_SCENERY_OCCUPIED = TRUE;
		}
		break;

	//////////////////////////////////////////////////////////////////////////
	//mdlg items
	case ID_IO_OUTPUT:		
		s_test::dinfo.show_io1_dlg ^= TRUE;
		IO_DISPLAY_FLAG=ON;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_io1_dlg?MF_CHECKED:MF_UNCHECKED);	
		ShowWindow(HWND_IO_OUTPUT, s_test::dinfo.show_io1_dlg?SW_NORMAL:SW_HIDE);
		BUTTON_VALUE = 399;
		break;

	case ID_IO_INPUT:		
		s_test::dinfo.show_io2_dlg ^= TRUE;
		IO_DISPLAY_FLAG=ON;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_io2_dlg?MF_CHECKED:MF_UNCHECKED);	
		ShowWindow(HWND_IO_INPUT, s_test::dinfo.show_io2_dlg?SW_NORMAL:SW_HIDE);
		BUTTON_VALUE = 399;
		break;
	case ID_TCPIP:		
		s_test::dinfo.show_tcpip ^= TRUE;
		CheckMenuItem(s_test::hmenu, id, s_test::dinfo.show_tcpip?MF_CHECKED:MF_UNCHECKED);	
		ShowWindow(HWND_TCPIP, s_test::dinfo.show_tcpip?SW_NORMAL:SW_HIDE);
		BUTTON_VALUE = 399;
		break;
		
	case IDC_START:	
		if (TEST_START == FALSE){
			log("Cmd:[Start]");
			set_status(APP_START);
			TEST_START = TRUE;
			ITEM_GRAYED(hdlg, id);
		}
		break;

	case IDC_STOP:
		log("Cmd:[Stop]");
		set_status(APP_STOP);
		TEST_STOP = TRUE;
		TEST_START = FALSE;
		IMG_DEVICE_INITED = FALSE;
		s_test::test.set_test_stop();
		s_test::cap_dev->stop();
		
		SetEvent(s_test::test.get_event());
		ITEM_ENABLED(hdlg, IDC_START);
		s_test::test.reset_test_items();

		ERROR_ON = FALSE;
		IMG_CAPTURE_READY = FALSE;

		IMG_DEVICE_RUN = FALSE;
		disconnect_device(IMG_DEVICE_INDEX);

		break;

	case IDC_SNAPSHOT:
		if (IMG_CAPTURE_READY == TRUE){
			save_img_file(hdlg);
		}
		else log("Image Capture not valid.");
		break;

	case IDC_SAVE:
		save_reg_mark_report_c(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT,SAVE_REG_REPORT);
		log("report saved.");

	case IDC_CLEAR_ALARM:
		led_flag::led_alarm_flag = FALSE;
		RED_LED_END;
		GREEN_LED_END;
		YELLOW_LED_START;
		break;

	case IDC_AUTO_PATTERN:
		t_flag::equipment_start = TRUE;
		YELLOW_LED_END;
		GREEN_LED_START;
		break;

	case IDC_CONTROL_PATTERN:
		t_flag::equipment_start = TRUE;
		RED_LED_END;
		GREEN_LED_END;
		YELLOW_LED_START;
		if (HWND_MOTION == NULL){
			HWND_MOTION = CreateDialog((HINSTANCE)GetWindowLong(hdlg, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_MOTION), hdlg, (DLGPROC)Dlg_Motion_Config);
		}
		ShowWindow(HWND_MOTION, SW_NORMAL);
		break;
		
	default:
		{
			//////////////////////////////////////////////////////////////////////////
			int i = id - ID_MENU_TOOL(0);
			if (i>=0 && i<TOOL_CNT){
				if (RESULT_LOAD_TOOL(i) == TRUE){
					char mystr[TOOL_CNT][MAX_PATH] = {0};
					strcpy(mystr[0], s_test::dir);
					strcpy(mystr[1], s_test::recipe_name);
					if (HWND_TOOL(i) == NULL){
						HWND_TOOL(i) = s_test::imtool[i].create_dlg(hdlg, s_test::img_buf, s_test::bmp_buf, s_test::ram_int, mystr, log);
					}
					ShowWindow(HWND_TOOL(i), SW_NORMAL);
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//hw
			i = id - ID_MENU_HW(0);
			if (i>=0 && i<HW_CNT){
				if (RESULT_LOAD_HW(i) == TRUE && HWND_HW(i) == NULL){
					HWND_HW(i) = s_test::imhw[i].create_dlg(hdlg, 0);
				}
				ShowWindow(HWND_HW(i), SW_NORMAL);
			}
		}

		break;
	}
}

LRESULT MDlg::ctlcolor_static(HWND hdlg, HDC hdc, HWND hcolor, int mark /* = CTLCOLOR_STATIC */ )
{	
	COLORREF cr;
	char text[20] = {0};
	if (static_brush != NULL) DeleteObject(static_brush);

	static_brush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)/*BRUSH_LIGHTGRAY*/);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, (HBRUSH)static_brush);
	HFONT hfont = CreateFont(14, 0,0,0,20,0,0,0,ANSI_CHARSET,3,2,1, VARIABLE_PITCH | FF_ROMAN, "黑体");
	HFONT old_font = (HFONT)SelectObject(hdc, hfont);

	SetBkMode(hdc, TRANSPARENT);

	switch (GetDlgCtrlID(hcolor))
	{
	case IDC_STATUS:
		DeleteObject(hfont);
		hfont = CreateFont(26, 0,0,0,20,0,0,0,ANSI_CHARSET,3,2,1, VARIABLE_PITCH | FF_ROMAN, "黑体");
		old_font = (HFONT)SelectObject(hdc, hfont);
		DeleteObject(static_brush);
		SetTextColor(hdc, COLOR_LIGHT_DARK);
		switch (APP_STATUS)
		{
		case APP_ERR:	  cr = COLOR_RED;				strcpy(text, "Error");		break;
		case APP_NOTHOME:  cr = COLOR_TOMATO;			strcpy(text, "Not home");	break;
		case APP_READY:	  cr = COLOR_MEDIUM_STATE_BLUE;	strcpy(text, "Ready");		break;
		case APP_INIT:	  cr = COLOR_MAGENTA;			strcpy(text, "Init...");	break;
		case APP_TESTING: cr = COLOR_GOLD;				strcpy(text, "Test...");	break;
		case APP_START:	  cr = COLOR_YELLOW_GREEN;		strcpy(text, "Start");		break;
		case APP_NEXT:	  cr = COLOR_MAGENTA;			strcpy(text, "Next");		break;
		case APP_STOP:	  cr = COLOR_TOMATO;			strcpy(text, "Stop");		break;
		case APP_PASS:    cr = COLOR_GREEN;				strcpy(text, "Pass");		break;
		case APP_FAIL:	  cr = COLOR_RED;				strcpy(text, "Fail");		break;
		case APP_RESET:	  cr = COLOR_PLUM;				strcpy(text, "Reset");		break;

		default:		  cr = COLOR_RED;				strcpy(text, "Unknown");	break;
		}
		static_brush = CreateSolidBrush(cr);
		SetDlgItemText(hdlg, IDC_STATUS, text);
		break;

	case IDC_STATIC_result:
	case IDC_STATIC_mtfScore:
	case IDC_STATIC_process:
	case IDC_STATIC_sn:
	case IDC_STATIC_PLC:
	case IDC_STATIC_IMG_SOURCE:
	case IDC_STATIC_c:
	case IDC_STATIC_lt:
	case IDC_STATIC_rt:
	case IDC_STATIC_lb:
	case IDC_STATIC_rb:
		SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
		DeleteObject(static_brush);
		static_brush = CreateSolidBrush(RGB(189,199,215));//注释背景色
		break;

	case IDC_LINK_PLC:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);
		if (!strcmp(text, "Connected")){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	
	case IDC_REPORT_SFR_C:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);

		if (mysbw.sfr_val1[0][AA_SELECT_ROI]>(1.0*mysbw.spec1[0])){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		break;
	case IDC_REPORT_SFR_RB:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);

		if (mysbw.sfr_val1[4][0]>(1.0*mysbw.spec1[1])){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		break;
	case IDC_REPORT_SFR_RT:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);

		if (mysbw.sfr_val1[2][0]>(1.0*mysbw.spec1[1])){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		break;
	case IDC_REPORT_SFR_LB:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);

		if (mysbw.sfr_val1[3][2]>(1.0*mysbw.spec1[1])){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		break;
	case IDC_REPORT_SFR_LT:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);

		if (mysbw.sfr_val1[1][0]>(1.0*mysbw.spec1[1])){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		break;
	case IDC_REPORT_CENTER_X:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(abs(mysbw.oc_x[0]-IMG_WIDTH/2)<(double)mysbw.oc_limit){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;	
	case IDC_REPORT_CENTER_Y:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(abs(mysbw.oc_y[0]-IMG_HEIGHT/2)<(double)mysbw.oc_limit){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;	
	case IDC_REPORT_eCoorDiff:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(abs(mysbw.eCoorDiff)<(double)mysbw.coorDiff_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;	
	case IDC_REPORT_eSfrDiff:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(abs(mysbw.eSfrDiff)<(double)mysbw.sfrDiff_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_REPORT_SFR_CENTER:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(mysbw.sfr_val1[0][2]>(double)mysbw.spec2[0]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_REPORT_SFR_EDGE:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		
		for(int i=1;i<5;i++){
			DeleteObject(static_brush);
			if(mysbw.sfr_val1[i][0]>(double)mysbw.spec2[1]||mysbw.sfr_val1[i][2]>(double)mysbw.spec2[1]){
				static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
			}
			else{
				static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
			}
		}
		break;
	case IDC_GRAY_1:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(y_ratio[0]>=st.gray_luminance_change_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_GRAY_2:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(y_ratio[1]>=st.gray_luminance_change_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_GRAY_3:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(y_ratio[2]>=st.gray_luminance_change_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_GRAY_4:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(y_ratio[3]>=st.gray_luminance_change_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_GRAY_5:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(y_ratio[4]>=st.gray_luminance_change_spec){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_1:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[0]<=st.color_spec[0]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_2:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[1]<=st.color_spec[1]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_3:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[2]<=st.color_spec[2]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_4:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[3]<=st.color_spec[3]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_5:
		SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[4]<=st.color_spec[4]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
	case IDC_COLOR_DE_6:
		SetTextColor(hdc,RGB(0xff, 0xff, 0xff));
		DeleteObject(static_brush);
		if(color_delta_val[5]<=st.color_spec[5]){
			static_brush = CreateSolidBrush(RGB(0x70, 0xad, 0x47));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;
/*
	case IDC_PROCESS_AA_MOTION:
	case IDC_PROCESS_CHECK1:*/
/*
	case IDC_PROCESS_CHECK2:
		GetDlgItemText(hdlg, GetDlgCtrlID(hcolor), text, sizeof(text));
		DeleteObject(static_brush);
		if (!strcmp(text, "OK")){
			static_brush = CreateSolidBrush(RGB(0x00, 0xff, 0x00));
		}
		else if (!strcmp(text, "NG")){
			static_brush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
		}
		else{
			static_brush = CreateSolidBrush(RGB(0xff, 0x11, 0x11));
		}
		break;*/
}
	DeleteObject(hfont);
	SelectObject(hdc, (HFONT)old_font);
	return (LRESULT)static_brush;
}

void MDlg::draw_item(HWND hdlg, const DRAWITEMSTRUCT *pdis)
{
	int icon_id = 0;
	switch (pdis->CtlID)
	{
	case IDC_START:		icon_id = IDI_START; break;
	case IDC_STOP:		icon_id = IDI_STOP;	 break;
	case IDC_NEXT:		icon_id = IDI_NEXT;	 break;
	case IDC_SAVE:		icon_id = IDI_SAVE;	 break;
	case IDC_SNAPSHOT:	icon_id = IDI_SNAPSHOT;	 break;
	default:
		return;
		break;
	}

	if (pdis->CtlType == ODT_BUTTON){
		int style = BUTTON_FRAME_FLAT | BUTTON_IMG_ICON | BUTTON_INNER_IMGTEXT;
		if (icon_id==IDI_SNAPSHOT){
			style = BUTTON_FRAME_FLAT | BUTTON_IMG_ICON | BUTTON_INNER_IMGONLY;
		}
		
		ai_win::draw_own_button(hdlg, pdis, icon_id, style);
	}
}

void MDlg::close(HWND hdlg)
{
	IMG_CAPTURE_READY = FALSE;
	if (TEST_START)	ITEM_ACTIVE(HWND_MAIN, IDC_STOP);

	Sleep(100);

	RECT rt_mdlg, rt_tc;
	GetWindowRect(HWND_MAIN, &rt_mdlg);
	s_test::dinfo.mdlg_x = rt_mdlg.left;
	s_test::dinfo.mdlg_y = rt_mdlg.top;
	GetWindowRect(HWND_TEST_CONFIG, &rt_tc);
	s_test::dinfo.tcdlg_x = rt_tc.left;
	s_test::dinfo.tcdlg_y = rt_tc.top;

	RECT rt;
	GetWindowRect(HWND_APP, &rt);
	s_test::dinfo.rt_win = rt;
	GetWindowRect(HWND_LOG, &rt);
	s_test::dinfo.rt_log = rt;
	GetWindowRect(HWND_MOTOR_MOTION, &rt);
	s_test::dinfo.aadlg_x = rt.left;
	s_test::dinfo.aadlg_y = rt.top;
	ai_win::save_keys(s_test::config_path, "dlg", NULL, config_dlg, 100);

	for (int i=0; i<TOOL_CNT; i++){
		if (s_test::ram_int[200+i] == TRUE){
			s_test::imtool[i].destroy_dlg();
			Sleep(10);
		}
	}
	s_test::dinfo.show_io1_dlg = FALSE;
	s_test::dinfo.show_io2_dlg = FALSE;
	PostQuitMessage(0);
}

DWORD CALLBACK extern_in_handshake_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	int index = 0;
	int cnt = 0;
	int status = TRUE;
	HANDSHAKE_GET_SHM = FALSE;

	log("Thread:3rd Party Image Device Handshake Loop Ready.");

	while (1){
		while (HANDSHAKE_GET_SHM == FALSE) {
			if (SELECT_IMG_SOURCE == IMG_SOURCE_FILE){
				SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_LINK, "Image File");
				if (HANDSHAKE_GET_SHM == TRUE){
					close_shm();
					HANDSHAKE_GET_SHM = FALSE;
				}
			}
			else if (SELECT_IMG_SOURCE == IMG_SOURCE_INTERNAL_DEVICE){
				SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_LINK, "Internal Device");
				if (HANDSHAKE_GET_SHM == TRUE){
					close_shm();
					HANDSHAKE_GET_SHM = FALSE;
				}
			}
			else{
				SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_LINK, connect_status[status]);
				status ^=TRUE;
				index = 0;
				if (SELECT_IMG_SOURCE == IMG_SOURCE_EXTERN_DEVICE){
					close_shm();
					HANDSHAKE_GET_SHM = open_shm(HWND_APP);
					ITEM_GRAYED(hdlg,  IDC_START);
					ITEM_GRAYED(hdlg,  IDC_STOP);
				}
			}
			Sleep(500); 
		}

		switch (index)
		{
		case 0: // linked, disable all function related to image buf;
			ITEM_GRAYED(hdlg, IDC_START);
			ITEM_GRAYED(hdlg, IDC_STOP);
			ITEM_GRAYED(hdlg, IDC_NEXT);

			SetDlgItemText(hdlg, IDC_CONNECT_STATUS, "Stream Mode OK!");
			SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_LINK, "Stream Connected");
			index = 10;
			break;

		case 10:  // send trigger signal, wait for feedback
			HANDSHAKE_TRIGGER_SIGNAL = TRUE;
			index = 20;
			break;

		case 20:// wait for image capture b/d send ready signal
			if (HANDSHAKE_TRIGGER_SIGNAL == FALSE){
				ITEM_ENABLED(hdlg, IDC_START);
				ITEM_ENABLED(hdlg, IDC_STOP);
				IMG_WIDTH  = EXT_CAM_IMG_WIDTH;
				IMG_HEIGHT = EXT_CAM_IMG_HEIGHT;
				IMG_DEPTH  = EXT_CAM_IMG_DEPTH;

				DISPLAY_IMG_START_X = 0;
				DISPLAY_IMG_START_Y = 0;
				DISPLAY_IMG_WIDTH  = IMG_WIDTH;
				DISPLAY_IMG_HEIGHT = IMG_HEIGHT;

				HANDSHAKE_LINKED = TRUE;
				SetDlgItemText(hdlg, IDC_CONNECT_STATUS, "Connected");
				index = 30;
			}
			break;

		case 30:
			HANDSHAKE_TRIGGER_SIGNAL = TRUE;
			index = 40;
			break;

		case 40:
			if (SELECT_IMG_SOURCE == IMG_SOURCE_FILE || SELECT_IMG_SOURCE == IMG_SOURCE_INTERNAL_DEVICE){
				index = 100;
			}
			else{
				if (HANDSHAKE_TRIGGER_SIGNAL == FALSE){
					index = 30;
					cnt = 0;
				}
				else{
					if (++cnt >5){
						cnt = 0;
						index = 100;
					}
				}
			}
			Sleep(500);
			break;

		case 100:
			HANDSHAKE_LINKED  = FALSE;
			HANDSHAKE_GET_SHM = FALSE;
			log("Handshake:Lost 3rd Party Image Device Link!");
			break;

		}
		Sleep(10);
	}
}
DWORD CALLBACK led_display(LPVOID temp)
{
	COMPUTER_LED_START;
	while(1)
	{
		if (EQUIPMENT_START_SENSOR)
		{
			EQUIPMENT_LED_START;
			Sleep(50);
		}
		if (STOP_SENSOR)
		{
			EQUIPMENT_LED_END;
			RED_LED_START;
			Sleep(50);
		}
		if (!EMERGENCY_STOP_SENSOR)
		{		
			ALL_CYLINDER_CLOSE;
			EQUIPMENT_LED_END;
			RED_LED_START;
			Sleep(20);
		}
		while(led_flag::led_alarm_flag == TRUE)					//复位按键
		{
			if (RESET_SENSOR)
			{
				RED_LED_END;
				YELLOW_LED_START;
				RESET_LED_END;
				led_flag::led_alarm_flag = FALSE;
			}
			Sleep(50);
		}
		if(S67)
		{
			if(S63)
			{
				Y35_ON;
				Y46_ON;
				Y47_ON;
				Sleep(50);
			}
			if (S64)
			{
				//t_flag::ng_tray_reset = TRUE;
				Y46_OFF;
				Y35_OFF;
				Y47_OFF;
			}
		}
		if(S68)
		{
			if(S65)
			{
				Y37_ON;
				Y48_ON;
				Y49_ON;
			}
			if (S66)
			{
				Y37_OFF;
				Y48_OFF;
				Y49_OFF;
			}
		}
		Sleep(50);
	}
}
DWORD CALLBACK mouse_operation_thread(LPVOID temp)
{
	char str[MAX_PATH] = {0};
	while (1){
		if (MOUSE_LDBCLICK == TRUE){
			if (ENABLE_IMG_ZOOM_IN == TRUE){

				DRAW_ZOOM_IN_AREA_DONE = FALSE;
				DISPLAY_IMG_START_X = 0;
				DISPLAY_IMG_START_Y = 0;
				DISPLAY_IMG_WIDTH  = IMG_WIDTH;
				DISPLAY_IMG_HEIGHT = IMG_HEIGHT;
				SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_TEXT, "");
			}
			MOUSE_LDBCLICK = FALSE;
		}

		if (MOUSE_LDOWN == TRUE){
			//zoom in;
			if (ENABLE_IMG_ZOOM_IN && DRAW_ZOOM_IN_AREA_DONE == FALSE){
				MOUSE_START_X = MOUSE_X-628;
				MOUSE_START_Y = MOUSE_Y-30;
			}

			if (ENABLE_MEASURE == TRUE && DRAW_ZOOM_IN_AREA_DONE != TRUE){
				MEASURE_DRAW_RECT_DONE = FALSE;
				MEASURE_START_X = MOUSE_X-628;
				MEASURE_START_Y = MOUSE_Y-30;
			}
			MOUSE_LDOWN = FALSE;
		}

		if (MOUSE_LDOWN_MOVE == TRUE){
			if (ENABLE_IMG_ZOOM_IN && DRAW_ZOOM_IN_AREA_DONE == FALSE){
				MOUSE_END_X = MOUSE_X-628;
				MOUSE_END_Y = MOUSE_Y-30;
				DRAW_MOVING_MOUSE = TRUE;
			}

			if (ENABLE_MEASURE == TRUE && DRAW_ZOOM_IN_AREA_DONE != TRUE){
				MEASURE_END_X = MOUSE_X-628;
				MEASURE_END_Y = MOUSE_Y-30;
			}
			MOUSE_LDOWN_MOVE = FALSE;
		}

		if (MOUSE_LUP == TRUE){
			if (ENABLE_IMG_ZOOM_IN && DRAW_ZOOM_IN_AREA_DONE == FALSE && DRAW_MOVING_MOUSE == TRUE){
				DRAW_MOVING_MOUSE = FALSE;
				Sleep(100);

				MOUSE_END_X = MOUSE_X;
				MOUSE_END_Y = MOUSE_Y;
				DISPLAY_IMG_START_X += (int)(MOUSE_START_X*IMG_RATIO_X);
				DISPLAY_IMG_START_Y += (int)(MOUSE_START_Y*IMG_RATIO_Y);
				DISPLAY_IMG_WIDTH    = (int)((MOUSE_END_X-MOUSE_START_X)*IMG_RATIO_X);
				DISPLAY_IMG_HEIGHT   = (int)((MOUSE_END_Y-MOUSE_START_Y)*IMG_RATIO_Y);

				log("zoom in: x=%d, y=%d, w=%d, h=%d", DISPLAY_IMG_START_X, DISPLAY_IMG_START_Y, DISPLAY_IMG_WIDTH, DISPLAY_IMG_HEIGHT);
				DRAW_ZOOM_IN_AREA_DONE = TRUE;
			}

			if (ENABLE_MEASURE && DRAW_ZOOM_IN_AREA_DONE != TRUE){
				MEASURE_END_X = MOUSE_X;
				MEASURE_END_Y = MOUSE_Y;

				if (MEASURE_END_X>MEASURE_START_X && MEASURE_END_Y>MEASURE_START_Y){
					MEASURE_DRAW_RECT_DONE = TRUE;
				}
			}
			MOUSE_LUP = FALSE;
		}

		Sleep(10);
	}//while loop

	return FALSE;
}
DWORD CALLBACK error_monitor_thread(LPVOID temp)
{
	char err_str[MAX_PATH] = {0};
	while(1)
	{
		while (ERROR_ON == FALSE) Sleep(10);

		set_status(APP_ERR);
		sprintf(err_str, "[%d]Place Error Code Str Here!!!", ERROR_CODE);
		SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_TEXT, err_str);
	}
	return FALSE;
}
DWORD CALLBACK data_display(LPVOID temp)
{
	while(1)
	{
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_1,color_delta_val[0],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_2,color_delta_val[1],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_3,color_delta_val[2],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_4,color_delta_val[3],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_5,color_delta_val[4],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_COLOR_DE_6,color_delta_val[5],2);
		
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_GRAY_1,y_ratio[0],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_GRAY_2,y_ratio[1],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_GRAY_3,y_ratio[2],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_GRAY_4,y_ratio[3],2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_GRAY_5,y_ratio[4],2);

		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_CENTER_X,		 mysbw.oc_x[0], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_CENTER_Y,		 mysbw.oc_y[0], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_SFR_C,			 mysbw.sfr_val1[0][AA_SELECT_ROI], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_SFR_LT,		 mysbw.sfr_val1[1][2], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_SFR_RT,		 mysbw.sfr_val1[2][0], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_SFR_LB,		 mysbw.sfr_val1[3][2], 2);
		ai_win::set_dlg_item_float(HWND_MAIN, IDC_REPORT_SFR_RB,		 mysbw.sfr_val1[4][0], 2);

		Sleep(50);
	}
}
//============自动模式==============
DWORD CALLBACK site_com(LPVOID temp)
{
	while(1)
	{
		while(t_flag::equipment_start == TRUE)
		{
			if (S75)
			{
				t_flag::feed_product = TRUE;
				Sleep(50);
			}
			else if (!S75)
			{
				t_flag::feed_product = FALSE;
				Sleep(50);
			}
			if (S74)
			{
				t_flag::feed_request_product = TRUE;
				Sleep(50);
			}
			else if (!S74)
			{
				t_flag::feed_request_product = FALSE;
				Sleep(50);
			}
			Sleep(10);
		}
		Sleep(50);
	}
	return 0 ;
}