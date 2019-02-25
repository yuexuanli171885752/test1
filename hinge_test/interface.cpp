#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "inf_motion_aa.h"

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "s_win.lib")
#pragma comment(lib, "s_img.lib")
#pragma comment(lib, "s_chart.lib")
#pragma comment(lib, "s_sfr.lib")
#pragma comment(lib, "s_white.lib")
#pragma comment(lib, "s_math.lib")
#pragma comment(lib, "s_comm.lib")
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "DXMediaCap.lib")
#pragma comment(lib, "PS400dll.lib")
#pragma comment(lib, "UniDAQ.lib")



extern int NG_SIGNAL;
// extern int PLC_DB_INDEX;
// extern int PLC_DB_START;
// extern int PLC_DB_LENS;
extern SlantedBW mysbw;


SHM *shm = NULL;
extern HANDLE plc_mutex;
namespace s_test
{
	HMENU  hmenu = NULL;
	HANDLE hshm	 = NULL;

	DLGINFO				dinfo;
	TestConfig			tconf;
	GuideLine			gline;
	MyImage				myimg;
	ai_img::Convert		cimg;
	Test				test;
	TestOption			test_option[TEST_ITEM_NUM];

	ImportTool			imtool[TOOL_CNT];
	ImportHW			imhw[HW_CNT];
	ImportHW			*pimhw = NULL;
	SlantedBW_SM		mysbw_sm;

	
	ImgCapDev			*cap_dev; 
	ImgFile				img_file;	
	CVBS_TSW_306E		img_hw_cvbs_tsw_306e;
	CTS					cts;
	Hinge				hinge;
	IplImage			*frame ; 

	Device_Info dev[23];
	char dir[MAX_PATH]			   = {0};
	char config_path[MAX_PATH]	   = {0};
	char ps_config_path[MAX_PATH]  = {0};
	char io_output_path[MAX_PATH]  = {0};
	char io_input_path[MAX_PATH]  = {0};
	char recipe_name[100] = {0};

	char io_display_string_output[500][500];
	char io_display_string_input[500][500];

	char shm_name[MAX_PATH]	   = {"share_video"};
	char img_format_str[7][100] = {"RAW", "BMP", "JPEG", "PNG", "TIFF", "GIF", "MIPIRAW"};
	char sn_str[MAX_PATH] = {0};
	char plc_ip[20] = {0};

	HWND hwnd[100] = {NULL};
	int	 ram_int[RAM_INT_NUM] = {0};
	double ram_double[RAM_DOUBLE_NUM] = {0};
	BYTE img_buf[MAX_IMG_SIZE*2] = {0};
	BYTE raw8_buf[MAX_IMG_SIZE]  = {0};
	BYTE bmp_buf[MAX_IMG_SIZE*3] = {0};
	BYTE bmp_buf_delay[MAX_IMG_SIZE*3] = {0};
	BYTE bmp_buf2[MAX_IMG_SIZE*3] = {0};
	BYTE bmp_buf_t[MAX_IMG_SIZE*3] = {0};
	//white test
	BYTE bmp_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]          = {0};
	BYTE shrink_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]       = {0};
	BYTE scan_y_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]         = {0};
	BYTE filter_close_buf[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3] = {0};
	BYTE bmp_f[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*3]            = {0};
	BYTE bmp_large_x[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]        = {0};
	BYTE bmp_large_y[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]        = {0};
	BYTE bmp_y_buf_copy[MAX_IMG_WIDTH*MAX_IMG_HEIGHT]     = {0};

	BYTE bmp_bin[MAX_IMG_WIDTH*MAX_IMG_HEIGHT] = {0};
	BYTE bmp_y_cutareas[MAX_IMG_WIDTH*MAX_IMG_HEIGHT] = {0};
}

void log(const char *format, ...)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	HWND hlog = GetDlgItem(HWND_LOG, IDC_LOG);
	HWND hmlog = GetDlgItem(HWND_MAIN, IDC_LOG_EX);
	char temp[1024] = {0};
	char log_str[1024] = {0};
	va_list args;
	va_start(args,format);

	vsprintf(temp,format,args);
	sprintf(log_str, "[%02d:%02d:%02d]>%s", st.wHour, st.wMinute, st.wSecond, temp);//jishi
	int index =SendMessage(hlog, LB_ADDSTRING, 0, (LPARAM) log_str);
	SendMessage(hlog, LB_SETCURSEL, index, 0);
	index =SendMessage(hmlog, LB_ADDSTRING, 0, (LPARAM) log_str);
	SendMessage(hmlog, LB_SETCURSEL, index, 0);
	ai_win::save_log(log_str, "sys", "sys_log", 512);
}

void set_status(int status)
{
	APP_STATUS = status;
	InvalidateRect(GetDlgItem(HWND_MAIN, IDC_STATUS), NULL, TRUE);
	UpdateWindow(GetDlgItem(HWND_MAIN, IDC_STATUS));
}

int open_shm(HWND hdlg)
{
	NG_SIGNAL=0;

	s_test::hshm = OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, s_test::shm_name);
	if (GetLastError() == ERROR_ALREADY_EXISTS){
		close_shm();
		s_test::hshm = OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, s_test::shm_name);
		if (GetLastError() == ERROR_ALREADY_EXISTS){
			MessageBox(hdlg, "mapping already exists.", "error", 0);
			CloseHandle(s_test::hshm);
			return FALSE;
		}
	}

	if (s_test::hshm != NULL){
		shm = (SHM *)MapViewOfFile(s_test::hshm, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
		if (shm != NULL) log("Link:Input Image Buffer OK.");
		else{
			log("Link:Input Image Buffer Fail.");
			return FALSE;
		}
	}
	else return FALSE;

	return TRUE;
}

int close_shm()
{
	int result = UnmapViewOfFile(shm);
	if (result == TRUE){
		CloseHandle(s_test::hshm);
	}

	return result;
}

void create_report_folder()
{
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\report", s_test::dir);
	_mkdir(path);

}

