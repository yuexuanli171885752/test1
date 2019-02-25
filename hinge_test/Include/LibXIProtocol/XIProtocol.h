
// XIStreamEx.h : header file
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

#ifdef LIBXIPROTOCOL_EXPORTS
#define LIBXIPROTOCOL_API __declspec(dllexport)
#else
#define LIBXIPROTOCOL_API __declspec(dllimport)
#endif

BOOL
LIBXIPROTOCOL_API
XPL_Initialize(
	);

void
LIBXIPROTOCOL_API
XPL_Uninitialize(
	);

typedef enum XPL_EVENT {
	XPL_DISCONNECTED,
	XPL_CONNECTED,
	XPL_TIMEOUT,
	XPL_SENDFAIL,
	XPL_RECEIVEFAIL,
	XPL_SHUTDOWNSUCCESS,
	XPL_SHUTDOWNFAIL,
	XPL_SENDABORTED,
	XPL_FORCEKEYFRAME,
	XPL_SENDOK,

	XPL_NEEDAUTH,
	XPL_WRONGPASSWORD,
	XPL_NOSUCHUSER,
	XPL_UNKNOWNAUTHMODE,
	XPL_NOTALOWED,
};

typedef void (*LPFN_XPLEVENT_CALLBACK)(XPL_EVENT ent, void * pvParam);

/////// Test Connect
BOOL
LIBXIPROTOCOL_API
XPL_TestConnect(
	const char*				lpszUrl,
	DWORD					dwTimeOutInMS
	);


/////// RTMP Client

HANDLE
LIBXIPROTOCOL_API
XPL_CreateRTMPClient(
	const char*				lpszUrl,
	const char*				lpszUserName,
	const char*				lpszPassword,
	DWORD					dwTimeOutInMS,
	LPFN_XPLEVENT_CALLBACK	lpCallback,
	void*					pvParam
	);

BOOL 
LIBXIPROTOCOL_API
XPL_SetRTMPMetaData(
	HANDLE			hRTMPClient,
	const char*		pszKey,
	const char*		pszValue
	);

BOOL 
LIBXIPROTOCOL_API
XPL_SetRTMPVideoFormat(
	HANDLE			hRTMPClient,
	int				nWidth,
	int				nHeight,
	int				nFrameDuration,
	int				nGopSize
	);

BOOL 
LIBXIPROTOCOL_API
XPL_SetRTMPAudioFormat(
	HANDLE			hRTMPClient,
	int				nSamplerate,
	int				nChannels,
	int				nSamplePerFrame
	);

BOOL 
LIBXIPROTOCOL_API
XPL_WriteRTMPData(
	HANDLE			hRTMPClient,
	BOOL			bVideo,
	const BYTE*		pbFrame,
	int				cbFrame,
	const BYTE*		pbFrameInfo,
	int				cbFrameInfo
	);

void
LIBXIPROTOCOL_API
XPL_DestroyRTMPClient(
	HANDLE			hRTMPClient
	);

void
LIBXIPROTOCOL_API
XPL_SetRTMPAutoReconnect(
	HANDLE			hRTMPClient,
	int				nMillisecond
	);
/////// TS Sender

HANDLE
LIBXIPROTOCOL_API
XPL_CreateTSSender(
	const char*		lpszUrl,
	BOOL			bHasAudio
	);

BOOL 
LIBXIPROTOCOL_API
XPL_WriteTSData(
	HANDLE			hTSSender,
	BOOL			bVideo,
	const BYTE*		pbFrame,
	int				cbFrame,
	const BYTE*		pbFrameInfo,
	int				cbFrameInfo
	);

void
LIBXIPROTOCOL_API
XPL_DestroyTSSender(
	HANDLE			hTSSender
	);
