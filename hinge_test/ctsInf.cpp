#define _CRT_SECURE_NO_WARNINGS
#include "inf_cap_bd.h"
#include "img_cap_device.h"
#include "cts.h"
#include "interface.h"

extern BOOL WaitForFirstFrameArrived(DWORD devInst);

// CTS::CTS()
// {
// 	BOOL bRes;
// 	bRes = Cam_Init();
// }

int CTS::init(int id){
	//log("recv CTS init.");
	DEVINFO	stDevInfo[DEVNUM];
	DWORD nDevCount = 0;
	char dir[MAX_PATH] = {0};
	ai_win::get_current_directory(dir);

	char sensor_name[100] = {0};
	char path[MAX_PATH] = {0};
	sprintf(path, "%s\\recipe\\config.ini", dir);
	ai_win::read_string_key(path, "system", "default_recipe_setting", sensor_name, sizeof(sensor_name));
	sprintf(sensor_path, "%s\\recipe\\%s\\%s.ini", dir, sensor_name, sensor_name);
	BOOL bRes = 1;
	//bRes = Cam_Init();

	log("cam init result: %d,path:%s",bRes,sensor_path);
	bRes = Cam_EnumAllDevInfo(stDevInfo, nDevCount, 0);
	id = stDevInfo[0].nDevID;
	log("id=%d,devCount=%d",id,nDevCount);
	dev_id = id;
	TESTER_ID = id;

	//BOOL bRes;

	bRes = Cam_SetConfigEx(dev_id, sensor_path); // 0: success

	int width = Cam_GetSensorOutputWidth(dev_id); 
	int height = Cam_GetSensorOutputHeight(dev_id);
	IMG_WIDTH = width;
	IMG_HEIGHT = 500;
	log("img w=%d, img h=%d",IMG_WIDTH,IMG_HEIGHT);


	BYTE dataformat = 1; // 0:RAW8,1: YUV422, 2:MJPGorRGB24, 3:MIPIRAW10,4:DVPRAW10,5:MIPIRAW12			
	BYTE displayformat = 1; //0: RAW8, 1:RGB24
	BYTE BayerOrder = 1; // 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR
	
	dataformat = Cam_GetSensorDataFormat(dev_id);
	BayerOrder = Cam_GetSensorPixelOrder(dev_id);
	//HWND hwndPreview = ((CStatic*)GetDlgItem(HWND_MAIN,IDC_DISPLAY))->GetSafeHwnd();
	//bRes = FALSE;
	if(bRes == 0){
		bRes = Cam_BuildPreviewGraph(dev_id, NULL/*HWND_MAIN*/, width, height, dataformat, displayformat, BayerOrder);
	}
	log("dataformat=%d,order=%d",dataformat,BayerOrder);
	log("dev id:%d, run result: %d",dev_id,bRes);
 	//bRes = Cam_SetConfigEx(id, sensor_path);
// 	log("set config result: %d",bRes);
// 	IMG_WIDTH  = Cam_GetSensorOutputWidth(id); 
// 	IMG_HEIGHT = Cam_GetSensorOutputHeight(id);

// 	BYTE dataformat = 1; // 0:RAW8,1: YUV422, 2:MJPGorRGB24, 3:MIPIRAW10,4:DVPRAW10,5:MIPIRAW12			
// 	BYTE displayformat = 1; //0: RAW8, 1:RGB24
// 	BYTE BayerOrder = 0; // 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR
// 	float fps;
// //	HWND hwndPreview = ((CStatic*)GetDlgItem(HWND_MAIN,IDC_DISPLAY))->GetSafeHwnd();
// 	if(bRes == TRUE){
// 		Cam_BuildPreviewGraph(id, HWND_MAIN, IMG_WIDTH, IMG_HEIGHT, dataformat, displayformat, BayerOrder);
// 	}
// 	if(WaitForFirstFrameArrived){
// 		bRes = Cam_MediaControlEx(id, 0); //run
// 		fps = Cam_GetVideoRealTimeFPS(id);
// 		long buffersize = 0;
// 		bRes &= Cam_GrabFrame(id, s_test::img_buf, &buffersize);
// 		return TRUE;
// 	}
	
		
	return TRUE;
}

int CTS::run()
{	
	BOOL bRes = TRUE;
//	float fps;
//	if(WaitForFirstFrameArrived(dev_id)){
		bRes = Cam_MediaControlEx(dev_id, 0); //run
//		fps = Cam_GetVideoRealTimeFPS(dev_id);
// 		Cam_SetGPIOOutputValue(dev_id,3,1);
// 		bRes = Cam_GPIOPull(dev_id,3,0);//no pull
		log("result0: %d",bRes);
// 		bRes = Cam_GPIODir(dev_id,3,1);//output model
// 		log("result1: %d",bRes);
//  		Sleep(500);
// 		bRes = Cam_SetGPIOOutputValue(dev_id,3,0);
// 		log("result2: %d",bRes);
// 		Sleep(500);
// 		bRes = Cam_SetGPIOOutputValue(dev_id,3,1);
// 		log("dev id:%d, run result3: %d",dev_id,bRes);
		return bRes;
//	}
// 	log("run fail.");
// 	return FALSE;
}

int CTS::cap(BYTE *buf, long buf_size)
{
	//log("cap dev.");
	BOOL bRes;
// 	float fps;
// 	char date_path[MAX_PATH] = {0};
// 	SYSTEMTIME st;
// 	GetLocalTime(&st);
//  	if(WaitForFirstFrameArrived(dev_id)){
//  		bRes = Cam_MediaControlEx(dev_id, 0); //run
// 		fps = Cam_GetVideoRealTimeFPS(dev_id);
//		log("dev id:%d",dev_id);
		int width = Cam_GetSensorOutputWidth(dev_id); 
		int height = Cam_GetSensorOutputHeight(dev_id);
		buf_size = width*height*3;
		bRes &= Cam_GrabFrameEx(dev_id, buf, &buf_size, 0);

		Sleep(20);
//		log("data type :%d",datatype);
 		return bRes;
// 	}
// 	return FALSE;
// 	bRes = Cam_MediaControlEx(dev_id, 0); //run loop
// 	fps = Cam_GetVideoRealTimeFPS(dev_id);
// 	log("dev id:%d, run result: %d",dev_id,bRes);
// 	buf_size = IMG_WIDTH*IMG_HEIGHT*3;
// 	bRes &= Cam_GrabFrameEx(dev_id, buf, &buf_size, datatype);
// 	log("data type :%d",datatype);
// 	return bRes;
}

int CTS::stop()
{
	BOOL bRes = Cam_MediaControlEx(dev_id, 1);
	return TRUE;
}

int CTS::destroy()
{
	return TRUE;

}

BOOL WaitForFirstFrameArrived(DWORD devInst)
{
	int count = 500;
	while(!Cam_IsFirstFrameArrived(devInst) && count > 0)
	{
		count--;
		Sleep(10);
	}
	if(count > 0)
		return TRUE;
	else		
		return FALSE;
}