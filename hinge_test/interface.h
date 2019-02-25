#ifndef _INTERFACE_H_
#define _INTERFACE_H_
// #define BCCWIN
// #define DOEXPORT
// #include "nodavesimple.h"
// #include "openSocket.h"
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <direct.h>
#include <assert.h>
#include <CommCtrl.h>
#include <io.h>
#include "CameraDS.h"
#include "s_win.h"
#include <s_img.h>
#include <s_chart.h>
#include <s_sfr.h>
#include <s_white.h>
#include <s_math.h>
#include <s_comm.h>
#include "FreeImage.h"
#include "s_define.h"
#include "resource.h"
#include "import_dll.h"
#include "DXMediaCap.h"
#include "test_reg_mark_chart.h"
#include "test_std_chart.h"
#include "mn200.h"
#include "tcp_ip.h"
#include "img_cap_device.h"
#include "color_table.h"
#include "defio.h"
#include "test_white.h"
#include "tcpip.h"
#include "motion_cfg.h"
#include "test_light.h"



#define WINDOW_INITIAL_WIDTH		1024
#define WINDOW_INITIAL_HEIGHT		768

#define MDLG_DEFAULT_HEIGHT			140
#define MDLG_EXTEND_HEIGHT			210

#define APP_ERR						-1
#define APP_READY					0
#define APP_START					1
#define APP_STOP					2
#define APP_NEXT					3
#define APP_NOTHOME					4
#define APP_INIT					10
#define APP_TESTING					11
#define APP_RESET					12
#define MAX_MARK_CNT				2000
#define APP_FAIL					99
#define APP_PASS					100

//#define OFFLINE_TEST
//#define COMMUNICATION_WITHOUT_AA
#define USE_FINE_CROSS_SEARCH

#define PACK_SIZE		256

typedef struct mark
{
	int x;
	int y;
}MARK_RED;
/*
typedef struct _tag_TCPIP_V
{
	ai_comm::TCPIP inf;
	int is_linked;
	int port;
	char ip[20];
	char pack[PACK_SIZE];
	int  is_pack_empty;
}TCPIP_V;*/
extern int CARRY_START;
extern int CARRY_ERR;

typedef struct DEVICE_INFO
{
	//	CChannelWnd	*m_pWnd;
	int dwHeight;
	int dwWidth;
	float dwFrameRate;
	int dwColorspace;
	int dwVidStandard;
	unsigned  dwOpenDevState;
	BOOL dwConnectState;
	BOOL  dwPrviewState;
	HANDLE dwChannelHandle;
	HWND dwPrvHandle;
	RECT dwPrvRect;
	PRECT dwSnapPicRect;

	DWORD			dwDispWidth;                     //预览显示宽度
	DWORD			dwDispHeight;                    //预览显示高度
	DWORD			dwDispPosX;                      //预览显示水平起始位置
	DWORD			dwDispPosY; 

}Device_Info;

typedef struct _tagDlgInfo
{
	int mdlg_x;
	int mdlg_y;
	int tcdlg_x;
	int tcdlg_y;
	int aadlg_x;
	int aadlg_y;
	RECT rt_win;
	RECT rt_log;
	int show_log;
	int show_tc;
	int mdlg_extend;
	int show_recipe;
	int show_img_info;
	int show_hw_config;
	int show_aa_dlg;
	int show_io1_dlg;
	int show_io2_dlg;
	int show_tcpip;

}DLGINFO;

typedef struct _tagTestConfig
{
	int test_item_cnt;
	char item_str[TEST_CNT][100];
	int  item_on[TEST_CNT];
	int  item_mode[TEST_CNT];
	int  item_order[TEST_CNT];
	int  under_testing[TEST_CNT];

	float test_time[TEST_CNT];
	int   test_result[TEST_CNT];
	int   activated[TEST_CNT];

	char fixed_item_str[FIXED_TEST_CNT][100];
	int	 fixed_item_on[FIXED_TEST_CNT];
	int  fixed_item_mode[FIXED_TEST_CNT];
	int  fixed_item_order[FIXED_TEST_CNT];
	int  fixed_under_testing[FIXED_TEST_CNT];

	float fixed_test_time[FIXED_TEST_CNT];
	int fixed_test_result[FIXED_TEST_CNT];
	int fixed_activated[FIXED_TEST_CNT];

	int  display_cycle_image;
}TestConfig;

