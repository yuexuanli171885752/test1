
// XIPHDCapture.h : header file
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

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Input Type
typedef enum _XIPHD_INPUT_TYPE
{
	XIPHD_INPUT_DVI_HDMI,
	XIPHD_INPUT_VGA,
	XIPHD_INPUT_Component,
	XIPHD_INPUT_CVBS,
	XIPHD_INPUT_SVideo,
	XIPHD_INPUT_SDI,
	XIPHD_INPUT_Second_Component,
	XIPHD_INPUT_HDMI_CH1,
	XIPHD_INPUT_HDMI_CH2,
	XIPHD_INPUT_HDMI_CH3,
	XIPHD_INPUT_HDMI_CH4
} XIPHD_INPUT_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPHD_IsAutoDetectInputType(
	HANDLE hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetAutoDetectInputType(
	HANDLE hXIProperty,
	BOOL bAutoDetectInputType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSupportedInputTypes(
	HANDLE hXIProperty,
	int *pnSupportedInputTypeMasks
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetInputType(
	HANDLE hXIProperty,
	XIPHD_INPUT_TYPE *pnInputType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetInputType(
	HANDLE hXIProperty,
	XIPHD_INPUT_TYPE nInputType
	);

////////////////////////////////////////////////////////////////////////////////
// Signal Status
HRESULT 
LIBXIPROPERTY_API 
XIPHD_IsSignalPresent(
	HANDLE hXIProperty
	);

HRESULT
LIBXIPROPERTY_API 
XIPHD_IsSignalProgressive(
	HANDLE	hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_IsSignalChanged(
	HANDLE	hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSignalFormat(
	HANDLE	hXIProperty,
	int *	pnSignalWidth,
	int *	pnSignalHeight,
	int *	pnSignalFrameDuration
	);

////////////////////////////////////////////////////////////////////////////////
// Clip
HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetDefClip(
	HANDLE	hXIProperty,
	int *	pnLeft,
	int	*	pnTop,
	int	*	pnWidth,
	int	*	pnHeight
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetClip(
	HANDLE	hXIProperty,
	int *	pnLeft,
	int	*	pnTop,
	int	*	pnWidth,
	int	*	pnHeight
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetClip(
	HANDLE	hXIProperty,
	int		nLeft,
	int		nTop,
	int		nWidth,
	int		nHeight
	);

////////////////////////////////////////////////////////////////////////////////
// Sampling
// NOTE:
//   1. Only valid for VGA input mode
//   2. Call SetSamplingCount will change signal format
HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSamplingPhaseRange(
	HANDLE	hXIProperty,
	BYTE *	pbyPhaseMin,
	BYTE *	pbyPhaseMax,
	BYTE *	pbyPhaseDef
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSamplingPhase(
	HANDLE	hXIProperty,
	BYTE *	pbyPhase
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetSamplingPhase(
	HANDLE	hXIProperty,
	BOOL	bAutoDetectPhase,
	BYTE 	byPhase
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSamplingCountRange(
	HANDLE	hXIProperty,
	int *	pcxSamplingCountMin,
	int *	pcxSamplingCountMax,
	int *	pcxSamplingCountDef
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetSamplingCount(
	HANDLE	hXIProperty,
	int *	pcxSamplingCount
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetSamplingCount(
	HANDLE	hXIProperty,
	int 	cxSamplingCount
	);

////////////////////////////////////////////////////////////////////////////////
// Scale Type
typedef enum _XIPHD_SCALE_TYPE
{
	XIPHD_SCALE_DO_NOT_KEEP_ASPECT,
	XIPHD_SCALE_FILL_TO_KEEP_ASPECT,
	XIPHD_SCALE_ZOOM_TO_KEEP_ASPECT,
} XIPHD_SCALE_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetScaleType(
	HANDLE hXIProperty,
	XIPHD_SCALE_TYPE * pnScaleType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetScaleType(
	HANDLE hXIProperty,
	XIPHD_SCALE_TYPE nScaleType
	);

////////////////////////////////////////////////////////////////////////////////
// Deinterlace Type
typedef enum _XIPHD_DEINTERLACE_TYPE
{
	XIPHD_DEINTERLACE_NONE,
	XIPHD_DEINTERLACE_BLEND,
	XIPHD_DEINTERLACE_MOTION_ADAPTIVE,
	XIPHD_DEINTERLACE_TOP_FIELD,
	XIPHD_DEINTERLACE_BOTTOM_FIELD
} XIPHD_DEINTERLACE_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetDeinterlaceType(
	HANDLE hXIProperty,
	XIPHD_DEINTERLACE_TYPE * pnDeinterlaceType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetDeinterlaceType(
	HANDLE hXIProperty,
	XIPHD_DEINTERLACE_TYPE nDeinterlaceType
	);

////////////////////////////////////////////////////////////////////////////////
// Image Flip
HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetFlipMode(
	HANDLE hXIProperty,
	BOOL * pbFlipHorz,
	BOOL * pbFlipVert
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetFlipMode(
	HANDLE hXIProperty,
	BOOL bFlipHorz,
	BOOL bFlipVert
	);

////////////////////////////////////////////////////////////////////////////////
// Brightness, Contrast, Hue, Gamma Parameters
typedef enum _XIPHD_VIDEO_PROC_PARAM_TYPE
{
	XIPHD_VIDEO_PROC_BRIGHTNESS,
	XIPHD_VIDEO_PROC_CONTRAST,
	XIPHD_VIDEO_PROC_HUE,
	XIPHD_VIDEO_PROC_SATURATION
} XIPHD_VIDEO_PROC_PARAM_TYPE;

HRESULT
LIBXIPROPERTY_API 
XIPHD_GetVideoProcParamRange(
	HANDLE hXIProperty,
	XIPHD_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long * plParamValueMin,
	long * plParamValueMax,
	long * plParamValueDef
	);

HRESULT
LIBXIPROPERTY_API 
XIPHD_GetVideoProcParam(
	HANDLE hXIProperty,
	XIPHD_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long * plParamValue
	);

HRESULT
LIBXIPROPERTY_API 
XIPHD_SetVideoProcParam(
	HANDLE hXIProperty,
	XIPHD_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long lParamValue
	);

////////////////////////////////////////////////////////////////////////////////
// Image R/G/B Component Adjustment
HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetRGBComponentProcParam(
	HANDLE hXIProperty,
	int * pnRedBrightness, 
	int * pnRedContrast, 
	int * pnGreenBrightness, 
	int * pnGreenContrast, 
	int * pnBlueBrightness, 
	int * pnBlueContrast
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetRGBComponentProcParam(
	HANDLE hXIProperty,
	int nRedBrightness, 
	int nRedContrast, 
	int nGreenBrightness, 
	int nGreenContrast, 
	int nBlueBrightness, 
	int nBlueContrast
	);

////////////////////////////////////////////////////////////////////////////////
// HDMI related routines
typedef enum _XIPHD_HDMI_INFO_FRAME_TYPE 
{
	XIPHD_HDMI_VS_INFO_FRAME			= 0x01,
	XIPHD_HDMI_AVI_INFO_FRAME			= 0x02,
	XIPHD_HDMI_SPD_INFO_FRAME			= 0x03,
	XIPHD_HDMI_AUDIO_INFO_FRAME			= 0x04,
	XIPHD_HDMI_MPEG_SOURCE_INFO_FRAME	= 0x05
} XIPHD_HDMI_INFO_FRAME_TYPE;

#pragma pack(push)
#pragma pack(1)

#define XIPHD_MAX_DOWNSTREAM_KSVS		127
#define XIPHD_MAX_INFO_FRAME_DATA_SIZE	64

typedef struct _XIPHD_HDMI_INFO_FRAME
{
	BOOL			bValid;
	BYTE			byType;
	BYTE			byVersion;
	BYTE			byLength;
	BYTE			byCheckSum;
	BYTE			abyData[XIPHD_MAX_INFO_FRAME_DATA_SIZE];
} XIPHD_HDMI_INFO_FRAME;

typedef struct _XIPHD_KSV
{
	BYTE			data[5];
} XIPHD_KSV;

typedef struct _XIPHD_HDMI_REPEATER_MODE
{
	BOOL 			bRepeaterMode;
	int				cDownstreamKSVs;
	int				cDownstreamDepth;
	XIPHD_KSV		aDownstreamKSVs[XIPHD_MAX_DOWNSTREAM_KSVS];
} XIPHD_HDMI_REPEATER_MODE;

#pragma pack(pop)

HRESULT 
LIBXIPROPERTY_API 
XIPHD_IsHDMIPropertySupported(
	HANDLE hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetHDMIInfoFrame(
	HANDLE hXIProperty,
	XIPHD_HDMI_INFO_FRAME_TYPE infoFrameType,
	XIPHD_HDMI_INFO_FRAME * pInfoFrame
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetAKSV(
	HANDLE hXIProperty,
	XIPHD_KSV * pAKSV
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetRepeaterMode(
	HANDLE hXIProperty,
	XIPHD_HDMI_REPEATER_MODE * pHDMIRepeaterMode
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_SetEDID(
	HANDLE hXIProperty,
	void * pvEDIDData,
	int cbEDIDData
	);

HRESULT 
LIBXIPROPERTY_API 
XIPHD_GetEDID(
	HANDLE hXIProperty,
	void * pvEDIDData,
	int * pcbEDIDData
	);

typedef enum  
{
	XIPHD_HDMI_EVENT_AVI_INFO_FRAME_UPDATE,			
	XIPHD_HDMI_EVENT_AUDIO_INFO_FRAME_UPDATE,			
	XIPHD_HDMI_EVENT_SPD_INFO_FRAME_UPDATE,	
	XIPHD_HDMI_EVENT_MPEG_SOURCE_INFO_FRAME_UPDATE,
	XIPHD_HDMI_EVENT_VS_INFO_FRAME_UPDATE,
	XIPHD_HDMI_EVENT_AKSV_UPDATE
} XIPHD_HDMI_EVENT_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPHD_EnableEventNotify(
	HANDLE hXIProperty,
	XIPHD_HDMI_EVENT_TYPE eventType,
	HANDLE hEvent
	);

#ifdef __cplusplus
}
#endif
