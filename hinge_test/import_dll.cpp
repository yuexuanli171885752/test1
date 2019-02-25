#define _CRT_SECURE_NO_WARNINGS
#include "interface.h"
extern int item_id[];
extern int order_id[];
extern int loop_id[];
extern int check_id[];
extern int result_id[];
extern int time_id[];
static HWND hnew = NULL;

ImportTool::ImportTool()
{
	tool_module = NULL;
}

ImportTool::~ImportTool()
{
	if (tool_module != NULL){
		FreeLibrary(tool_module);
	}
}

int ImportTool::load_module(const char *module_name)
{
	char module_path[MAX_PATH] = {0};
	pa_add_menu			  = NULL;
	pa_create_dlg		  = NULL;
	pa_draw_on_global_img = NULL;
	pa_get_dll_version	  = NULL;
	pa_destroy_dlg		  = NULL;

	sprintf(module_path, ".\\Tools\\%s", module_name);
	tool_module = LoadLibrary(module_path);
	if (tool_module != NULL){
		pa_add_menu			  = (fun1)GetProcAddress(tool_module, "add_menu");
		pa_create_dlg		  = (fun0)GetProcAddress(tool_module, "create_dlg");
		pa_draw_on_global_img = (fun3)GetProcAddress(tool_module, "draw_on_global_img");
		pa_get_dll_version    = (fun2)GetProcAddress(tool_module, "get_dll_version");
		pa_destroy_dlg		  = (fun4)GetProcAddress(tool_module, "destroy_dlg");
		return TRUE;
	}

	return FALSE;
}

int ImportTool::add_menu(HMENU menu, int menu_id)
{
	if (tool_module!=NULL && pa_add_menu != NULL){
		return pa_add_menu(menu, menu_id);
	}
	return FALSE;
}

HWND ImportTool::create_dlg(HWND parent,BYTE *img_buf, BYTE *bmp_buf, int *para, char (*str)[MAX_PATH], TLOG mylog)
{
	if (tool_module != NULL && pa_create_dlg != NULL){
		return pa_create_dlg(parent, img_buf, bmp_buf, para, str, mylog);
	}
	return NULL;
}

void ImportTool::get_dll_version(char *ver)
{
	if (tool_module != NULL && pa_get_dll_version != NULL){
		pa_get_dll_version(ver);
	}
}

void ImportTool::draw_on_global_img(HDC memdc)
{
	if (tool_module != NULL && pa_draw_on_global_img != NULL){
		pa_draw_on_global_img(memdc);
	}
}

void ImportTool::destroy_dlg()
{
	if (tool_module != NULL && pa_destroy_dlg != NULL){
		pa_destroy_dlg();
	}
}
//////////////////////////////////////////////////////////////////////////
ImportHW::ImportHW()
{
	hw_module = NULL;
}

ImportHW::~ImportHW()
{
	if (hw_module != NULL){
		FreeLibrary(hw_module);
	}
}

