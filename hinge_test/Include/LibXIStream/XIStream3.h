

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

#include "XIStream.h"


int
LIBXISTREAM_API
XIS_GetAudioCaptureCount3(
	);


BOOL
LIBXISTREAM_API
XIS_GetAudioCaptureInfo3(
	int iDevice,
	AUDIO_CAPTURE_INFO *pAudioCaptureInfo
	);


HANDLE
LIBXISTREAM_API
XIS_OpenAudioCapture3(
	const GUID& guidDevice
	);


void
LIBXISTREAM_API
XIS_CloseAudioCapture3(
	HANDLE hAudioCapture
	);


BOOL
LIBXISTREAM_API
XIS_SetAudioCaptureFormat3(
	HANDLE hAudioCapture,
	int cSamplesPerSec,
	int cChannels, 
	int cSamplesPerFrame, 
	int cBufferedFrames
	);


BOOL
LIBXISTREAM_API
XIS_SetAudioCaptureCallback3(
	HANDLE hAudioCapture,
	LPFN_AUDIO_CAPTURE_CALLBACK lpfnAudioCaptureCallback,
	void * pvParam
	);

BOOL
LIBXISTREAM_API
XIS_StartAudioCapture3(
	HANDLE hAudioCapture
	);

BOOL
LIBXISTREAM_API
XIS_StopAudioCapture3(
	HANDLE hAudioCapture
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioWaveFileOutput3(
	HANDLE hAudioCapture,
	LPCTSTR	lpszFilePath
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioMonitor3(
	HANDLE hAudioCapture,
	BOOL bEnableMonitor
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioMeterCallback3(
	HANDLE hAudioCapture,
	LPFN_AUDIO_METER_CALLBACK pCallback, 
	void* pvParam
	);

BOOL
LIBXISTREAM_API
XIS_SetAudioMeterCallbackEx3(
	HANDLE hAudioCapture,
	LPFN_AUDIO_METER_CALLBACKEX pCallback, 
	void* pvParam
	);


HANDLE
XIS_CreateMP4MuxerEx3(
	LPCTSTR		lpszPath,
	int			nTimeScale,
	BOOL		bCreateInfo,
	HANDLE		hVideoCapture,
	HANDLE		hAudioCapture,	// for AudioCapture3
	int			nChannel
	);
