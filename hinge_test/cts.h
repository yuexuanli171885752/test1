#ifndef _CTS_H_
#define _CTS_H_

#include<atlstr.h>
#include<vector>
using namespace std;

#define FREEBUF(x) {if(x!=NULL) {delete [] x; x = NULL;}}
#define FREEOBJ(p) {if(p!=NULL) {delete    p; p = NULL;}}	

//#define CLIP(min, max, value) (value < min ? min : (value > max ? max : value))

#pragma comment (lib,"ctscam.lib")

#define CTS_API extern "C"  __declspec(dllimport)

#define DEVNUM	100
#define MULTIRESOLUTIONMAX	20
#define PWRNUM 5

#define TRIGGERGRABFRAMENUMMAX	100	



#define	USBALL		"ALL"
#define USBNO 		"NO USB"            
#define USB20 		"USB2.0"
#define USB30 		"USB3.0"
#define USBDAMAGED 	"DAMAGED"


#define DEVICEALL	"ALL"
//PM4
#define DEVPM4		"PM4" 	
#define DEVPM4C		"PM4C" 	
//PM2
#define DEVFU225	"FU225" 	
#define DEVFU225S	"FU225S" 	
#define DEVPM200	"PM200" 	
#define DEVPU300S	"PU300S" 	
#define DEVPM230C	"PM230C"
#define DEVPM230	"PM230" 	
#define DEVPM230S	"PM230S" 	
#define DEVPM230SS	"PM230SS"
			//m4
#define DEVFU360	"FU360"
#define DEVFU360S	"FU360S"
#define DEVM430C	"M430C"
#define DEVM430		"M430" 	
#define DEVM430S	"M430S" 	
#define DEVM430SS	"M430SS" 	
#define DEVNODEV	"NO DEV"


//classify
#define DEVICEPM4	"PM4"
#define DEVICEPM4C	"PM4C"
#define DEVICEPM2	"PM2"
#define DEVICEM4	"M4"


// Message ID
#define CTS_DEVICERENUM				(WM_USER+0xa100)
#define CTS_DEVICEINSERT			(WM_USER+0xa101)
#define CTS_DEVICEREMOVE			(WM_USER+0xa102)


typedef enum {
	MEDIACTRL_RUN,
	MEDIACTRL_STOP,
} tagMediaCtrl;

typedef enum{
	DATAFORMAT_RAW = 0,
	DATAFORMAT_YUV422,
	DATAFORMAT_MJPGORRGB24,
	DATAFORMAT_MIPIRAW10,		
	DATAFORMAT_DVPRAW10,
	DATAFORMAT_MIPIRAW12,
}tagDataFormat;

typedef enum{ //// 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR
	BAYERORDER_RGGB = 0,
	BAYERORDER_GRBG,
	BAYERORDER_GBRG,
	BAYERORDER_BGGR,		
}tagBayerOrder;


typedef enum{
	PLAYFORMAT_RAW = 0,
	PLAYFORMAT_RGB24,
}tagDisplayFormat;


typedef struct
{
	int addr;
	int data;
}tagREG;

#define REGMAX 10
typedef struct 
{
	int InitGain;
	int MaxGain;
	int RReg[REGMAX];
	int GReg[REGMAX];
	int BReg[REGMAX];

	int GbReg[REGMAX];
	int GrReg[REGMAX];
}tagAWBREG;

typedef struct _DEVINFOWORKMODE
{
	BYTE bCamSpecialMode; // 0: disable, 1: enable dual camera or array camera
	BYTE byChannel; //  indicate which channel current device belong to.
	BYTE byGroup;   // indicate which group current device belong to
	BYTE byCamIdx;  // indicate which camera index current device belong to

}DEVINFOWORKMODE;

#define MAXLEN 260
#pragma pack (4)
typedef struct _DEVINFO
{
	DWORD nDevID;
	DWORD nHWVer;

	char strDevPathName[MAXLEN];
	char strDevFriendlyName[MAXLEN];

	char strDev[MAXLEN];
	char strUsb[MAXLEN];	

}DEVINFO;
#pragma pack ()