typedef struct _tagGuideLine
{
	int show_center_line;
	int center_line_width;
	int center_line_type;
	int center_line_color;
	COLORREF center_cr;

	int show_field_line;
	int field_line_width;
	int field_line_type;
	int field_line_color;
	float field_location;
	COLORREF field_cr;

}GuideLine;

class MyImage:public ai_img::Output
{
public:
	virtual void draw_on_window(HDC memdc, LPVOID temp);

	int get_start_x(void){return start_x;}
	int get_start_y(void){return start_y;}
	int get_full_sized(void){return full_sized;}
	int get_img_option(void){return img_option;}

	void set_start_x(int x){start_x = x;}
	void set_start_y(int y){start_y = y;}
	void set_full_sized(int b){full_sized = b;}
	void set_img_option(int option){img_option = option;}

private:
	int start_x;
	int start_y;
	int full_sized;
	int img_option;
	HPEN pen, old_pen;
	HFONT hfont, old_font;
};

typedef struct _tagTestOption
{
	int on;
	int processed;
	int draw_result_on;
	int order;
	int mode;	// 1. cycle, 0. check
}TestOption;

typedef struct _tagSlantedBW_SM
{
	double pixel_size;
	int	   fs_index;
	int	   fs_peak_index[SLANTED_BW_NUM];
	int    select_roi;
	int	   find_target_num;
	double tilt_angle[2];
	double sfr_val[4][SLANTED_BW_NUM][TF_MAX_CNT];
	double oc_x[TF_MAX_CNT][SLANTED_BW_NUM];
	double oc_y[TF_MAX_CNT][SLANTED_BW_NUM];
	double fs_z_pos[TF_MAX_CNT];

	//////////////////////////////////////////////////////////////////////////
// 	double oc_x[SLANTED_BW_NUM];
// 	double oc_y[SLANTED_BW_NUM];
// 	double sfr_val1[SLANTED_BW_NUM][4];

}SlantedBW_SM;


/************************************************************************/
/* test on/off: normal test process on and off status control
   test stop: sometimes we need sudden stop or interrupt test process.	
*/
/************************************************************************/
class Test
{
public:
	Test();
	
// 	int  check_unprocessed_items(void);
 	void reset_test_items(void);
	int  get_next_item(void);
	void set_item_tested(int item_id);

	void draw_test_item(int item_id);

	int set_test_on(void);
	void set_test_off(void);
	int get_test_on(void);

	void set_test_stop(void);
	int get_test_stop(void);

	HANDLE get_event(void);
	int create_event(void);
	void close_event(void);

	HANDLE get_mutex(void);
	int create_mutex(void);
	void close_mutex(void);

private:

	HANDLE test_event;
	HANDLE test_mutex;
	int is_under_testing;
	int stop_testing;

};

namespace s_test
{
	extern HMENU	hmenu;
	extern HANDLE   hshm;

	extern DLGINFO				dinfo;
	extern TestConfig			tconf;
	extern GuideLine			gline;

	extern MyImage				myimg;
	extern ai_img::Convert		cimg;

	extern Test					test;
	extern TestOption			test_option[TEST_ITEM_NUM];

	extern char io_display_string_output[500][500];
	extern char io_display_string_input[500][500];

	extern ImportTool			imtool[TOOL_CNT];
	extern ImportHW				imhw[HW_CNT];
	extern ImportHW				*pimhw;

	extern SlantedBW_SM			mysbw_sm;

	

	extern ImgCapDev			*cap_dev;
	extern ImgFile				img_file;	
	extern CVBS_TSW_306E		img_hw_cvbs_tsw_306e;
	extern CTS					cts;
	extern Hinge				hinge;

	extern void read_io_display_string_output(HWND hdlg);
	extern void read_io_display_string_input(HWND hdlg);
	extern void io_status_display();

	extern Device_Info dev[23];
	extern IplImage *frame ; 

	extern char dir[MAX_PATH];
	extern char config_path[MAX_PATH];
	extern char ps_config_path[MAX_PATH];
	extern char io_output_path[MAX_PATH] ;
	extern char io_input_path[MAX_PATH];
	extern char recipe_name[100];
	extern char shm_name[MAX_PATH];
	extern char img_format_str[7][100];
	extern char sn_str[MAX_PATH];
	extern char plc_ip[20];

