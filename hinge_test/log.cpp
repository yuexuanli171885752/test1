#include <Windows.h>
#include "interface.h"

class LogDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void size(HWND hwnd, unsigned int state, int width, int height);
	virtual void close(HWND hdlg);
	virtual LRESULT ctlcolor_listbox(HWND hdlg, HDC hdc, HWND hcolor, int mark/* =CTLCOLOR_EDIT */);

private:
	HBRUSH mybrush;
};

LogDlg log_dlg;
BOOL CALLBACK dlg_log(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,	    log_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		    log_dlg.command);
		HANDLE_DLGMSG(hdlg, WM_SIZE,		    log_dlg.size);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		    log_dlg.close);
		HANDLE_DLGMSG(hdlg, WM_CTLCOLORLISTBOX, log_dlg.ctlcolor_listbox);
	}
	return FALSE;
}

LRESULT LogDlg::ctlcolor_listbox(HWND hdlg, HDC hdc, HWND hcolor, int mark)
{
	DeleteObject(mybrush);
	SetBkMode(hdc, TRANSPARENT);
	mybrush = CreateSolidBrush(RGB(0x88,0x88,0x88));
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, mybrush);

	HFONT font     = CreateFont(15, 0, 0, 0, FW_BLACK, 0,0,0, ANSI_CHARSET, 0,0,0,0, "ºÚÌå");
	HFONT old_font = (HFONT)SelectObject(hdc, font);
 	LISTBOX_SETLHEIGHT(hdlg, IDC_LOG, 17);
	SetTextColor(hdc, COLOR_YELLOW_GREEN);

	DeleteObject(font);

	return (LRESULT)mybrush;
}

BOOL LogDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	RECT dlg_rect;
	ai_win::set_icon(hdlg, IDI_LOG, IDI_LOG);
	GetClientRect(hdlg, &dlg_rect);
	MoveWindow(GetDlgItem(hdlg, IDC_LOG), 0, 0, dlg_rect.right, dlg_rect.bottom, TRUE);

	return TRUE;
}

void LogDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (code_notify)
	{
	case LBN_DBLCLK:
		if (hctrl == GetDlgItem(hdlg, IDC_LOG)){
			LISTBOX_RESET(hdlg, IDC_LOG);
		}
		break;
	}
}

void LogDlg::size(HWND hdlg, unsigned int state, int width, int height)
{
	MoveWindow(GetDlgItem(hdlg, IDC_LOG), 0, 0, width, height, TRUE);
}

void LogDlg::close(HWND hdlg)
{
	s_test::dinfo.show_log = FALSE;
	CheckMenuItem(s_test::hmenu, ID_FILE_LOG, MF_UNCHECKED);

	EndDialog(hdlg, FALSE);
}