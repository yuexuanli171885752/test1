
// XIStream.h : header file
//
// MAGEWELL PROPRIETARY INFORMATION
// This file is a part of the Magewell Xtreme Imaging SDK.
// Nanjing Magewell Electronics Co., Ltd., All Rights Reserved.
//
// CONTACT INFORMATION:
// james.liu@magewell.com
// http://www.magewell.com/
//
//////////////////////////////////////////////////////////////////////


#pragma once

#ifdef LIBXISTREAM_EXPORTS
#define LIBXISTREAM_API __declspec(dllexport)
#else
#define LIBXISTREAM_API __declspec(dllimport)
#endif

#include <mmsystem.h>

#include "../DeviceDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Module (un)intialization
BOOL
LIBXISTREAM_API
XIS_Initialize(
	);

void
LIBXISTREAM_API
XIS_Uninitialize(
	);

BOOL
LIBXISTREAM_API
XIS_RefreshDevices(
	);

////////////////////////////////////////////////////////////////////////////////
// Device Query
int
LIBXISTREAM_API
XIS_GetVideoCaptureCount(
	);

#define _MAX_VIDEO_CAPTURE_NAME			256
#define _MAX_VIDEO_CAPTURE_DSHOW_ID		256

typedef struct _VIDEO_CAPTURE_INFO {
	XI_ADAPTER_MODEL	adapterModel;
	XI_DEVICE_TYPE		deviceType;
	TCHAR				szName[_MAX_VIDEO_CAPTURE_NAME];
	TCHAR				szDShowID[_MAX_VIDEO_CAPTURE_DSHOW_ID];
} VIDEO_CAPTURE_INFO;

BOOL
LIBXISTREAM_API
XIS_GetVideoCaptureInfo(
	int iDevice,
	VIDEO_CAPTURE_INFO *pVideoCaptureInfo
	);

typedef struct _VIDEO_CAPTURE_INFO_EX {
	XI_ADAPTER_MODEL	adapterModel;
	XI_DEVICE_TYPE		deviceType;
	TCHAR				szName[_MAX_VIDEO_CAPTURE_NAME];
	TCHAR				szDShowID[_MAX_VIDEO_CAPTURE_DSHOW_ID];
	int					cxMin;
	int					cxMax;
	int					cxGranularity;
	int					cyMin;
	int					cyMax;
	int					cyGranularity;
	int					nFrameDurationMin;
	int					nFrameDurationMax;
	DWORD				dwColorFormatMasks;
} VIDEO_CAPTURE_INFO_EX;

BOOL
LIBXISTREAM_API
XIS_GetVideoCaptureInfoEx(
	int iDevice,
	VIDEO_CAPTURE_INFO_EX *pVideoCaptureInfoEx
	);

////////////////////////////////////////////////////////////////////////////////
// Audio Device Relationship
typedef enum _XI_AUDIO_DEVICE_TYPE
{
    XI_AUDIO_DEVICE_RENDER,
    XI_AUDIO_DEVICE_CAPTURE
} XI_AUDIO_DEVICE_TYPE;

typedef struct _XI_AUDIO_DEVICE_ENUM_DATA {
	XI_AUDIO_DEVICE_TYPE	type;
	ULONG					ulWaveID;
	GUID					guidDirectSound;
	LPTSTR                  lpszDescription;
	LPTSTR					lpszDeviceID;
} XI_AUDIO_DEVICE_ENUM_DATA;

typedef void 
(*LPFNENUM_RELATED_AUDIO_DEVICE_PROC)(
	XI_AUDIO_DEVICE_ENUM_DATA * pEnumData,
	LPVOID pvParam
	);

BOOL
LIBXISTREAM_API
XIS_EnumRelatedAudioDevice(
	LPCTSTR lpszVideoDeviceDShowID,
	LPFNENUM_RELATED_AUDIO_DEVICE_PROC lpfnEnumProc,
	LPVOID pvParam
	);

////////////////////////////////////////////////////////////////////////////////
// Device Open/Close
HANDLE
LIBXISTREAM_API
XIS_OpenVideoCapture(
	LPCTSTR lpszDShowID
	);


