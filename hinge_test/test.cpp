//#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "inf_motion_aa.h"
#include "inf_cap_bd.h"
#include "cts.h"
using namespace s_test;

#define TEST_LOOP_MODE	0
#define TEST_CHECK_MODE	1

extern INIEdit st_fm[];
extern INIEdit st_sfr[];
extern INIEdit st_color[];
extern INIEdit st_gray[];
extern INIEdit st_etc[];
extern SlantedBW mysbw;

extern void YCbCrITUToBmp(int iMode, BYTE *pYuvBuffer, BYTE *pBmpBuffer, int size_x, int size_y);
extern BOOL CALLBACK reg_mark_chart_sfrcurve_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam);
extern BOOL WaitForFirstFrameArrived(DWORD devInst);
unsigned __stdcall raw_video_callback(unsigned char* buffer, unsigned colorSpace,
	unsigned width, unsigned height, unsigned bytesWidth, void* context)
{

	s_test::cimg.yuv422_to_bmp24(0, buffer, s_test::bmp_buf, width, height);

	return 0;
}

ai_img::Output myout;

Test::Test()
{
	test_event = NULL;
	is_under_testing = FALSE;

	for (int i=0; i<TEST_ITEM_NUM; i++){
		s_test::test_option[i].on = FALSE;
		s_test::test_option[i].processed = FALSE;
		s_test::test_option[i].draw_result_on = FALSE;
		s_test::test_option[i].order = 0;
	}
}

int Test::set_test_on()
{
	is_under_testing = TRUE;
	stop_testing = FALSE;
	return TRUE;
}

void Test::set_test_off()
{
	is_under_testing = FALSE;
}

int Test::get_test_on()
{
	return is_under_testing;
}

void Test::set_test_stop()
{
	stop_testing = TRUE;
}

int Test::get_test_stop()
{
	return stop_testing;
}

HANDLE Test::get_event()
{
	return test_event;
}

int Test::create_event()
{
	if (test_event == NULL){
		test_event = CreateEvent(NULL, TRUE, TRUE, "test_event");
	}

	return (test_event==NULL)?FALSE:TRUE;
}

void Test::close_event()
{
	CloseHandle(test_event);
	test_event = NULL;
}

HANDLE Test::get_mutex()
{
	return test_mutex;
}

int Test::create_mutex()
{
	if (test_mutex == NULL){
		test_mutex = CreateMutex(NULL, FALSE, "test_mutex");
	}

	return (test_mutex==NULL)?FALSE:TRUE;
}

void Test::close_mutex()
{
	int result = ReleaseMutex(test_mutex);
	log("release mutex result:%d", result);
}

void Test::reset_test_items()
{
	for (int i=0; i<TEST_ITEM_NUM; i++){
		s_test::test_option[i].processed = FALSE;
		s_test::test_option[i].draw_result_on = FALSE;
	}
}

int Test::get_next_item()
{
	int min_index = 100;
	int next_item_index = -1;
	for (int i=0; i<TEST_ITEM_NUM; i++){
		if (s_test::test_option[i].on && s_test::test_option[i].processed == FALSE){
			if (min_index>s_test::test_option[i].order){
				min_index = s_test::test_option[i].order;
				next_item_index = i;
			}
		}
	}

	return next_item_index;
}

void Test::set_item_tested(int item_id)
{
	if (item_id>=0 && item_id<TEST_ITEM_NUM){
		s_test::test_option[item_id].processed = TRUE;
	}
}

void Test::draw_test_item(int item_id)
{
	for (int i=0; i<TEST_ITEM_NUM; i++){
		if (i == item_id){
			s_test::test_option[i].draw_result_on = TRUE;
		}
		/*else s_test::test_option[i].draw_result_on = FALSE;*/
	}
}