int ImportHW::load_module(const char *module_name)
{
	char module_path[MAX_PATH] = {0};
	pa_add_menu			= NULL;
	pa_create_dlg		= NULL;
	pa_destroy_dlg		= NULL;
	pa_get_hw_str		= NULL;
	pa_get_dll_version	= NULL;
	pa_init				= NULL;

	pa_init_device		= NULL;
	pa_close_device		= NULL;
	pa_start_stream		= NULL;
	pa_stop_stream		= NULL;
	pa_get_image		= NULL;
	pa_set_i2c			= NULL;
	pa_write_i2c		= NULL;
	pa_read_i2c			= NULL;
	pa_set_gpio			= NULL;
	pa_get_image_info	= NULL;

	sprintf(module_path, ".\\hw\\%s", module_name);
	hw_module = LoadLibrary(module_path);
	if (hw_module != NULL){
		pa_add_menu			= (fun1 )GetProcAddress(hw_module, "add_menu");
		pa_create_dlg		= (fun5 )GetProcAddress(hw_module, "create_dlg");
		pa_destroy_dlg		= (fun4 )GetProcAddress(hw_module, "destroy_dlg");
		pa_get_hw_str		= (fun8 )GetProcAddress(hw_module, "get_hw_str");
		pa_get_dll_version	= (fun2 )GetProcAddress(hw_module, "get_dll_version");
		pa_init				= (fun10)GetProcAddress(hw_module, "init");

		pa_init_device		= (fun11)GetProcAddress(hw_module, "init_device");
		pa_close_device		= (fun11)GetProcAddress(hw_module, "close_device");
		pa_start_stream		= (fun11)GetProcAddress(hw_module, "start_stream");
		pa_stop_stream		= (fun11)GetProcAddress(hw_module, "stop_stream");
		pa_get_image		= (fun12)GetProcAddress(hw_module, "get_image");
		pa_set_i2c			= (fun13)GetProcAddress(hw_module, "set_i2c");
		pa_write_i2c		= (fun14)GetProcAddress(hw_module, "write_i2c");
		pa_read_i2c			= (fun15)GetProcAddress(hw_module, "read_i2c");
		pa_set_gpio			= (fun16)GetProcAddress(hw_module, "set_gpio");
		pa_get_image_info	= (fun17)GetProcAddress(hw_module, "get_image_info");
		return TRUE;
	}
	return FALSE;
}

int  ImportHW::add_menu(HMENU menu, int menu_id)
{
	if (hw_module != NULL && pa_add_menu != NULL){
		return pa_add_menu(menu, menu_id);
	}
	return FALSE;
}

HWND ImportHW::create_dlg(HWND parent, int index)
{
	if (hw_module != NULL && pa_create_dlg != NULL){
		return pa_create_dlg(parent, index);
	}

	return NULL;
}
void ImportHW::destroy_dlg(void)
{
	if (hw_module !=NULL && pa_destroy_dlg != NULL){
		pa_destroy_dlg();
	}
}

int ImportHW::get_hw_str(char *str)
{
	if (hw_module != NULL && pa_get_hw_str != NULL){
		return pa_get_hw_str(str);
	}

	return FALSE;
}

void ImportHW::get_dll_version(char *ver)
{
	if (hw_module !=NULL && pa_get_dll_version != NULL){
		pa_get_dll_version(ver);
	}
}

int  ImportHW::init(HWND hparent, int *para, char (*str)[MAX_PATH], TLOG mylog)
{
	if (hw_module !=NULL && pa_init != NULL){
		return pa_init(hparent, para, str, mylog);
	}

	return FALSE;
}

int  ImportHW::init_device(void)
{
	if (hw_module !=NULL && pa_init_device != NULL){
		return pa_init_device();
	}

	return FALSE;
}
int  ImportHW::close_device(void)
{
	if (hw_module !=NULL && pa_close_device != NULL){
		return pa_close_device();
	}
	return FALSE;
}
int  ImportHW::start_stream(void)
{
	if (hw_module !=NULL && pa_start_stream != NULL){
		return pa_start_stream();
	}
	return FALSE;
}
int  ImportHW::stop_stream(void)
{
	if (hw_module !=NULL && pa_stop_stream != NULL){
		return pa_stop_stream();
	}

	return FALSE;
} 

int  ImportHW::get_image(BYTE *output)
{
	if (hw_module !=NULL && pa_get_image != NULL){
		return pa_get_image(output);
	}
	return FALSE;
}

void ImportHW::set_i2c(int slave_id, int mode)
{
	if (hw_module !=NULL && pa_set_i2c != NULL){
		set_i2c(slave_id, mode);
	}

}

int  ImportHW::write_i2c(WORD addr, WORD data)
{
	if (hw_module !=NULL && pa_write_i2c != NULL){
		return pa_write_i2c(addr, data);
	}
	return FALSE;
}

WORD ImportHW::read_i2c(WORD addr)
{
	if (hw_module !=NULL && pa_read_i2c != NULL){
		return pa_read_i2c(addr);
	}

	return FALSE;
}