typedef BOOL (*pfn_Cam_Callback_DeviceChange)(BYTE byDeviceChangeMode, DWORD nDevID, DWORD nHWSN);  
CTS_API BOOL Cam_SetCallBackFunction(pfn_Cam_Callback_DeviceChange callbackfunction); 



CTS_API BOOL Cam_Init();			
CTS_API VOID Cam_UnInit();			

CTS_API BYTE Cam_EnumAllDevInfo(DEVINFO stDevInfo[], DWORD &nDevCount, BYTE byCamMode);	

CTS_API BOOL Cam_EnforceDevBeDualMode(DWORD nHWSN_M, DWORD nHWSN_S, BYTE bI2CDualMode);

CTS_API BOOL Cam_GetDeviceWorkMode(DWORD devInst, DEVINFOWORKMODE &workmode);
CTS_API BOOL Cam_SetDeviceWorkMode(DWORD devInst, DEVINFOWORKMODE workmode);

CTS_API BOOL Cam_RegisterDeviceMessageHandle(UINT handle);
CTS_API BOOL Cam_IsDeviceAlive(DWORD devInst);
CTS_API BOOL Cam_IsCTS(DWORD devInst);

CTS_API BOOL Cam_ReEnum(DWORD devInst);
CTS_API DWORD Cam_SetConfigExWithBuffer(DWORD devInst, char* cBuffer, int length);
CTS_API DWORD Cam_SetConfigEx(DWORD devInst, char* strPath);
CTS_API DWORD Cam_SetConfigUnEnum(DWORD devInst, char* strPath);
CTS_API DWORD Cam_SetConfigMultiResolution(DWORD devInst, char cPath[MULTIRESOLUTIONMAX][MAXLEN], int nMultiResolutionNum, int nInitSettingIdx);

CTS_API BOOL Cam_ReadSensorI2C(DWORD devInst, DWORD dwAddr, UINT uLength, unsigned short* lpData);	
CTS_API BOOL Cam_WriteSensorI2C(DWORD devInst, DWORD dwAddr, UINT uLength, unsigned short* lpData);	
CTS_API BOOL Cam_ReadNormalI2C(DWORD devInst, BYTE bySlaveAddr, UINT uLength, unsigned char* lpData);
CTS_API BOOL Cam_WriteNormalI2C(DWORD devInst, BYTE bySlaveAddr, UINT uLength, unsigned char* lpData);
CTS_API BOOL Cam_ReadNormalI2CEx(DWORD devInst, BYTE bySlaveAddr, WORD wAddr, UINT uLength, unsigned char* lpData, BYTE byMode);
CTS_API BOOL Cam_WriteNormalI2CEx(DWORD devInst, BYTE bySlaveAddr, WORD wAddr, UINT uLength, unsigned char* lpData, BYTE byMode);
CTS_API BOOL Cam_ReadNormalI2CExWithDelay(DWORD devInst, BYTE bySlaveAddr, WORD wAddr, UINT uLength, BYTE* lpData, BYTE byMode, WORD wPageSize = 0, WORD uPageDelay = 0);
CTS_API BOOL Cam_WriteNormalI2CExWithDelay(DWORD devInst, BYTE bySlaveAddr, WORD wAddr, UINT uLength, LPBYTE lpData, BYTE byMode, WORD wPageSize = 0, WORD uPackDelay = 0, WORD uPageDelay = 0);

