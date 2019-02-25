#ifndef _IMPORT_DLL_H_
#define _IMPORT_DLL_H_
#include <s_img.h>

#define EDGE_CENTER_WIDTH			32 
#define FFT_NUM						1024	//2^FFT_COFF
#define LSF_EFFECTIVE_DATA_OFFSET	(((FFT_NUM)-(EDGE_CENTER_WIDTH)*4)/2)
#define FFT_DISPLAY_NUM				(FFT_NUM/4)

// typedef struct _tagBlock
// {
// 	int is_white;
// 	int width;
// 	int height;
// 	int center_x;
// 	int qualified;
// 	int center_y;
// 	float sfr_val[4];
// 	float esf[EDGE_CENTER_WIDTH*4];
// 	float lsf[FFT_NUM];
// 	float freq[FFT_DISPLAY_NUM];
// 	float amp[FFT_DISPLAY_NUM];
// }Block;


typedef void (*TLOG)(const char *, ...);
// typedef void (*TSET_I2C)(int, int);
// typedef int  (*TWRITE_I2C)(WORD, WORD);
// typedef WORD (*TREAD_I2C)(WORD);


extern "C"{
	typedef HWND   (*fun0)(HWND, BYTE *, BYTE *, int *, char (*)[MAX_PATH], TLOG);
	typedef int	   (*fun1)(HMENU, int);
	typedef void   (*fun2)(char *);
	typedef void   (*fun3)(HDC);
	typedef void   (*fun4)();
	typedef HWND   (*fun5)(HWND, int);
	typedef int	   (*fun6)(BYTE *, BYTE *, int *, char (*str)[MAX_PATH], TLOG);
	typedef int    (*fun7)(HWND, BYTE *, BYTE *, int, int);
	typedef int	   (*fun8)(char *);
	typedef void   (*fun9)(HBITMAP);
	typedef int	   (*fun10)(HWND, int *, char (*str)[MAX_PATH], TLOG);
	typedef int    (*fun11)();
	typedef int	   (*fun12)(BYTE *);
	typedef void   (*fun13)(int ,int);
	typedef int	   (*fun14)(WORD, WORD);
	typedef WORD   (*fun15)(WORD);
	typedef int	   (*fun16)(int, int);
	typedef int	   (*fun17)(ImgProp *);
};


class ImportHW
{
public:
	ImportHW();
	~ImportHW();
	int  load_module(const char *module_name);
	int  add_menu(HMENU menu, int menu_id);
	HWND create_dlg(HWND parent, int index); 
	void destroy_dlg(void);
	int  get_hw_str(char *str);
	void get_dll_version(char *ver);			
	int  init(HWND, int *para, char (*str)[MAX_PATH], TLOG mylog);

	int  init_device(void);	
	int  close_device(void);	
	int  start_stream(void);	
	int  stop_stream(void);	
	int  get_image(BYTE *output);
	int  get_image_info(ImgProp *ipp);
	void set_i2c(int slave_id, int mode); 
	int  write_i2c(WORD addr, WORD data); 
	WORD read_i2c(WORD addr);			
	int  set_gpio(int num, int status); 

protected:
	HMODULE hw_module;
	fun1  pa_add_menu;
	fun5  pa_create_dlg;
	fun4  pa_destroy_dlg;
	fun8  pa_get_hw_str;
	fun2  pa_get_dll_version;
	fun10 pa_init;

	fun11 pa_init_device;
	fun11 pa_close_device;
	fun11 pa_start_stream;
	fun11 pa_stop_stream;
	fun12 pa_get_image;
	fun13 pa_set_i2c;
	fun14 pa_write_i2c;
	fun15 pa_read_i2c;
	fun16 pa_set_gpio;
	fun17 pa_get_image_info;
};

class ImportTool
{
public:
	ImportTool();
	~ImportTool();
	int  load_module(const char *module_name);
	HWND create_dlg(HWND parent, BYTE *img_buf, BYTE *bmp_buf, int *para, char (*str)[MAX_PATH], TLOG mylog);
	void destroy_dlg(void);
	int  add_menu(HMENU menu, int menu_id);
	void get_dll_version(char *ver);
	void draw_on_global_img(HDC memdc);

protected:
	HMODULE tool_module;
	fun0 pa_create_dlg;
	fun1 pa_add_menu;
	fun2 pa_get_dll_version;
	fun3 pa_draw_on_global_img;
	fun4 pa_destroy_dlg;
};


class ImportTest
{
public:
	ImportTest();
	~ImportTest();
	int  load_module(const char *module_name);
	HWND create_dlg(HWND parent, int index);
	void destroy_dlg(void);
	int  add_menu(HMENU menu, int menu_id);
	void get_dll_version(char *ver);
	void draw_on_global_img(HDC memdc);
	int  init_test(BYTE *img_buf, BYTE *bmp_buf, int *para, char (*str)[MAX_PATH], TLOG mylog);
	int  exec_test(HWND parent, BYTE *img, BYTE *bmp, int w, int h);
	int  get_test_str(char *str);
	void save_test_report(HBITMAP hbit);

protected:
	HMODULE test_module;
	fun5  pa_create_dlg;
	fun4  pa_destroy_dlg;
	fun1  pa_add_menu;
	fun2  pa_get_dll_version;
	fun3  pa_draw_on_global_img;
	fun6  pa_init_test;
	fun7  pa_exec_test;
	fun8  pa_get_test_str;
	fun9  pa_save_test_report;
};








#endif