void
LIBXISTREAM_API
XIS_CloseVideoCapture(
	HANDLE hVideoCapture
	);


////////////////////////////////////////////////////////////////////////////////
// Device Properties
BOOL
LIBXISTREAM_API
XIS_ShowVideoCapturePropertyDialog(
	HANDLE hVideoCapture,
	HWND hWndParent
	);

BOOL
LIBXISTREAM_API
XIS_ShowVideoCapturePinPropertyDialog(
	HANDLE hVideoCapture,
	HWND hWndParent
	);

HANDLE
LIBXISTREAM_API
XIS_OpenVideoCapturePropertyHandle(
	HANDLE hVideoCapture
	);

BOOL
LIBXISTREAM_API
XIS_QueryInterface(
	HANDLE hVideoCapture,
	const GUID& iid, 
	LPVOID *ppvInterface
	);

////////////////////////////////////////////////////////////////////////////////
// Capture Format
BOOL
LIBXISTREAM_API
XIS_TestVideoCaptureFormat(
	HANDLE hVideoCapture,
	XI_COLOR_FORMAT colorFormat,
	int cx,
	int cy,
	int nFrameDuration
	);

BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureFormat(
	HANDLE hVideoCapture,
	XI_COLOR_FORMAT colorFormat,
	int cx,
	int cy,
	int nFrameDuration
	);

BOOL
LIBXISTREAM_API
XIS_GetVideoCaptureFormat(
	HANDLE hVideoCapture,
	XI_COLOR_FORMAT * pcolorFormat,
	int * pcx,
	int * pcy,
	int * pnFrameDuration
	);

BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureBuffering(
	HANDLE hVideoCapture,
	BOOL bBuffering
	);

////////////////////////////////////////////////////////////////////////////////
// Video Capture
typedef void (*LPFN_VIDEO_CAPTURE_CALLBACK)(const BYTE * pbyImage, int cbImageStride, void * pvParam);

typedef void (*LPFN_VIDEO_CAPTURE_CALLBACK_EX)(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp);

BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureCallback(
	HANDLE hVideoCapture,
	LPFN_VIDEO_CAPTURE_CALLBACK lpfnVideoCaptureCallback,
	void * pvParam
	);

BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureCallbackEx(
	HANDLE hVideoCapture,
	LPFN_VIDEO_CAPTURE_CALLBACK_EX lpfnVideoCaptureCallbackEx,
	void * pvParam,
	BOOL bLatencyMode
	);

BOOL
LIBXISTREAM_API
XIS_StartVideoCapture(
	HANDLE hVideoCapture
	);

BOOL
LIBXISTREAM_API
XIS_StopVideoCapture(
	HANDLE hVideoCapture
	);

BOOL
LIBXISTREAM_API
XIS_IsVideoCaptureStarted(
	HANDLE hVideoCapture
	);

BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureCallback(
	HANDLE hVideoCapture,
	LPFN_VIDEO_CAPTURE_CALLBACK lpfnVideoCaptureCallback,
	void * pvParam
	);

////////////////////////////////////////////////////////////////////////////////
// Audio Device Query

int
LIBXISTREAM_API
XIS_GetAudioCaptureCount(
	);

#define _MAX_AUDIO_CAPTURE_NAME			256

typedef struct _AUDIO_CAPTURE_INFO {
	TCHAR				szName[_MAX_AUDIO_CAPTURE_NAME];
	GUID				guid;
} AUDIO_CAPTURE_INFO;

typedef struct _AUDIO_CAPTURE_INFO_EX {
	TCHAR				szName[_MAX_AUDIO_CAPTURE_NAME];
	GUID				guid;
	TCHAR				szID[_MAX_AUDIO_CAPTURE_NAME];
} AUDIO_CAPTURE_INFO_EX;


BOOL
LIBXISTREAM_API
XIS_GetAudioCaptureInfo(
	int iDevice,
	AUDIO_CAPTURE_INFO *pAudioCaptureInfo
	);