int create_report_file(const char *test_item_str, char *target_report_folder_path, char *target_folder_name)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	//////////////////////////////////////////////////////////////////////////
	//look for the target folder and file
	char dir[MAX_PATH] = {0};
	ai_win::get_current_directory(dir);

	char new_path[MAX_PATH] = {0};
	char new_folder[MAX_PATH] = {0};
	char new_folder_path[MAX_PATH] = {0};

	sprintf(new_folder, test_item_str);
	sprintf(new_path, "%s\\report", dir);
	sprintf(new_folder_path, "%s\\%s", new_path, new_folder);  //..\report\laser

	if (!ai_win::search_directory(new_path, new_folder)) //search laser folder
	{
		_mkdir(new_folder_path);
	}

	sprintf(new_folder, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(new_path, "%s", new_folder_path);
	sprintf(new_folder_path, "%s\\%s", new_path, new_folder);

	if (!ai_win::search_directory(new_path, new_folder)){
		_mkdir(new_folder_path);									//..\report\laser\year_month_day
	}

	char report_path[MAX_PATH] = {0};
	char report_name[MAX_PATH] = {0};
	sprintf(report_name, "%s_report_%s.csv", test_item_str, new_folder);
	sprintf(report_path, "%s\\%s", new_folder_path, report_name);

	strcpy(target_report_folder_path, new_folder_path);
	strcpy(target_folder_name, new_folder);
	if (!ai_win::search_file(new_folder_path, report_name)){
		return 0;
	}
	else{
		return 1;
	}
}

void update_scenery_title(char *img_name, char *img_size, char *img_type, char *img_fps)
{
	char wintxt_str[MAX_PATH] = {0};
	GetWindowText(HWND_APP, wintxt_str, sizeof(wintxt_str));
	char mystr[4][MAX_PATH] = {0};
	char sep[] = ", ";
	int index = 0;
	char *token = NULL;
	token = strtok(wintxt_str, sep);

	while (token != NULL){
		strcpy(mystr[index], token);
		index++;
		token = strtok(NULL, sep);
	}

	if (img_name != NULL){
		memset(mystr[0], 0, sizeof(mystr[0]));
		strcpy(mystr[0], img_name);
	}
	if (img_size != NULL){
		memset(mystr[1], 0, sizeof(mystr[0]));
		strcpy(mystr[1], img_size);
	}

	if (img_type != NULL){
		memset(mystr[2], 0, sizeof(mystr[0]));
		strcpy(mystr[2], img_type);
	}
	if (img_fps != NULL){
		memset(mystr[3], 0, sizeof(mystr[0]));
		strcpy(mystr[3], img_fps);
	}

	char mystr2[MAX_PATH] = {0};
	sprintf(mystr2, "%s %s %s %s", mystr[0], mystr[1], mystr[2], mystr[3]);
	SetWindowText(HWND_APP, mystr2);
}

void update_mdlg_title(HWND hdlg)
{
	char str[100] = {0};
	sprintf(str, "Saker AA Software [%s]", s_test::recipe_name);
	SetWindowText(hdlg, str);
}

int connect_device(int id)
{
	int result = 0;
	DXSetVideoPara(s_test::dev[id].dwChannelHandle, 
		s_test::dev[id].dwVidStandard, 
		s_test::dev[id].dwColorspace, 
		s_test::dev[id].dwWidth, 
		s_test::dev[id].dwHeight,
		s_test::dev[id].dwFrameRate);
	if (DXDeviceRun(s_test::dev[id].dwChannelHandle) != 0x00){
		log("run device [%d] fail.", id);
		result = FALSE;
	}
	else{
		IMG_DEVICE_INDEX = id;
		log("use img device index = %d", id);
		result = TRUE;
	}

	return result;
}

void disconnect_device(int id)
{
	DXStopPreview(s_test::dev[id].dwChannelHandle);
	DXDeviceStop(s_test::dev[id].dwChannelHandle);
}







/*
DWORD CALLBACK plc_online_counting_thread(LPVOID temp)
{
	BYTE write_b = 0;
	BYTE zPos  = 0;
	int status = 0;
	int timeout = 200;
	while (1){
		while (IS_PLC_CONNECTED == FALSE) Sleep(100);
	//	WaitForSingleObject(s_test::plc_mutex, INFINITE);
		zPos = (int)(TABLE_X_POSITION * 1.0)/(TABLE_X_RATIO);
		write_b++;
		write_b %=255;
		status = write_to_plc_db(PLC_DB_INDEX/ *145* /, PLC_DB_START, PLC_DB_LENS, &write_b);

		status = write_to_plc_db(PLC_DB_INDEX/ *145* /, PLC_DB_START+4, PLC_DB_LENS, &zPos);
	//	log("zPosition = %d",zPos);
//		write_to_plc_db(144/ *145* /, 0, 1, &write_b);
		Sleep(100);
		if (status != 0x00){
			IS_PLC_CONNECTED = FALSE;
			log("write err, plc not connected.");
		}
		
// 		ReleaseMutex(s_test::plc_mutex);
// 		log("write plc byte0.");
		Sleep(100);
	}

	return FALSE;
}*/

DWORD CALLBACK plc_emergency_stop_thread(LPVOID temp)
{
	BYTE s = 0x00;
	int status = 0;
//	int read_err_cnt = 0;
	while(1){
		status=read_from_plc_db(144,3,1,&s);
		if (status != 0){
			
			IS_PLC_CONNECTED = FALSE;
			log("read err, no data in this address.");
			
		}
		else
			if(s==0){
				log("emergency stop");
			}
			else{
				Sleep(100);
			}
	}
	return FALSE;
}

			

DWORD CALLBACK  plc_online_check_thread(LPVOID temp)
{
	BYTE last_b = 0;
	BYTE read_b = 0;
	int err_cnt = 0;
	int status = 0;
	int read_err_cnt = 0;
	int timeout = 200;

	while (1){
		while (IS_PLC_CONNECTED == FALSE) Sleep(100);
//		WaitForSingleObject(s_test::plc_mutex, INFINITE);
	//	Sleep(1000);
	//	log("line z status:%d",LINE_Z_STATUS);
		status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R, PLC_DB_LENS_R, &read_b);
		SetDlgItemInt(HWND_MAIN, IDC_LINK_PLC2, read_b, FALSE);
		if (status != 0){
			read_err_cnt++;
			if (read_err_cnt>10){
				IS_PLC_CONNECTED = FALSE;
				log("read err, plc not connected[online check].");
			//	break;
			}
		}
		else{
			read_err_cnt = 0;
			err_cnt++;
			if (read_b != last_b){
				err_cnt = 0;
			}

			last_b = read_b;
			if (err_cnt>100){
				IS_PLC_CONNECTED = FALSE;
				log("over err cnt=100, plc not connected.");
			}
			


		}
//		ReleaseMutex(s_test::plc_mutex);
	//	log("read plc byte0.");
		Sleep(100);
	}

	return FALSE;
}

