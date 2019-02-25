#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"

INIEdit test_config[] = {
	{IDC_DISPLAY_IMAGE,  DLGTYPE_CHECK_BOOL,  &s_test::test_option[TI_DISPLAY_IMAGE].on,	"item_display_image_on"},
	{IDC_STD_CHART,		 DLGTYPE_CHECK_BOOL, &s_test::test_option[TI_STD_CHART].on,			"item_std_chart_on" },
	{IDC_REGMARK_CHART,	 DLGTYPE_CHECK_BOOL, &s_test::test_option[TI_REG_MARK_CHART].on,	"item_reg_mark_chart_on"},
	{IDC_WHITE,			 DLGTYPE_CHECK_BOOL, &s_test::test_option[TI_WHITE].on,				"item_white_on"},
	
	{IDC_STD_CHART_ORDER,	  DLGTYPE_COMBO_INT, &s_test::test_option[TI_STD_CHART].order,		 "item_std_chart_order"},
	{IDC_REGMARK_CHART_ORDER, DLGTYPE_COMBO_INT, &s_test::test_option[TI_REG_MARK_CHART].order,	 "item_reg_mark_chart_order"},
	{IDC_WHITE_ORDER,		  DLGTYPE_COMBO_INT, &s_test::test_option[TI_WHITE].order,			 "item_white_order"},

	{IDC_STD_CHART_LOOP|(IDC_STD_CHART_CHECK<<16),		   DLGTYPE_RADIO_BOOL, &s_test::test_option[TI_STD_CHART].mode,      "item_std_chart_mode"},
	{IDC_REGMARK_CHART_LOOP|(IDC_REGMARK_CHART_CHECK<<16), DLGTYPE_RADIO_BOOL, &s_test::test_option[TI_REG_MARK_CHART].mode, "item_reg_mark_chart_mode"},
	{IDC_WHITE_LOOP|(IDC_WHITE_CHECK<<16),				   DLGTYPE_RADIO_BOOL, &s_test::test_option[TI_WHITE].mode,			 "item_white_mode"},

	{NULL, NULL, NULL, NULL}

};

int item_id[]   = {IDC_STD_CHART,		 IDC_REGMARK_CHART,		   IDC_WHITE};
int order_id[]  = {IDC_STD_CHART_ORDER,  IDC_REGMARK_CHART_ORDER,  IDC_WHITE_ORDER};
int loop_id[]   = {IDC_STD_CHART_LOOP,	 IDC_REGMARK_CHART_LOOP,   IDC_WHITE_LOOP};
int check_id[]  = {IDC_STD_CHART_CHECK,  IDC_REGMARK_CHART_CHECK,  IDC_WHITE_CHECK};
int result_id[] = {IDC_STD_CHART_RESULT, IDC_REGMARK_CHART_RESULT, IDC_WHITE_RESULT};
int time_id[]   = {IDC_STD_CHART_TIME,	 IDC_REGMARK_CHART_TIME,   IDC_WHITE_TIME};

class TestConfigDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);
private:
	int is_init_done;
};

TestConfigDlg tcdlg;

BOOL CALLBACK dlg_test_config(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG, tcdlg.init);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,	   tcdlg.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,	   tcdlg.close);
	}

	return FALSE;
}

BOOL TestConfigDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	is_init_done = FALSE;
	s_test::tconf.test_item_cnt = IMPORT_TEST_MODULE_CNT;
	char tmp[10] = {0};
	for (int j=0; j<TEST_ITEM_NUM; j++){
		for (int i=0; i<TEST_ITEM_NUM; i++){
			sprintf(tmp, "%d", i);
			COMBO_ADDSTR(hdlg, order_id[j], tmp);
		}

// 		ITEM_GRAYED(hdlg, item_id[j]);
// 		ITEM_GRAYED(hdlg, order_id[j]);
// 		ITEM_GRAYED(hdlg, loop_id[j]);
// 		ITEM_GRAYED(hdlg, check_id[j]);
// 		ITEM_GRAYED(hdlg, result_id[j]);
// 		ITEM_GRAYED(hdlg, time_id[j]);
	}
	ai_win::load_keys(s_test::config_path, "test", hdlg, test_config, sizeof(test_config)/sizeof(test_config[0]));
	is_init_done = TRUE;
	log("Dlg:Test Config Init OK.");
	return TRUE;
}

void TestConfigDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (code_notify)
	{
	case EN_CHANGE:
	case BN_CLICKED:
		ai_win::save_keys(s_test::config_path, "test", hdlg, test_config, sizeof(test_config)/sizeof(test_config[0]));
		break;
	}
}

void TestConfigDlg::close(HWND hdlg)
{
	s_test::dinfo.show_tc = FALSE;
	CheckMenuItem(s_test::hmenu, ID_TEST_CONFIG, MF_UNCHECKED);

	EndDialog(hdlg, FALSE);
}