CTS_API BOOL Cam_GPIOPull(DWORD devInst, BYTE gpioNum, BYTE pullAttr);
CTS_API BOOL Cam_GPIODir(DWORD devInst, BYTE gpioNum, BYTE dirAttr);
CTS_API BOOL Cam_GPIOTriggerEnable(DWORD devInst, BYTE gpioNum, BYTE triggerType, BYTE bEnable);
CTS_API BOOL Cam_GPIOGetTriggerStat(DWORD devInst, BYTE gpioNum, BYTE triggerType, BYTE& byValue);
CTS_API BOOL Cam_SetGPIOOutputValue(DWORD devInst, BYTE gpioNum, BYTE outputValue);
CTS_API BOOL Cam_GetGPIOInputValue(DWORD devInst, BYTE gpioNum, BYTE& byValue);

//trig grab
CTS_API BOOL Cam_GPIOTriggerCounterEnable(DWORD devInst, BYTE gpioNum, BYTE bEnable, BYTE byTrigGrabMode);
CTS_API BOOL Cam_GPIOGetTriggerCounterStat(DWORD devInst, BYTE gpioNum, BYTE &bValid, WORD &wCounter);


CTS_API BOOL Cam_SetSensorExtClock(DWORD devInst, double dbClk);
CTS_API BOOL Cam_SetI2CSpeed(DWORD devInst, BYTE byI2CSpeed);


CTS_API BOOL Cam_IsSupportOpenShort(DWORD devInst);
CTS_API BOOL Cam_IsSupportCurrentMeasure(DWORD devInst, WORD &nChannelCount);


CTS_API BOOL Cam_GetPowerCurrentEx(DWORD devInst, BOOL bCur[PWRNUM], BYTE byValid[PWRNUM], int nValue[PWRNUM]);
CTS_API BOOL Cam_SetSensorPwdnRst(DWORD devInst, BYTE type);
CTS_API BOOL Cam_SetPowerDown(DWORD devInst, BYTE byStatus);
CTS_API BOOL Cam_SetReset(DWORD devInst, BYTE byStatus);

CTS_API BOOL Cam_SetCurrentRange(DWORD devInst, BYTE range);
CTS_API BOOL Cam_GetCurrentRange(DWORD devInst, BYTE &range);

CTS_API BOOL Cam_SetPowerVoltageEnable(DWORD devInst, BYTE index, BYTE bEnable);

CTS_API BOOL Cam_GetPowerVoltage(DWORD devInst, BOOL bCur[PWRNUM], int nValue[PWRNUM]);
CTS_API BOOL Cam_SetPowerVoltageValue(DWORD devInst, BYTE index, float fVolt);


CTS_API DWORD Cam_SetSensorMode(DWORD devInst, BYTE mode);

CTS_API BOOL Cam_GetHardwareSerialNum(DWORD devInst, DWORD &dwValue);


CTS_API BOOL Cam_WriteVCMEx(DWORD devInst, WORD value);
CTS_API BOOL Cam_ReadVCMEx(DWORD devInst, WORD& value);

CTS_API DWORD Cam_VCMI2CInit(DWORD devInst);
CTS_API DWORD Cam_Dev1I2CInit(DWORD devInst);
CTS_API DWORD Cam_Dev2I2CInit(DWORD devInst);

CTS_API BOOL Cam_GetConnectionStatus(DWORD devInst, unsigned char* usbStaus, unsigned char* devStatus);


CTS_API BOOL Cam_LedOff(DWORD devInst);
CTS_API BOOL Cam_SetMCLKOff(DWORD devInst, BYTE mode);
CTS_API BOOL Cam_SetBuzzer(DWORD devInst, BOOL bEnable);

CTS_API DWORD Cam_GetDevPath(DWORD devInst, char* pDevPath);
CTS_API DWORD Cam_GetFriendlyName(DWORD devInst, char* pDevPath);

CTS_API BOOL Cam_GetMIPICRCCheck(DWORD devInst, WORD& dwValue);
CTS_API BOOL Cam_GetMIPIDPHYECC(DWORD devInst, WORD& dwValue);
CTS_API BOOL Cam_GetUSBSIECheck(DWORD devInst, WORD& dwValue);
CTS_API BOOL Cam_FramesReceivedStatistics(DWORD devInst, BYTE &byFrames);
CTS_API BOOL Cam_ClearMIPICRC(DWORD devInst);
CTS_API BOOL Cam_ClearMIPIDPHYECC(DWORD devInst);	
CTS_API BOOL Cam_ClearUSBSIE(DWORD devInst);
CTS_API BOOL Cam_ClearFramesCounter(DWORD devInst);