void FloatToByte(float floatNum, unsigned char* byteArry)////浮点数到十六进制转换2
{

	char* pchar=(char*)&floatNum;

	for(int i=0;i<sizeof(float);i++){
		*byteArry=*pchar;
		pchar++;
		byteArry++;
	}
}

DWORD CALLBACK plc_read_thread(LPVOID temp)
{
	int status  = 0;
	BYTE read_b = 0;
	int err_cnt = 0;
	int timeout = 0;
	while (1){
 		while (IS_PLC_CONNECTED == FALSE)
			Sleep(100);
		status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+1, PLC_DB_LENS_R, &read_b);

		if (status != 0x00){
			err_cnt++;
			if (err_cnt>10){
				IS_PLC_CONNECTED = FALSE;
				log("read err, plc not connected.[read db]");
			}
		}
		else{
			err_cnt = 0;
			switch (read_b){
			case 0:

				break;
			case 1:	//z down P1
				log("recv 1");
				read_b=2;//running
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				read_b=1;//ERR
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+5, PLC_DB_LENS, &read_b);//open
				Sleep(100);
				if(HOME_FLAG==TRUE){
					BUTTON_VALUE=190;//auto m1
					Sleep(100);
					while(MANUAL_MOVING==TRUE) Sleep(50);
					if(!MANUAL_ERROR){
						read_b=2;//
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+5, PLC_DB_LENS, &read_b);//close	
						read_b=4;//motion down
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}
					else{
						read_b=3;//motion err
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}

												
				}
				else{
					read_b=3;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);				
					log("aa not home.");
					read_b = 0;	//homed
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+2, PLC_DB_LENS, &read_b);
					log("is not home.home flag = %d",HOME_FLAG);
				}
 				break;

			case 2: //z up P2
				log("recv 2");
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);

				if(HOME_FLAG==TRUE){
					BUTTON_VALUE=191;//x
					Sleep(100);
					while(MANUAL_MOVING==TRUE) Sleep(50);
					if(!MANUAL_ERROR){	
						read_b=4;//motion down
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}
					else{
						read_b=3;//motion err
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}							
				}
				else{
					read_b=3;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					
					log("aa not home.");
					read_b = 0;	//homed
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+2, PLC_DB_LENS, &read_b);					
				}
				break;

			case 3: //aa
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				
				read_b = 0; //clear ok/ng
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);

				if(HOME_FLAG==TRUE){
					log("receive AA cmd");			
					timeout = 2000;

					BUTTON_VALUE = 207;
					Sleep(150);
					while(timeout-->0 && MANUAL_MOVING == TRUE)
						Sleep(50);
					
					/*ITEM_ACTIVE(HWND_MAIN,IDC_STOP);*/
					
					log("bUV_result:%d",mysbw.B_UVResult);

					if(mysbw.B_UVResult == 1){
						ITEM_ACTIVE(HWND_MAIN,IDC_STOP);
						Sleep(200);
						set_status(APP_PASS);
						log("1pass");
						ITEM_ACTIVE(HWND_MOTOR_MOTION, IDC_MOVE_OFFSET);
					}
					else{//*********second aa
						EXECUTE_ERROR = FALSE;
						timeout = 2000;
						BUTTON_VALUE = 207;
						Sleep(150);
						while(timeout-->0 && MANUAL_MOVING == TRUE)
							Sleep(50);							
						
						ITEM_ACTIVE(HWND_MAIN,IDC_STOP);
						if(mysbw.B_UVResult == 1){
							set_status(APP_PASS);
							log("11pass");
							ITEM_ACTIVE(HWND_MOTOR_MOTION, IDC_MOVE_OFFSET);
						}
						else
							set_status(APP_FAIL);
					}
// 	
					log("reset db, status=%d", status);
// 					if(timeout<=0){
// 						read_b=3;
// 						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 					}
					
					if (mysbw.B_UVResult == FALSE || EXECUTE_ERROR == TRUE /*||mysbw.test_result == FALSE*/){
						log("NG Signal:%d, err:%d",NG_SIGNAL, EXECUTE_ERROR);
						SetDlgItemText(HWND_MAIN, IDC_PROCESS_AA_MOTION, "NG");	
						save_reg_mark_report_c(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT,SAVE_NG_REG_PRE_REPORT);
						log("ng pre report saved.");
						read_b = 2;	//fail				
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);
						Sleep(200);
						read_b = 4;	//active down			
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						while(1){
							status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
							log("write err.");
							if(read_b == 4) break;
							read_b=4;
							status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						}
						log("AA fail.");
						set_status(APP_FAIL);
					//	ITEM_ACTIVE(HWND_MAIN,IDC_STOP);
					}
					else{
						SetDlgItemText(HWND_MAIN, IDC_PROCESS_AA_MOTION, "OK");
						SetDlgItemText(HWND_MAIN, IDC_PROCESS_CHECK1, "OK");
						if(GET_CHECKER(HWND_MOTOR_MOTION,IDC_MOVE_OFFSET2)){
							BUTTON_VALUE = 225;
						}
						save_reg_mark_report_c(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT,SAVE_OK_REG_PRE_REPORT);
						
						log("AA ok, report saved, UV start cuving.");
						read_b = 1; //finished.ok
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);	
						Sleep(100);
					//	if(status){
							read_b = 4;	//active down				
							status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
							log("return 4.");
