
#ifndef __DXMEDIACAP_2011__
#define __DXMEDIACAP_2011__

#include "datastru.h"

/*
������
	��ʼ�� SDK �������ڵ�����������֮ǰ����
������
	��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXInitialize();
void __stdcall DXUninitialize();

/*
������
	��ȡ��ʼ���ɹ����豸����
������
	��
����ֵ��
	ƥ����豸����
// */
unsigned __stdcall DXGetDeviceCount();

/*
������
	ö������Ƶ���������
������
	devTags - [out] ���ڷ��ػ�õĹ������� TAG ������
	num - [in/out] ָ�� devTags �����Ԫ�ظ���������ʱ����ʵ���ϻ�õĹ������� TAG �ĸ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnumVideoCodecs(PDEVICE_TAG devTags, unsigned& num);
unsigned __stdcall DXEnumAudioCodecs(PDEVICE_TAG devTags, unsigned& num);

/*
������
	ö����Ƶ��׽�豸������
������
	devTags - [out] ���ڷ��ػ�õĹ������� TAG ������
	num - [in/out] ָ�� devTags �����Ԫ�ظ���������ʱ����ʵ���ϻ�õĹ������� TAG �ĸ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnumVideoDevices(PDEVICE_TAG devTags, unsigned& num);

/*
������
	ö����Ƶ��׽����Ⱦ�豸������
������
	devTags - [out] ���ڷ��ػ�õĹ������� TAG ������
	num - [in/out] ָ�� devTags �����Ԫ�ظ���������ʱ����ʵ���ϻ�õĹ������� TAG �ĸ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnumAudioDevices(PDEVICE_TAG devTags, unsigned& num);
unsigned __stdcall DXEnumSoundDevices(PDEVICE_TAG devTags, unsigned& num);

/*
������
	ռ�òɼ��豸
������
	idx - [in] �豸��ö�ٹ����е����
	err - [out] ���ش������
����ֵ��
	�ɹ��򷵻��豸��������򷵻� NULL����ͨ�� err ��ô������
// */
device_handle __stdcall DXOpenDevice(unsigned idx, unsigned* err = NULL);
void __stdcall DXCloseDevice(device_handle device);

