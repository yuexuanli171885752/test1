
// XIPCVBS.h : header file
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
// Signal Status
HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_IsSignalPresent(
	HANDLE	hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_IsSignalChanged(
	HANDLE hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_GetSignalFormat(
	HANDLE	hXIProperty,
	int *	pnSignalWidth,
	int *	pnSignalHeight,
	int *	pnSignalFrameDuration
	);

////////////////////////////////////////////////////////////////////////////////
// Square Pixel Mode
// NOTE:
//	 1. Change square pixel mode will change signal format & clip
HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_IsSquarePixelMode(
	HANDLE	hXIProperty
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_SetSquarePixelMode(
	HANDLE	hXIProperty,
	BOOL 	bSquarePixelMode
	);

////////////////////////////////////////////////////////////////////////////////
// Clip
HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_GetClip(
	HANDLE	hXIProperty,
	int *	pnLeft,
	int	*	pnTop,
	int	*	pnWidth,
	int	*	pnHeight
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_SetClip(
	HANDLE	hXIProperty,
	int		nLeft,
	int		nTop,
	int		nWidth,
	int		nHeight
	);

////////////////////////////////////////////////////////////////////////////////
// Scale Type
typedef enum _XIPCVBS_SCALE_TYPE
{
	XICVBS_SCALE_DO_NOT_KEEP_ASPECT,
	XICVBS_SCALE_FILL_TO_KEEP_ASPECT,
	XICVBS_SCALE_ZOOM_TO_KEEP_ASPECT,
} XIPCVBS_SCALE_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_GetScaleType(
	HANDLE hXIProperty,
	XIPCVBS_SCALE_TYPE * pnScaleType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_SetScaleType(
	HANDLE hXIProperty,
	XIPCVBS_SCALE_TYPE nScaleType
	);

////////////////////////////////////////////////////////////////////////////////
// Deinterlace Type
typedef enum _XIPCVBS_DEINTERLACE_TYPE
{
	XIPCVBS_DEINTERLACE_NONE,
	XIPCVBS_DEINTERLACE_BLEND,
	XIPCVBS_DEINTERLACE_MOTION_ADAPTIVE
} XIPCVBS_DEINTERLACE_TYPE;

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_GetDeinterlaceType(
	HANDLE hXIProperty,
	XIPCVBS_DEINTERLACE_TYPE * pnDeinterlaceType
	);

HRESULT 
LIBXIPROPERTY_API 
XIPCVBS_SetDeinterlaceType(
	HANDLE hXIProperty,
	XIPCVBS_DEINTERLACE_TYPE nDeinterlaceType
	);

////////////////////////////////////////////////////////////////////////////////
// Brightness, Contrast, Hue, Gamma Parameters
typedef enum _XIPCVBS_VIDEO_PROC_PARAM_TYPE
{
	XIPCVBS_VIDEO_PROC_BRIGHTNESS,
	XIPCVBS_VIDEO_PROC_CONTRAST,
	XIPCVBS_VIDEO_PROC_HUE,
	XIPCVBS_VIDEO_PROC_SATURATION
} XIPCVBS_VIDEO_PROC_PARAM_TYPE;

HRESULT
LIBXIPROPERTY_API 
XIPCVBS_GetVideoProcParamRange(
	HANDLE hXIProperty,
	XIPCVBS_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long * plParamValueMin,
	long * plParamValueMax,
	long * plParamValueDef
	);

HRESULT
LIBXIPROPERTY_API 
XIPCVBS_GetVideoProcParam(
	HANDLE hXIProperty,
	XIPCVBS_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long * plParamValue
	);

HRESULT
LIBXIPROPERTY_API 
XIPCVBS_SetVideoProcParam(
	HANDLE hXIProperty,
	XIPCVBS_VIDEO_PROC_PARAM_TYPE videoProcParamType,
	long lParamValue
	);

#ifdef __cplusplus
}
#endif