BOOL
LIBXISTREAM_API
XIS_GetAudioCaptureInfoEx(
	int iDevice,
	AUDIO_CAPTURE_INFO_EX *pAudioCaptureInfoEx
	);

// XIS_OpenAudioCapture has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_OpenAudioCapture(
	const GUID& guidDevice
	);

HANDLE
LIBXISTREAM_API
XIS_OpenAudioCaptureEx(
	const TCHAR* pszID
	);

void
LIBXISTREAM_API
XIS_CloseAudioCapture(
	HANDLE hAudioCapture
	);	

BOOL
LIBXISTREAM_API
XIS_SetAudioCaptureFormat(
	HANDLE hAudioCapture,
	int cSamplesPerSec,
	int cChannels, 
	int cSamplesPerFrame, 
	int cBufferedFrames
	);

typedef void (*LPFN_AUDIO_CAPTURE_CALLBACK)(const BYTE * pbFrame, int cbFrame, void * pvParam);

typedef void (*LPFN_AUDIO_CAPTURE_CALLBACK_EX)(const BYTE * pbFrame, int cbFrame, void * pvParam, UINT64 u64TimeStamp);

BOOL
LIBXISTREAM_API
XIS_SetAudioCaptureCallback(
	HANDLE hAudioCapture,
	LPFN_AUDIO_CAPTURE_CALLBACK lpfnAudioCaptureCallback,
	void * pvParam
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioCaptureCallbackEx(
	HANDLE hAudioCapture,
	LPFN_AUDIO_CAPTURE_CALLBACK_EX lpfnAudioCaptureCallback,
	void * pvParam
	);

BOOL
LIBXISTREAM_API
XIS_StartAudioCapture(
	HANDLE hAudioCapture
	);

BOOL
LIBXISTREAM_API
XIS_StopAudioCapture(
	HANDLE hAudioCapture
	);


BOOL
LIBXISTREAM_API
XIS_SetAudioWaveFileOutput(
	HANDLE hAudioCapture,
	LPCTSTR	lpszFilePath
	);


BOOL
LIBXISTREAM_API
XIS_SetAudioMonitor(
	HANDLE hAudioCapture,
	BOOL bEnableMonitor
	);

typedef void (*LPFN_AUDIO_METER_CALLBACK)(int nValue, void * pvParam);

BOOL
LIBXISTREAM_API
XIS_SetAudioMeterCallback(
	HANDLE hAudioCapture,
	LPFN_AUDIO_METER_CALLBACK pCallback, 
	void* pvParam
	);


typedef void (*LPFN_AUDIO_METER_CALLBACKEX)(int nLeftValue, int nRightValue, void * pvParam);

BOOL
LIBXISTREAM_API
XIS_SetAudioMeterCallbackEx(
	HANDLE hAudioCapture,
	LPFN_AUDIO_METER_CALLBACKEX pCallback, 
	void* pvParam
	);

////////////////////////////////////////////////////////////////////////////////
// Video Renderer
HANDLE
LIBXISTREAM_API
XIS_CreateVideoRenderer(
	const GUID * pGUID, 
	HWND hWnd, 
	XI_COLOR_FORMAT colorFormat, 
	int nWidth, 
	int nHeight, 
	BOOL bUseOverlay,
	BOOL bWaitForVerticalBlank
	);

void
LIBXISTREAM_API
XIS_DestroyVideoRenderer(
	HANDLE hVideoRenderer
	);

BOOL
LIBXISTREAM_API
XIS_ResetVideoRenderer(
	HANDLE hVideoRenderer,
	const GUID * pGUID, 
	HWND hWnd, 
	XI_COLOR_FORMAT colorFormat, 
	int nWidth, 
	int nHeight, 
	BOOL bUseOverlay,
	BOOL bWaitForVerticalBlank
	);

BOOL
LIBXISTREAM_API
XIS_SetVideoRendererPosition(
	HANDLE hVideoRenderer,
	LPCRECT lprcSource,
	LPCRECT lprcDest
	);

BOOL 
LIBXISTREAM_API
XIS_VideoRendererDrawImage(
	HANDLE hVideoRenderer,
	const BYTE *pbImage, 
	int cbStride, 
	BOOL bSrcTopDown
	);

BOOL
LIBXISTREAM_API
XIS_VideoRendererClearImage(
	HANDLE hVideoRenderer
	);

BOOL
LIBXISTREAM_API
XIS_VideoRendererBeginBatchDraw(
	HANDLE hVideoRenderer
	);

BOOL 
LIBXISTREAM_API
XIS_VideoRendererBatchDrawRect(
	HANDLE hVideoRenderer,
	const BYTE *pbImage, 
	int cxSrc, int cySrc, int cbSrcStride, BOOL bSrcTopDown, 
	int xDest, int yDest
	);

BOOL 
LIBXISTREAM_API
XIS_VideoRendererEndBatchDraw(
	HANDLE hVideoRenderer,
	BOOL bPresent
	);

BOOL 
LIBXISTREAM_API
XIS_VideoRendererRepaintRect(
	HANDLE hVideoRenderer,
	LPCRECT lprcRepaint
	);

BOOL 
LIBXISTREAM_API
XIS_GetMonitorGUIDFromWindow(
	HWND hWnd,
	GUID* pGuidMonitor
	);

////////////////////////////////////////////////////////////////////////////////
// D3D Renderer

HANDLE 
LIBXISTREAM_API
XIS_CreateD3DRenderer(
	int				cx,
	int				cy,
	HWND			hWnd,
	DWORD			dwDuration,
	XI_COLOR_FORMAT color
	);

BOOL
LIBXISTREAM_API
XIS_DestroyD3DRenderer(
	HANDLE			hD3DRenderer
	);

BOOL
LIBXISTREAM_API
XIS_D3DRendererSetPos(
	HANDLE			hD3DRenderer,
	LPCRECT			prcSrc,
	LPCRECT			prcDst
	);

BOOL 
LIBXISTREAM_API
XIS_D3DRendererDrawImage(
	HANDLE			hD3DRenderer,
	const BYTE *	pbImage, 
	int				cbStride, 
	BOOL			bSrcTopDown
	);

////////////////////////////////////////////////////////////////////////////////
// D3D RendererEx

HANDLE 
LIBXISTREAM_API
XIS_CreateD3DRendererEx(
	int				cx,
	int				cy,
	HWND			hWnd,
	DWORD			dwDuration,
	XI_COLOR_FORMAT color
	);

BOOL
LIBXISTREAM_API
XIS_DestroyD3DRendererEx(
	HANDLE	hD3DRenderer
	);

HANDLE
LIBXISTREAM_API
XIS_OpenVideoCaptureEx(
	LPCTSTR lpszDShowID,
	HANDLE	hD3DRenderer
	);

BOOL 
LIBXISTREAM_API
XIS_D3DRendererDrawImageEx(
	HANDLE		hD3DRenderer,
	const BYTE *pbImage, 
	int			cbStride, 
	BOOL		bSrcTopDown
	);

////////////////////////////////////////////////////////////////////////////////
// Snapshot

BOOL 
LIBXISTREAM_API
XIS_VideoCaptureCreateSnapShot(
	HANDLE				hVideoCapture,
	LPCTSTR				lpszFilePath,
	int					nQuality
	);

BOOL 
LIBXISTREAM_API
XIS_VideoCaptureCreateSnapShotEx(
	HANDLE				hVideoCapture,
	LPCTSTR				lpszNameTemplate,
	int					nCount,
	int					nQuality
	);

////////////////////////////////////////////////////////////////////////////////
// H.264 / AAC Encoder

typedef void (*LPFN_H264_ENCODER_CALLBACK)(const BYTE * pbFrame, int cbFrame, const BYTE * pbFrameInfo, int cbFrameInfo, void * pvParam);


// XIS_IsSupportH264 has been abolished. Don't call it.
BOOL
LIBXISTREAM_API
XIS_IsSupportH264(
	);

// XIS_CreateH264Encoder has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_CreateH264Encoder(
	XI_COLOR_FORMAT		 colorFormat, 
	int					 cx, 
	int					 cy, 
	int					 nFrameDuration,
	XI_COMPRESSION_LEVEL level
	);