DWORD CALLBACK showtimeThread(LPVOID temp){
	HWND hdlg = (HWND) temp;
	SYSTEMTIME st;

	char sTimeBuf[1024] = {0};
	log("start timer thread.");

	// 设置字体参数
	LOGFONT LogFont;
	::memset(&LogFont, 0, sizeof(LOGFONT));
	lstrcpy(LogFont.lfFaceName, "黑体");
 	LogFont.lfWeight = 0;
 	LogFont.lfHeight = -25; // 字体大小
	LogFont.lfEscapement = 5;

	// 创建字体
	HFONT hFont = CreateFontIndirect(&LogFont);

	// 取得控件句柄
	HWND hWndStatic = GetDlgItem(hdlg, IDC_TIME);

	// 设置控件字体
	::SendMessage(hWndStatic, WM_SETFONT, (WPARAM)hFont, 0); 
	while (1){
		GetLocalTime(&st);
		sprintf(sTimeBuf, "%04d/%02d/%02d %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond);
	//	SetTimer(hdlg, 1, 1000, NULL);//设定时器
		Sleep(1000);
		SetDlgItemText(HWND_MAIN, IDC_TIME, sTimeBuf);
		
	}

	return FALSE;
}

DWORD CALLBACK test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	int timeout = 0;
	int result = FALSE;
//	RECT rt;
	if (s_test::test.create_event() != TRUE){
		ERROR_ON = TRUE;
		ERROR_CODE = ERR_CREATE_TEST_EVENT;
	}

	if (s_test::test.create_mutex() != TRUE){
		ERROR_ON = TRUE;
		ERROR_CODE = ERR_CREATE_TEST_MUTEX;
	}

	int test_item_id = 0;
	while (1){
		while(TEST_START == FALSE) Sleep(10);
		//////////////////////////////////////////////////////////////////////////
		//initial image source 
		if (IMG_DEVICE_INITED == FALSE){
			log("start init img device.tester id = %d",TESTER_ID);

			result = s_test::cap_dev->init(TESTER_ID);
			log("init done.result = %d",result);
			if (result == TRUE){
				result = s_test::cap_dev->run();
			}

//			log("run done.result = %d",result);
			if (result == TRUE){
				IMG_DEVICE_INITED = TRUE;
// 				result = s_test::test.set_test_on();
// 				log("set test on.");
			}
			else{
				ERROR_ON = TRUE;
				ERROR_CODE = ERR_INIT_IMG_DEVICE;
				TEST_START = FALSE;
				continue;
			}
		}

		DISPLAY_IMG_WIDTH = IMG_WIDTH;
		DISPLAY_IMG_HEIGHT = IMG_HEIGHT;
		DISPLAY_IMG_START_X = 0;
		DISPLAY_IMG_START_Y = 0;
		IMG_DEPTH = 3;

		//////////////////////////////////////////////////////////////////////////
	//	log("test start active.");
		DWORD event_rt = WaitForSingleObject(s_test::test.get_event(), INFINITE);
		switch (event_rt){
		case WAIT_OBJECT_0:
			if (TEST_START == FALSE){
				continue;
			}
			break;
		default:
			log("wait event error.");
			TEST_START = FALSE;
			break;
		}

		log("received test event.");
		//////////////////////////////////////////////////////////////////////////
		//if test is processing, interrupt and stop!
		if (s_test::test.get_test_on() == TRUE){
			s_test::test.set_test_stop();
			timeout = 100;
			while (s_test::test.get_test_on() == TRUE && timeout-->0){
				Sleep(100);
			}

			if (timeout<=0){
				ERROR_ON = TRUE;
				ERROR_CODE = ERR_TEST_CAN_NOT_STOP;
				TEST_START = FALSE;
				continue;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		test_item_id = s_test::test.get_next_item();
		log("test_item_id=%d", test_item_id);

		switch (test_item_id){
		case TI_DISPLAY_IMAGE:
			START_DISPLAY_IMAGE = TRUE;
			log("process display image:");
			break;

		case TI_STD_CHART:
			START_STD_CHART_TEST = TRUE;
			log("process std chart test:");
			break;

		case TI_REG_MARK_CHART:
			START_REG_MARK_CHART_TEST = TRUE;
			log("process reg mark chart test:");
			break;

		case TI_WHITE:
			START_WHITE_TEST = TRUE;
			log("process white test:");
			break;
		}

		ResetEvent(s_test::test.get_event());
		Sleep(100);
	}

	log("quit test thread.");
	return FALSE;
}

DWORD CALLBACK get_image_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	log("start get img thread.");
	while (1){
		while (START_DISPLAY_IMAGE==FALSE&&START_REG_MARK_CHART_TEST == FALSE && IMG_DEVICE_INITED == FALSE) Sleep(10);

	//	log("Img w:%d, h:%d", DISPLAY_IMG_WIDTH, DISPLAY_IMG_HEIGHT);
	//	buf_size = DISPLAY_IMG_WIDTH*DISPLAY_IMG_HEIGHT*3;
		if(!WaitForFirstFrameArrived(TESTER_ID) && SELECT_IMG_SOURCE == IMG_DEV_CTS){
			log("wait for first frame.");
			continue;
		}
		while (s_test::test.get_test_stop() == FALSE){
			START_CHECK_TEST = TRUE;
			wait_rt = s_test::cap_dev->cap(s_test::img_buf,0);
			
//			CAP_READY = TRUE;
		}
		log("no cap.");
		START_DISPLAY_IMAGE = FALSE;
		s_test::test.close_mutex();	
		s_test::test.set_test_off();
	}

	return FALSE;
}

DWORD CALLBACK display_image_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	log("start display thread.");

	while (1){
		while (START_DISPLAY_IMAGE == FALSE) Sleep(10);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_DISPLAY_IMAGE = FALSE;
			continue;
		}
		s_test::test.set_test_on();
		s_test::test.set_item_tested(TI_DISPLAY_IMAGE);
		//////////////////////////////////////////////////////////////////////////
		//get image
		log("Img w:%d, h:%d", DISPLAY_IMG_WIDTH, DISPLAY_IMG_HEIGHT);
		while (s_test::test.get_test_stop() == FALSE){
			
			YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);//tsw306e
		//	log("mirror_x=%d,mirror_y=%d",IMG_MIRROR_X,IMG_MIRROR_Y);
			if(IMG_MIRROR_X){
				if(IMG_MIRROR_Y){
					for (int y=0; y<IMG_HEIGHT; y++){
						for (int x=0; x<IMG_WIDTH; x++){
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+1];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+2];
						}
					}
				}
				else{
					for (int y=0; y<IMG_HEIGHT; y++){
						for (int x=0; x<IMG_WIDTH; x++){
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+1];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+2];
						}
					}
				}
			}
			else {
				if(IMG_MIRROR_Y){
					for (int y=0; y<IMG_HEIGHT; y++){
						for (int x=0; x<IMG_WIDTH; x++){
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
						}
					}
				}
				else{
					for (int y=0; y<IMG_HEIGHT; y++){
						for (int x=0; x<IMG_WIDTH; x++){
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
							s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
						}
					}
				}
			}

			s_test::myimg.clear_bmp_stream();
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
							   s_test::bmp_buf, 
							   DISPLAY_IMG_WIDTH, 
							   DISPLAY_IMG_HEIGHT, 
							   IMG_DEPTH/*,
							   DISPLAY_IMG_START_X,
							   DISPLAY_IMG_START_Y,
							   FALSE,
							   IMG_DISPLAY_MODE*/);
			IMG_CAPTURE_READY = TRUE;
	//		log("3");
			//Sleep(1);
		}

		START_DISPLAY_IMAGE = FALSE;
		s_test::test.close_mutex();	
		s_test::test.set_test_off();
	}

	return FALSE;
}