/*
������
	������ҳ��ʾ���ƶ������ָ������
������
	device - [in] �豸���
	attrID - [in] �������
	hOwner - [in] ������ľ����NULL - ��������ҳ
	rect - [in] �ڸ���������ʾ������NULL - ��ʾ������������ͻ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXShowDeviceAttr(device_handle device, unsigned attrID, HWND hOwner, RECT* rect);

/*
������
	��ȡ��Ƶ����
������
	device - [in] �豸���
	standard - [out] ��Ƶ��ʽ
	colorspace - [out] ɫ�ʿռ�
	width - [out] ��Ƶ��ȣ���λ�����أ�
	height - [out] ��Ƶ�߶ȣ���λ�����أ�
	framerate - [out] ��Ƶ֡�ʣ���λ��֡/�룩
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetVideoPara(device_handle device, unsigned& standard, unsigned& colorspace,
								  unsigned& width, unsigned& height, float& framerate);

/*
������
	ö����Ƶ�����С
������
	device - [in] �豸���
	devTags - [out] ���ڷ��ػ�õĹ������� TAG ������
	num - [in/out] ָ�� vidCaps �����Ԫ�ظ���������ʱ����ʵ���ϻ�õ������ʽ Caps �ĸ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnumVideoCaps(device_handle device, PVIDEOCAPS vidCaps, unsigned& num);

/*
������
	������Ƶ����
������
	device - [in] �豸���
	standard - [in] ��Ƶ��ʽ
	colorspace - [in] ɫ�ʿռ�
	width - [in] ��Ƶ��ȣ���λ�����أ�
	height - [in] ��Ƶ�߶ȣ���λ�����أ�
	framerate - [in] ��Ƶ֡�ʣ���λ��֡/�룩
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetVideoPara(device_handle device, unsigned standard, unsigned colorspace,
								  unsigned width, unsigned height, float framerate);

/*
������
	��ȡ��ʾ���Ե�ȡֵ��Χ��Ĭ��ֵ����������־
������
	device - [in] �豸���
	paraType - [in] ��������
					0 - ���ȣ�1 - �Աȶȣ�2 - ���Ͷȣ�3 - ɫ�ȣ�4 - ���
	pMin - [in] ��Сֵ
	pMax - [in] ���ֵ
	pSteppingDelta - [in] ����
	pDefault - [in] Ĭ��ֵ
	pCapsFlags - [in] �Զ�/�ֶ���־
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetDisplayParaRange(device_handle device, unsigned paraType,
										 long* pMin, long* pMax, long* pSteppingDelta, long* pDefault, long* pCapsFlags);

/*
������
	��ȡ��ʾ����
������
	device - [in] �豸���
	paraType - [in] ��������
					0 - ���ȣ�1 - �Աȶȣ�2 - ���Ͷȣ�3 - ɫ�ȣ�4 - ���
	value - [out] ����ֵ
	flag - [out] �Զ�/�ֶ���־
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetDisplayPara(device_handle device, unsigned paraType, long& value, long& flags);

/*
������
	������ʾ����
������
	device - [in] �豸���
	paraType - [in] ��������
					0 - ���ȣ�1 - �Աȶȣ�2 - ���Ͷȣ�3 - ɫ�ȣ�4 - ���
	value - [in] ����ֵ
	flag - [in] �Զ�/�ֶ���־
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetDisplayPara(device_handle device, unsigned paraType, long value, long flags);

/*
������
	��ȡ�豸��ǰѡ��Ķ��ӺͰ������������
������
	device - [in] �豸���
	curSource - [out] ��ǰѡ���������ӵ���š�NULL - ����
	sources - [out] ������������ӵ��������飨���磺S-VIDEO��DV...����NULL - ����
	num - [in/out] ����������NULL - ���ԣ���ʱ���ɻ�ȡ��ǰѡ����������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetVideoSources(device_handle device, unsigned* curSource, unsigned* sources = NULL, unsigned char* num = NULL);

/*
������
	�����豸���������
������
	device - [in] �豸���
	source - [in] ��ǰѡ���������ӵ����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetVideoSource(device_handle device, unsigned source);

/*
������
	��ȡ�豸���ź�״̬
������
	device - [in] �豸���
	signal - [out] �ź�״̬��0 - �źŶ�ʧ��1 - �ź�����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetSignalPresent(device_handle device, unsigned& signal);

/*
������
	�����豸����״̬
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXDeviceRun(device_handle device);
unsigned __stdcall DXDevicePause(device_handle device);
unsigned __stdcall DXDeviceStop(device_handle device);

/*
������
	��ȡ�豸����״̬
������
	device - [in] �豸���
	state - [out] �豸����״̬
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetDeviceState(device_handle device, unsigned& state);

/*
������
	����Ƶ��ʾ��ָ������ľ���������
������
	device - [in] �豸���
	hWnd - [in] ��ʾ��Ƶ�Ĵ�������NULL - ֹͣ��ʾ��Ƶ
	rect - [in] ��Ƶ��ʾ�ľ�������NULL - ռ����������
	vvmrtype - [in] ��Ƶ��ʾģʽ��0ΪOVERLAY��ʾ��1Ϊ������ʾ,2ΪVMR9��ʾģʽ
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStartPreview(device_handle device, HWND hWnd, PRECT rect, unsigned vmrtype);

/*
������
	ֹͣԤ��
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStopPreview(device_handle device);

/*
������
	������Ƶ��ʾ����
������
	device - [in] �豸���
	bFreeze - [in] TRUE - ������ʾ��FALSE - �ⶳ��ʾ
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXFreezePreview(device_handle device, BOOL bFreeze);

/*
������
	����豸�Ƿ������Ƶ
������
	device - [in] �豸���
	bHasAudio - [out] TRUE - ������Ƶ��FALSE - û����Ƶ
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXHasAudio(device_handle device, BOOL& bHasAudio);

/*
������
	Ϊ�豸������Ƶ�ɼ��豸��
		ע�⣺!!! ֻ���� state_stopped ״̬�µ���
������
	device - [in] �豸���
	bSound - [in] TRUE - ��ʼ���������FALSE - ��ֹ�������
	audioDevice - [in] ��Ƶ�ɼ��豸��NULL - ʹ���豸�Դ�����Ƶ
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetAudioDevice(device_handle device, PDEVICE_TAG audioDevice = NULL);

/*
������
	��ʼ������ֹ���������
		ע�⣺!!! ֻ�п�ʼ����������������������������ڡ�¼��ʱ������Ƶ�Ȳ������ܳɹ�
������
	device - [in] �豸���
	bSound - [in] TRUE - ��ʼ���������FALSE - ��ֹ�������
	soundDevice - [in] ��������豸��NULL - ʹ��Ĭ���豸
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetSoundOut(device_handle device, BOOL bSound/*, PDEVICE_TAG soundDevice = NULL*/);