// XIS_CreateH264EncoderEx has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_CreateH264EncoderEx(
	XI_COLOR_FORMAT		 colorFormat, 
	int					 cx, 
	int					 cy, 
	int					 nFrameDuration,
	XI_COMPRESSION_LEVEL level,
	int					 nDstX,
	int					 nDstY,
	int					 nDstFrameDuration
	);

// XIS_DestroyH264Encoder has been abolished. Don't call it.
void
LIBXISTREAM_API
XIS_DestroyH264Encoder(
	HANDLE hH264Encoder
	);

// XIS_SetVideoCaptureEncoderOutput has been abolished. Don't call it.
BOOL
LIBXISTREAM_API
XIS_SetVideoCaptureEncoderOutput(
	HANDLE hVideoCapture,
	HANDLE hH264Encoder
	);

// XIS_SetH264EncoderCallback has been abolished. Don't call it.
BOOL
LIBXISTREAM_API
XIS_SetH264EncoderCallback(
	HANDLE					   hH264Encoder,
	LPFN_H264_ENCODER_CALLBACK lpfnH264EncoderCallback,
	void *					   pvParam
	);

// XIS_H264EncoderGetExtraData has been abolished. Don't call it.
int 
LIBXISTREAM_API
XIS_H264EncoderGetExtraData(
	HANDLE hH264Encoder,
	LPVOID* ppvExtraData
	);

