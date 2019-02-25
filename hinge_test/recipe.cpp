#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <XTrace.h>
extern HANDLE unique_mutex;
class RecipeDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, UINT code_notify);
	virtual void close(HWND hdlg);

private:
	void change_recipe(HWND hdlg, char *recipe_name);
};

RecipeDlg rec_dlg;
BOOL CALLBACK dlg_recipe(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, rec_dlg.init	 );
		HANDLE_DLGMSG(hdlg, WM_COMMAND,    rec_dlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   rec_dlg.close);
	}

	return 0;
}

BOOL RecipeDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
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
	sprintf(path, "%s\\recipe\\%s\\", s_test::dir, s_test::recipe_name);

	int file_cnt = ai_win::get_sub_files_name(path, file_name, 50, "*.ini");
	for (int i=0; i<file_cnt; i++){
		LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_FILE, file_name[i]);
	}

	log("Dlg:recipe setting init OK, select recipe[%s]", s_test::recipe_name);
	return TRUE;
}

void RecipeDlg::command(HWND hdlg, int id, HWND hctrl, UINT code_notify)
{
	switch (id)
	{
	case IDC_UPDATE:
		{
			int select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_CONFIG);
			char name[MAX_PATH] = {0};
			GetDlgItemText(hdlg, IDC_CONFIG_ITEM, name, sizeof(name));
			ai_win::clear_str(name, sizeof(name));
			strcat(name, "\n");
			LISTBOX_DELSTR(hdlg, IDC_RECIPE_LIST_CONFIG, select_num);
			LISTBOX_INSERTSTR(hdlg, IDC_RECIPE_LIST_CONFIG, select_num, name);
		}

		break;

	case IDC_CANCEL:
		{
			ITEM_ENABLED(hdlg, IDC_RECIPE_LIST);
			ITEM_ENABLED(hdlg, IDC_RECIPE_LIST_FILE);
			int select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_FILE);
			char name[FOLDER_NAME_LENGTH] = {0};
			LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST_FILE, select_num, name);
			char path[MAX_PATH] = {0};
			sprintf(path, "%s\\recipe\\%s\\%s", s_test::dir, s_test::recipe_name, name);
			char line[MAX_PATH] = {0};
			FILE *fp = fopen(path, "r");
			if (fp != NULL){
				LISTBOX_RESET(hdlg, IDC_RECIPE_LIST_CONFIG);
				while (fgets(line, MAX_PATH, fp)){
					LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_CONFIG, line);
				}
			}
			SetDlgItemText(hdlg, IDC_CONFIG_ITEM, "");
		}
		break;

	case IDC_SNAPSHOT:
		{
			int cnt = LISTBOX_TOTALCOUNT(hdlg, IDC_RECIPE_LIST_CONFIG);
			char buf[102400] = {0};
			char item_buf[100] = {0};
			for (int i=0; i<cnt; i++){
				LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST_CONFIG, i, item_buf);
				strcat(buf, item_buf);
			}

			int select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_FILE);
			char name[FOLDER_NAME_LENGTH] = {0};
			LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST_FILE, select_num, name);
			char path[MAX_PATH] = {0};
			sprintf(path, "%s\\recipe\\%s\\%s", s_test::dir, s_test::recipe_name, name);

			int write_byte = 0;
			int fh = _open(path, _O_RDWR|_S_IREAD|_S_IWRITE);
			if (fh != -1){
				write_byte = _write(fh, buf, strlen(buf));
				if (write_byte == -1){
					log("write fail.");
				}
				else log("write file :%s[%d]", name, write_byte);
			}
			_close(fh);

			ITEM_ACTIVE(hdlg, IDC_CANCEL);
		}
		break;

	case IDC_DEL_LINE:
		{
			int select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_CONFIG);
			LISTBOX_DELSTR(hdlg, IDC_RECIPE_LIST_CONFIG, select_num);
		}


		break;
	}
	switch (code_notify)
	{
	case LBN_DBLCLK:
		{
			int select_num = 0;
			char name[FOLDER_NAME_LENGTH] = {0};
			char str[MAX_PATH] = {0};
			if (id == IDC_RECIPE_LIST){
				select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST);
				LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST, select_num, name);
				sprintf(str, "Do u want change to recipe:%s? Software need to close and restart.", name);
				if (MessageBox(hdlg, str, "Info", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES){
					change_recipe(hdlg, name);
					CloseHandle(unique_mutex);
					PostQuitMessage(0);
					Sleep(500);

					char mybuf[MAX_PATH] = {0};
					GetCurrentDirectory(MAX_PATH, mybuf);
					char filename[MAX_PATH] = {0};
					GetModuleFileName(NULL, filename, MAX_PATH);

					STARTUPINFO sinf;
					PROCESS_INFORMATION pif;
					memset(&sinf, 0, sizeof(STARTUPINFO));
					sinf.cb = sizeof(STARTUPINFO);
					::CreateProcess(
						(LPCTSTR)filename,
						NULL, 
						NULL,
						NULL,
						FALSE,
						NORMAL_PRIORITY_CLASS,
						NULL,
						NULL,
						&sinf,
						&pif);
				}
			}
			else if (id == IDC_RECIPE_LIST_CONFIG){
				select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_CONFIG);
				LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST_CONFIG, select_num, name);
				SetDlgItemText(hdlg, IDC_CONFIG_ITEM, name);
				ITEM_GRAYED(hdlg, IDC_RECIPE_LIST);
				ITEM_GRAYED(hdlg, IDC_RECIPE_LIST_FILE);
			}
		}

		break;

	case LBN_SELCHANGE:
		if (id == IDC_RECIPE_LIST_FILE){
			int select_num = LISTBOX_GETCURSEL(hdlg, IDC_RECIPE_LIST_FILE);
			char name[FOLDER_NAME_LENGTH] = {0};
			LISTBOX_GETTEXT(hdlg, IDC_RECIPE_LIST_FILE, select_num, name);
			char path[MAX_PATH] = {0};
			sprintf(path, "%s\\recipe\\%s\\%s", s_test::dir, s_test::recipe_name, name);
			char line[MAX_PATH] = {0};
			FILE *fp = fopen(path, "r");
			if (fp != NULL){
				LISTBOX_RESET(hdlg, IDC_RECIPE_LIST_CONFIG);
				while (fgets(line, MAX_PATH, fp)){
					ai_win::replace_char(line, strlen(line), '\t', ' ');
					LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_CONFIG, line);
				}
			}
		}
		break;
	}
}

