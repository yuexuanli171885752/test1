#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
extern DWORD WINAPI image_info_thread(LPVOID temp);

class ImageInfoDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);
};

BOOL ImageInfoDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	COMBO_ADDSTR(hdlg, IDC_IMG_DISPLAY_MODE, "Compressed");
	COMBO_ADDSTR(hdlg, IDC_IMG_DISPLAY_MODE, "Uncompressed");

	char config_path[MAX_PATH] = {0};
	sprintf(config_path, "%s\\recipe\\config.ini", s_test::dir);
	IMG_DISPLAY_MODE = ai_win::read_integer_key(config_path, "img", "img_display_mode");

	if (IMG_DISPLAY_MODE != HALFTONE && IMG_DISPLAY_MODE != COLORONCOLOR){
		IMG_DISPLAY_MODE = HALFTONE;
	}

	if (IMG_DISPLAY_MODE == COLORONCOLOR) COMBO_SETCURSEL(hdlg, IDC_IMG_DISPLAY_MODE, 0);
	else								  COMBO_SETCURSEL(hdlg, IDC_IMG_DISPLAY_MODE, 1);


	//////////////////////////////////////////////////////////////////////////
	//set image para
	s_test::myimg.set_start_x(0);
	s_test::myimg.set_start_y(0);
	s_test::myimg.set_full_sized(FALSE);

	DWORD img_info_id	  = 0;
	HANDLE himginfo	  = CreateThread(NULL, 0, image_info_thread,		  (LPVOID)hdlg, 0, &img_info_id);
	CloseHandle(himginfo);
	
	return FALSE;
}

void ImageInfoDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch(id){
	case IDC_IMG_DISPLAY_MODE:
		{
			int index = COMBO_GETCURSEL(hdlg, id);
			IMG_DISPLAY_MODE = ((index ==0)?COLORONCOLOR:HALFTONE);
			char config_path[MAX_PATH] = {0};
			sprintf(config_path, "%s\\recipe\\config.ini", s_test::dir);
			ai_win::write_integer_key(config_path, "img", "img_display_mode", IMG_DISPLAY_MODE);
		}

		break;
		
	}
}

void ImageInfoDlg::close(HWND hdlg)
{
	CheckMenuItem(s_test::hmenu, ID_EDIT_IMAGEINFO, MF_UNCHECKED);
	EndDialog(hdlg, TRUE);
}

ImageInfoDlg imginfodlg;

BOOL CALLBACK dlg_img_info(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, imginfodlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   imginfodlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   imginfodlg.close);
	}

	return FALSE;
}

DWORD CALLBACK image_info_thread(LPVOID temp)
{
	HWND hdlg = (HWND)temp;
//	char format_str[][100] = {"Raw", "Bmp", "Jpeg", "Png", "Tiff", "Gif", "MIPIRaw"};
	char raw_format[][100] = {"YUV", "8bit", "10bit", "12bit"};
	char yuv_pixel_order[][100] = {"YCbYCr..", "YCrYCb..", "CbYCrY..", "CrYCbY.."};
	char raw_pixel_order[][100] = {"BG..GR..", "RG..GB..", "GB..RG..", "GR..BG.."};
	char demosaic[][100] = {"none", "2x2", "3x3"};

	while (1){
		while (!IMG_CAPTURE_READY) Sleep(10);
		SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_WIDHT,  IMG_WIDTH,  FALSE);
		SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_HEIGHT, IMG_HEIGHT, FALSE);
		SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_DEPTH,  IMG_DEPTH,  FALSE);
		SetDlgItemText(HWND_IMG_INFO, IDC_IMAGE_FORMAT, s_test::img_format_str[IMG_FORMAT]);

		if (IMG_FORMAT == IMG_RAW){
			if (IMG_RAW_FORMAT == RAW_YUV){
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, yuv_pixel_order[IMG_PIXEL_ORDER-1]);
				SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT*2, FALSE);
				ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT*2)/(1024*1024), 2);
			}
			else {
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, raw_pixel_order[IMG_PIXEL_ORDER-1]);
				if (IMG_RAW_FORMAT == RAW_8BIT){
					SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT, FALSE);
					ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT)/(1024*1024), 2);
				}
				else if (IMG_RAW_FORMAT == RAW_10BIT || IMG_RAW_FORMAT == RAW_12BIT){
					SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT*2, FALSE);
					ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT*2)/(1024*1024), 2);
				}
			}

			SetDlgItemText(HWND_IMG_INFO, IDC_RAW_FORMAT, raw_format[IMG_RAW_FORMAT]);
			SetDlgItemInt(HWND_IMG_INFO, IDC_RAW_HEAD_SIZE, IMG_RAW_HEADER, FALSE);
			SetDlgItemText(HWND_IMG_INFO, IDC_RAW_DEMOSAIC, demosaic[IMG_SELECT_DEMOSAIC]);
		}
		else if (IMG_FORMAT == IMG_MIPIRAW){
			if (IMG_FORMAT == RAW_8BIT){
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, raw_pixel_order[IMG_PIXEL_ORDER-1]);
				SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT, FALSE);
				ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT)/(1024*1024), 2);
			}
			else if (IMG_FORMAT == RAW_10BIT){
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, raw_pixel_order[IMG_PIXEL_ORDER-1]);
				SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT*5/4, FALSE);
				ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT*5/4)/(1024*1024), 2);
			}
			else if (IMG_FORMAT == RAW_12BIT){
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, raw_pixel_order[IMG_PIXEL_ORDER-1]);
				SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT*3/2, FALSE);
				ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT*3/2)/(1024*1024), 2);
			}
			else{
				SetDlgItemText(HWND_IMG_INFO, IDC_PIXEL_ORDER, "error");
				SetDlgItemText(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, "error");
			}

			SetDlgItemText(HWND_IMG_INFO, IDC_RAW_FORMAT, raw_format[IMG_RAW_FORMAT]);
			SetDlgItemInt(HWND_IMG_INFO, IDC_RAW_HEAD_SIZE, IMG_RAW_HEADER, FALSE);
			SetDlgItemText(HWND_IMG_INFO, IDC_RAW_DEMOSAIC, demosaic[IMG_SELECT_DEMOSAIC]);

		}
		else/* if (IMG_FORMAT == IMG_TYPE_BMP)*/{
			SetDlgItemInt(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE, IMG_WIDTH*IMG_HEIGHT*IMG_DEPTH, FALSE);
			ai_win::set_dlg_item_float(HWND_IMG_INFO, IDC_IMAGE_TOTAL_SIZE2, (float)(IMG_WIDTH*IMG_HEIGHT*IMG_DEPTH)/(1024*1024), 2);
		}

		

		Sleep(10);
	}



	return FALSE;
}