#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"

class Histogram:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void timer(HWND hdlg, unsigned int id);
	virtual void close(HWND hdlg);

private:
	void show_histogram(HWND hdlg);
	ai_win::Painting pt;
};

void Histogram::show_histogram(HWND hdlg)
{
	int cx = IMG_WIDTH;
	int cy = IMG_HEIGHT;
	int drawing_select = COMBO_GETCURSEL(hdlg, IDC_DRAW_TYPE);
	HWND hwnd_hist = GetDlgItem(hdlg, IDC_HISTOGRAM_DISPLAYER);
	int r_hist[256] = {0};
	int b_hist[256] = {0};
	int g_hist[256] = {0};
	int y_hist[256] = {0};
	int gr_hist[256] = {0};
	int gb_hist[256] = {0};
	BYTE val = 0;
	int max=0;
	if (GET_CHECKER(hdlg, IDC_SELECT_RGB_24BIT)){

		BYTE *af_bmp = new BYTE[cx*cy*3];
		BYTE *y_buf  = new BYTE[cx*cy*3];

		memcpy(af_bmp,s_test::bmp_buf, cx*cy*3);
		s_test::cimg.bmp24_to_y8(af_bmp, cx, cy, NULL, y_buf);

		for (int j=0; j<cy; j++){
			for (int i=0; i<cx; i++){
				val = af_bmp[j*cx*3+i*3];	b_hist[val]++;
				val = af_bmp[j*cx*3+i*3+1];	g_hist[val]++;
				val = af_bmp[j*cy*3+i*3+2];	r_hist[val]++;
				val = y_buf[j*cx+i];		y_hist[val]++;
			}	
		}

		for (int i=0; i<256; i++){
			if (GET_CHECKER(hdlg, IDC_CHANNEL_R) && max<r_hist[i])	max = r_hist[i];
			if (GET_CHECKER(hdlg, IDC_CHANNEL_G) && max<g_hist[i])	max = g_hist[i];
			if (GET_CHECKER(hdlg, IDC_CHANNEL_B) && max<b_hist[i])	max = b_hist[i];
			if (GET_CHECKER(hdlg, IDC_CHANNEL_Y) && max<y_hist[i])  max = y_hist[i];
		}
		delete [] y_buf;
		delete [] af_bmp;
	}
	else{
		BYTE *raw8 = new BYTE[IMG_WIDTH*IMG_HEIGHT];
		if (IMG_RAW_FORMAT == RAW_8BIT){
			memcpy(raw8, s_test::img_buf, IMG_WIDTH*IMG_HEIGHT);
		}
		else if (IMG_RAW_FORMAT == RAW_10BIT){
			ai_img::raw10_to_raw8(s_test::img_buf, raw8, IMG_WIDTH, IMG_HEIGHT);
		}
		else if (IMG_RAW_FORMAT == RAW_12BIT){
			ai_img::raw12_to_raw8(s_test::img_buf, raw8, IMG_WIDTH, IMG_HEIGHT);
		}
		else{
			log("it is not a correct format![%d]", IMG_RAW);
			return;
		}

		if (IMG_PIXEL_ORDER == RAW_OUTPUT_BGGR){
			for (int j=0; j<IMG_HEIGHT; j+=2){
				for (int i=0; i<IMG_WIDTH; i+=2){
					val = raw8[j*IMG_WIDTH+i];		 b_hist[val]++;
					val = raw8[j*IMG_WIDTH+i+1];	 gb_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i];	 gr_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i+1]; r_hist[val]++;
				}
			}
		}
		else if (IMG_PIXEL_ORDER == RAW_OUTPUT_RGGB){
			for (int j=0; j<IMG_HEIGHT; j+=2){
				for (int i=0; i<IMG_WIDTH; i+=2){
					val = raw8[j*IMG_WIDTH+i];		 r_hist[val]++;
					val = raw8[j*IMG_WIDTH+i+1];	 gr_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i];	 gb_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i+1]; b_hist[val]++;
				}
			}
		}
		else if (IMG_PIXEL_ORDER == RAW_OUTPUT_GBRG){
			for (int j=0; j<IMG_HEIGHT; j+=2){
				for (int i=0; i<IMG_WIDTH; i+=2){
					val = raw8[j*IMG_WIDTH+i];		 gb_hist[val]++;
					val = raw8[j*IMG_WIDTH+i+1];	 b_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i];	 r_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i+1]; gr_hist[val]++;
				}
			}
		}
		else if (IMG_PIXEL_ORDER == RAW_OUTPUT_GRBG){
			for (int j=0; j<IMG_HEIGHT; j+=2){
				for (int i=0; i<IMG_WIDTH; i+=2){
					val = raw8[j*IMG_WIDTH+i];		 gr_hist[val]++;
					val = raw8[j*IMG_WIDTH+i+1];	 r_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i];	 b_hist[val]++;
					val = raw8[(j+1)*IMG_WIDTH+i+1]; gb_hist[val]++;
				}
			}
		}
		else{
			log("it is not a correct pixel order![%d]", IMG_PIXEL_ORDER);
			return;
		}

		for (int i=0; i<256; i++){
			if (GET_CHECKER(hdlg, IDC_RAW_R) && max<r_hist[i])	 max = r_hist[i];
			if (GET_CHECKER(hdlg, IDC_RAW_GR) && max<gr_hist[i]) max = gr_hist[i];
			if (GET_CHECKER(hdlg, IDC_RAW_B) && max<b_hist[i])	 max = b_hist[i];
			if (GET_CHECKER(hdlg, IDC_RAW_Gb) && max<gb_hist[i]) max = gb_hist[i];
		}

		delete [] raw8;
	}

	int w = 256*5, h = max;
	if (max == 0){
		h = 1000;
		max = 1000;
	}
	int start_x = w/16, end_x = w/16;
	int start_y = h/16, end_y = h/16;
	int factor_x = 5, interval_x = 32;
	int factor_y = 1, interval_y = max/10;
	int full_x = w+start_x+end_x;
	int full_y = h+start_y+end_y;
	int font_size =max/25;
	int axis_x_offset[4] = {-15, 5, 15, 5};
	int axis_y_offset[4] = {-35, -8, -30, -22};

	if (interval_y == 0) interval_y = 1;

	HDC hdc = pt.create(hwnd_hist, w, h, start_x, end_x, start_y, end_y, RGB(0X66, 0X66, 0X66));
	SetTextColor(hdc, COLOR_WHITE);
	pt.set_axis(font_size, axis_x_offset, axis_y_offset);
	pt.draw_axis_x_int(hdc, factor_x, interval_x, "");
	pt.draw_axis_y_int(hdc, factor_y, interval_y, NULL);

	HFONT font     = CreateFont(font_size, 0, 0, 0, FW_BLACK, 0,0,0, ANSI_CHARSET, 0,0,0,0, "ºÚÌå");
	HFONT old_font = (HFONT)SelectObject(hdc, font);
	char title[100] = {0};
	sprintf(title, "Histogram (Max Y:%d)", max);
	TextOut(hdc, start_x+100, 10, title, strlen(title));
	DeleteObject(font);
	const int pen_width = 1;

	if (GET_CHECKER(hdlg, IDC_SELECT_RGB_24BIT)){
		HPEN pen = CreatePen(PS_SOLID, pen_width, COLOR_RED);
		HPEN old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_CHANNEL_R)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+start_x, full_y - start_y, NULL);
				LineTo(hdc, i*factor_x+start_x, full_y - (r_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, COLOR_LIME_GREEN);
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_CHANNEL_G)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+1+start_x, full_y-start_y, NULL);
				LineTo(hdc, i*factor_x+1+start_x, full_y-(g_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, COLOR_BLUE);
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_CHANNEL_B)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+2+start_x, full_y-start_y, NULL);	
				LineTo(hdc, i*factor_x+2+start_x, full_y-(b_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, RGB(0x66, 0x66, 0x66));
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_CHANNEL_Y)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+3+start_x, full_y-start_y, NULL);
				LineTo(hdc, i*factor_x+3+start_x, full_y-(y_hist[i]+start_y));
			}
		}
		DeleteObject(pen);
	}
	else{
		HPEN pen = CreatePen(PS_SOLID, pen_width, COLOR_RED);
		HPEN old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_RAW_R)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+start_x, full_y - start_y, NULL);
				LineTo(hdc, i*factor_x+start_x, full_y - (r_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, COLOR_LIME_GREEN);
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_RAW_GR)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+1+start_x, full_y-start_y, NULL);
				LineTo(hdc, i*factor_x+1+start_x, full_y-(gr_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, COLOR_BLUE);
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_RAW_B)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+2+start_x, full_y-start_y, NULL);	
				LineTo(hdc, i*factor_x+2+start_x, full_y-(b_hist[i]+start_y));
			}
		}

		DeleteObject(pen);
		pen = CreatePen(PS_SOLID, pen_width, COLOR_GREEN);
		old_pen = (HPEN)SelectObject(hdc, pen);

		if (GET_CHECKER(hdlg, IDC_RAW_Gb)){
			MoveToEx(hdc, start_x, full_y-start_y, NULL);
			for (int i=0; i<256; i++){
				if (drawing_select == 0) MoveToEx(hdc, i*factor_x+3+start_x, full_y-start_y, NULL);
				LineTo(hdc, i*factor_x+3+start_x, full_y-(gb_hist[i]+start_y));
			}
		}
		DeleteObject(pen);
	}

	pt.release(hwnd_hist, hdc);
}