// 						}
// 						else{
// 							read_b = 3;	//active down				
// 							status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 							log("return 3.");
// 						}

						
					}
				}
				else{
					ITEM_ACTIVE(HWND_MAIN, IDC_STOP);
					read_b = 2;	//结果fail				
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);
					Sleep(100);
					read_b = 3;	//动作失败
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					while(1){
						status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						log("write err.");
						if(read_b == 3) break;
						read_b=3;
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}
					log("aa,AA not home.");
					read_b = 0;	//homed
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+2, PLC_DB_LENS, &read_b);
					log("is not home.home flag = %d",HOME_FLAG);
					
					set_status(APP_NOTHOME);
				}
				break;
 
 			case 4: //sfr check
				log("receive sfr check cmd.");
				START_CHECK_TEST = FALSE;
				SM104_SERVO_OFF;
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				
 				read_b = 1;
 				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+5,PLC_DB_LENS, &read_b);

				read_b = 0; //clear ok/ng
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);

				ITEM_ACTIVE(HWND_MAIN,IDC_START);
				Sleep(500);
				while(START_CHECK_TEST == FALSE) Sleep(100);
				Sleep(500);
				ITEM_ACTIVE(HWND_MAIN,IDC_STOP);
				SM104_SERVO_ON;
				if(GET_CHECKER(HWND_MOTOR_MOTION,IDC_MOVE_OFFSET2) == TRUE && mysbw.A_UVResult == TRUE){
					mysbw.test_result = TRUE;
				}

				if (mysbw.test_result == TRUE/*&& SHOW_IMG == TRUE*//*||mysbw.use_field_a == FALSE*/){
					set_status(APP_PASS);
					log("111pass");
					SetDlgItemText(HWND_MAIN, IDC_PROCESS_CHECK2, "OK");
					save_reg_mark_report_c(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT,SAVE_OK_REG_REPORT);
					log("sfr check ok report saved.test result = %d",mysbw.test_result);
					read_b = 1; //finished.ok
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);
					read_b = 4; //active down
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					
					log("SFR Check Pass.");
					
					read_b = 0;
				}
				else{
					//ITEM_ACTIVE(HWND_MAIN, IDC_STOP);
					set_status(APP_FAIL);
					SetDlgItemText(HWND_MAIN, IDC_PROCESS_CHECK2, "NG");
					save_reg_mark_report_c(s_test::bmp_buf, IMG_WIDTH, IMG_HEIGHT,SAVE_NG_REG_REPORT);
					log("sfr check ng report saved.");
					read_b = 2; //finished.ng
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);
					read_b = 4; //active down
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					
					
					read_b = 0;
				}				
 				break;
 
			case 5: //move home
				log("rev 5, move home.");
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);

				read_b = 1;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+5,PLC_DB_LENS, &read_b);
// 				Sleep(500);
// 				read_b=4;
// 				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);

				log("receive execute home cmd");
				EXECUTE_ERROR = FALSE;
				EXECUTE_CMD = TRUE;
				EXECUTE_HOME = TRUE;
				timeout = 20000;
				BUTTON_VALUE = 221;
		//		ITEM_ACTIVE(HWND_MOTOR_MOTION, IDC_HOME_ALL);
				
				while(timeout-->0 && EXECUTE_CMD == TRUE && EXECUTE_HOME == TRUE){
					Sleep(10);
				}
				if (timeout<=0 || EXECUTE_ERROR == TRUE){
					read_b = 3;	//fail
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					while(1){
						status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						log("write err.");
						if(read_b == 3) break;
						read_b=3;
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}
					log("home fail.");
				}
				else{
					read_b = 5; //avtive down.
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					while(1){
						status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						log("write err.");
						if(read_b == 5) break;
						read_b=5;
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					}
					log("home ok.");
					set_status(APP_READY);
					read_b = 1;	//homed
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+2, PLC_DB_LENS, &read_b);
					log("is homed.home flag = %d",HOME_FLAG);
					
				}
 				
				break;
 
 			case 6: //move org

				log("recv 6");
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				
				read_b = 1;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+5,PLC_DB_LENS, &read_b);//open

				Sleep(500);
// 				read_b=4;
// 				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 				log("return 4");
				if(HOME_FLAG==TRUE){
					/*move to P4*/
					log("home_flag=%d",HOME_FLAG);
					BUTTON_VALUE = 209;
					Sleep(200);
					while(MANUAL_MOVING == TRUE) Sleep(50);
					BUTTON_VALUE=208;
					Sleep(100);
					while(MANUAL_MOVING == TRUE) Sleep(100);
					if(EXECUTE_ERROR!=0x00){
						log("move err:%d",EXECUTE_ERROR);
						read_b=3;//ERR
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						
					}
					else{
						read_b=4;
						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
						
						log("move org, ok.");
					}					
				}
				else{
					read_b=3;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					
					log("aa not home.");
					read_b = 0;	//homed
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+2, PLC_DB_LENS, &read_b);
					log("is not home.home flag = %d",HOME_FLAG);
				}
				break;

			case 7: //check img exist
				log("receive check img exist cmd.");
				
				read_b = 2;	//运行中		
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				
// 				ITEM_ACTIVE(HWND_MAIN, IDC_START);
// 				Sleep(500);
				mysbw.oc_x[0] = 0;
				mysbw.oc_y[0] = 0;
				ITEM_ACTIVE(HWND_MAIN,IDC_STOP);
				Sleep(500);
				ITEM_ACTIVE(HWND_MAIN, IDC_START);
				log("check img:%d",SHOW_IMG);
				err_cnt=0;
				Sleep(3000);
				while(SHOW_IMG==FALSE){
					Sleep(500);
					log("no img.");
					err_cnt++;
					if(err_cnt>10)
						break;
				}
				if(err_cnt<=10){
					read_b=3;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);

					//start read sn.


					read_b = 4;	//active down			
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);

					log("check img exist ok.");
					break;
				}
				else{
					read_b=4;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+3, PLC_DB_LENS, &read_b);
					read_b = 4;	//active down			
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 					while(1){
// 						status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 						log("write err.");
// 						if(read_b == 4) break;
// 						read_b=4;
// 						status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
// 					}
					log("no img.[%d]",err_cnt);
					NG_SIGNAL=1;
					ITEM_ACTIVE(HWND_MAIN, IDC_STOP);
					break;
				}
			
			case 10: //read sn
				log("recv 10.read sn.");
				read_b=2;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				
				read_b = 0;
				status = write_to_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+1,PLC_DB_LENS_R, &read_b);
				BYTE read;
				
				status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+4, 2, &read);
				mysbw.sn_cnt=read;
				log("sn cnt:%d",mysbw.sn_cnt);