/*
������
	��������
������
	device - [in] �豸���
	bMute - [in] TRUE - ������FALSE - �Ǿ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش�����룬һ��ʧ��ԭ������Ϊû�п�ʼ�������
// */
unsigned __stdcall DXEnableMute(device_handle device, BOOL bMute);

/*
������
	����������ƽ��
������
	device - [in] �豸���
	volume - [in] ������С��ȡֵ��Χ(0, 100]
	balance - [in] ����ƽ�⣬ȡֵ��Χ[-10, 10]��������ʾ������ǿ����������������ǿ��0 - ����ƽ��
				ע�⣺!!! NULL - ��ʾ������ƽ��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش�����룬һ��ʧ��ԭ������Ϊû�п�ʼ�������
// */
unsigned __stdcall DXSetAudioVolume(device_handle device, unsigned char volume, unsigned char* const balance = NULL);

/*
������
	��ȡ������ƽ��
������
	device - [in] �豸���
	volume - [out] ������С��ȡֵ��Χ(0, 100]
				ע�⣺!!! NULL - ��ʾ����ȡ����
	balance - [out] ����ƽ�⣬ȡֵ��Χ[-10, 10]��������ʾ������ǿ����������������ǿ��0 - ����ƽ��
				ע�⣺!!! NULL - ��ʾ����ȡƽ��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش�����룬һ��ʧ��ԭ������Ϊû�п�ʼ�������
// */
unsigned __stdcall DXGetAudioVolume(device_handle device, unsigned char* volume, unsigned char* balance = NULL);

