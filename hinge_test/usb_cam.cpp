#include "interface.h"



int USBCam::init(int id)
{
	dev_id = id;
	return TRUE;
}

int USBCam::run()
{
	int result = TRUE;
	int cam_cnt = CCameraDS::CameraCount();
	if (cam_cnt<=0){
		log("There is no camera connected.");
		result = FALSE;
	}
	else{
		result = TRUE;
		log("Find %d camera connected.", cam_cnt);

		char cam_name[1024] = {0};
		int rt_val = s_aa::cam_ds.CameraName(dev_id, cam_name, sizeof(cam_name));
		if (rt_val>0){
			log("camera name is %s", cam_name);
		}
		else{
			log("can not get camera name.");
			result = FALSE;
		}
	}

	result = s_aa::cam_ds.OpenCamera(dev_id, FALSE, WEB_CAM_WIDTH, WEB_CAM_HEIGHT);
	log("Open Cam:%d, web w:%d, h:%d", result, WEB_CAM_WIDTH, WEB_CAM_HEIGHT);

	if (result == TRUE){
		s_aa::cam_ds.QueryFrame();
	}

	return result;
}

int USBCam::cap(BYTE *buf, long buf_size)
{
	int result = TRUE;
	//	if (ait::cam_ds.OpenCamera(WEB_CAM_NUM, FALSE, ait::sensor.img_w, ait::sensor.img_h) == TRUE){
	//	ait::cam_ds.QueryFrame();


	if((s_aa::frame = s_aa::cam_ds.QueryFrame()) != NULL){
		for (int i=0; i<WEB_CAM_HEIGHT; i++){
			memcpy(buf+i*WEB_CAM_WIDTH*3, (BYTE *)(s_aa::frame->imageData+(WEB_CAM_HEIGHT-1-i)*WEB_CAM_WIDTH*3), WEB_CAM_WIDTH*3);
		}
		result = TRUE;
	//	log("capture one image.");
	}
	else{
		result = FALSE;
		log("capture image fail.");

	}
	//	}
	//	else{
	//		tlog("capture image fail.");
	//		result = FALSE;
	//		ait::update_status(ST_IMG_BD_NO_IMG_FAIL);
	//	}
//	Sleep(10000);

	return result;
}

int USBCam::stop()
{
	s_aa::cam_ds.CloseCamera();
	log("close camera.");
	return TRUE;
}

int USBCam::destroy()
{

	return TRUE;
}