	extern HWND	 hwnd[100];
	extern int	 ram_int[RAM_INT_NUM];
	extern double ram_double[RAM_DOUBLE_NUM];
	extern BYTE  img_buf[MAX_IMG_SIZE*2];
	extern BYTE  raw8_buf[MAX_IMG_SIZE];
	extern BYTE  bmp_buf[MAX_IMG_SIZE*3];
	extern BYTE  bmp_buf2[MAX_IMG_SIZE*3];
	extern BYTE  bmp_buf_t[MAX_IMG_SIZE*3];

	extern BYTE bmp_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE shrink_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE scan_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE filter_close_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3];
	extern BYTE bmp_f[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3];
	extern BYTE bmp_large_x[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE bmp_large_y[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE bmp_y_buf_copy[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];

	extern BYTE bmp_bin[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
	extern BYTE bmp_y_cutareas[MAX_IMG_WIDTH*MAX_IMG_HEIGHT];
}

extern void log(const char *format, ...);
extern void set_status(int status);
extern int  open_shm(HWND hdlg);	
extern int  close_shm(void);
extern void create_report_folder(void);
extern void update_scenery_title(char *img_name, char *img_size, char *img_type, char *img_fps);
extern void update_mdlg_title(HWND hdlg);

//////////////////////////////////////////////////////////////////////////
//image.cpp
extern int get_img_stream(void);
extern int parse_raw(BYTE *raw, BYTE *bmp, int w, int h, float read_pixel_width);
extern int read_img_file(char *img_path);
extern int save_img_file(HWND hdlg);
extern int save_scenery_image(HWND hdlg);
extern int save_info_image(HWND hdlg);
extern int save_stream_to_image_file(BYTE *stream, int img_format, char *path);
extern int average_img(BYTE *data, int *buffer, BYTE **data_array, int pixel_count, int *counting, int avg_cnt);

extern int create_report_file(const char *test_item_str, char *target_report_folder_path, char *target_folder_name);

extern BOOL open_single_img_file(HWND hdlg);

//////////////////////////////////////////////////////////////////////////
//standard sfr chart
extern int init_std_chart_test(void);
extern int exec_std_chart_test(BYTE *bmp, int bmp_w, int bmp_h);
extern void draw_std_chart(HDC memdc);

//reg mark chart
extern void draw_reg_mark_chart(HDC memdc);
extern int init_reg_mark_chart_test(void);
extern int SfrResult(int model);
extern int exec_reg_mark_chart_test(BYTE *img, BYTE *bmp, int w, int h);
extern void save_reg_mark_report(BYTE *bmp, int img_w, int img_h);
extern void save_reg_mark_report_c(BYTE *bmp, int img_w, int img_h, int type);
extern void save_reg_mark_report_ok(BYTE *bmp, int img_w, int img_h);
extern void save_reg_mark_report_ng(BYTE *bmp, int img_w, int img_h);

//white test
extern int init_white_test(void);
extern int exec_white_test(BYTE *bmp, int w, int h);
extern void draw_white_test(HDC memdc);

extern int connect_device(int id);
extern void disconnect_device(int id);

extern DWORD WINAPI position_display_thread(LPVOID temp);
extern DWORD WINAPI button_process_thread(LPVOID temp);

extern int motor_product(void);
extern int motor_ng_product(void);
extern int motor_home_product(void);
extern int feed_cylinder_step2(void);
extern int motion_product(void);
extern int motion_ng_product(void);

extern int first_cylinder_cfg(int i);	
extern int second_cylinder_cfg(int i);	
extern int third_cylinder_cfg(int i);	
extern int fourth_cylinder_cfg(int i);	
extern int dark_box_cfg(int i);			
extern int light_box_cfg(int i);		
extern int feed_cylinder_cfg(int i);	
extern int gripper_move_cfg(int i);		
extern int product_cylinder_cfg(int i);
extern int gripper_support_cfg(int i);	
extern int gripper_cylinder_cfg(int i);
extern void all_cylinder_cfg(void);
extern int product_gripper_support_cfg(int i);
extern int product_gripper_cfg(int i);
extern int pass_tray_cylinder(int i);
extern int stack_cylinder(int i);
extern int tray_block_cylinder(int i);
extern int ng_tray_cylinder(int i);

extern void ng_reset_led(void);
extern void pass_reset_led(void);

extern int left_gripper_motion(void);
extern int all_gripper_motion(void);
extern int right_gripper_motion(void);
#endif