BOOL
LIBXISTREAM_API
XIS_IsSupportH264HD(
	);

typedef struct _H264_ENCODER_PARAM {
	int							cx;
	int							cy;
	int							nFrameDuration;
	int							nKbps;
	XI_H264_TARGETUSAGE			usage;		 // XI_TARGETUSAGE_BEST_QUALITY, XI_TARGETUSAGE_BALANCED, XI_TARGETUSAGE_BEST_SPEED
	XI_H264_PROFILE				profile;	 // XI_PROFILE_BASELINE, XI_PROFILE_MAIN, XI_PROFILE_HIGH
	int							nGopPicSize; // default = 0
	int							nGopRefSize; // default = 0, if = 1 -> no b-frame
	int							nDstX;		 // default = 0
	int							nDstY;		 // default = 0
	int							nLowLatency; // default = 0, if = 1 -> LowLatency
} H264_ENCODER_PARAM, *PH264_ENCODER_PARAM;

#define MAX_H264_CHANNEL 4

BOOL
LIBXISTREAM_API
XIS_OpenVideoH264Encoder(
	HANDLE						hVideoCapture,
	int							nChannel, // from 0 to (MAX_H264_CHANNEL - 1)
	const H264_ENCODER_PARAM*	pH264Param,
	LPFN_H264_ENCODER_CALLBACK	lpfnH264EncoderCallback,
	void *						pvParam
	);

BOOL
LIBXISTREAM_API
XIS_H264EncoderForceKeyFrame(
	HANDLE	hVideoCapture,
	int		nChannel
	);

void
LIBXISTREAM_API
XIS_CloseVideoH264Encoder(
	HANDLE	hVideoCapture,
	int		nChannel
	);

typedef void (*LPFN_AUDIO_AACDATA_CALLBACK)(const BYTE * pbFrame, int cbFrame, void * pvParam);

typedef void (*LPFN_AUDIO_AACDATA_CALLBACK_EX)(const BYTE * pbFrame, int cbFrame, const BYTE * pbInfo, int cbInfo, void * pvParam);

typedef struct tagAAC_ENCODER_PARAM {
	XI_AAC_OBJECT	object;
	int				nKbps;
	int				nQuality;
	BOOL			bADTS;
} AAC_ENCODER_PARAM, *PAAC_ENCODER_PARAM;

// XIS_SetAudioAACDataOutput has been abolished. Don't call it.
BOOL
LIBXISTREAM_API
XIS_SetAudioAACDataOutput(
	HANDLE hAudioCapture,
	LPFN_AUDIO_AACDATA_CALLBACK lpCallback,
	void * pvParam,
	WAVEFORMATEX ** ppwfx,
	int* pnSamplesPerFrame
	);

