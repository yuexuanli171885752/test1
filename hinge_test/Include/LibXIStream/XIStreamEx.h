
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

#include "XIStream.h"

#define XI_DESTROY_MODULE(p)		\
	if (p) { p->Destroy(); p = NULL; }

///////////////////////////////////////////////////////////////////
////// Common

class IVoIPSink
{
public:
	virtual void PutFrame(const BYTE * pbFrame, int cbFrame, const BYTE * pbFrameInfo, int cbFrameInfo) = NULL;
};

class IVoIPSource
{
public:
	virtual void GetFrame(BYTE * pbFrame, int *pcbFrame, BYTE * pbFrameInfo, int *pcbFrameInfo) = NULL;
};


class ISinkConnectorModule
{
public:
	static LIBXISTREAM_API BOOL Create(ISinkConnectorModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;

	virtual void AddOutputSink(IVoIPSink * pSink) = NULL;
	virtual BOOL RemoveOutputSink(IVoIPSink * pSink) = NULL;
};

class ISourceConnectorModule
{
public:
	static LIBXISTREAM_API BOOL Create(ISourceConnectorModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual void SetInputSource(IVoIPSource * pSource) = NULL;
	virtual IVoIPSource * GetOutputSource() = NULL;

	virtual void AddOutputSink(IVoIPSink * pSink) = NULL;
	virtual BOOL RemoveOutputSink(IVoIPSink * pSink) = NULL;
};

///////////////////////////////////////////////////////////////////
////// Video IO

class IDShowCaptureEnumModule
{
public:
	static LIBXISTREAM_API BOOL Create(IDShowCaptureEnumModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual int GetCount() = NULL;
	virtual const TCHAR* GetDeviceID(int iIndex) = NULL;
	virtual const TCHAR* GetDeviceName(int iIndex) = NULL;
};

class IDShowCaptureModule
{
public:
	static LIBXISTREAM_API BOOL Create(IDShowCaptureModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual BOOL Open(const TCHAR* pszDevID) = NULL;

	virtual BOOL ShowPropertyDialog(HWND hWndParent) = NULL;

	virtual BOOL TestFormat(XI_COLOR_FORMAT colorFormat, int cx, int cy, int nFrameDuration) = NULL;
	virtual BOOL SetFormat(XI_COLOR_FORMAT colorFormat, int cx, int cy, int nFrameDuration) = NULL;
	virtual BOOL GetFormat(XI_COLOR_FORMAT& colorFormat, int& cx, int& cy, int& nFrameDuration) = NULL;

	virtual BOOL Start() = NULL;
	virtual void Stop() = NULL;
	virtual BOOL GetState(DWORD* pdwFrameCount, DWORD* pdwCaptureByte) = NULL;

	virtual void SetOutputSink(IVoIPSink *pSink) = NULL;

	virtual BOOL SetBuffering(BOOL bBuffering) = NULL;
};


class IDDrawRendererModule
{
public:
	static LIBXISTREAM_API BOOL Create(const GUID *			pGUID,
										HWND hWnd,
										XI_COLOR_FORMAT	colorFormat,
										int		nWidth, 
										int		nHeight, 
										BOOL	bUseOverlay,
										BOOL	bWaitForVerticalBlank,
										IDDrawRendererModule *& pInstance
										);

	static LIBXISTREAM_API BOOL GetMonitorGUIDFromWindow(HWND hWnd, GUID* pGuidMonitor);

public:
	virtual void Destroy() = NULL;

	virtual BOOL SetPosition(LPCRECT lprcSrc, LPCRECT lprcDest) = NULL;
	virtual BOOL Repaint(LPCRECT lprcRepaint = NULL) = NULL;

	virtual BOOL BeginBatchDraw() = NULL;
	virtual BOOL BatchDrawRect(const BYTE *pbImage, int cbStride, 
						  int cxSrc, int cySrc,
						  int xDest, int yDest, 
						  BOOL bSrcTopDown) = NULL;
	virtual BOOL EndBatchDraw(BOOL bPresent) = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;

