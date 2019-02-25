#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"

char shared_raw_img_path[MAX_PATH] = {0};
class RawConfigDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
private:
	int set_raw_config(HWND hdlg);
};

RawConfigDlg raw_conf_dlg;


BOOL CALLBACK raw_config_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, raw_conf_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   raw_conf_dlg.command);
	}

	return FALSE;
}

BOOL RawConfigDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	char *token = NULL;
	char sep[] = ",";
	int index = 0;

	char buf[256] = {0};
	char img_width[20][10]  = {0};
	char img_height[20][10] = {0};
	char img_header[20][10] = {0};
	ai_win::read_string_key(s_test::config_path, "img", "width_pair", buf, sizeof(buf));
	int select_w = ai_win::read_integer_key(s_test::config_path, "img", "select_width");
	token = strtok(buf, sep);

	while (token != NULL){
		strcpy(img_width[index], token);
		ai_win::clear_str(img_width[index], sizeof(img_width[0]));
		COMBO_ADDSTR(hdlg, IDC_SELECT_IMG_W, img_width[index]);
		index++;
		token = strtok(NULL, sep);
	}
	COMBO_SETCURSEL(hdlg, IDC_SELECT_IMG_W, select_w);

	index = 0;
	memset(buf, 0, sizeof(buf));
	ai_win::read_string_key(s_test::config_path, "img", "height_pair", buf, sizeof(buf));
	int select_h = ai_win::read_integer_key(s_test::config_path, "img", "select_height");
	token = strtok(buf, sep);

	while (token != NULL){
		strcpy(img_height[index], token);
		ai_win::clear_str(img_height[index], sizeof(img_height[0]));
		COMBO_ADDSTR(hdlg, IDC_SELECT_IMG_H, img_height[index]);
		index++;
		token = strtok(NULL, sep);
	}
	COMBO_SETCURSEL(hdlg, IDC_SELECT_IMG_H, select_h);

	int enable_customized_size = ai_win::read_integer_key(s_test::config_path, "img", "enable_customized_img_size");
	SET_CHECKER(hdlg, IDC_ENABLE_CUSTOMIZED_SIZE, enable_customized_size);
	int customized_img_w = ai_win::read_integer_key(s_test::config_path, "img", "customzied_img_w");
	int customzied_img_h = ai_win::read_integer_key(s_test::config_path, "img", "customzied_img_h");
	SetDlgItemInt(hdlg, IDC_CUSTOMIZED_IMG_W, customized_img_w, FALSE);
	SetDlgItemInt(hdlg, IDC_CUSTOMIZED_IMG_H, customzied_img_h, FALSE);
	if (enable_customized_size){
		ITEM_GRAYED(hdlg, IDC_SELECT_IMG_W);
		ITEM_GRAYED(hdlg, IDC_SELECT_IMG_H);
		ITEM_ENABLED(hdlg, IDC_CUSTOMIZED_IMG_W);
		ITEM_ENABLED(hdlg, IDC_CUSTOMIZED_IMG_H);
	}
	else{
		ITEM_ENABLED(hdlg, IDC_SELECT_IMG_W);
		ITEM_ENABLED(hdlg, IDC_SELECT_IMG_H);
		ITEM_GRAYED(hdlg, IDC_CUSTOMIZED_IMG_W);
		ITEM_GRAYED(hdlg, IDC_CUSTOMIZED_IMG_H);
	}
	
	index = 0;
	memset(buf, 0, sizeof(buf));
	ai_win::read_string_key(s_test::config_path, "img", "header_pair", buf, sizeof(buf));
	int select_header = ai_win::read_integer_key(s_test::config_path, "img", "select_header");
	token = strtok(buf, sep);

	while (token != NULL){
		strcpy(img_header[index], token);
		ai_win::clear_str(img_header[index], sizeof(img_header[0]));
		COMBO_ADDSTR(hdlg, IDC_RAW_HEAD_OFFSET, img_header[index]);
		index++;
		token = strtok(NULL, sep);
	}
	COMBO_SETCURSEL(hdlg, IDC_RAW_HEAD_OFFSET, select_header);

	COMBO_ADDSTR(hdlg, IDC_PIXEL_ORDER, "BG..GR..");
	COMBO_ADDSTR(hdlg, IDC_PIXEL_ORDER, "RG..GB..");
	COMBO_ADDSTR(hdlg, IDC_PIXEL_ORDER, "GB..RG..");
	COMBO_ADDSTR(hdlg, IDC_PIXEL_ORDER, "GR..BG..");

	IMG_PIXEL_ORDER = ai_win::read_integer_key(s_test::config_path, "img", "pixel_oder");
	if (IMG_PIXEL_ORDER<1 || IMG_PIXEL_ORDER>4) IMG_PIXEL_ORDER = 1;
	COMBO_SETCURSEL(hdlg, IDC_PIXEL_ORDER, IMG_PIXEL_ORDER-1);

	COMBO_ADDSTR(hdlg, IDC_RAW_FORMAT, "YUV422");
	COMBO_ADDSTR(hdlg, IDC_RAW_FORMAT, "RAW 8bit");
	COMBO_ADDSTR(hdlg, IDC_RAW_FORMAT, "RAW 10bit");
	COMBO_ADDSTR(hdlg, IDC_RAW_FORMAT, "RAW 12bit");
	COMBO_ADDSTR(hdlg, IDC_RAW_FORMAT, "RAW Mipi10bit");

	IMG_RAW_FORMAT = ai_win::read_integer_key(s_test::config_path, "img", "raw_format");
	if (IMG_RAW_FORMAT<0 || IMG_RAW_FORMAT >4) IMG_RAW_FORMAT = 2;
	COMBO_SETCURSEL(hdlg, IDC_RAW_FORMAT, IMG_RAW_FORMAT);

	COMBO_ADDSTR(hdlg, IDC_RAW_DEMOSAIC, "none");
	COMBO_ADDSTR(hdlg, IDC_RAW_DEMOSAIC, "2x2");
	COMBO_ADDSTR(hdlg, IDC_RAW_DEMOSAIC, "3x3");
	IMG_SELECT_DEMOSAIC = ai_win::read_integer_key(s_test::config_path, "img", "select_demosaic");