DWORD CALLBACK std_chart_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	while (1){
		while (START_STD_CHART_TEST == FALSE) Sleep(10);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_STD_CHART_TEST = FALSE;
			continue;
		}

		result = s_test::test.set_test_on();
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_STD_CHART_TEST = FALSE;
			continue;
		}
		s_test::test.set_item_tested(TI_STD_CHART);

		//////////////////////////////////////////////////////////////////////////
		//init test

		init_std_chart_test();
		log("init std chart test.");
		if (s_test::test_option[TI_STD_CHART].mode == 0x00){ //loop
			log("enter into std chart loop test.");
			while (s_test::test.get_test_stop() == FALSE){
				//
				exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				s_test::test.draw_test_item(TI_STD_CHART);
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf, 
					IMG_WIDTH, 
					IMG_HEIGHT, 
					IMG_DEPTH);
				IMG_CAPTURE_READY = TRUE;
				Sleep(10);
			}
		}
		else{
			exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_STD_CHART);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
				s_test::bmp_buf, 
				IMG_WIDTH, 
				IMG_HEIGHT, 
				IMG_DEPTH);
		}

		START_STD_CHART_TEST = FALSE;
		s_test::test.close_mutex();
		
		s_test::test.set_test_off();
	}

	return FALSE;
}