	virtual BOOL Reset(const GUID *		pGUID,
					   HWND				hWnd,
					   XI_COLOR_FORMAT	colorFormat,
					   int				nWidth, 
					   int				nHeight, 
					   BOOL				bUseOverlay,
					   BOOL				bWaitForVerticalBlank
					   ) = NULL;
};

///////////////////////////////////////////////////////////////////
////// Audio IO

class IDSoundCaptureEnumModule
{
public:
	static LIBXISTREAM_API BOOL Create(IDSoundCaptureEnumModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual LONG GetCount() = NULL;
	virtual const GUID * GetDeviceID(int iIndex) = NULL;
	virtual LPCTSTR GetDeviceName(int iIndex) = NULL;
	virtual int GetMixerID(int iIndex) = NULL;
	virtual LPCTSTR GetDevicePnpID(int iIndex) = NULL;
};

class IDSoundCaptureModule 
{
public:
	static LIBXISTREAM_API BOOL Create(const GUID& guidDevice,
									   IDSoundCaptureModule *& pInstance
									   );

public:
	virtual void Destroy() = NULL;

	virtual BOOL SetFormat(int cSamplesPerSec, int cChannels, int cSamplesPerFrame, int cBufferedFrames) = NULL;

	virtual BOOL Run() = NULL;
	virtual void Stop() = NULL;
	virtual BOOL GetState(DWORD* pdwCaptureByte) = NULL;

	virtual void SetOutputSink(IVoIPSink * pSink) = NULL;
};

class IDSoundRendererEnumModule
{
public:
	static LIBXISTREAM_API BOOL Create(IDSoundRendererEnumModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual LONG GetCount() = NULL;
	virtual const GUID * GetDeviceID(int iIndex) = NULL;
	virtual LPCTSTR GetDeviceName(int iIndex) = NULL;
	virtual int GetMixerID(int iIndex) = NULL;
	virtual LPCTSTR GetDevicePnpID(int iIndex) = NULL;
};

class IDSoundRendererModule
{
public:
	static LIBXISTREAM_API BOOL Create(const GUID& guidDevice, 
							 int cSamplesPerSec, 
							 int cChannels, 
							 int cSamplesPerFrame, 
							 int cBufferedFrames, 
							 IDSoundRendererModule *& pInstance
							 );

public:
	virtual void Destroy() = NULL;

	virtual BOOL Run() = NULL;
	virtual void Stop() = NULL;

	virtual BOOL SetPan(LONG lPan) = NULL;
	virtual BOOL SetVolume(LONG lVolume) = NULL;

	virtual void SetInputSource(IVoIPSource * pSource) = NULL;
};

class IAudioJitterBufferModule
{
public:
	static LIBXISTREAM_API BOOL Create(int cChannels, 
							 int cSamplesPerFrame, 
							 int cMaxBufferedFrames,
							 IAudioJitterBufferModule *& pInstance
							 );

public:
	virtual void Destroy() = NULL;

	virtual void Reset() = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;
	virtual IVoIPSource * GetOutputSource() = NULL;

	virtual BOOL SetDuration(int cFrames) = NULL;
	virtual BOOL SetDuration(int cFrames, int cMinFrames, int cMaxFrames) = NULL;

	virtual int  GetBufferingCount() = NULL;
};

class IBaseMuxerModule
{

public:
	virtual void Destroy() = NULL;

	virtual BOOL AddVideoStream(IVoIPSink ** pStreamSink, const VOID * pvExtraData, int cbExtraData, XI_COLOR_FORMAT xiColor, int cx, int cy, int nFrameDuration) = NULL;
	virtual BOOL AddAudioStream(IVoIPSink ** pStreamSink, int nSamplesPerSec, int nChannels, WORD wBitsPerSample) = NULL;

