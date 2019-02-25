
// CTS_DemoDlg.h : header file
//

#pragma once

#include "cts/cts.h"
#include "cts/ctsvcm.h"
#include "cts/ctsalgo.h"

#include "utility.h"
#include "dtparse.h"

#include "CTSOSDlg.h"
#include "CTSAFDlg.h"
#include "CTS_CurrentDlg.h"
#include "CTSISPDlg.h"
#include "CTSDualModeDlg.h"


#define TIMEEVENT_I2CTEST		1
#define TIMEEVENT_PREVIEWTEST	2
#define TIMEEVENT_CURRENTTEST	3
#define TIMEEVENT_DUALGRABTEST 4
#define TIMEEVENT_DOAWB		5
#define TIMEEVENT_DOAE			6
#define TIMEEVENT_FPS				7

#define WM_INITDEV (WM_USER + 1000)
#define WM_UPDATEFPS (WM_USER + 1001)


typedef struct
{
	BOOL bDevExist;
	BOOL bInited;
	BOOL bOnRunning;

	DEVINFO devinfo;

	BOOL bDualCam;

	//i2c
	BYTE byI2CID;
	BYTE byAddrWidth;
	BYTE byDataWidth;
	
	int nImageWidth;
	int nImageHeight;

	BYTE *pBuffer;
	BYTE *pCaptureBuf;
	BYTE *pTripleBuffer;
	
	long lBufferSize;

	BOOL bTripleBufferDirty;

	int nExpInit;

	BOOL bCloseRender;

	CString strSensorFile;

	tagREG ExpReg[REGMAX];
	tagREG GainReg[REGMAX];
	tagAWBREG AWBReg;

}tagCTSDEV;


// CCTS_DemoDlg dialog
class CCTS_DemoDlg : public CDialogEx
{
// Construction
public:
	CCTS_DemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CCTS_DemoDlg();

	BOOL	m_bI2CTest;
	int		m_nI2CCount;

	BOOL	m_bPreviewTest;
	int		m_nPreviewCount;
	BOOL	m_bPreviewStatus;

	// dual grab test
	BOOL 	m_bDualGrabTest;
	int		m_nDualGrabCount;
	BOOL 	m_bGrabNoDraw;

	BOOL m_bDualCamMode;

	cdtparse dtparse;

	BOOL m_bThreadGrabFrame;
	BOOL m_bThreadGrabFrameDone;
	HANDLE m_hGrabFrameThreadEventDead;

	BOOL m_bThreadGrabFrame2;
	BOOL m_bThreadGrabFrameDone2;
	HANDLE m_hGrabFrameThreadEventDead2;
			

	CString m_strINIFile[2];
	CString m_strHWSN[2];

	CCTSOSDlg *m_pOsDlg;
	CCTSAFDlg *m_pAfDlg;
	CCTS_CurrentDlg *m_pCurrentDlg;
	CCTSISPDlg *m_pISPDlg;
	CCTSDualModeDlg *m_pDualModeDlg;

	CRITICAL_SECTION m_csLock;

	void LoadINI();
	void SaveINI();
	void KillFPSTimer();
	void SetFPSTimer();
	void UpdateFPS(int DevIdx);

	void UpdateUI();
	BOOL InitCTS();
	void UnInitCTS();

	void LocalLock();
	void LocalUnLock();

	BOOL UpdateAfterEnumCTS(BYTE bRes);

	BYTE EnumCTS();
	void UnEnumCTS();

	BOOL UpdateDeviceStatusWhenDeviceRemove(DWORD nHWSN);
	BOOL UpdateDeviceStatusWhenDeviceInsert(DWORD nHWSN);

	static BOOL CTS_CallBackDeviceChange(BYTE byDeviceChangeMode, DWORD nDevID, DWORD nHWSN);

	BOOL InitDev(int DevIdx);
	BOOL UnInitDev(int DevIdx);
	
	BOOL StartPreview(int DevIdx);
	void StopPreview(int DevIdx);

	BOOL StartPlay(int DevIdx);
	void StopPlay(int DevIdx);

	void Scaling(int DevIdx);
	BOOL Display(int mode);

	void CloseRender(int DevIdx);


	void CreateGrabFrameThread(int DevIdx);
	void CloseGrabFrameThread(int DevIdx);
	BOOL GrabFrame(int DevIdx);
	BOOL TripleBufferFlip(int DevIdx, LPBYTE *pBuf, BOOL bPrimaryBuffer);

	
	
	DWORD Cam_SetConfigExFromAPI(DWORD devInst, char* strPath);
	DWORD Cam_SetConfigExFromAPIDT(DWORD devInst, char* strPath);

	BOOL WaitForFirstFrameArrived(DWORD devInst);
	
// Dialog Data
	enum { IDD = IDD_CTS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButtonEnforcedualmode();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonI2ctest();
	afx_msg void OnBnClickedButtoninitdev();
	afx_msg void OnBnClickedButtoninitdev0();
	afx_msg void OnBnClickedButtoninitdev1();
	afx_msg void OnBnClickedButtonplay0();
	afx_msg void OnBnClickedButtonstop0();
	afx_msg void OnBnClickedButtonplay1();
	afx_msg void OnBnClickedButtonstop1();
	afx_msg void OnBnClickedButtonuninitdev0();
	afx_msg void OnBnClickedButtonuninitdev1();
	CComboBox m_cbo_dev;
		
	afx_msg void OnBnClickedButtonPreviewtest();	
	CComboBox m_cbo_scaling;
	afx_msg void OnCbnSelchangeComboScaling();
	afx_msg void OnBnClickedButtoninitdualcam();
	afx_msg void OnBnClickedButtonuninitdualcam();
	afx_msg void OnBnClickedButtonplaydualcam();
	afx_msg void OnBnClickedButtonstopdualcam();
	afx_msg void OnBnClickedButtongrabdualframe();
	afx_msg void OnBnClickedButtongrableftframe();
	afx_msg void OnBnClickedButtongrabrightframe();
	afx_msg void OnCbnSelchangeComboDisplayformat();
	CStatic m_static_workingmode;
	CComboBox m_cbo_displayformat;
	CComboBox m_cbo_sensorconfig0;
	CComboBox m_cbo_sensorconfig1;
	afx_msg void OnSelchangeComboSensorconfig0();
	afx_msg void OnSelchangeComboSensorconfig1();
	CButton m_chk_dotogether;
	afx_msg void OnBnClickedButtonDualgrabtest();
	afx_msg void OnBnClickedButtonOs();
	afx_msg void OnBnClickedButtonAf();	
	afx_msg void OnBnClickedButtonGrabaframe();
	afx_msg void OnBnClickedButtonCurrent();
	CStatic m_static_fps[2];	
	afx_msg void OnBnClickedCheckCloserender0();
	afx_msg void OnBnClickedCheckCloserender1();
	afx_msg void OnClickedButtonEepromread();
	afx_msg void OnClickedButtonEepromwrite();
	afx_msg void OnBnClickedCheckPreviewonly();
	afx_msg void OnBnClickedButtonIsp();
	CButton m_btn_autorecovermode;
};
