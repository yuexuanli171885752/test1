#include "interface.h"
#include "inf_motion_aa.h"
#include "defio.h"
#include "s_define.h"

#pragma warning(disable:4996)
extern MOTION_SHM mshm;
extern void SetInPutStatus();

class ioStatusDlg_input:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
	virtual void close(HWND hdlg);

private:
};
ioStatusDlg_input io_dlg_input;

BOOL CALLBACK ioStatus_input_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, io_dlg_input.init	 );
		HANDLE_DLGMSG(hdlg, WM_COMMAND,    io_dlg_input.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   io_dlg_input.close);
	}
	return 0;
}

BOOL ioStatusDlg_input::init(HWND hdlg, HWND hfocus, LPARAM linit)
{

	//============================================================================================================
	initial_setting();
	//log("IO Card Init OK.");

	ai_win::read_string_key(s_test::config_path, "system", "default_recipe_setting", s_test::recipe_name, sizeof(s_test::recipe_name));
	SetDlgItemText(hdlg, IDC_SELECTED_RECIPE, s_test::recipe_name);

	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\", s_test::dir);
	char folder_name[50][FOLDER_NAME_LENGTH] = {0};
	int folder_cnt = ai_win::get_sub_folders_name(path, folder_name, 50);

	int index = 0;
	for (int i=0; i<folder_cnt; i++){
		LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST, folder_name[i]);
		if (!strcmp(folder_name[i], s_test::recipe_name)){
			index = i;
		}
	}

	LISTBOX_SETCURSEL(hdlg, IDC_RECIPE_LIST, index);

	char title_name[MAX_PATH] = {0};
	sprintf(title_name, "Recipe Select: [%s]", s_test::recipe_name);
	SetWindowText(hdlg, title_name);

	char file_name[50][FOLDER_NAME_LENGTH] = {0};
	sprintf(path, "%s\\recipe\\io_display_input.ini", s_test::dir);

	int file_cnt = ai_win::get_sub_files_name(path, file_name, 50, "*.ini");
	for (int i=0; i<file_cnt; i++){
		LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_FILE, file_name[i]);
	}

	log("Dlg:recipe setting init OK, select recipe[%s]", s_test::recipe_name);
	return TRUE;
}

void ioStatusDlg_input::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{

	
}


void ioStatusDlg_input::close(HWND hdlg)
{
	s_test::dinfo.show_io2_dlg = FALSE;
	CheckMenuItem(s_test::hmenu, ID_IO, MF_UNCHECKED);
	EndDialog(hdlg, FALSE);
}

DWORD CALLBACK ioStatusThread(LPVOID temp){
	HWND hdlg = (HWND) temp;
	//	SET_CHECKER(HWND_IO_OUTPUT,IDC_OUT_Y1,);

	while(1){
		Sleep(10);
		SetInPutStatus();
		
		//		IO_monitor_menu();
	}

	return FALSE;
}

void SetInPutStatus(){
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S1,S01);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S2,S02);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S3,S03);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S4,S04);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S5,S05);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S6,S06);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S7,S07);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S8,S08);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S9,S09);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S10,S10);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S11,S11);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S12,S12);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S13,S13);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S14,S14);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S15,S15);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S16,S16);

	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S17,S17);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S18,S18);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S19,S19);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S20,S20);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S21,S21);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S22,S22);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S23,S23);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S24,S24);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S25,S25);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S26,S26);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S27,S27);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S28,S28);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S29,S29);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S30,S30);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S31,S31);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S32,S32);

	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S33,S33);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S34,S34);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S35,S35);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S36,S36);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S37,S37);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S38,S38);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S39,S39);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S40,S40);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S41,S41);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S42,S42);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S43,S43);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S44,S44);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S45,S45);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S46,S46);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S47,S47);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S48,S48);

	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S49,S49);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S50,S50);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S51,S51);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S52,S52);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S53,S53);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S54,S54);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S55,S55);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S56,S56);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S57,S57);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S58,S58);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S59,S59);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S60,S60);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S61,S61);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S62,S62);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S63,S63);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S64,S64);

	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S65,S65);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S66,S66);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S67,S67);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S68,S68);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S69,S69);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S70,S70);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S71,S71);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S72,S72);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S73,S73);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S74,S74);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S75,S75);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S76,S76);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S77,S77);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S78,S78);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S79,S79);
	SET_CHECKER(HWND_IO_INPUT,IDC_IN_S80,S80);
}