void RecipeDlg::close(HWND hdlg)
{
	s_test::dinfo.show_recipe = FALSE;
	CheckMenuItem(s_test::hmenu, ID_SETTING_RECIPE, MF_UNCHECKED);
	EndDialog(hdlg, FALSE);
}

void RecipeDlg::change_recipe(HWND hdlg, char *recipe_name)
{
	if (strcmp(recipe_name, s_test::recipe_name) != 0x00){
		strcpy(s_test::recipe_name, recipe_name);
		ai_win::write_string_key(s_test::config_path, "system", "default_recipe_setting", s_test::recipe_name);

		update_mdlg_title(HWND_MAIN);
		char title_name[MAX_PATH] = {0};
		sprintf(title_name, "Recipe Select: [%s]", s_test::recipe_name);
		SetWindowText(hdlg, title_name);

		SetDlgItemText(hdlg, IDC_SELECTED_RECIPE, s_test::recipe_name);

		COMBO_RESET(hdlg, IDC_RECIPE_LIST_FILE);
		COMBO_RESET(hdlg, IDC_RECIPE_LIST_CONFIG);

		char path[MAX_PATH] = {0};
		char file_name[50][FOLDER_NAME_LENGTH] = {0};
		sprintf(path, "%s\\recipe\\%s\\", s_test::dir, s_test::recipe_name);
		int file_cnt = ai_win::get_sub_files_name(path, file_name, 50, "ini");
		for (int i=0; i<file_cnt; i++){
			LISTBOX_ADDSTR(hdlg, IDC_RECIPE_LIST_FILE, file_name[i]);
		}

		log("Change recipe to [%s] success.", s_test::recipe_name);
	}
	else{
		log("You selected the same recipe!");
	}
}