BOOL
LIBXISTREAM_API
XIS_OpenAudioAACEncoder(
	HANDLE						hAudioCapture,
	const AAC_ENCODER_PARAM*	pAACParam,
	LPFN_AUDIO_AACDATA_CALLBACK lpCallback,
	void *						pvParam
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioAACEncoderCallback(
	HANDLE							hAudioCapture,
	LPFN_AUDIO_AACDATA_CALLBACK_EX	lpCallbackEx,
	void *							pvParam
	);

void
LIBXISTREAM_API
XIS_CloseAudioAACEncoder(
	HANDLE	hAudioCapture
	);

////////////////////////////////////////////////////////////////////////////////
// MP4 Muxer

// XIS_CreateMP4Muxer has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_CreateMP4Muxer(
	LPCTSTR		lpszPath,
	int			nTimeScale
	);

// XIS_MP4MuxerAddVideoStream has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_MP4MuxerAddVideoStream(
	HANDLE hMP4Muxer,
	const VOID * pvExtraData,
	int cbExtraData,
	int cx,
	int cy,
	int nFrameDuration,
	XI_COMPRESSION_LEVEL level,
	DWORD dwAspectRatioX,
	DWORD dwAspectRatioY
	);

// XIS_MP4MuxerAddAudioStream has been abolished. Don't call it.
HANDLE
LIBXISTREAM_API
XIS_MP4MuxerAddAudioStream(
	HANDLE hMP4Muxer,
	const WAVEFORMATEX * pwfx,
	int	nSamplesPerFrame
	);

// XIS_MP4MuxerPutFrame has been abolished. Don't call it.
BOOL
LIBXISTREAM_API
XIS_MP4MuxerPutFrame(
	HANDLE hStream,
	const BYTE * pbFrame,
	int cbFrame,
	const BYTE * pbFrameInfo,
	int cbFrameInfo
	);

HANDLE
LIBXISTREAM_API
XIS_CreateMP4MuxerEx(
	LPCTSTR		lpszPath,
	int			nTimeScale,
	BOOL		bCreateInfo,
	HANDLE		hVideoCapture,
	HANDLE		hAudioCapture,
	int			nChannel
	);



void
LIBXISTREAM_API
XIS_DestroyMP4Muxer(
	HANDLE hMP4Muxer
	);

BOOL
LIBXISTREAM_API
XIS_StartMP4Muxer(
	HANDLE hMP4Muxer
	);

void
LIBXISTREAM_API
XIS_StopMP4Muxer(
	HANDLE hMP4Muxer
	);

void
LIBXISTREAM_API
XIS_ReCloseBadMP4(
	LPCTSTR		lpszFilePath,
	LPCTSTR		lpszInfoPath
	);

BOOL
LIBXISTREAM_API
XIS_PauseMP4Muxer(
	HANDLE hMP4Muxer
	);

BOOL
LIBXISTREAM_API
XIS_ResumeMP4Muxer(
	HANDLE hMP4Muxer
	);

////////////////////////////////////////////////////////////////////////////////
// AVI Muxer

HANDLE
LIBXISTREAM_API
XIS_CreateAVIMuxer(
	LPCTSTR lpszPath,
	HANDLE hVideoCapture,
	HANDLE hAudioCapture
	);

void
LIBXISTREAM_API
XIS_DestroyAVIMuxer(
	HANDLE hAVIMuxer
	);

BOOL
LIBXISTREAM_API
XIS_StartAVIMuxer(
	HANDLE hAVIMuxer
	);

void
LIBXISTREAM_API
XIS_StopAVIMuxer(
	HANDLE hAVIMuxer
	);


////////////////////////////////////////////////////////////////////////////////
// Video Mixer

HANDLE
LIBXISTREAM_API
XIS_CreateVideoMixer(
	int cx,
	int cy,
	int nFrameDuration,
	XI_COLOR_FORMAT colorFormat
	);

void
LIBXISTREAM_API
XIS_DestroyVideoMixer(
    HANDLE hVideoMixer
	);