/*
������
	������Ƶ������
		ע�⣺!!! �����ڽ���¼��ʱ�������ʧ��
������
	device - [in] �豸���
	videoEncoder - [in] ��Ƶ�������� TAG��NULL - ��ʾ���ñ��룬��¼��ʱ����ԭʼ����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetVideoCodec(device_handle device, PDEVICE_TAG videoEncoder);

/*
������
	������Ƶ�������ľ������ԣ��ı���������͵�ʱ������ô˺�������ΪĬ��ֵ�ᵼ��¼��������
������
	device - [in] �豸���
	codecType--[in]������������,��:codec_x264
	pPara - [in]�������ľ������ԣ�codec_x264,codec_xvid����������Ŀǰ����VidCodecX264Para����NULL - ��ʾʹ��Ĭ������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetVideoCodecPara(device_handle device, unsigned codecType, void* pPara);
/*
������
	��ȡ��Ƶ�������ľ�������
������
	device - [in] �豸���
	codecType--[out]������������,��:codec_x264
	pPara - [out]�������ľ�������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetVideoCodecPara(device_handle device, unsigned& codecType, void* pPara);

/*
������
	������Ƶ������
		ע�⣺!!! �����ڽ���¼��ʱ�������ʧ��
������
	device - [in] �豸���
	audioEncoder - [in] ��Ƶ�������� TAG��NULL - ��ʾ���ñ��룬��¼��ʱ����ԭʼ����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetAudioCodec(device_handle device, PDEVICE_TAG audioEncoder);

/*
������
	��ʼ¼��
������
	device - [in] �豸���
	szFileName - [in] ָ��¼���ļ�ȫ·��(x264����Ϊmp4��ʽ��)��NULL - ֹͣ¼��
	saveAudio - [in] �Ƿ���ƵҲ¼�Ƶ��ļ���
	timeSize - [in] �޶�¼���ļ���ʱ�䳤�ȣ���λ���룩��NULL - ������
	dataSize - [in] �޶�¼���ļ������ݳ��ȣ���λ���ֽڣ���NULL - ������
	saveNotify - [in] ¼���¼�֪ͨ�ӿ�ָ��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStartCapture(device_handle device, TCHAR* szFileName, BOOL saveAudio,
										  unsigned* timeSize = NULL, unsigned* dataSize = NULL,
										  ISaveNotify* saveNotify = NULL);

/*
������
	ֹͣ¼��
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStopCapture(device_handle device);

/*
������
	����¼�񲿷֣�������Ƶ����Ƶ��
	ע�⣺!!! ��������Ҫ���ʱ��¼������
������
	device - [in] �豸���
	bFreeze - [in] TRUE - ����¼��FALSE - �ⶳ¼��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXFreezeCaputre(device_handle device, BOOL bFreeze);

/*
������
	������ƵЧ�������ԭʼ��Ƶ���ݣ�
	������
	device - [in] �豸���
	ispTask - [in] �²������ƵЧ���Ľӿ�ָ��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXAttachIspTask(device_handle device, IispTask* ispTask);
unsigned __stdcall DXDetachIspTask(device_handle device, IispTask* ispTask);

/*
������
	������Ƶ���ݻص��ĺ���ԭ��
������
	fourcc - [in] ��ȡ�ı������ݵı�������FOURCC
	buffer - [in] ������ָ��
	bufferSize - [out] ��ȡ�ı������ݵĻ�������С����λ���ֽڣ�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
typedef unsigned (__stdcall *fnEncVideoCallback)(unsigned fourcc, unsigned char* buffer, 
	unsigned bufferSize, void* context);

/*
������
	���ñ�����Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetEncVideoCallback(device_handle device, fnEncVideoCallback fn, void* context);

/*
������
	��ʼ������Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStartEncVideoCallback(device_handle device, fnEncVideoCallback fn, void* context);

/*
������
	ֹͣ������Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStopEncVideoCallback(device_handle device);

/*
������
	ʹ��ȥ���й���
������
	device - [in] �豸���
	deinterlace - [in] ȥ���л�������     0 -- ����Ӧ���д���
	                                      1 -- SimpleBob
//                                        2 -- TomsMoComp
//                                        3 -- YADIF
//                                        ����ֵ��ʾ�Զ�ȡ�����й���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnableDeinterlace(device_handle device, unsigned deinterlace);

/*
������
	ʹ��ȥ�빦��
������
	device - [in] �豸���
	denoise - [in] �����׼ (5 - 100)������ֵ��ʾ�Զ�ȡ��ȥ�빦��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnableDenoise(device_handle device, unsigned denoise = 35);

/*
������
	�񻯴�����
������
	device - [in] �豸���
	deSharpness - [in] �񻯱�׼(0 - 255)������ֵ��ʾ�Զ�ȡ���񻯹���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnableSharpen(device_handle device, unsigned deSharpness = 128);

/*
������
	ʹ��α�ʹ���
������
	device - [in] �豸���
	szPColorTemplateFile - [in] α�ʵ�ģ���ļ���NULL - ֹͣʹ��α��
	pcolorDepth - [in] α�ʵ�ɫ��˲���������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnablePColor(device_handle device, TCHAR* szPColorTemplateFile, unsigned pcolorDepth);

unsigned __stdcall DXConvertYUVtoRGB(device_handle device, void* pYUVBuf,
							  void* pRGBBuf,
							  long lImgWidth, 
							  long lImgHeight,
							  BOOL bInverted,
                              BOOL bInvertColor);

/*
������
	��Ƶԭʼ���ݻص��ĺ���ԭ��
������
	buffer - [in] ������ָ��
	bufferSize - [out] ��ȡ�ı������ݵĻ�������С����λ���ֽڣ�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
typedef unsigned (__stdcall *fnAudDataCallback)(unsigned char* buffer, unsigned bufferSize, void* context);

/*
������
	������Ƶԭʼ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetAudDataCallback(device_handle device, fnAudDataCallback fn, void* context);

/*
������
	��ʼ��Ƶԭʼ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStartAudDataCallback(device_handle device, fnAudDataCallback fn, void* context);

/*
������
	ֹͣ��Ƶԭʼ���ݻص��ĺ���
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStopAudDataCallback(device_handle device);

/*
������
	����ʱ��OSD�����ĺ���
������
	device - [in] �豸���
	x,y - [in]ʱ��OSD��λ������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetTimeOSD
(
	/*in*/ device_handle device, 
	/*in*/ int x, 
	/*in*/ int y,  
	/*in*/ int pointSize,
	/*in*/ LPCTSTR faceName = TEXT("����"),
	/*in*/ COLORREF color = RGB(0,0,255),
	/*in*/ COLORREF bgcolor = RGB(0, 0, 0),
	/*in*/ BOOL transparent = TRUE
);

