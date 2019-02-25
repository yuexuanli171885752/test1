#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
#include "test_light.h"
using namespace s_test;
Light_V light_v;
void image_cut(BYTE *buf,int w,int h,int leftupx,int leftupy,int CutWidth,int CutHeight,BYTE *buf1)
{

	for (int i = 0; i < CutHeight; i++)
	{
		for (int j = 0; j < CutWidth; j++)
		{
			buf1[(i*CutWidth+j)]  =buf[(leftupx+j)+w*(leftupy+i)];
		}
	}
}

INIEdit light_image_config[] =
{
	{IDC_LIGHT_VALUE,	    DLGTYPE_INPUT_INT,		&light_v.light_mix_value,					"scan_distance"}
};
int init_light_test(void)
{
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\%s\\light.ini", s_test::dir, s_test::recipe_name);

	ai_win::load_keys(path, "setting", HWND_WHITE, light_image_config, 100);

	return TRUE;
}
int exec_light_test(BYTE *bmp, int w, int h)
{
	int width,high,sum;
	float light_value;

	BYTE *bmp_y = new BYTE[w*h];
	BYTE *bmp_left = new BYTE[18*32];
	BYTE *bmp_right = new BYTE[18*32];
	int result =TRUE;
	cimg.bmp24_to_y8(bmp,w,h,NULL,bmp_y);

	image_cut(bmp_y,w,h,481,(h-609),18,32,bmp_left);
	image_cut(bmp_y,w,h,737,(h-609),18,32,bmp_right);
	
	for ( high= 0;  high<= 18; high++)
		for(width = 0; width <= 32;width ++)
		{
			sum = bmp_left[high*18+width] + bmp_right[high*18+width];
			Sleep(1);
		}
	if (light_value = (sum/(18*32)) <= light_v.light_mix_value)	
		result = FALSE;
	
	return result;
}
class LightDlg:public ai_win::Dlg
{
public:
	virtual BOOL init(HWND hdlg, HWND hfocus, LPARAM linit);
	virtual void command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify);
	virtual void close(HWND hdlg);

private:
	char path[MAX_PATH];
	int is_initial_finished;

};
BOOL LightDlg::init(HWND hdlg, HWND hfocus, LPARAM linit)
{
	is_initial_finished = FALSE;

	sprintf(path, "%s\\recipe\\%s\\white.ini", s_test::dir, s_test::recipe_name);
	ai_win::load_keys(path, "setting", hdlg, light_image_config, sizeof(light_image_config)/sizeof(light_image_config[0]));

	is_initial_finished = TRUE;

	return TRUE;
}

void LightDlg::command(HWND hdlg, int id, HWND hctrl, unsigned int code_notify)
{
	switch (id){
	case IDC_SAVE:
		ai_win::save_keys(path, "setting", HWND_LIGHT, light_image_config, 100);
		ai_win::load_keys(path, "setting", HWND_LIGHT, light_image_config, 100);
		
		break;
	}

	switch (code_notify){
	case EN_CHANGE:
	case BN_CLICKED:
		if (is_initial_finished){
			ai_win::save_keys(path, "setting", hdlg, light_image_config, sizeof(light_image_config)/sizeof(INIEdit));
		}
		break;
	}
}

void LightDlg::close(HWND hdlg)
{
	DestroyWindow(hdlg);
	HWND_LIGHT = NULL;
}

LightDlg light;
BOOL CALLBACK light_test_dlg_process(HWND hdlg, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_DLGMSG(hdlg, WM_INITDIALOG,	light.init	);
		HANDLE_DLGMSG(hdlg, WM_COMMAND,		light.command);
		HANDLE_DLGMSG(hdlg, WM_CLOSE,		light.close  );
	}
	return FALSE;
}