//	if (IMG_SELECT_DEMOSAIC<0 || IMG_SELECT_DEMOSAIC >2) IMG_SELECT_DEMOSAIC = 0;
	COMBO_SETCURSEL(hdlg, IDC_RAW_DEMOSAIC, IMG_SELECT_DEMOSAIC);

	return TRUE;
}

void RawConfigDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{
	switch (id)
	{
	case IDC_APPLY:
		{
			int result = set_raw_config(hdlg);
			float read_pixel_width = 0.0f;
			if (result == TRUE){
				if (result == TRUE){
					BYTE *mybuf = new BYTE[MAX_IMG_WIDTH*MAX_IMG_HEIGHT*2];
					IMG_DEPTH = 3;
					BYTE head_buf[500] = {0};
					float read_pixel_width = ai_img::read_raw(shared_raw_img_path, mybuf, IMG_WIDTH, IMG_HEIGHT, IMG_RAW_HEADER, head_buf);
					BYTE *mybmp = new BYTE[IMG_WIDTH*IMG_HEIGHT*3];
					result = parse_raw(mybuf, mybmp, IMG_WIDTH, IMG_HEIGHT, read_pixel_width);
					if (result == TRUE){
						log("img_depth:%d", IMG_DEPTH);
						IMG_WIDTH = (IMG_WIDTH+3)/4*4;
						s_test::myimg.display(GetDlgItem(hdlg, IDC_DISPLAY), mybmp, IMG_WIDTH, IMG_HEIGHT, IMG_DEPTH);
						s_test::myimg.clear_bmp_stream();
					}
					else{
						MessageBox(hdlg, "Parse raw format is not correct.", "Error", 0);
					}

					delete [] mybuf;
					delete [] mybmp;
				}
			}
		}
		break;

	case IDC_OK:
		{
			int result = set_raw_config(hdlg);
			EndDialog(hdlg, result);
		}
		break;

	case IDC_ENABLE_CUSTOMIZED_SIZE:
		if (GET_CHECKER(hdlg, id)){
			ITEM_GRAYED(hdlg, IDC_SELECT_IMG_W);
			ITEM_GRAYED(hdlg, IDC_SELECT_IMG_H);
			ITEM_ENABLED(hdlg, IDC_CUSTOMIZED_IMG_W);
			ITEM_ENABLED(hdlg, IDC_CUSTOMIZED_IMG_H);
		}
		else{
			ITEM_ENABLED(hdlg, IDC_SELECT_IMG_W);
			ITEM_ENABLED(hdlg, IDC_SELECT_IMG_H);
			ITEM_GRAYED(hdlg, IDC_CUSTOMIZED_IMG_W);
			ITEM_GRAYED(hdlg, IDC_CUSTOMIZED_IMG_H);
		}
		break;

	case IDC_CANCEL:

		EndDialog(hdlg, FALSE);
		break;
	}

}

