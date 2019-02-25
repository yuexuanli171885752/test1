#include "interface.h"
#include "Include/LibXIStream/XIStream.h"
#include "Include/LibXIProperty/XIProperty.h"
#include "inf_motion_aa.h"
#pragma comment(lib, "LibXIStreamA2.lib")
#pragma comment(lib, "LibXIPropertyA.lib")
#define CAPTURE_COLORSPACE		XI_COLOR_YUYV // XI_COLOR_NV12
#define FRAME_DURATION_25		400000

#define CAPTURE_WIDTH			640  /*720*/
#define CAPTURE_HEIGHT			480

HANDLE hproperty = NULL;
HANDLE hvideo_device = NULL;
HANDLE hrender = NULL;

GUID   guid_monitor;
int	   frame_rate = FRAME_DURATION_25;
BOOL   use_custom = FALSE;
int		video_device_select = -1;


BYTE	RtblITU[256][256];
BYTE	GtblITU[256][256][256];
BYTE	BtblITU[256][256];



void	MakeSpaceTableITU(void)
{
	int		Y,Cr,Cb,tmp;
	for (Y=0; Y<256; Y++) {	//Y
		for (Cr=0; Cr<256; Cr++) { //Cr
			for(Cb=0; Cb<256; Cb++) { //Cb
				//B = 1.164(Y-16)+2.018(Cr-128)
				tmp = (int)(1.164*(Y-16)+2.018*(Cb-128));
				if (tmp > 255) tmp = 255;
				if (tmp < 0) tmp = 0;
				BtblITU[Y][Cb] = tmp;
				tmp= (int)(1.164*(Y-16)+1.596*(Cr-128));
				if (tmp > 255) tmp = 255;
				if (tmp < 0) tmp = 0;
				RtblITU[Y][Cr] = tmp;
			}
		}
	}
}