CTS_API BOOL Cam_CRCECCErrorFrameDropMode(DWORD devInst, BYTE mode);


CTS_API DWORD Cam_GetSensorOutputWidth(DWORD devInst);
CTS_API DWORD Cam_GetSensorOutputHeight(DWORD devInst);

CTS_API BYTE Cam_GetSensorDataFormat(DWORD devInst);
CTS_API BYTE Cam_GetSensorPixelOrder(DWORD devInst);

//preview
CTS_API BOOL Cam_USBStreamControl(DWORD devInst, BYTE byValue);

CTS_API int Cam_PreviewVideoPlay(DWORD devInst, HWND hwnd, int width, int height, unsigned char nDataFormat, unsigned char nDisplayFormat, unsigned char nBayerOrder); // old function, recommend to use Cam_BuildPreviewGraph
CTS_API void Cam_PreviewVideoStop(DWORD devInst); // old function, recommend to use Cam_StopPreviewGraph
CTS_API int Cam_BuildPreviewGraph(DWORD devInst, HWND hwnd, WORD width, WORD height, unsigned char nDataFormat, unsigned char nDisplayFormat, unsigned char nBayerOrder); // same as Cam_PreviewVideoPlay
CTS_API int Cam_BuildPreviewGraphEx(DWORD devInst, HWND hwnd, unsigned char nDisplayFormat); // same as Cam_BuildPreviewGraph
CTS_API void Cam_StopPreviewGraph(DWORD devInst); // same as Cam_PreviewVideoStop

CTS_API BOOL Cam_MediaControl(DWORD devInst, unsigned char ctrl);
CTS_API DWORD Cam_MediaControlEx(DWORD devInst, unsigned char ctrl);

CTS_API BOOL Cam_SetDropFrameAfterI2C(DWORD devInst, BOOL bEnable); // 0: not drop, 1: drop(default)
CTS_API BOOL Cam_ClearCurrentFramesBuffer(DWORD devInst, BYTE byClearFramesBufferCount);


//grab frame
CTS_API BOOL Cam_GrabFrameOrg(DWORD devInst, unsigned char *pbuf, long *bufsize);
CTS_API BOOL Cam_GrabFrameEx(DWORD devInst, unsigned char *pbuf, long *bufsize, BYTE byDataType);
CTS_API BOOL Cam_GrabFrame(DWORD devInst, unsigned char *pbuf, long *bufsize);
CTS_API BOOL Cam_GrabFrameRaw10(DWORD devInst, unsigned char *pbuf, long *bufsize);
CTS_API BOOL Cam_GrabFrameRaw12(DWORD devInst, unsigned char *pbuf, long *bufsize);
CTS_API BOOL Cam_GrabFrameRGB24(DWORD devInst, unsigned char *pbuf, long *bufsize);
CTS_API BOOL Cam_GrabFrameGray(DWORD devInst, unsigned char *pbuf, long *bufsize);


CTS_API BOOL Cam_SetVideoWindow(DWORD devInst, HWND hwnd);
CTS_API BOOL Cam_ResizeVideoWindow(DWORD devInst, RECT rc);
CTS_API BOOL Cam_GetVideoDisplayRect(DWORD devInst, RECT &rc);

CTS_API long Cam_GetFrameCount(DWORD devInst);

CTS_API unsigned char Cam_IsCCMLost(DWORD devInst);
CTS_API BOOL Cam_IsFirstFrameArrived(DWORD devInst);
CTS_API float Cam_GetVideoRealTimeFPS(DWORD devInst);
CTS_API BOOL Cam_SetDropRenderFrames(DWORD devInst, BYTE mode);