	virtual BOOL Run() = NULL;
	virtual void Stop() = NULL;
};

class IAVIMuxerModule : public IBaseMuxerModule
{
public:
	static LIBXISTREAM_API BOOL Create(const TCHAR* lpszPath,
									   IAVIMuxerModule *& pInstance
									   );
};

class IColorSpaceConvertorModule
{
public:
	static LIBXISTREAM_API BOOL Create(int cx, int cy, XI_COLOR_FORMAT xiColorSrc, XI_COLOR_FORMAT xiColorDest, BOOL bDestTopDown, IColorSpaceConvertorModule *& pInstance);

public:
	virtual void Destroy() = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;
	virtual void SetOutputSink(IVoIPSink *pSink) = NULL;
};

class IVideoEncoderModule
{
public:
	virtual void Destroy() = NULL;

	virtual DWORD GetFOURCC() = NULL;

	virtual int GetExtraData(LPVOID& pvExtraData) = NULL;
	virtual int GetMaxOutputFrameSize() = NULL;

	virtual void ForceKeyFrame() = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;
	virtual void SetOutputSink(IVoIPSink *pSink) = NULL;
};

class IH264EncoderModule :
	public IVideoEncoderModule
{
public:
	static LIBXISTREAM_API BOOL Create(XI_COLOR_FORMAT colorFormat,
									   const H264_ENCODER_PARAM*	pH264Param,
									   IH264EncoderModule *& pInstance
									   );

	virtual BOOL GetState(DWORD* pdwEncodeFrame, DWORD* pdwEncodeByte, DWORD* pdwEncodeBps, double* pdEncodeFps) = NULL;
};

class IAudioEncoderModule
{
public:
	virtual void Destroy() = NULL;

	virtual int GetSamplesPerFrame() = NULL;
	virtual int GetMaxOutputFrameSize() = NULL;

	virtual IVoIPSink * GetInputSink() = NULL;
	virtual void SetOutputSink(IVoIPSink *pSink) = NULL;

	virtual int GetExtraData(LPVOID& pvExtraData) = NULL;
};

class IAACEncoderModule : public IAudioEncoderModule
{
public:
	static LIBXISTREAM_API BOOL Create(int cSamplesPerSecInput,
							 int cSamplesPerSecOutput,
							 int cChannels, 
							 int nBitRateBps,
							 int cSamplesPerFrame,
							 BOOL bADTS,
							 IAACEncoderModule *& pInstance
							 );
public:
	virtual BOOL Run() = NULL;
	virtual void Stop() = NULL;

	virtual const WAVEFORMATEX * GetWaveFormat() = NULL;

	virtual BOOL GetState(DWORD* pdwEncodeByte, DWORD* pdwEncodeBps) = NULL;
};

class IMP4MuxerModule : public IBaseMuxerModule
{
public:
	static LIBXISTREAM_API BOOL Create(const TCHAR* lpszPath,
									   int nTimeScale,
									   IMP4MuxerModule *& pInstance
									   );
public:
	virtual BOOL AddVideoStream(IVoIPSink ** pStreamSink, const VOID * pvExtraData, int cbExtraData, int cx, int cy, int nFrameDuration, XI_COMPRESSION_LEVEL level,DWORD dwAspectRatioX, DWORD dwAspectRatioY) = NULL;
	virtual BOOL AddAudioStream(IVoIPSink ** pStreamSink, const WAVEFORMATEX * pwfx, int nSamplesPerFrame) = NULL;
};

class IVideoSnapshotModule
{
public:
	static LIBXISTREAM_API BOOL Create(IVideoSnapshotModule*& pInstance);

public:
	virtual void Destroy() = NULL;
	virtual IVoIPSink * GetInputSink() = NULL;

	virtual BOOL Reset(XI_COLOR_FORMAT xiColorSrc, int nWidth, int nHeight) = NULL;
	virtual BOOL BeginCapture(const TCHAR* lpszNameTemplate, int cCount, int nQuality) = NULL;
};