typedef void (*LPFN_VIDEOMIXER_COMPOSITE_CALLBACK)(void * pvParam);

typedef void (*LPFN_VIDEOMIXER_OUTPUT_CALLBACK)(const BYTE * pbFrame, int cbFrame, const BYTE * pbFrameInfo, int cbFrameInfo, void * pvParam);

BOOL
LIBXISTREAM_API
XIS_SetVideoMixerCallback(
	HANDLE								hVideoMixer,
	LPFN_VIDEOMIXER_COMPOSITE_CALLBACK	lpfnCompositeCallback,
	void*								pvCompositeParam,
	LPFN_VIDEOMIXER_OUTPUT_CALLBACK		lpfnOutputCallback,
	void*								pvOutputParam
	);

HANDLE
LIBXISTREAM_API
XIS_VideoMixerAddChannel(
    HANDLE			hVideoMixer,
	int				cx,
	int				cy,
	XI_COLOR_FORMAT	colorFormat,
	HANDLE			hVideoCapture
	);

void 
LIBXISTREAM_API
XIS_VideoMixerDelChannel(
    HANDLE	hVideoMixer,
	HANDLE	hChannel
	);


typedef struct XIS_CHANNEL_ITEM {
	HANDLE	hChannel;
	RECT	rcSrc;
	RECT	rcDest;
	BYTE	byAlpha;
	int		nLevel;
} XIS_CHANNEL_ITEM, *PXIS_CHANNEL_ITEM;


BOOL
LIBXISTREAM_API
XIS_VideoMixerSetChannels(
    HANDLE					hVideoMixer,
	const XIS_CHANNEL_ITEM*	pChannelItems,
	int						cChannelItems
	);

HANDLE
LIBXISTREAM_API
XIS_VideoMixerAddImage(
    HANDLE			hVideoMixer,
	LPCTSTR			lpszPath,
	int				cx,
	int				cy
	);

BOOL
LIBXISTREAM_API
XIS_VideoMixerSetImage(
    HANDLE			hVideoMixer,
	HANDLE			hImage,
	int				x,
	int				y
	);

void
LIBXISTREAM_API
XIS_VideoMixerDelImage(
    HANDLE			hVideoMixer,
	HANDLE			hImage
	);

typedef struct tagXI_CAPTION_FONT {
	int				nSize;
	int				nWeight;
	COLORREF		clrFont;
	COLORREF		clrBorder;
	BOOL			bUnderLine;
	BOOL			bStyleBold;
	BOOL			bStyleItalic;
} XI_CAPTION_FONT, *PXI_CAPTION_FONT;

HANDLE
LIBXISTREAM_API
XIS_VideoMixerAddCaption(
    HANDLE					hVideoMixer,
	LPCTSTR					lpszCaption,
	LPCTSTR					lpszFont,
	const XI_CAPTION_FONT*	font,
	SIZE*					psiCaption
	);

BOOL
LIBXISTREAM_API
XIS_VideoMixerSetCaption(
    HANDLE			hVideoMixer,
	HANDLE			hCaption,
	int				x,
	int				y
	);

void
LIBXISTREAM_API
XIS_VideoMixerDelCaption(
    HANDLE			hVideoMixer,
	HANDLE			hCaption
	);

BOOL
LIBXISTREAM_API
XIS_VideoMixerRun(
    HANDLE					hVideoMixer
	);

void
LIBXISTREAM_API
XIS_VideoMixerStop(
    HANDLE					hVideoMixer
	);

BOOL
LIBXISTREAM_API
XIS_OpenVideoMixerH264Encoder(
	HANDLE						hVideoMixer,
	const H264_ENCODER_PARAM*	pH264Param,
	LPFN_H264_ENCODER_CALLBACK	lpfnH264EncoderCallback,
	void *						pvParam
	);

void
LIBXISTREAM_API
XIS_CloseVideoMixerH264Encoder(
	HANDLE					hVideoMixer
	);

HANDLE
LIBXISTREAM_API
XIS_CreateVideoMixerMP4Muxer(
	LPCTSTR		lpszPath,
	int			nTimeScale,
	HANDLE		hVideoMixer,
	HANDLE		hAudioCapture
	);

