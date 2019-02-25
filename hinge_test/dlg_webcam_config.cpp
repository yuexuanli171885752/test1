#define _CRT_SECURE_NO_WARNINGS
#include "inf_cap_bd.h"
#include "s_win.h"


class WebCamDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
};


WebCamDlg webcam_dlg;

BOOL CALLBACK webcam_dlg_proc(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, webcam_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   webcam_dlg.command);
	}

	return FALSE;
}

BOOL WebCamDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{

	return TRUE;
}

void WebCamDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{




}