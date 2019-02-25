#define _CRT_SECURE_NO_WARNINGS
#include "windows.h"
#include "stdio.h"
#include "avb_dec.h"
#include "img_cap_device.h"
#include "interface.h"

#pragma  comment(lib,"libavb_dec.lib")


//extern void on_raw_data_cb(char *data, int len, int width, int lenght, int err);

void on_raw_data_cb(char *data, int len, int width, int lenght, int err)
{
	// 处理视频数据
	if (err == 0){
		memcpy(s_test::img_buf, data, len);
	}

}

int Hinge::init(int id){
	return TRUE;
}

int Hinge::run(){
	int err;
	err = set_raw_data_callback(on_raw_data_cb);
	
	if(RESULT_SUCCESS == err){
		err = hinge_start();
		if(RESULT_SUCCESS == err)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

int Hinge::cap(BYTE *buf, int buf_size){

	return TRUE;
}


int Hinge::stop(){
	hinge_stop();
	return TRUE;
}

int Hinge::destroy()
{
	return TRUE;

}

// void on_raw_data_cb(char *data, int len, int width, int lenght, int err)
// {
// 	// 处理视频数据
// 	if (err = 0){
// 		memcpy(s_test::bmp_buf_t, data, IMG_WIDTH*IMG_HEIGHT*3);
// 	}
// 
// 		
// }