BOOL 
LIBXISTREAM_API
XIS_VideoMixerCreateSnapShot(
	HANDLE				hVideoMixer,
	LPCTSTR				lpszFilePath,
	int					nQuality
	);



////////////////////////////////////////////////////////////////////////////////
// D9 Capture & Renderer & Encoder

BOOL
LIBXISTREAM_API
XD9_Init(
	);

void
LIBXISTREAM_API
XD9_Exit(
	);

BOOL
LIBXISTREAM_API
XD9_Start(
	DWORD dwDuration
	);

void
LIBXISTREAM_API
XD9_Stop(
	);

HANDLE
LIBXISTREAM_API
XD9_GetInstance(
	);

typedef void (*LPFN_XD9_RENDERER_CALLBACK)(void * pvParam);

HANDLE
LIBXISTREAM_API
XD9_CreateMainRenderer(
	int							cx,
	int							cy,
	XI_COLOR_FORMAT				colorFormat,
	DWORD						dwDuration,
	HWND						hWnd,
	LPFN_XD9_RENDERER_CALLBACK	lpfnRendererCallback,
	void *						pvParam
	);

HANDLE
LIBXISTREAM_API
XD9_CreateChildRenderer(
	int							cx,
	int							cy,
	HWND						hWnd,
	LPFN_XD9_RENDERER_CALLBACK	lpfnRendererCallback,
	void *						pvParam
	);

void
LIBXISTREAM_API
XD9_DestroyRenderer(
	HANDLE		hD9Renderer
	);

BOOL
LIBXISTREAM_API
XD9_MapChildRenderer(
	HANDLE		hD9Renderer,
	HWND		hWnd
	);

BOOL
LIBXISTREAM_API
XD9_UnmapChildRenderer(
	HANDLE		hD9Renderer,
	HWND		hWnd
	);

BOOL
LIBXISTREAM_API
XD9_StartRenderer(
	);

void
LIBXISTREAM_API
XD9_StopRenderer(
	);

HANDLE
LIBXISTREAM_API
XD9_CreateCapture(
	LPCTSTR			lpszDShowID,
	int				cx,
	int				cy,
	XI_COLOR_FORMAT color,
	int				nDuration
	);

void
LIBXISTREAM_API
XD9_DestroyCapture(
	HANDLE		hD9Capture
	);

HANDLE
LIBXISTREAM_API
XD9_CreateImage(
	const TCHAR*	pszFilePath
	);

void
LIBXISTREAM_API
XD9_DestroyImage(
	HANDLE		hD9Image
	);

typedef enum {
	D9_NOTSET = -1,
	D9_HIDE = 0,
	D9_SHOW = 1,
};

HANDLE
LIBXISTREAM_API
XD9_CreatePosition(
	HANDLE	hD9Renderer,
	HANDLE	hD9Item		// capture or image
	);

BOOL
LIBXISTREAM_API
XD9_SetPosition(
	HANDLE	hD9Pos,
	int		x,
	int		y,
	int		cx,
	int		cy,
	int		z,
	int		byAlpha,
	int		show		// D9_TEXTURE_NOSET, D9_TEXTURE_HIDE, D9_TEXTURE_SHOW
	);

void
LIBXISTREAM_API
XD9_DestroyPosition(
	HANDLE	hD9Pos
	);

BOOL
LIBXISTREAM_API
XD9_OpenH264Encoder(
	HANDLE						hD9Renderer,
	const H264_ENCODER_PARAM*	pH264Param,
	LPFN_H264_ENCODER_CALLBACK	lpfnH264EncoderCallback,
	void *						pvParam
	);

void
LIBXISTREAM_API
XD9_CloseH264Encoder(
	HANDLE	hD9Renderer
	);

HANDLE
LIBXISTREAM_API
XD9_CreateMP4Muxer(
	LPCTSTR lpszPath,
	int nTimeScale,
	HANDLE hD9Renderer,
	HANDLE hAudioCapture
	);

#ifdef __cplusplus
}
#endif
