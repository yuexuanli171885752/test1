
// XIProperty.h : header file
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

#ifdef LIBXIPROPERTY_EXPORTS
#define LIBXIPROPERTY_API __declspec(dllexport)
#else
#define LIBXIPROPERTY_API __declspec(dllimport)
#endif

#include "../DeviceDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Open/Close Property Controller
//   NULL: Failed
HANDLE 
LIBXIPROPERTY_API 
XIP_OpenPropertyHandle(
	IUnknown *pCaptureFilter
	);

void 
LIBXIPROPERTY_API 
XIP_ClosePropertyHandle(
	HANDLE hXIProperty
	);

// Get device type
XI_DEVICE_TYPE
LIBXIPROPERTY_API 
XIP_GetDeviceType(
	HANDLE hXIProperty
	);

typedef struct _XIP_HARDWARE_INFO {
	TCHAR	szDeviceID[256];
	TCHAR	szInstanceID[32];
	ULONG	ulBusNumber;
	USHORT	usDeviceNumber;
	USHORT	usFunctionNumber;
} XIP_HARDWARE_INFO;

HRESULT 
LIBXIPROPERTY_API 
XIP_GetHardwareInfo(
	HANDLE	hXIProperty,
	XIP_HARDWARE_INFO * pInfo
	);

// Firmware APIs
typedef enum _XIP_FIRMWARE_UPDATE_STATE {
	XIP_FIRMWARE_INITIAL,
	XIP_FIRMWARE_READING,
	XIP_FIRMWARE_ERASING,
	XIP_FIRMWARE_WRITING,
	XIP_FIRMWARE_VERIFING,
	XIP_FIRMWARE_ERROR,
	XIP_FIRMWARE_COMPLETE
} XIP_FIRMWARE_UPDATE_STATE;

typedef 
void 
(*XIP_FIRMWARE_UPDATE_CALLBACK)(
	XIP_FIRMWARE_UPDATE_STATE state,
	int nProgress,
	LPVOID pvParam
	);

HRESULT 
LIBXIPROPERTY_API 
XIP_GetFirmwareVerInfo(
	HANDLE	hXIProperty,
	CHAR * pchHardwareVersion, 
	DWORD * pdwFirmwareVersion,
	BYTE * pbyInstanceID
	);

HRESULT 
LIBXIPROPERTY_API 
XIP_GetFirmwareVerInfoEx(
	HANDLE	hXIProperty,
	CHAR * pchHardwareVersion, 
	DWORD * pdwFirmwareVersion,
	DWORD * pdwDriverVersion,
	BYTE * pbyInstanceID
	);

#define MAX_SERIAL_NO 64

HRESULT 
LIBXIPROPERTY_API 
XIP_GetSerialNo(
	HANDLE	hXIProperty,
	CHAR * pchSerialNo
	);

#define MAX_FEATURE_DATA 8

HRESULT 
LIBXIPROPERTY_API 
XIP_GetFeatureData(
	HANDLE	hXIProperty,
	WORD	wFeatureID,
	BYTE*	pbyFeatureData
	);

HRESULT 
LIBXIPROPERTY_API 
XIP_BackupFirmware(
	HANDLE	hXIProperty,
	LPCTSTR lpszFileName,
	XIP_FIRMWARE_UPDATE_CALLBACK lpfnFirmwareUpdateCallback,
	LPVOID pvParam
	);

HRESULT 
LIBXIPROPERTY_API 
XIP_UpgradeFirmware(
	HANDLE	hXIProperty,
	LPCTSTR lpszFileName,
	XIP_FIRMWARE_UPDATE_CALLBACK lpfnFirmwareUpdateCallback,
	LPVOID pvParam
	);

HRESULT 
LIBXIPROPERTY_API 
XIP_FinishFirmwareOperations(
	HANDLE	hXIProperty,
	BOOL	bAbort
	);

#ifdef __cplusplus
}
#endif

#include "XIPCVBS.h"
#include "XIPHDCapture.h"