BOOL Histogram::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	COMBO_ADDSTR(hdlg, IDC_DRAW_TYPE, "curve");
	COMBO_ADDSTR(hdlg, IDC_DRAW_TYPE, "bar");
	COMBO_SETCURSEL(hdlg, IDC_DRAW_TYPE, 0);
	SET_CHECKER(hdlg, IDC_CHANNEL_R, TRUE);
	SET_CHECKER(hdlg, IDC_CHANNEL_G, TRUE);
	SET_CHECKER(hdlg, IDC_CHANNEL_B, TRUE);
	SET_CHECKER_ON(hdlg, IDC_RAW_R);
	SET_CHECKER_ON(hdlg, IDC_RAW_GR);
	SET_CHECKER_ON(hdlg, IDC_RAW_B);
	SET_CHECKER_ON(hdlg, IDC_RAW_Gb);
	IS_HISTOGRAM_DLG_SHOW = TRUE;
	SET_CHECKER(hdlg, IDC_SELECT_RGB_24BIT, TRUE);
	SET_CHECKER(hdlg, IDC_SELECT_RAW_8BIT, FALSE);
	SetTimer(hdlg, 1, 500, NULL);

	log("open histogram dialog.");

	return TRUE;
}

void Histogram::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (id)
	{
	case IDC_SAVE_HISTOGRAM:
		{
			char file_path[MAX_PATH] = {0};
			ai_win::save_file(hdlg, file_path, FILE_TYPE_BMP_ONLY);
			ai_img::save_drawing_image(GetDlgItem(hdlg, IDC_HISTOGRAM_DISPLAYER), file_path);
		}

		break;

	case IDC_SELECT_RGB_24BIT:
		if (GET_CHECKER(hdlg, id)){
			ITEM_GRAYED(hdlg, IDC_RAW_R);
			ITEM_GRAYED(hdlg, IDC_RAW_GR);
			ITEM_GRAYED(hdlg, IDC_RAW_B);
			ITEM_GRAYED(hdlg, IDC_RAW_Gb);
			ITEM_ENABLED(hdlg, IDC_CHANNEL_B);
			ITEM_ENABLED(hdlg, IDC_CHANNEL_G);
			ITEM_ENABLED(hdlg, IDC_CHANNEL_R);
			ITEM_ENABLED(hdlg, IDC_CHANNEL_Y);
		}
		break;

	case IDC_SELECT_RAW_8BIT:
		if (GET_CHECKER(hdlg, id)){
			ITEM_ENABLED(hdlg, IDC_RAW_R);
			ITEM_ENABLED(hdlg, IDC_RAW_GR);
			ITEM_ENABLED(hdlg, IDC_RAW_B);
			ITEM_ENABLED(hdlg, IDC_RAW_Gb);

			ITEM_GRAYED(hdlg, IDC_CHANNEL_B);
			ITEM_GRAYED(hdlg, IDC_CHANNEL_G);
			ITEM_GRAYED(hdlg, IDC_CHANNEL_R);
			ITEM_GRAYED(hdlg, IDC_CHANNEL_Y);
		}

		break;
	}
}