/*
������
	�����ı�OSD�����ĺ���
������
	device - [in] �豸���
	x,y - [in]ʱ��OSD��λ������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */							
unsigned __stdcall DXSetTextOSD
(
	/*in*/ device_handle device, 
	/*in*/ int x, 
	/*in*/ int y,
	/*in*/ int TextNO,
	/*in*/ LPCTSTR osdText,
	/*in*/ int pointSize,
	/*in*/ LPCTSTR faceName = TEXT("����"),
	/*in*/ COLORREF color  = RGB(0,0,255),
	/*in*/ COLORREF bgcolor = RGB(0, 0, 0),
	/*in*/ BOOL transparent = TRUE 
);

/*
������
	����ͼƬOSD�����ĺ���
������
	device - [in] �豸���
	x,y - [in]ʱ��OSD��λ������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetPictureOSD
(
	/*in*/ device_handle device, 
	/*in*/ int x, 
	/*in*/ int y,
	/*in*/ int PicNO,
	/*in*/ LPCTSTR picFileName,
	/*in*/ BOOL transparent = TRUE,
	/*in*/ unsigned char alpha = 255
);

/*
������
	����ͼƬOSD�����ĺ���
������
	device - [in] �豸���
	osdType - [in]��������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnOSDDisp
(
	/*in*/ device_handle device, 
	/*in*/ unsigned osdType,       //0��ʾʱ����ӣ�1��ʾ�ı����ӣ�2��ʾͼƬ����
	/*in*/ int number,				/* ���ӱ�ţ���number = -1ʱΪȫ�� */
	/*in*/ BOOL enable
);

