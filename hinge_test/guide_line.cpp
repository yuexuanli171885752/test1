#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"

INIEdit gl_conf[] ={
	{IDC_SHOW_IMAGE_CENTER_LINE,	DLGTYPE_CHECK_BOOL,  &s_test::gline.show_center_line,	"show_center_line"},
	{IDC_IMAGE_CENTER_LINE_WIDTH,	DLGTYPE_INPUT_INT,	 &s_test::gline.center_line_width,	"center_line_width"},
	{IDC_IMAGE_CENTER_LINE_TYPE,	DLGTYPE_COMBO_INT,	 &s_test::gline.center_line_type,	"center_line_type"},
	{IDC_IMAGE_CENTER_LINE_COLOR,	DLGTYPE_COMBO_INT,	 &s_test::gline.center_line_color,	"center_line_color"},
	{IDC_SHOW_IMAGE_FIELD_CIRCLE,	DLGTYPE_CHECK_BOOL,  &s_test::gline.show_field_line,	"show_field_line"},
	{IDC_IMAGE_FIELD_LINE_WIDTH,	DLGTYPE_INPUT_INT,	 &s_test::gline.field_line_width,	"field_line_width"},
	{IDC_IMAGE_FIELD_LINE_TYPE,		DLGTYPE_COMBO_INT,	 &s_test::gline.field_line_type,	"field_line_type"},
	{IDC_IMAGE_FIELD_LINE_COLOR,	DLGTYPE_COMBO_INT,	 &s_test::gline.field_line_color,	"field_line_color"},
	{IDC_IMAGE_FIELD_LINE_LOCATION, DLGTYPE_INPUT_FLOAT, &s_test::gline.field_location,    "field_line_location"},
	{NULL, NULL, NULL, NULL}
};

class GuideLineDlg:ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);
};

void set_guide_line_color(void)
{
	if (s_test::gline.center_line_color == 0)	    s_test::gline.center_cr = COLOR_RED;
	else if (s_test::gline.center_line_color == 1) s_test::gline.center_cr = COLOR_GREEN;
	else if (s_test::gline.center_line_color == 2) s_test::gline.center_cr = COLOR_BLUE;
	else if (s_test::gline.center_line_color == 3) s_test::gline.center_cr = COLOR_YELLOW;
	else if (s_test::gline.center_line_color == 4) s_test::gline.center_cr = COLOR_YELLOW_GREEN;

	if (s_test::gline.field_line_color == 0)	   s_test::gline.field_cr = COLOR_RED;
	else if (s_test::gline.field_line_color == 1) s_test::gline.field_cr = COLOR_GREEN;
	else if (s_test::gline.field_line_color == 2) s_test::gline.field_cr = COLOR_BLUE;
	else if (s_test::gline.field_line_color == 3) s_test::gline.field_cr = COLOR_YELLOW;
	else if (s_test::gline.field_line_color == 4) s_test::gline.field_cr = COLOR_YELLOW_GREEN;
}

BOOL GuideLineDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	char str[50] = {0};
	for (int i=0; i<10; i++){
		sprintf(str, "%d", i+1);
		COMBO_ADDSTR(hdlg, IDC_IMAGE_CENTER_LINE_WIDTH, str);
		COMBO_ADDSTR(hdlg, IDC_IMAGE_FIELD_LINE_WIDTH, str);
	}

	char line_type[][20] = {"Solid", "Dash", "Dot", "DashDot", "DashDotDot"};
	char line_color[][20] = {"Red", "Green", "Blue", "Yellow", "YellowGreen"};

	int index = sizeof(line_type)/sizeof(line_type[0]);
	for (int i=0; i<index; i++){
		COMBO_ADDSTR(hdlg, IDC_IMAGE_CENTER_LINE_TYPE, line_type[i]);
		COMBO_ADDSTR(hdlg, IDC_IMAGE_FIELD_LINE_TYPE, line_type[i]);
	}

	index = sizeof(line_color)/sizeof(line_color[0]);
	for (int i=0; i<index; i++){
		COMBO_ADDSTR(hdlg, IDC_IMAGE_CENTER_LINE_COLOR, line_color[i]);
		COMBO_ADDSTR(hdlg, IDC_IMAGE_FIELD_LINE_COLOR, line_color[i]);
	}
	char path[MAX_PATH] = {0};

	ai_win::load_keys(s_test::config_path, "guide_line", hdlg, gl_conf, sizeof(gl_conf)/sizeof(gl_conf[0]));
	set_guide_line_color();
	return TRUE;
}

void GuideLineDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (id)
	{
	case IDC_SAVE:
		ai_win::save_keys(s_test::config_path, "guide_line", hdlg, gl_conf, sizeof(gl_conf)/sizeof(gl_conf[0]));
		set_guide_line_color();
		log("guide line configure saved.");
		break;
	}
}

void GuideLineDlg::close(HWND hdlg)
{
	EndDialog(hdlg, TRUE);
}

GuideLineDlg guidedlg;

BOOL CALLBACK guide_line_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, guidedlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   guidedlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   guidedlg.close);
	}

	return FALSE;
}