void Histogram::timer(HWND hdlg, unsigned int id)
{
	switch (id)
	{
	case 1:
		if (IMG_CAPTURE_READY == TRUE && IS_HISTOGRAM_DLG_SHOW){
			if (IMG_FORMAT == IMG_RAW){
				ITEM_ENABLED(hdlg, IDC_SELECT_RAW_8BIT);
			}
			else{
				SET_CHECKER_ON(hdlg, IDC_SELECT_RGB_24BIT);
				ITEM_GRAYED(hdlg, IDC_SELECT_RAW_8BIT);
				ITEM_GRAYED(hdlg, IDC_RAW_R);
				ITEM_GRAYED(hdlg, IDC_RAW_GR);
				ITEM_GRAYED(hdlg, IDC_RAW_B);
				ITEM_GRAYED(hdlg, IDC_RAW_Gb);
				ITEM_ENABLED(hdlg, IDC_CHANNEL_B);
				ITEM_ENABLED(hdlg, IDC_CHANNEL_G);
				ITEM_ENABLED(hdlg, IDC_CHANNEL_R);
				ITEM_ENABLED(hdlg, IDC_CHANNEL_Y);
			}

		}
		show_histogram(hdlg);
		Sleep(100);
		break;
	}
}

void Histogram::close(HWND hdlg)
{
	IS_HISTOGRAM_DLG_SHOW = FALSE;
	EndDialog(hdlg, TRUE);
	log("close histogram dialog.");
}

Histogram histo;
BOOL CALLBACK histogram_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, histo.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   histo.command);
		HANDLE_DLGMSG(hdlg, WM_TIMER,	   histo.timer);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   histo.close);
	}
	return FALSE;
}