/*
������
	�Ӽ���оƬ�û�����ȡ���ݵĺ���
������
	device - [in] �豸���
	chPassWord - [in]����
	chData------[out]��ȡ���ݿռ�
	chLen-------[out]��ȡ���ݳ���,������0x20
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXReadDeviceSerial(device_handle device,unsigned char * chPassWord,
						 unsigned char * chData,  unsigned char& chLen);

//�Ӽ���оƬ�û�����������
/*
������
	�����оƬ�û���д�����ݵĺ���
������
	device - [in] �豸���
	chPassWord - [in]����
	chData------[in]д�����ݿռ�
	chLen-------[in]д�����ݳ���,������0x20
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXWriteDeviceSerial(device_handle device,unsigned char * chPassWord,
						 unsigned char * chData,  unsigned char chLen);

/*
������
	ԭʼ��Ƶ���ݻص��ĺ���ԭ��
������
	buffer - [in] ������ָ��
	colorSpace - [in] ��ȡ��ԭʼ���ݵ�ɫ�ʿռ�����
	width - [in] ��ȡ��ԭʼ���ݵ����ؿ��
	height - [in] ��ȡ��ԭʼ���ݵ����ظ߶�
	bytesWidth - [in] ��ȡ��ԭʼ���ݵ��ֽڿ��
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
typedef unsigned (__stdcall *fnRawVideoCallback)(unsigned char* buffer, unsigned colorSpace,
									   unsigned width, unsigned height, unsigned bytesWidth, void* context);

/*
������
	����ԭʼ��Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSetRawVideoCallback(device_handle device, fnRawVideoCallback fn, void* context);

/*
������
	��ʼԭʼ��Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
	fn - [in] �ص�����ָ�룬NULL - ֹͣ�ص�
	context - [in] �ص�������������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStartRawVideoCallback(device_handle device, fnRawVideoCallback fn, void* context);

/*
������
	ֹͣԭʼ��Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXStopRawVideoCallback(device_handle device);

/*
������
	��ͣԭʼ��Ƶ���ݻص��ĺ���
������
	device - [in] �豸���
	bPause---[in] true:��ͣԭʼ���ص�;false:���¿�ʼԭʼ���ص�
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXPauseRawVideoCallback(device_handle device, bool bPause);

/*
������
	��ȡָ�������ԭʼ��Ƶ֡���ݵ�������
������
	device - [in] �豸���
	buffer - [in] ָ�򻺳�����NULL - ��������������Ӧ�����ݣ����磬���軺�����Ĵ�С����ɫ�ռ䡢��Ƶ�ߴ�
	bufferLen - [in] ָ��Ļ������Ĵ�С����λ���ֽڣ�
	gotBufferLen - [out] ʵ�ʻ�ȡ�����ݵĴ�С����λ���ֽڣ�
	colorSpace - [out] ��ȡ��ԭʼ���ݵ�ɫ�ʿռ�����
	width - [out] ��ȡ��ԭʼ���ݵ����ؿ��
	height - [out] ��ȡ��ԭʼ���ݵ����ظ߶�
	bytesWidth - [out] ��ȡ��ԭʼ���ݵ��ֽڿ��
	rect - [in] ָ����ȡ���ݵ�����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXGetFrameBuffer(device_handle device, unsigned char* buffer, unsigned bufferLen, unsigned* gotBufferLen = NULL,
									unsigned* colorSpace = NULL, unsigned* width = NULL, unsigned* height = NULL, unsigned* bytesWidth = NULL,
									PRECT rect = NULL);

/*
������
	����ԭʼ��Ƶ֡���ݵ� BMP�ļ�
������
	szFileName - [in] BMP�ļ�·��
	buffer - [in] ָ�򻺳���
	bufferLen - [in] ָ��Ļ������Ĵ�С����λ���ֽڣ�
	colorSpace - [in] ��ȡ��ԭʼ���ݵ�ɫ�ʿռ�����
	width - [in] ��ȡ��ԭʼ���ݵ����ؿ��
	height - [in] ��ȡ��ԭʼ���ݵ����ظ߶�
	bytesWidth - [in] ��ȡ��ԭʼ���ݵ��ֽڿ��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSaveBMPFile(TCHAR* szFileName, unsigned char* buffer, unsigned bufferLen,
								 unsigned colorSpace, unsigned width, unsigned height, unsigned bytesWidth);

/*
������
	����ԭʼ��Ƶ֡���ݵ� JPG�ļ�
������
	szFileName - [in] JPG�ļ�·��
	buffer - [in] ָ�򻺳���
	bufferLen - [in] ָ��Ļ������Ĵ�С����λ���ֽڣ�
	colorSpace - [in] ��ȡ��ԭʼ���ݵ�ɫ�ʿռ�����
	width - [in] ��ȡ��ԭʼ���ݵ����ؿ��
	height - [in] ��ȡ��ԭʼ���ݵ����ظ߶�
	bytesWidth - [in] ��ȡ��ԭʼ���ݵ��ֽڿ��
	quality - [in] JPG�ļ��Ļ�������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSaveJPGFile(TCHAR* szFileName, unsigned char* buffer, unsigned bufferLen,
								 unsigned colorSpace, unsigned width, unsigned height, unsigned bytesWidth, unsigned quality);

/*
������
	ֱ��ץȡԭʼ��Ƶ֡���ݵ� BMP�ļ�
������
	device - [in] �豸���
	szFileName - [in] BMP�ļ�·��
	rect - [in] ָ����ȡ���ݵ�����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSnapToBMPFile(device_handle device, TCHAR* szFileName, PRECT rect = NULL);

/*
������
	ֱ��ץȡԭʼ��Ƶ֡���ݵ� BMP�ļ�
������
	device - [in] �豸���
	szFileName - [in] BMP�ļ�·��
	buffer - [in] ָ�򻺳���
	bufferLen - [in] ָ��Ļ������Ĵ�С����λ���ֽڣ�
	colorSpace - [in] ��ȡ��ԭʼ���ݵ�ɫ�ʿռ�����
	width - [in] ��ȡ��ԭʼ���ݵ����ؿ��
	height - [in] ��ȡ��ԭʼ���ݵ����ظ߶�
	bytesWidth - [in] ��ȡ��ԭʼ���ݵ��ֽڿ��
	rect - [in] ָ����ȡ���ݵ�����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXSnapToJPGFile(device_handle device, TCHAR* szFileName, unsigned quality, PRECT rect = NULL);

/*
������
	��ת��Ƶ
������
	device - [in] �豸���
	flip - [in] TRUE - ���÷�ת��FALSE - ���÷�ת
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXFlipVideo(device_handle device, BOOL flip);

/*
������
	������Ƶ
������
	device - [in] �豸���
	mirror - [in] TRUE - ���þ���FALSE - ���þ���
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXMirrorVideo(device_handle device, BOOL mirror);

/*
������
	��ͼƬ���崴��Ԥ������
������
	imgbuffer - [in] ͼƬ������
	buffersize - [in] ͼƬ�������Ĵ�С����λ���ֽڣ�
	colorspace - [in] ͼƬ��������ݵ�ɫ�ʿռ�
	width - [in] ͼƬ�Ŀ�ȣ���λ�����أ�
	height - [in] ͼƬ�ĸ߶ȣ���λ�����أ�
	err - [out] ���ش������
����ֵ��
	�ɹ��򷵻�ͼ���������򷵻� NULL����ͨ�� err ��ô������
// */
image_handle __stdcall DXCreateBufferImg(unsigned char* imgbuffer, unsigned buffersize,
										  unsigned colorspace, unsigned width, unsigned height,
										  unsigned* err);

