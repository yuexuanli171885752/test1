#define _CRT_SECURE_NO_WARNINGS
#include "inf_cap_bd.h"
#include "interface.h"


class AIU3ConfigDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
private:
	char sensor_path[MAX_PATH];
	int reg_list_index;
	void fill_combo_edit(HWND hdlg, int idd, char (*name)[FOLDER_NAME_LENGTH], int count);
};

void AIU3ConfigDlg::fill_combo_edit(HWND hdlg, int idd, char (*name)[FOLDER_NAME_LENGTH], int count)
{
	COMBO_RESET(hdlg, idd);
	for (int i=0; i<count; i++){
		COMBO_ADDSTR(hdlg, idd, name[i]);
	}
}

AIU3ConfigDlg u3_dlg;
BOOL CALLBACK aiu3_config_dlg_proc(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, u3_dlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   u3_dlg.command);
	}

	return FALSE;
}

BOOL AIU3ConfigDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	
	return TRUE;
}

void AIU3ConfigDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{
	
}