// 				status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+132, 1, &read);
// 				mysbw.white_result=read;
// 				status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+133, 1, &read);
// 				mysbw.dispensing_result=read;
// 				status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+134, 1, &read);
// 				mysbw.dispensing_result=read;
				read=0;
				for(int i=0;i<mysbw.sn_cnt;i++){
					status = read_from_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+6+i, 1, &read);
				//	read = read%16;
				//	log("read_byte:%d",read);
					mysbw.sn[i]=read;
					log("sn[%d]=%c",i,mysbw.sn[i]);
						
				}
				SetDlgItemText(HWND_MAIN,IDC_SN_DISPLAY,mysbw.sn);
				//	log("read sn=%d%d%d%d%d%d%d%d%d%d%d%d",sn);
				//	mysbw.sn=sn;
				read_b = 4;
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				while(1){
					status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					log("write err.");
					if(read_b == 4) break;
					read_b=4;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				}
				log("read down.");
				break;

			default:
				log("receive other cmd:%d", read_b);
				read_b = 0;
				status = write_to_plc_db(PLC_DB_INDEX_R, PLC_DB_START_R+1,PLC_DB_LENS_R, &read_b);
				read_b = 4;	//active down			
				status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				while(1){
					status = read_from_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
					log("write err.");
					if(read_b == 4) break;
					read_b=4;
					status = write_to_plc_db(PLC_DB_INDEX, PLC_DB_START+1, PLC_DB_LENS, &read_b);
				}
				break;
			}

// 			read_b = 0;
// 			status = write_to_plc_db(144, 1, 1, &read_b);
			if (status != 0x00){
				IS_PLC_CONNECTED = FALSE;
			}

		}
	}
}

DWORD CALLBACK check_img_exist_thread(LPVOID){
	log("start check img thread.");
	int w=IMG_WIDTH;
	int h=IMG_HEIGHT;
	int sum_g1,sum_g2,sum;
	int i,j;
	SHOW_IMG=FALSE;
	sum_g1=sum_g2=sum=0;
	i=100;
	while(1){
		sum_g1=0;
		for(j=0;j<w-1;j++)					
			sum_g1+=s_test::bmp_buf[(i*w+j)*3+1];
		if(sum_g1==sum_g2){
			SHOW_IMG=FALSE;
	//		log("no img.1=%d,2=%d",sum_g1,sum_g2);
		}
		else{
			SHOW_IMG=TRUE;
			sum=0;
	//		log("exist img.1=%d,2=%d",sum_g1,sum_g2);
		}
		sum_g2=sum_g1;
		Sleep(200);
	}
}



