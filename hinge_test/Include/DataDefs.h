
#pragma once


#if (defined( _WIN32 ) || defined ( _WIN64 )) && !defined (__GNUC__)
	#define __INT64   __int64
	#define __UINT64  unsigned __int64
#else
	#define __INT64   long long
	#define __UINT64  unsigned long long
#endif


typedef enum FRAME_TYPE
{
	UNKNOWN_FRAME,
	AUDIO_FRAME,
	VIDEO_FRAME
};

typedef struct _RAWVIDEO_FRAME_INFO
{
	int		cbStride;
	BOOL	bTopDown;
	RECT	rectUpdate;
} RAWVIDEO_FRAME_INFO, *PRAWVIDEO_FRAME_INFO;

typedef struct _ENCODED_FRAME_INFO
{
	BOOL		bKeyFrame;
	__UINT64	lPts;
	__UINT64	lDts;
	FRAME_TYPE	type;
} ENCODED_FRAME_INFO, *PENCODED_FRAME_INFO;

/*
typedef struct _XI_H264_ENCODER_DATA {
	BOOL		bKeyFrame;
	__UINT64	u64TimeStamp;
	__UINT64	u64DecodeTimeStamp;
	FRAME_TYPE	type; // to compatible with ENCODED_FRAME_INFO
} XI_H264_ENCODER_DATA, *PXI_H264_ENCODER_DATA;
*/