/*
DWORD CALLBACK reg_mark_chart_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	while (1){
		log("start reg %d,START_DISPLAY_IMAGE=%d",START_REG_MARK_CHART_TEST,START_DISPLAY_IMAGE);
		while (START_REG_MARK_CHART_TEST == FALSE) {
		//	log("waiting...");
			Sleep(10);
			mysbw.oc_x[0] = mysbw.oc_y[0] = 0;
		}
		/ *while(START_SITE_REG_MARK_CHART_TEST == FALSE)	Sleep(100);* /
		log("start reg2 %d",START_REG_MARK_CHART_TEST);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_REG_MARK_CHART_TEST = FALSE;
			continue;
		}
		result = s_test::test.set_test_on();
		log("test on result: %d",result);
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_REG_MARK_CHART_TEST = FALSE;
			continue;
		}
	//	s_test::test.set_item_tested(TI_REG_MARK_CHART);
		//////////////////////////////////////////////////////////////////////////
		//init test
		result = init_reg_mark_chart_test();
		log("init reg result: %d",result);

		if (s_test::test_option[TI_REG_MARK_CHART].mode == 0x00){ //loop display
			log("rm w=%d, h=%d, img source=%d", IMG_WIDTH, IMG_HEIGHT,SELECT_IMG_SOURCE);
			while (s_test::test.get_test_stop() == FALSE){
				if(SELECT_IMG_SOURCE == IMG_DEV_CTS){ 
 			 		YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);
				}
				else if(SELECT_IMG_SOURCE == IMG_DEV_360E)
 					YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);//tsw306e
				
			//	log("trans bmp.");

				if(IMG_MIRROR_X){
					if(IMG_MIRROR_Y){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+2];
							}
						}
					}
				}
				else {
					if(IMG_MIRROR_Y){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
				}
			//	memcpy(s_test::bmp_buf,s_test::bmp_buf,IMG_WIDTH*480*3);
				exec_reg_mark_chart_test(NULL, s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				SfrResult(3);//画面显示红绿判断
				exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				s_test::test.draw_test_item(TI_STD_CHART);
				s_test::test.draw_test_item(TI_REG_MARK_CHART);
			//	START_CHECK_TEST = TRUE;
				IMG_CAPTURE_READY = TRUE;
				Sleep(10);
				s_test::myimg.clear_bmp_stream();
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf, 
					IMG_WIDTH,IMG_HEIGHT,IMG_DEPTH,0,0);
			}
		}
		else{
		//	get_rm_image(TRUE);
			exec_reg_mark_chart_test(NULL, s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_STD_CHART);
			s_test::test.draw_test_item(TI_REG_MARK_CHART);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY),
				s_test::bmp_buf, 
				IMG_WIDTH,
				IMG_HEIGHT,
				IMG_DEPTH);
			IMG_CAPTURE_READY = TRUE;
		}	
	//	ShowWindow(HWND_REG_MARK_SFRCURVE, SW_HIDE);
		START_REG_MARK_CHART_TEST = FALSE;
		
		s_test::test.close_mutex();
		s_test::test.set_test_off();
	}

	return FALSE;
}*/
DWORD CALLBACK reg_mark_chart_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	while (1){
		log("start reg %d,START_DISPLAY_IMAGE=%d",START_REG_MARK_CHART_TEST,START_DISPLAY_IMAGE);
		while (START_REG_MARK_CHART_TEST == FALSE) {
			//	log("waiting...");
			Sleep(10);
			mysbw.oc_x[0] = mysbw.oc_y[0] = 0;
		}
		log("start reg2 %d",START_REG_MARK_CHART_TEST);
		while(START_SITE_REG_MARK_CHART_TEST == FALSE)  Sleep(10);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_REG_MARK_CHART_TEST = FALSE;
			continue;
		}
		result = s_test::test.set_test_on();
		log("test on result: %d",result);
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_REG_MARK_CHART_TEST = FALSE;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//init test
		result = init_reg_mark_chart_test();
		log("init reg result: %d",result);
		result = init_std_chart_test();
		log("init std result: %d",result);
		if (s_test::test_option[TI_REG_MARK_CHART].mode == 0x00){ //loop display
			log("rm w=%d, h=%d, img source=%d", IMG_WIDTH, IMG_HEIGHT,SELECT_IMG_SOURCE);
			while (s_test::test.get_test_stop() == FALSE){
				memcpy(s_test::bmp_buf_t,s_test::img_buf,IMG_HEIGHT*IMG_WIDTH*3);
				//	log("trans bmp.");

				if(mysbw.mirror[0]){
					if(mysbw.mirror[1]){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+2];
							}
						}
					}
				}
				else {
					if(mysbw.mirror[1]){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
				}
				//	memcpy(s_test::bmp_buf,s_test::bmp_buf,IMG_WIDTH*480*3);
				exec_reg_mark_chart_test(NULL, s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				SfrResult(3);//画面显示红绿判断
				exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				s_test::test.draw_test_item(TI_STD_CHART);
				s_test::test.draw_test_item(TI_REG_MARK_CHART);
				//	START_CHECK_TEST = TRUE;
				IMG_CAPTURE_READY = TRUE;
				Sleep(10);
				s_test::myimg.clear_bmp_stream();
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf, 
					IMG_WIDTH,IMG_HEIGHT, IMG_DEPTH,
					0,0);
			}
		}
		else{
			//	get_rm_image(TRUE);
			exec_reg_mark_chart_test(NULL, s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			SfrResult(3);//画面显示红绿判断
			exec_std_chart_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_STD_CHART);
			s_test::test.draw_test_item(TI_REG_MARK_CHART);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
				s_test::bmp_buf, 
				IMG_WIDTH, 
				IMG_HEIGHT, 
				IMG_DEPTH);
			IMG_CAPTURE_READY = TRUE;
		}
		//	ShowWindow(HWND_REG_MARK_SFRCURVE, SW_HIDE);
		START_REG_MARK_CHART_TEST = FALSE;

		s_test::test.close_mutex();
		s_test::test.set_test_off();
	}

	return FALSE;
}
/*
DWORD CALLBACK white_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	char result_str[20] = {0};
	char bmp_name[150] = {0};
	while (1){
		log("start reg %d,START_DISPLAY_IMAGE=%d",START_WHITE_TEST,START_DISPLAY_IMAGE);
		while (START_WHITE_TEST == FALSE) {
			//	log("waiting...");
			Sleep(10);
		}
		memset(result_str, 0, sizeof(result_str));
		memset(bmp_name, 0, sizeof(bmp_name));
		log("start reg2 %d",START_WHITE_TEST);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_WHITE_TEST = FALSE;
			continue;
		}
		result = s_test::test.set_test_on();
		log("test on result: %d",result);
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_WHITE_TEST = FALSE;
			continue;
		}
		s_test::test.set_item_tested(TI_WHITE);

		//////////////////////////////////////////////////////////////////////////
		//init test
		init_white_test();
		log("init reg result: %d",result);

		if (s_test::test_option[TI_WHITE].mode == 0x00){ //loop display
			log("rm w=%d, h=%d, img source=%d", IMG_WIDTH, IMG_HEIGHT,SELECT_IMG_SOURCE);
			while (s_test::test.get_test_stop() == FALSE){
				if(SELECT_IMG_SOURCE == IMG_DEV_CTS){
					YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);

				}
				else if(SELECT_IMG_SOURCE == IMG_DEV_360E)
					YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);//tsw306e

				//	log("trans bmp.");

				if(IMG_MIRROR_X){
					if(IMG_MIRROR_Y){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+x*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+x*3+2];
							}
						}
					}
				}
				else {
					if(IMG_MIRROR_Y){
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[y*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
					else{
						for (int y=0; y<IMG_HEIGHT; y++){
							for (int x=0; x<IMG_WIDTH; x++){
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+1] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+1];
								s_test::bmp_buf[y*3*IMG_WIDTH+x*3+2] = s_test::bmp_buf_t[(IMG_HEIGHT-1-y)*3*IMG_WIDTH+(IMG_WIDTH-1-x)*3+2];
							}
						}
					}
				}
				exec_white_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
				//	memcpy(s_test::bmp_buf,s_test::bmp_buf,IMG_WIDTH*480*3);
				s_test::test.draw_test_item(TI_WHITE);
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf, 
					IMG_WIDTH, 
					IMG_HEIGHT, 
					IMG_DEPTH);
				s_test::test.draw_test_item(TI_WHITE);
				//	START_CHECK_TEST = TRUE;
				IMG_CAPTURE_READY = TRUE;
				Sleep(10);
				s_test::myimg.clear_bmp_stream();
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf, 
					IMG_WIDTH,IMG_HEIGHT, IMG_DEPTH,
					0,0);
			}
		}
		else{
			//	get_rm_image(TRUE);
			exec_white_test(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_WHITE);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
				s_test::bmp_buf, 
				IMG_WIDTH, 
				IMG_HEIGHT, 
				IMG_DEPTH);
			IMG_CAPTURE_READY = TRUE;
		}
		//	ShowWindow(HWND_REG_MARK_SFRCURVE, SW_HIDE);
		START_WHITE_TEST = FALSE;

		s_test::test.close_mutex();
		s_test::test.set_test_off();
	}
	return FALSE;
}*/
DWORD CALLBACK white_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	while (1){
		while (START_WHITE_TEST == FALSE) Sleep(10);
		while (START_SITE_WHITE_TEST == FALSE) Sleep(100);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_WHITE_TEST = FALSE;
			continue;
		}

		result = s_test::test.set_test_on();
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_WHITE_TEST = FALSE;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//init test
		init_white_test();

		if (s_test::test_option[TI_WHITE].mode == 0x00){ //loop
			while (s_test::test.get_test_stop() == FALSE){
				//
				exec_white_test(s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);
				s_test::test.draw_test_item(TI_WHITE);
				
				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf_t, 
					IMG_WIDTH, 
					IMG_HEIGHT, 
					IMG_DEPTH);
				IMG_CAPTURE_READY = TRUE;

				Sleep(10);
			}
		}
		else{
			exec_white_test(s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_WHITE);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
				s_test::bmp_buf_t, 
				IMG_WIDTH, 
				IMG_HEIGHT, 
				IMG_DEPTH);
			IMG_CAPTURE_READY = TRUE;
		}
		START_WHITE_TEST = FALSE;
		s_test::test.close_mutex();
		s_test::test.set_test_off();
	}

	return FALSE;
}
DWORD CALLBACK light_test_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
	DWORD wait_rt = 0;
	int result = FALSE;
	while (1){
		
		while (START_SITE_LIGHT_TEST == FALSE) Sleep(100);
		wait_rt = WaitForSingleObject(s_test::test.get_mutex(), 1000);
		if (wait_rt != WAIT_OBJECT_0){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_WAIT_TEST_MUTEX;
			START_WHITE_TEST = FALSE;
			continue;
		}

		result = s_test::test.set_test_on();
		if (result == FALSE){
			ERROR_ON = TRUE;
			ERROR_CODE = ERR_TEST_OCCUPIED;
			START_WHITE_TEST = FALSE;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//init test
	//	init_light_test();

		if (s_test::test_option[TI_LIGHT].mode == 0x00){ //loop
			while (s_test::test.get_test_stop() == FALSE){
				//
	//			exec_light_test(s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);
				s_test::test.draw_test_item(TI_LIGHT);

				s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
					s_test::bmp_buf_t, 
					IMG_WIDTH, 
					IMG_HEIGHT, 
					IMG_DEPTH);
				IMG_CAPTURE_READY = TRUE;

				Sleep(10);
			}
		}
		else{
			exec_white_test(s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);
			s_test::test.draw_test_item(TI_WHITE);
			s_test::myimg.display(GetDlgItem(HWND_MAIN, IDC_DISPLAY), 
				s_test::bmp_buf_t, 
				IMG_WIDTH, 
				IMG_HEIGHT, 
				IMG_DEPTH);
			IMG_CAPTURE_READY = TRUE;
		}
		START_WHITE_TEST = FALSE;
		s_test::test.close_mutex();
		s_test::test.set_test_off();
	}

	return FALSE;
}