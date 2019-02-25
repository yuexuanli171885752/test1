#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"

class MeasureDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);
};


BOOL MeasureDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	SET_CHECKER(hdlg, IDC_SELECT_RGB24, TRUE);
	SET_CHECKER(hdlg, IDC_SELECT_RAW8, FALSE);
	ITEM_GRAYED(hdlg, IDC_SELECT_RAW8);

	return TRUE;
}

void MeasureDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{

}

void MeasureDlg::close(HWND hdlg)
{
	SET_STATUSBAR_TEXT(HWND_STATUS, SBINDEX_TEXT, "");
	ENABLE_MEASURE = FALSE;
	IS_IMAGE_SCENERY_OCCUPIED = FALSE;
	log("mutex scenery released.");
	EndDialog(hdlg, TRUE);
}

MeasureDlg measuredlg;
BOOL CALLBACK measure_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, measuredlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   measuredlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   measuredlg.close);
	}

	return FALSE;
}