//dual cam grab frame
CTS_API BOOL Cam_DualMediaControl(DWORD devInstM, DWORD devInstS, unsigned char ctrl);
CTS_API BOOL Cam_DualGrabFrameOrg(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);
CTS_API BOOL Cam_DualGrabFrameEx(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS, BYTE byDataType);
CTS_API BOOL Cam_DualGrabFrame(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);
CTS_API BOOL Cam_DualGrabFrameRaw10(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);
CTS_API BOOL Cam_DualGrabFrameRaw12(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);
CTS_API BOOL Cam_DualGrabFrameRGB24(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);
CTS_API BOOL Cam_DualGrabFrameGray(DWORD devInstM, DWORD devInstS, unsigned char *pbufM, long *bufsizeM, unsigned char *pbufS, long *bufsizeS);


//trigger grab frame
CTS_API BOOL Cam_TriggerGrabFrameOrg(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameEx(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx, BYTE byDataType);
CTS_API BOOL Cam_TriggerGrabFrame(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameRaw8(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameRaw10(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameRaw12(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameRGB24(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);
CTS_API BOOL Cam_TriggerGrabFrameGray(DWORD devInst, BYTE *pbuf, long *size, BYTE gpioNum, WORD &nCurTrigFrameIdx, WORD &nMaxTrigFrameIdx);


CTS_API BOOL Cam_GetTriggerGrabFrameCount(DWORD devInst, BYTE gpioNum, WORD wTrigFrameIndexList[TRIGGERGRABFRAMENUMMAX], WORD &nTrigFrameCnt);
CTS_API BOOL Cam_SetTriggerGrabFrameOffset(DWORD devInst, BYTE nFrameOffset);
CTS_API BOOL Cam_GetTriggerGrabFrameUpdateFlag(DWORD devInst, BYTE gpioNum, BOOL &bTrigFlag, WORD &nCurTrigFrameIdx);
CTS_API BOOL Cam_SetTriggerGrabFrameGPIOFlipOverMode(DWORD devInst, BYTE gpioNum, BOOL bEnable, BOOL bInitStatus);


//isp
CTS_API BOOL Cam_GetROIRGB(DWORD devInst, float &fRavg, float &fGavg, float &fBavg, WORD wROIx, WORD wROIy, WORD wROIw, WORD wROIh);
CTS_API BOOL Cam_SetAWBOnSensor(DWORD devInst, tagAWBREG AWBREG, BOOL bAWBEn, BOOL bKeepWBAfterAWBOFF, WORD wTolerance, WORD wROIx, WORD wROIy, WORD wROIw, WORD wROIh);
CTS_API BOOL Cam_SetAWBOnCTS(DWORD devInst, BOOL bAWBEn, BOOL bKeepWBAfterAWBOFF, WORD wTolerance, WORD wROIx, WORD wROIy, WORD wROIw, WORD wROIh);

CTS_API BYTE Cam_SetAEExOnSensor(DWORD devInst, tagREG reg[REGMAX], BOOL bYorG, WORD wValTarget, WORD wTolerance, WORD wROIx, WORD wROIy, WORD wROIw, WORD wROIh);
CTS_API BYTE Cam_SetAEOnSensor(DWORD devInst, tagREG reg[REGMAX], WORD wValTarget, WORD wValCur, WORD wTolerance, int &nExpInit);

CTS_API BOOL Cam_SetRGBGain(DWORD devInst, int reg[REGMAX], DWORD dwValue);
CTS_API BOOL Cam_GetRGBGain(DWORD devInst, int reg[REGMAX], DWORD &dwValue);

CTS_API BOOL Cam_SetExposure(DWORD devInst, tagREG reg[REGMAX], DWORD dwValue);
CTS_API BOOL Cam_GetExposure(DWORD devInst, tagREG reg[REGMAX], DWORD &dwValue);
CTS_API BOOL Cam_SetGain(DWORD devInst, tagREG reg[REGMAX], DWORD dwValue);
CTS_API BOOL Cam_GetGain(DWORD devInst, tagREG reg[REGMAX], DWORD &dwValue);

CTS_API BYTE Cam_CalcLSC(DWORD devInst, char *strLSCFile, BOOL bColorShadingCorrection, float fRatio);
CTS_API BOOL Cam_SaveLSC(DWORD devInst, char *strLSCFile);
CTS_API BOOL Cam_SetLSCOnCTS(DWORD devInst, BOOL bEnable);

//isp parse from ini
CTS_API BOOL CTS_ParseExposure(char *strFile, tagREG Reg[]);
CTS_API BOOL CTS_ParseGain(char *strFile, tagREG Reg[]);
CTS_API BOOL CTS_ParseAWBGain(char *strFile, tagAWBREG &AWBReg);


//parse from ini
CTS_API int CTS_ParseSENSOR_POWERDOWN_ACTIVE(char *strFile);
CTS_API int CTS_ParseSENSOR_RESET_ACTIVE(char *strFile);
CTS_API int CTS_ParseSENSOR_I2C_ID(char *strFile);
CTS_API int CTS_ParseI2C_SPEED(char *strFile);
CTS_API int CTS_ParseSENSOR_REG_ADDR_WIDTH(char *strFile);
CTS_API int CTS_ParseSENSOR_REG_DATA_WIDTH(char *strFile);
CTS_API int CTS_ParseSENSOR_FRAME_RATE(char *strFile);
CTS_API float CTS_ParseSENSOR_EXTCLK(char *strFile);
CTS_API int CTS_ParseSENSOR_OUTPUT_WIDTH(char *strFile);
CTS_API int CTS_ParseSENSOR_OUTPUT_HEIGHT(char *strFile);
CTS_API int CTS_ParseSENSOR_OUTPUT_FORMAT(char *strFile);
CTS_API int CTS_ParsePIX_ORDER(char *strFile);
CTS_API BOOL CTS_ParseChipID(char *strFile, tagREG Reg[]);

//isp parse from buffer
CTS_API BOOL CTS_ParseExposureFromBuffer(char *cBuffer, int length, tagREG Reg[]);
CTS_API BOOL CTS_ParseGainFromBuffer(char *cBuffer, int length, tagREG Reg[]);
CTS_API BOOL CTS_ParseAWBGainFromBuffer(char *cBuffer, int length, tagAWBREG &AWBReg);

//parse from buffer
CTS_API int CTS_ParseSENSOR_POWERDOWN_ACTIVEFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_RESET_ACTIVEFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_I2C_IDFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseI2C_SPEEDFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_REG_ADDR_WIDTHFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_REG_DATA_WIDTHFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_FRAME_RATEFromBuffer(char *cBuffer, int length);
CTS_API float CTS_ParseSENSOR_EXTCLKFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_OUTPUT_WIDTHFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_OUTPUT_HEIGHTFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParseSENSOR_OUTPUT_FORMATFromBuffer(char *cBuffer, int length);
CTS_API int CTS_ParsePIX_ORDERFromBuffer(char *cBuffer, int length);
CTS_API BOOL CTS_ParseChipIDFromBuffer(char *cBuffer, int length, tagREG Reg[]);



//for set ini 
CTS_API BOOL Cam_SetIniPower(DWORD devInst, float fpwr0, float fpwr1, float fpwr2, float fpwr3, float fpwr4, float fpwr5);
CTS_API BOOL Cam_SetIniSnrPwdnResetActive(DWORD devInst, BYTE byPwdnActive, BYTE byResetActive);

CTS_API BOOL Cam_SetIniOperationSequence(DWORD devInst, DWORD dwValue);
CTS_API BOOL Cam_SetIniSequenceDelay(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniSnrIoLevel(DWORD devInst, float fValue);

// SnrI2C_t
CTS_API BOOL Cam_SetIniSnrI2cId(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniSnrRegAddrDataWidth(DWORD devInst, BYTE byAddrWidth, BYTE byDataWidth);
CTS_API BOOL Cam_SetIniI2cSpeed(DWORD devInst, BYTE byValue);

//SnrParam_t
CTS_API BOOL Cam_SetIniSnrExtClk(DWORD devInst, float fValue);
CTS_API BOOL Cam_SetIniSnrOutputWidth(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniSnrOutputHeight(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniSnrFps(DWORD devInst, float fValue);

//USB3_BANDWIDTH
CTS_API BOOL Cam_SetIniUsb3Bandwidth(DWORD devInst, BYTE byValue);

// DATA_PATH_SELECTION
CTS_API BOOL Cam_SetIniDataPath(DWORD devInst, BYTE byValue);

// DATA_PATH_X	
CTS_API BOOL Cam_SetIniMipiLaneNumber(DWORD devInst, BYTE byValue);
	
CTS_API BOOL Cam_SetIniSnrPclkSampleEdge(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniSnrHsyncPolarity(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniSnrVsyncPolarity(DWORD devInst, BYTE byValue);
	
CTS_API BOOL Cam_SetIniSnrOutputFormat(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniBayerPixOrder(DWORD devInst, BYTE byValue);;

CTS_API BOOL Cam_SetIniDspOutputWidth(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniDspOutputHeight(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniYuvPixOrder(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniMipiTHsZeroMask(DWORD devInst, BYTE byValue);
CTS_API BOOL Cam_SetIniDspSampleStartPixX(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniDspSampleStartPixY(DWORD devInst, WORD wValue);
CTS_API BOOL Cam_SetIniMipiAutoSkewCalibration(DWORD devInst, BYTE byEnable, BYTE byData0, BYTE byData1, BYTE byData2, BYTE byData3);

CTS_API BOOL Cam_SetIniSnrRegSettings(DWORD devInst, tagREG regSettings[], WORD length, BYTE byAddrBytes, BYTE byDataBytes);
CTS_API BOOL Cam_SetIniDefault(DWORD devInst);


//OS
#define MAXPINCOUNT					128

typedef struct tagPIN
{
	BOOL isPass;//wether current pin is pass
	BOOL isISinkOverFlow;
	BOOL isISourOverFlow;
	unsigned int num;//pin index
	char name[MAXLEN];//pin name
	float voltISink;//voltage sink
	float voltISour;//voltage sour
	int stateISink;
	int stateISour;
	char warn[MAXLEN];//warn information
}PIN;

typedef struct tagRESISTANCE
{
	char name[MAXLEN];
	BOOL checked;
	BOOL pass;
	float value;
}RESISTANCE;

typedef struct tagLOGINFO
{
	BOOL isPass;// total result
	int lostTime; // 
	int count;
	PIN pin[MAXPINCOUNT];
	RESISTANCE restance[8];
}LOGINFO;

CTS_API BOOL OS_SetConfig(DWORD devInst, char *path);
CTS_API BOOL OS_CallConfigDialog(DWORD devInst, HWND hParnet, DWORD style, char *path);
CTS_API BOOL OS_TestScan(DWORD devInst, LOGINFO *info);
CTS_API BOOL OS_DualTestScan(DWORD devInstM, DWORD devInstS, LOGINFO *infoM, LOGINFO *infoS);
CTS_API BOOL OS_TestGetCCMType(DWORD devInst, BYTE *type, char *path);


CTS_API BOOL OS_SetIniDVPorMIPI(DWORD devInst, BOOL bDVPorMIPI);
CTS_API BOOL OS_SetIniShortPairEnable(DWORD devInst, BOOL bEnable);
CTS_API BOOL OS_SetIniPWRCapacitance(DWORD devInst, float fpwrCap);
CTS_API BOOL OS_SetIniISource(DWORD devInst, BOOL bISourceEN);

#endif