DWORD CALLBACK  plc_link_thread(LPVOID temp)
{
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\config.ini", s_test::dir);
	int status = 0;
	BYTE read_b = 0;
	plc_mutex = CreateMutex(NULL, FALSE, "plc_mutex");

	while (1){
		if (IS_PLC_CONNECTED == FALSE){
			SetDlgItemText(HWND_MAIN, IDC_LINK_PLC, "Connecting...");
			ai_win::read_string_key(path, "plc", "ip", s_test::plc_ip, sizeof(s_test::plc_ip));
			disconnect_from_plc();
			Sleep(100);
			status = connect_to_plc(s_test::plc_ip);
			Sleep(200);
			if (status == TRUE){
				IS_PLC_CONNECTED = TRUE;
				SetDlgItemText(HWND_MAIN, IDC_LINK_PLC, "Connected");
				
				log("plc link ok.");
				
// 				read_b = 0;
// 				write_to_plc_db(PLC_DB_INDEX, PLC_DB_START, PLC_DB_LENS, &read_b);///
// 				log("write to plc ready.");
			}
			else{
				SetDlgItemText(HWND_MAIN, IDC_LINK_PLC, "Connecting..");
				Sleep(500);
			}
		}
		else{
			SetDlgItemText(HWND_MAIN, IDC_LINK_PLC, "Connected");
		}
		Sleep(500);
	}

	return FALSE;
}
void s_test::read_io_display_string_output(HWND hdlg)
{
	FILE *fp;
	char buffer[300] = {0};
	ai_win::get_current_directory(s_test::dir);	
	sprintf(s_test::io_output_path,"%s\\recipe\\io_display_output.txt",s_test::dir);
	if((fp=fopen(s_test::io_output_path,"rt"))==NULL){ 
		
		MessageBox(HWND_MAIN, "IO_OUTPUT display description File can not be opened !", "Warning", 0); 
		return; 
	}
	for(int i=0;i<256;i++){
		if(fgets(buffer,sizeof(buffer)-1,fp)==NULL) i=300;
		else strcpy(io_display_string_output[i],buffer);
	}
	fclose(fp);

	SetDlgItemText(hdlg, IDC_STR_Y01, io_display_string_output[0]);
	SetDlgItemText(hdlg, IDC_STR_Y02, io_display_string_output[1]);
	SetDlgItemText(hdlg, IDC_STR_Y03, io_display_string_output[2]);
	SetDlgItemText(hdlg, IDC_STR_Y04, io_display_string_output[3]);
	SetDlgItemText(hdlg, IDC_STR_Y05, io_display_string_output[4]);
	SetDlgItemText(hdlg, IDC_STR_Y06, io_display_string_output[5]);
	SetDlgItemText(hdlg, IDC_STR_Y07, io_display_string_output[6]);
	SetDlgItemText(hdlg, IDC_STR_Y08, io_display_string_output[7]);
	SetDlgItemText(hdlg, IDC_STR_Y09, io_display_string_output[8]);
	SetDlgItemText(hdlg, IDC_STR_Y10, io_display_string_output[9]);
	SetDlgItemText(hdlg, IDC_STR_Y11, io_display_string_output[10]);
	SetDlgItemText(hdlg, IDC_STR_Y12, io_display_string_output[11]);
	SetDlgItemText(hdlg, IDC_STR_Y13, io_display_string_output[12]);
	SetDlgItemText(hdlg, IDC_STR_Y14, io_display_string_output[13]);
	SetDlgItemText(hdlg, IDC_STR_Y15, io_display_string_output[14]);
	SetDlgItemText(hdlg, IDC_STR_Y16, io_display_string_output[15]);
														  
	SetDlgItemText(hdlg, IDC_STR_Y17, io_display_string_output[16]);
	SetDlgItemText(hdlg, IDC_STR_Y18, io_display_string_output[17]);
	SetDlgItemText(hdlg, IDC_STR_Y19, io_display_string_output[18]);
	SetDlgItemText(hdlg, IDC_STR_Y20, io_display_string_output[19]);
	SetDlgItemText(hdlg, IDC_STR_Y21, io_display_string_output[20]);
	SetDlgItemText(hdlg, IDC_STR_Y22, io_display_string_output[21]);
	SetDlgItemText(hdlg, IDC_STR_Y23, io_display_string_output[22]);
	SetDlgItemText(hdlg, IDC_STR_Y24, io_display_string_output[23]);
	SetDlgItemText(hdlg, IDC_STR_Y25, io_display_string_output[24]);
	SetDlgItemText(hdlg, IDC_STR_Y26, io_display_string_output[25]);
	SetDlgItemText(hdlg, IDC_STR_Y27, io_display_string_output[26]);
	SetDlgItemText(hdlg, IDC_STR_Y28, io_display_string_output[27]);
	SetDlgItemText(hdlg, IDC_STR_Y29, io_display_string_output[28]);
	SetDlgItemText(hdlg, IDC_STR_Y30, io_display_string_output[29]);
	SetDlgItemText(hdlg, IDC_STR_Y31, io_display_string_output[30]);
	SetDlgItemText(hdlg, IDC_STR_Y32, io_display_string_output[31]);
													   
	SetDlgItemText(hdlg, IDC_STR_Y33, io_display_string_output[32]);
	SetDlgItemText(hdlg, IDC_STR_Y34, io_display_string_output[33]);
	SetDlgItemText(hdlg, IDC_STR_Y35, io_display_string_output[34]);
	SetDlgItemText(hdlg, IDC_STR_Y36, io_display_string_output[35]);
	SetDlgItemText(hdlg, IDC_STR_Y37, io_display_string_output[36]);
	SetDlgItemText(hdlg, IDC_STR_Y38, io_display_string_output[37]);
	SetDlgItemText(hdlg, IDC_STR_Y39, io_display_string_output[38]);
	SetDlgItemText(hdlg, IDC_STR_Y40, io_display_string_output[39]);
	SetDlgItemText(hdlg, IDC_STR_Y41, io_display_string_output[40]);
	SetDlgItemText(hdlg, IDC_STR_Y42, io_display_string_output[41]);
	SetDlgItemText(hdlg, IDC_STR_Y43, io_display_string_output[42]);
	SetDlgItemText(hdlg, IDC_STR_Y44, io_display_string_output[43]);
	SetDlgItemText(hdlg, IDC_STR_Y45, io_display_string_output[44]);
	SetDlgItemText(hdlg, IDC_STR_Y46, io_display_string_output[45]);
	SetDlgItemText(hdlg, IDC_STR_Y47, io_display_string_output[46]);
	SetDlgItemText(hdlg, IDC_STR_Y48, io_display_string_output[47]);
													  
	SetDlgItemText(hdlg, IDC_STR_Y49, io_display_string_output[48]);
	SetDlgItemText(hdlg, IDC_STR_Y50, io_display_string_output[49]);
	SetDlgItemText(hdlg, IDC_STR_Y51, io_display_string_output[50]);
	SetDlgItemText(hdlg, IDC_STR_Y52, io_display_string_output[51]);
	SetDlgItemText(hdlg, IDC_STR_Y53, io_display_string_output[52]);
	SetDlgItemText(hdlg, IDC_STR_Y54, io_display_string_output[53]);
	SetDlgItemText(hdlg, IDC_STR_Y55, io_display_string_output[54]);
	SetDlgItemText(hdlg, IDC_STR_Y56, io_display_string_output[55]);
	SetDlgItemText(hdlg, IDC_STR_Y57, io_display_string_output[56]);
	SetDlgItemText(hdlg, IDC_STR_Y58, io_display_string_output[57]);
	SetDlgItemText(hdlg, IDC_STR_Y59, io_display_string_output[58]);
	SetDlgItemText(hdlg, IDC_STR_Y60, io_display_string_output[59]);
	SetDlgItemText(hdlg, IDC_STR_Y61, io_display_string_output[60]);
	SetDlgItemText(hdlg, IDC_STR_Y62, io_display_string_output[61]);
	SetDlgItemText(hdlg, IDC_STR_Y63, io_display_string_output[62]);
	SetDlgItemText(hdlg, IDC_STR_Y64, io_display_string_output[63]);

}
void s_test::read_io_display_string_input(HWND hdlg)
{
	FILE *fp;
	char buffer[300] = {0};
	ai_win::get_current_directory(s_test::dir);
	sprintf(s_test::io_input_path,"%s\\recipe\\io_display_input.txt",s_test::dir);
	if((fp=fopen(s_test::io_input_path,"rt"))==NULL){ 
		MessageBox(HWND_MAIN, "IO_INPUT display description File can not be opened !", "Warning", 0); 
		return; 
	}
	for(int i=0;i<256;i++){
		if(fgets(buffer,sizeof(buffer)-1,fp)==NULL) i=300;
		else strcpy(io_display_string_input[i],buffer);
	}
	fclose(fp);

	SetDlgItemText(hdlg, IDC_STR_S01, io_display_string_input[0]);
	SetDlgItemText(hdlg, IDC_STR_S02, io_display_string_input[1]);
	SetDlgItemText(hdlg, IDC_STR_S03, io_display_string_input[2]);
	SetDlgItemText(hdlg, IDC_STR_S04, io_display_string_input[3]);
	SetDlgItemText(hdlg, IDC_STR_S05, io_display_string_input[4]);
	SetDlgItemText(hdlg, IDC_STR_S06, io_display_string_input[5]);
	SetDlgItemText(hdlg, IDC_STR_S07, io_display_string_input[6]);
	SetDlgItemText(hdlg, IDC_STR_S08, io_display_string_input[7]);
	SetDlgItemText(hdlg, IDC_STR_S09, io_display_string_input[8]);
	SetDlgItemText(hdlg, IDC_STR_S10, io_display_string_input[9]);
	SetDlgItemText(hdlg, IDC_STR_S11, io_display_string_input[10]);
	SetDlgItemText(hdlg, IDC_STR_S12, io_display_string_input[11]);
	SetDlgItemText(hdlg, IDC_STR_S13, io_display_string_input[12]);
	SetDlgItemText(hdlg, IDC_STR_S14, io_display_string_input[13]);
	SetDlgItemText(hdlg, IDC_STR_S15, io_display_string_input[14]);
	SetDlgItemText(hdlg, IDC_STR_S16, io_display_string_input[15]);
													   
	SetDlgItemText(hdlg, IDC_STR_S17, io_display_string_input[16]);
	SetDlgItemText(hdlg, IDC_STR_S18, io_display_string_input[17]);
	SetDlgItemText(hdlg, IDC_STR_S19, io_display_string_input[18]);
	SetDlgItemText(hdlg, IDC_STR_S20, io_display_string_input[19]);
	SetDlgItemText(hdlg, IDC_STR_S21, io_display_string_input[20]);
	SetDlgItemText(hdlg, IDC_STR_S22, io_display_string_input[21]);
	SetDlgItemText(hdlg, IDC_STR_S23, io_display_string_input[22]);
	SetDlgItemText(hdlg, IDC_STR_S24, io_display_string_input[23]);
	SetDlgItemText(hdlg, IDC_STR_S25, io_display_string_input[24]);
	SetDlgItemText(hdlg, IDC_STR_S26, io_display_string_input[25]);
	SetDlgItemText(hdlg, IDC_STR_S27, io_display_string_input[26]);
	SetDlgItemText(hdlg, IDC_STR_S28, io_display_string_input[27]);
	SetDlgItemText(hdlg, IDC_STR_S29, io_display_string_input[28]);
	SetDlgItemText(hdlg, IDC_STR_S30, io_display_string_input[29]);
	SetDlgItemText(hdlg, IDC_STR_S31, io_display_string_input[30]);
	SetDlgItemText(hdlg, IDC_STR_S32, io_display_string_input[31]);
													 
	SetDlgItemText(hdlg, IDC_STR_S33, io_display_string_input[32]);
	SetDlgItemText(hdlg, IDC_STR_S34, io_display_string_input[33]);
	SetDlgItemText(hdlg, IDC_STR_S35, io_display_string_input[34]);
	SetDlgItemText(hdlg, IDC_STR_S36, io_display_string_input[35]);
	SetDlgItemText(hdlg, IDC_STR_S37, io_display_string_input[36]);
	SetDlgItemText(hdlg, IDC_STR_S38, io_display_string_input[37]);
	SetDlgItemText(hdlg, IDC_STR_S39, io_display_string_input[38]);
	SetDlgItemText(hdlg, IDC_STR_S40, io_display_string_input[39]);
	SetDlgItemText(hdlg, IDC_STR_S41, io_display_string_input[40]);
	SetDlgItemText(hdlg, IDC_STR_S42, io_display_string_input[41]);
	SetDlgItemText(hdlg, IDC_STR_S43, io_display_string_input[42]);
	SetDlgItemText(hdlg, IDC_STR_S44, io_display_string_input[43]);
	SetDlgItemText(hdlg, IDC_STR_S45, io_display_string_input[44]);
	SetDlgItemText(hdlg, IDC_STR_S46, io_display_string_input[45]);
	SetDlgItemText(hdlg, IDC_STR_S47, io_display_string_input[46]);
	SetDlgItemText(hdlg, IDC_STR_S48, io_display_string_input[47]);
													   
	SetDlgItemText(hdlg, IDC_STR_S49, io_display_string_input[48]);
	SetDlgItemText(hdlg, IDC_STR_S50, io_display_string_input[49]);
	SetDlgItemText(hdlg, IDC_STR_S51, io_display_string_input[50]);
	SetDlgItemText(hdlg, IDC_STR_S52, io_display_string_input[51]);
	SetDlgItemText(hdlg, IDC_STR_S53, io_display_string_input[52]);
	SetDlgItemText(hdlg, IDC_STR_S54, io_display_string_input[53]);
	SetDlgItemText(hdlg, IDC_STR_S55, io_display_string_input[54]);
	SetDlgItemText(hdlg, IDC_STR_S56, io_display_string_input[55]);
	SetDlgItemText(hdlg, IDC_STR_S57, io_display_string_input[56]);
	SetDlgItemText(hdlg, IDC_STR_S58, io_display_string_input[57]);
	SetDlgItemText(hdlg, IDC_STR_S59, io_display_string_input[58]);
	SetDlgItemText(hdlg, IDC_STR_S60, io_display_string_input[59]);
	SetDlgItemText(hdlg, IDC_STR_S61, io_display_string_input[60]);
	SetDlgItemText(hdlg, IDC_STR_S62, io_display_string_input[61]);
	SetDlgItemText(hdlg, IDC_STR_S63, io_display_string_input[62]);
	SetDlgItemText(hdlg, IDC_STR_S64, io_display_string_input[63]);
													   
	SetDlgItemText(hdlg, IDC_STR_S65, io_display_string_input[64]);
	SetDlgItemText(hdlg, IDC_STR_S66, io_display_string_input[65]);
	SetDlgItemText(hdlg, IDC_STR_S67, io_display_string_input[66]);
	SetDlgItemText(hdlg, IDC_STR_S68, io_display_string_input[67]);
	SetDlgItemText(hdlg, IDC_STR_S69, io_display_string_input[68]);
	SetDlgItemText(hdlg, IDC_STR_S70, io_display_string_input[69]);
	SetDlgItemText(hdlg, IDC_STR_S71, io_display_string_input[70]);
	SetDlgItemText(hdlg, IDC_STR_S72, io_display_string_input[71]);
	SetDlgItemText(hdlg, IDC_STR_S73, io_display_string_input[72]);
	SetDlgItemText(hdlg, IDC_STR_S74, io_display_string_input[73]);
	SetDlgItemText(hdlg, IDC_STR_S75, io_display_string_input[74]);
	SetDlgItemText(hdlg, IDC_STR_S76, io_display_string_input[75]);
	SetDlgItemText(hdlg, IDC_STR_S77, io_display_string_input[76]);
	SetDlgItemText(hdlg, IDC_STR_S78, io_display_string_input[77]);
	SetDlgItemText(hdlg, IDC_STR_S79, io_display_string_input[78]);
	SetDlgItemText(hdlg, IDC_STR_S80, io_display_string_input[79]);
}