/*
������
	��ͼƬ�ļ�����Ԥ������
������
	szImgFile - [in] ͼƬ�ļ�ȫ·��
	err - [out] ���ش������
����ֵ��
	�ɹ��򷵻�ͼ���������򷵻� NULL����ͨ�� err ��ô������
// */
image_handle __stdcall DXCreateFileImg(TCHAR* szImgFile, unsigned* err);

/*
������
	�ɸ���Ԥ������ָ���Ĵ������Ӧ����
������
	img - [in] Ԥ������
	hPrevWnd - [in] Ԥ���Ĵ���
	rect - [in] Ԥ��������
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXUpdateImg(image_handle img, HWND hPrevWnd, RECT* rect);

/*
������
	ɾ��Ԥ������
������
	img - [in] Ԥ������
����ֵ��
	��
// */
void __stdcall DXDestroyImg(image_handle img);

/*
������
	�����������������ֵ
������
	device - [in] �豸���
	RateType--[in]�������Ե�����,��:drate_color
	pPara - [in]�������ľ�������ֵ��NULL - ��ʾֹͣ���������Ķ�Ӧ����
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXEnDataRaterPara(device_handle device, unsigned RateType, void* pPara);

/*
������
	�����������������ֵ
������
	src - [in] ԭʼ��
	dst--[out]�����
	pixelsPerLines - [in]ÿһ�е���������
	width - [in]��
	height - [in]��
����ֵ��
	0 - �ɹ���ʧ���򷵻ش������
// */
unsigned __stdcall DXYuy2Gray(unsigned char *src, unsigned char *dst, unsigned pixelsPerLines, unsigned width, unsigned height);

#endif // __DXMEDIACAP_2011__