int  ImportHW::set_gpio(int num, int status)
{
	if (hw_module !=NULL && pa_set_gpio != NULL){
		return pa_set_gpio(num, status);
	}

	return FALSE;
}

int ImportHW::get_image_info(ImgProp *ipp)
{
	if (hw_module != NULL && pa_get_image_info != NULL){
		return pa_get_image_info(ipp);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

ImportTest::ImportTest()
{
	test_module = NULL;
}

ImportTest::~ImportTest()
{
	if (test_module != NULL){
		FreeLibrary(test_module);
	}
}

int ImportTest::load_module(const char *module_name)
{
	char module_path[MAX_PATH] = {0};
	pa_add_menu			  = NULL;
	pa_create_dlg		  = NULL;
	pa_draw_on_global_img = NULL;
	pa_get_dll_version	  = NULL;
	pa_destroy_dlg		  = NULL;
	pa_init_test		  = NULL;
	pa_exec_test		  = NULL;
	pa_get_test_str		  = NULL;


	sprintf(module_path, ".\\test\\%s", module_name);
	test_module = LoadLibrary(module_path);
	if (test_module != NULL){
		pa_add_menu			  = (fun1)GetProcAddress(test_module, "add_menu");
		pa_create_dlg		  = (fun5)GetProcAddress(test_module, "create_dlg");
		pa_draw_on_global_img = (fun3)GetProcAddress(test_module, "draw_on_global_img");
		pa_get_dll_version    = (fun2)GetProcAddress(test_module, "get_dll_version");
		pa_destroy_dlg		  = (fun4)GetProcAddress(test_module, "destroy_dlg");
		pa_init_test		  = (fun6)GetProcAddress(test_module, "init_test");
		pa_exec_test		  = (fun7)GetProcAddress(test_module, "exec_test");
		pa_get_test_str		  = (fun8)GetProcAddress(test_module, "get_test_str");
		pa_save_test_report	  = (fun9)GetProcAddress(test_module, "save_test_report");

		return TRUE;
	}

	return FALSE;
}

int ImportTest::add_menu(HMENU menu, int menu_id)
{
	if (test_module != NULL && pa_add_menu != NULL){
		return pa_add_menu(menu, menu_id);
	}

	return FALSE;
}

HWND ImportTest::create_dlg(HWND parent, int index)
{
	if (test_module != NULL && pa_create_dlg != NULL){
		return pa_create_dlg(parent, index);
	}

	return NULL;
}

void ImportTest::destroy_dlg()
{
	if (test_module != NULL && pa_destroy_dlg != NULL){
		pa_destroy_dlg();
	}
}

void ImportTest::draw_on_global_img(HDC memdc)
{
	if (test_module != NULL && pa_draw_on_global_img != NULL){
		pa_draw_on_global_img(memdc);
	}
}

void ImportTest::get_dll_version(char *ver)
{
	if (test_module != NULL && pa_get_dll_version != NULL){
		pa_get_dll_version(ver);
	}
}

int ImportTest::init_test(BYTE *img_buf, BYTE *bmp_buf, int *para, char (*str)[MAX_PATH], TLOG mylog)
{
	if (test_module != NULL && pa_init_test != NULL){
		return pa_init_test(img_buf, bmp_buf, para, str, mylog);
	}

	return FALSE;
}

int ImportTest::exec_test(HWND parent, BYTE *img, BYTE *bmp, int w, int h)
{
	if (test_module != NULL && pa_exec_test != NULL){
		return pa_exec_test(parent, img, bmp, w, h);
	}

	return FALSE;
}

int ImportTest::get_test_str(char *str)
{
	if (test_module != NULL && pa_get_test_str != NULL){
		return pa_get_test_str(str);
	}

	return FALSE;
}

void ImportTest::save_test_report(HBITMAP hbit)
{
	if (test_module != NULL && pa_save_test_report != NULL){
		pa_save_test_report(hbit);
	}
}


