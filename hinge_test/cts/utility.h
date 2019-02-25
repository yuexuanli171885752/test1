
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "cts\cts.h"

typedef struct _SRCFILE
{
    WIN32_FIND_DATA fd;
    bool bIsNew;
}SRCFILE;


#define ROIRATIONUM 12
extern float g_fROIRatio[ROIRATIONUM];


CString GetSensorConfigString(CString strSection, CString strKey, CString strFile, CString strDefault = _T(""));
int SetSensorConfigString(CString strSection, CString strKey, CString strData, CString strFile);
int GetSensorConfigInt(CString strSection, CString strKey, CString strFile, CString strDefault = _T("0"));
int SetSensorConfigInt(CString strSection, CString strKey, int data, CString strFile);
float GetSensorConfigFloat(CString strSection, CString strKey, CString strFile, CString strDefault = _T("0"));
int SetSensorConfigFloat(CString strSection, CString strKey, float data, CString strFile);
bool ParseRegisters(vector<tagREG> &vecReg, CString section, CString strFile);

bool IsDirectoryExists(CString const& path);

int GetSensorConfigToComboBox(CComboBox* pbox);
int GetOpenShortConfigToComboBox(CComboBox* pbox);
void FindFilesInDir(TCHAR* rootDir,  SRCFILE srcFiles[], bool bInit = 0);

CString GetAppPath();
CString GetConfigFile();
CString GetSensorConfigPath();
CString GetOpenShortConfigPath();
CString GetcapturedImagesPath();

void SaveAsBMP(char *fname, UINT8 *imgbuf, const INT32 width, const INT32 height, unsigned char bitcount, bool binvert);

void DrawResult(CStatic *pstatic, BOOL bResult);
void DrawImage(CWnd *pwnd, BYTE *buffer, int width, int height, int bpp);

void VideoClear(CWnd *pwnd);

BOOL SaveBIN(CString strFilePath, BYTE* byteBuffer, long size);
BOOL SaveRawImage(CString strFilePath, int nWidth, int nHeight, BYTE* byteBuffer);
BOOL SaveJPG(unsigned char *pbuf, int w, int h, int bpp, CString path);

char *CStringToChar(CString str);
void MultiByteToWideCharEx(CString &str, char *cstr);

BOOL TransformRAW10toRaw8(BYTE *pbufout, BYTE *pbufin, int width, int height);
BOOL TransformRAW12toRaw8(BYTE *pbufout, BYTE *pbufin, int width, int height);

BOOL TransformMIPIRAW10toRaw8(BYTE *pbufout, BYTE *pbufin, int width, int height, long bufsize);
BOOL TransformMIPIRAW10toRaw10(unsigned short *pbufout, BYTE *pbufin, int width, int height, long bufsize);

BYTE GetI2CMode(BYTE byAddrWidth, BYTE byDataWidth);

void ListLog(CString strFile, CString str);
void MessageDelay(DWORD time);
#endif	//__UTILITY_H__