void s_test::io_status_display()
{
	//===== input port0 = X10 input =======================================================
	if((mshm.INPUT0_STATUS & 0x01)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S1);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S1);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x02)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S2);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S2);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x04)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S3);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S3);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x08)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S4);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S4);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x10)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S5);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S5);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x20)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S6);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S6);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x40)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S7);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S7);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x80)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S8);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S8);
	//===== input port1 = X10 input =======================================================
	if((mshm.INPUT0_STATUS & 0x0100)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S9);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S9);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x0200)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S10);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S10);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x0400)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S11);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S11);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x0800)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S12);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S12);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x1000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S13);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S13);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x2000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S14);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S14);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x4000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S15);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S15);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT0_STATUS & 0x8000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S16);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S16);

	//===== input port2 = X11 input =======================================================
	if((mshm.INPUT1_STATUS & 0x01)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S17);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S17);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x02)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S18);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S18);

	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x04)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S19);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S19);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x08)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S20);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S20);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x10)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S21);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S21);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x20)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S22);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S22);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x40)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S23);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S23);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x80)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S24);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S24);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x0100)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S25);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S25);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x0200)==0x00) SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S26);
	else									SET_CHECKER_OFF(HWND_IO_INPUT,  IDC_IN_S26);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x0400)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S27);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S27);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x0800)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S28);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S28);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x1000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S29);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S29);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x2000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S30);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S30);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x4000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S31);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S31);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT1_STATUS & 0x8000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S32);
	else                                    SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S32);

	//===== input port0 = X10 input =======================================================
	if((mshm.INPUT2_STATUS & 0x01)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S33);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S33);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x02)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S34);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S34);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x04)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S35);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S35);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x08)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S36);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S36);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x10)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S37);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S37);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x20)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S38);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S38);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x40)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S39);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S39);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x80)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S40);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S40);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x0100)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S41);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S41);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x0200)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S42);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S42);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x0400)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S43);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S43);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x0800)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S44);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S44);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x1000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S45);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S45);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x2000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S46);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S46);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x4000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S47);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S47);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT2_STATUS & 0x8000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S48);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S48);

	//===== input port3 = X49 input =======================================================
	if((mshm.INPUT3_STATUS & 0x01)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S49);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S49);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x02)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S50);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S50);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x04)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S51);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S51);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x08)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S52);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S52);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x10)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S53);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S53);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x20)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S54);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S54);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x40)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S55);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S55);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x80)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S56);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S56);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x0100)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S57);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S57);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x0200)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S58);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S58);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x0400)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S59);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S59);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x0800)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S60);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S60);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x1000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S61);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S61);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x2000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S62);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S62);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x4000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S63);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S63);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT3_STATUS & 0x8000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S64);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S64);
	//===== input port4 = X65input =======================================================
	if((mshm.INPUT4_STATUS & 0x01)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S65);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S65);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x02)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S66);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S66);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x04)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S67);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S67);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x08)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S68);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S68);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x10)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S69);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S69);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x20)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S70);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S70);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x40)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S71);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S71);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x80)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S72);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S72);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x0100)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S73);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S73);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x0200)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S74);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S74);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x0400)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S75);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S75);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x0800)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S76);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S76);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x1000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S77);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S77);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x2000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S78);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S78);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x4000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S79);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S79);
	//-------------------------------------------------------------------------------------
	if((mshm.INPUT4_STATUS & 0x8000)==0x00)	SET_CHECKER_ON(HWND_IO_INPUT,  IDC_IN_S80);
	else									SET_CHECKER_OFF(HWND_IO_INPUT, IDC_IN_S80);

	//===== output port0 = X10 output =====================================================
	if((mshm.OUTPUT0_STATUS & 0x01)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y1);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y1);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x02)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y2);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y2);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x04)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y3);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y3);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x08)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y4);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y4);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x10)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y5);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y5);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x20)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y6);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y6);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x40)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y7);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y7);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x80)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y8);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y8);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x0100)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y9);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y9);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x0200)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y10);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y10);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x0400)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y11);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y11);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x0800)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y12);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y12);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x1000)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y13);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y13);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x2000)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y14);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y14);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x4000)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y15);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y15);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT0_STATUS & 0x8000)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y16);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y16);

	//===== output port1 = X11 output =====================================================
	if((mshm.OUTPUT1_STATUS & 0x01)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y17);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y17);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x02)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y18);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y18);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x04)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y19);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y19);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x08)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y20);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y20);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x10)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y21);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y21);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x20)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y22);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y22);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x40)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y23);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y23);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x80)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y24);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y24);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x0100)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y25);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y25);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x0200)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y26);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y26);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x0400)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y27);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y27);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x0800)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y28);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y28);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x1000)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y29);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y29);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x2000)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y30);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y30);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x4000)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y31);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y31);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT1_STATUS & 0x8000)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y32);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y32);
	//===== output port0 = X10 output =====================================================
	if((mshm.OUTPUT2_STATUS & 0x01)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y33);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y33);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x02)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y34);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y34);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x04)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y35);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y35);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x08)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y36);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y36);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x10)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y37);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y37);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x20)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y38);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y38);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x40)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y39);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y39);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x80)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y40);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y40);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x0100)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y41);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y41);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x0200)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y42);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y42);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x0400)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y43);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y43);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x0800)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y44);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y44);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x1000)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y45);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y45);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x2000)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y46);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y46);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x4000)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y47);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y47);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT2_STATUS & 0x8000)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y48);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y48);

	//===== output port2 = X11 output =====================================================
	if((mshm.OUTPUT3_STATUS & 0x01)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y49);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y49);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x02)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y50);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y50);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x04)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y51);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y51);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x08)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y52);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y52);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x10)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y53);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y53);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x20)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y54);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y54);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x40)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y55);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y55);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x80)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y56);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y56);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x0100)==0x01)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y57);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y57);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x0200)==0x02)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y58);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y58);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x0400)==0x04)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y59);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y59);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x0800)==0x08)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y60);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y60);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x1000)==0x10)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y61);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y61);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x2000)==0x20)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y62);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y62);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x4000)==0x40)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y63);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y63);
	//-------------------------------------------------------------------------------------
	if((mshm.OUTPUT3_STATUS & 0x8000)==0x80)	SET_CHECKER_ON(HWND_IO_OUTPUT,  IDC_OUT_Y64);
	else									SET_CHECKER_OFF(HWND_IO_OUTPUT,  IDC_OUT_Y64);
	
}