void YCbCrITUToBmp(int iMode, BYTE *pYuvBuffer, BYTE *pBmpBuffer, int size_x, int size_y)
{
	DWORD		 Y0,Y1,Cb,Cr; //speed up!!!
	int			 tmp;
	int			 i,j;
	LPBYTE       pImageData, pImageTemp;

	pImageData = pYuvBuffer + size_x*2*(size_y - 1);

	switch(iMode) { 		 
	case 3: //CbYCrY
		for (i=0; i<size_y; i++) {
			pImageTemp = pImageData;
			for (j=0; j<size_x; j+=2) {
				Y0 = pImageTemp[1];
				Cr = pImageTemp[2];
				Y1 = pImageTemp[3];
				Cb = pImageTemp[0];
				*pBmpBuffer++ = BtblITU[Y0][Cb];    //B
				tmp = (int)(1192*(Y0-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				//G
				*pBmpBuffer++ = RtblITU[Y0][Cr];	//R

				*pBmpBuffer++ = BtblITU[Y1][Cb];	 //B
				tmp = (int)(1192*(Y1-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y1][Cr];	 //R
				pImageTemp+=4;  //next pair.
			}
			pImageData -= size_x*2;
		}
		break;
	case 2: //YCrYCb
		for (i=0; i<size_y; i++) {
			pImageTemp = pImageData;
			for (j=0; j<size_x; j+=2) {
				Y0 = pImageTemp[0];
				Cr = pImageTemp[1];
				Y1 = pImageTemp[2];
				Cb = pImageTemp[3];
				*pBmpBuffer++ = BtblITU[Y0][Cb];		 //B
				tmp = (int)(1192*(Y0-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y0][Cr];     //R

				*pBmpBuffer++ = BtblITU[Y1][Cb];	 //B	
				tmp = (int)(1192*(Y1-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y1][Cr];     //R
				pImageTemp+=4;	//next  pair.
			}
			pImageData -= size_x*2;
		}
		break;
	case 1: //YCbYCr
		for (i=0; i<size_y; i++) {
			pImageTemp = pImageData;
			for (j=0; j<size_x; j+=2) {
				Y0 = pImageTemp[0];
				Cr = pImageTemp[3];
				Y1 = pImageTemp[2];
				Cb = pImageTemp[1];
				*pBmpBuffer++ = BtblITU[Y0][Cb];		 //B
				tmp = (int)(1192*(Y0-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y0][Cr];	 //R

				*pBmpBuffer++ = BtblITU[Y1][Cb];	 //B
				tmp = (int)(1192*(Y1-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y1][Cr];	 //R
				pImageTemp+=4;  //next YCbYCr pair.
			}
			pImageData -= size_x*2;
		}
		break;
	case 4: //CrYCbY
		for (i=0; i<size_y; i++) {
			pImageTemp = pImageData;
			for (j=0; j<size_x; j+=2) {
				Y0 = pImageTemp[1];
				Cr = pImageTemp[0];
				Y1 = pImageTemp[3];
				Cb = pImageTemp[2];
				*pBmpBuffer++ = BtblITU[Y0][Cb];     //B
				tmp = (int)(1192*(Y0-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y0][Cr];	 //R

				*pBmpBuffer++ = BtblITU[Y1][Cb];   //B
				tmp = (int)(1192*(Y1-16) - 832*(Cr-128) - 400*(Cb-128)) >>10;
				if (tmp > 255)  tmp = 255;
				if (tmp < 0)    tmp = 0;
				*pBmpBuffer++ = tmp;				 //G
				*pBmpBuffer++ = RtblITU[Y1][Cr];	 //R
				pImageTemp+=4;  //next  pair
			}
			pImageData -= size_x*2;
		}
		break;
	}
}


void VideoCaptureCallbackInternal(const BYTE * pbyImage, int cbImageStride, UINT64 u64TimeStamp)
{
	XIS_VideoRendererDrawImage(hrender, pbyImage, cbImageStride, TRUE);
	memcpy(s_test::img_buf, pbyImage, IMG_WIDTH*IMG_HEIGHT*2);
//	log("get img");

//	YCbCrITUToBmp(1, s_test::img_buf, s_test::bmp_buf_t, IMG_WIDTH, IMG_HEIGHT);	
//	log("w:%d, h:%d", aif::para.image_width, aif::para.image_height);
}

void VideoCaptureCallback(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {

	VideoCaptureCallbackInternal(pbyImage, cbImageStride, u64TimeStamp);
}

BOOL resize_window(BOOL use_default)
{

	if (use_default && hproperty != NULL){
		XI_DEVICE_TYPE devType = XIP_GetDeviceType(hproperty);
		int nLeft, nTop, nWidth = 0, nHeight = 0;

		if (XI_DEVICE_HDVIDEO_CAPTURE == devType || PRO_CAPTURE == devType) {
			XIPHD_GetDefClip(hproperty, &nLeft, &nTop, &nWidth, &nHeight);
		}
		else {
			XIPCVBS_GetClip(hproperty, &nLeft, &nTop, &nWidth, &nHeight);
		}

		log("left=%d, top=%d, width=%d, height=%d", nLeft, nTop, nWidth, nHeight);

// 		if (nWidth == 0 || nHeight == 0){
// 			return FALSE;
// 		}

		nWidth = max(CAPTURE_WIDTH, min(2048, nWidth));
		nHeight = max(CAPTURE_HEIGHT, min(1536, nHeight));

		IMG_WIDTH  = nWidth;
		IMG_HEIGHT = nHeight;
		log("imgW = %d, imgH = %d",IMG_WIDTH,IMG_HEIGHT);
		use_custom = FALSE;
	}


	XIS_StopVideoCapture(hvideo_device);

	if (!XIS_SetVideoCaptureFormat(hvideo_device, CAPTURE_COLORSPACE, IMG_WIDTH, IMG_HEIGHT, frame_rate)){
		log("set video format fail.");
		return FALSE;
	}
	else{
		XIS_SetVideoCaptureCallbackEx(hvideo_device, VideoCaptureCallback, HWND_APP, FALSE);
		//	XIPCVBS_SetClip(hproperty, 0, 0, cx, cy);
	}

	if (!XIS_StartVideoCapture(hvideo_device)){
		log("start video capture fail.");
		return FALSE;
	}

	return TRUE;
}

BOOL open_device_306e(int id)
{
	if (video_device_select == id){
		return TRUE;
	}

	log("open cap device=%d", id);
	if (video_device_select != -1){
		if (hproperty){
			XIP_ClosePropertyHandle(hproperty);
			hproperty = NULL;
		}

		if (hvideo_device){
			XIS_StopVideoCapture(hvideo_device);
			XIS_CloseVideoCapture(hvideo_device);
			hvideo_device = NULL;
		}
		if (hrender){
			XIS_DestroyVideoRenderer(hrender);
			hrender = NULL;
		}		
	}

	VIDEO_CAPTURE_INFO vid_cap_info;
	if (!XIS_GetVideoCaptureInfo(id, &vid_cap_info)){
		log("get video capture info fail.");
		return FALSE;
	}
	else{
		log("id=%s", vid_cap_info.szDShowID);
		log("name=%s", vid_cap_info.szName);

	}

	hvideo_device = XIS_OpenVideoCapture(vid_cap_info.szDShowID);
	if (NULL == hvideo_device){
		log("open video capture fail.");
		return FALSE;
	}
	else{
		log("open video capture ok.");
	}

	XIS_SetVideoCaptureBuffering(hvideo_device, FALSE);
	hproperty = XIS_OpenVideoCapturePropertyHandle(hvideo_device);
	if (NULL != hproperty){
		char serail_no[MAX_SERIAL_NO] = {0};
		XIP_GetSerialNo(hproperty, serail_no);
		log("serial:%s", serail_no);
	}
	else{
		log("get video property fail.");
	}

	if (!XIS_GetMonitorGUIDFromWindow(HWND_MAIN, &guid_monitor)){
		guid_monitor = GUID_NULL;
	//	log("get monitor guid fail.");
	}
	log("init w:%d, h:%d", IMG_WIDTH, IMG_HEIGHT);// input config
	hrender = XIS_CreateVideoRenderer(&guid_monitor, HWND_APP, CAPTURE_COLORSPACE, IMG_WIDTH, 
		IMG_HEIGHT, TRUE, TRUE);
	if (hrender == NULL){
		log("create video render fail.");
		return FALSE;
	}

	video_device_select = id;

	if (!resize_window(TRUE)){
		log("resize window fail.");
		return FALSE;
	}
	//log("open window ok");
	return TRUE;
}

int CVBS_TSW_306E::init(int id)
{
	XIS_Initialize();
	int dev_cnt = XIS_GetVideoCaptureCount();
	if (dev_cnt<=0){
		return FALSE;
	}

	dev_id = id;
	log("306e init down.id=%d",id);
	return TRUE;
}

int CVBS_TSW_306E::run()
{
//	log("306e run.");
	return open_device_306e(dev_id);
}

int CVBS_TSW_306E::cap(BYTE *buf, long buf_size)
{
//	log("T306E capture.");
//	memcpy(buf, s_test::img_buf, IMG_WIDTH*IMG_HEIGHT*3);
	return TRUE;
}

int CVBS_TSW_306E::stop()
{
	if (hproperty) {
		XIP_ClosePropertyHandle(hproperty);
		hproperty = NULL;
	}

	if (hvideo_device) {
		XIS_StopVideoCapture(hvideo_device);
		XIS_CloseVideoCapture(hvideo_device);
		hvideo_device = NULL;
	}

	if (hrender) {
		XIS_DestroyVideoRenderer(hrender);
		hrender = NULL;
	}
	video_device_select = -1;

	return TRUE;
}

int CVBS_TSW_306E::destroy()
{
	XIS_Uninitialize();

	return TRUE;
}
