#ifndef _INF_CAP_BD_H_
#define _INF_CAP_BD_H_
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <direct.h>
#include <assert.h>
#include <CommCtrl.h>
#include "s_win.h"
#include "resource.h"

#define CAP_BD_CNT		2
#define REG_LIST_SIZE					10240

extern HWND hsubcap[CAP_BD_CNT];

extern int import_reg_cmd(HWND hdlg, const char *ini_path, const char *section);
extern BOOL set_sensor_power_on(void);
extern BOOL set_sensor_power_off(void);
extern BOOL set_sensor_mclk_on(void);
extern BOOL set_sensor_mclk_off(void);
extern BOOL set_sensor_io_on(void);
extern BOOL set_sensor_io_off(void);
extern BOOL run_register(const char *ini_path, char *section);
extern int  parse_cmd(char *line_buf);

extern BOOL decode_input_image_data(void);
extern int  init_sensor(char *path);
extern int get_image_pixel_cnt(void);

extern void log(const char *format, ...);

#endif