int RawConfigDlg::set_raw_config(HWND hdlg)
{
	IMG_PIXEL_ORDER	    = COMBO_GETCURSEL(hdlg, IDC_PIXEL_ORDER)+1;
	IMG_RAW_FORMAT	    = COMBO_GETCURSEL(hdlg, IDC_RAW_FORMAT);
	IMG_SELECT_DEMOSAIC = COMBO_GETCURSEL(hdlg, IDC_RAW_DEMOSAIC);

	ai_win::write_integer_key(s_test::config_path, "img", "pixel_oder",	  IMG_PIXEL_ORDER);
	ai_win::write_integer_key(s_test::config_path, "img", "raw_format",	  IMG_RAW_FORMAT);
	ai_win::write_integer_key(s_test::config_path, "img", "select_demosaic", IMG_SELECT_DEMOSAIC);

	char buf[20] = {0};
	int index = COMBO_GETCURSEL(hdlg, IDC_SELECT_IMG_W);
	COMBO_GETTEXT(hdlg, IDC_SELECT_IMG_W, index, buf);

	if (GET_CHECKER(hdlg, IDC_ENABLE_CUSTOMIZED_SIZE)){
		IMG_WIDTH = GetDlgItemInt(hdlg, IDC_CUSTOMIZED_IMG_W, NULL, FALSE);
		ai_win::write_integer_key(s_test::config_path, "img", "customzied_img_w", IMG_WIDTH);
		ai_win::write_integer_key(s_test::config_path, "img", "enable_customized_img_size", TRUE);
	}
	else{
		IMG_WIDTH = atoi(buf);
		ai_win::write_integer_key(s_test::config_path, "img", "select_width",	  index);
		ai_win::write_integer_key(s_test::config_path, "img", "enable_customized_img_size", FALSE);
	}

	memset(buf, 0, sizeof(buf));
	index = COMBO_GETCURSEL(hdlg, IDC_SELECT_IMG_H);
	COMBO_GETTEXT(hdlg, IDC_SELECT_IMG_H, index, buf);
	if (GET_CHECKER(hdlg, IDC_ENABLE_CUSTOMIZED_SIZE)){
		IMG_HEIGHT = GetDlgItemInt(hdlg, IDC_CUSTOMIZED_IMG_H, NULL, FALSE);
		ai_win::write_integer_key(s_test::config_path, "img", "customzied_img_h", IMG_HEIGHT);
	}
	else{
		IMG_HEIGHT = atoi(buf);
		ai_win::write_integer_key(s_test::config_path, "img", "select_height",	  index);
	}
	memset(buf, 0, sizeof(buf));
	index = COMBO_GETCURSEL(hdlg, IDC_RAW_HEAD_OFFSET);
	COMBO_GETTEXT(hdlg, IDC_RAW_HEAD_OFFSET, index, buf);
	IMG_RAW_HEADER = atoi(buf);
	ai_win::write_integer_key(s_test::config_path, "img", "select_header",	  index);

	if (IMG_WIDTH>MAX_IMG_WIDTH || IMG_HEIGHT>MAX_IMG_HEIGHT){
		MessageBox(hdlg, "Setting Image Size is over limit.", "Error", 0);
		return FALSE;
	}

	return TRUE;
}
