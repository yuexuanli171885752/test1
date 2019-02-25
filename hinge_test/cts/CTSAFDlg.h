#pragma once
#include "resource.h"

#include "cts/cts.h"
#include "utility.h"

#define TIMEEVENT_DOAF			1



//--------------------------------------
// MTF
//--------------------------------------
#define MTF_NORMAL_NUM				5
#define MTF_BLOCK_NUM				13
#define MTF_RATIO_NUM				2
#define MTF_MAXVALUE				100//1000
#define MTF_CENTERBLOCK_INDEX		2

#define MTFAF_BLOCK_NUM	5
#define VCM_TOTAL_STEP		1024 * 2

#define TESTSTATUS_BEGIN		0
#define TESTSTATUS_AFWAIT		1
#define TESTSTATUS_AE			2
#define TESTSTATUS_ADJUSTVCM	3
#define TESTSTATUS_TOCALC		4
#define TESTSTATUS_CALCED		5
#define TESTSTATUS_END			6


typedef struct _MTFAF
{	
	BOOL bAFON;
	
	BYTE m_nDataFormat;
	BYTE m_nBayerOrder;

	int m_nWidth;
	int m_nHeight;

	int nTestStatus;
	
	float nMtfValue;	

	//for mtfaf
	float nVCMMinSpec;
	float nVCMMaxSpec;
	float nVCMStepSpec;
	float nVCMStepMinSpec;

	
	int nVCMMin;
	int nVCMMax;
	int nVCMStep;
	bool bVCMLastCycle;

	float nVCMWeightMTFValue[VCM_TOTAL_STEP];

	int nVCMStepValue[VCM_TOTAL_STEP];
	int nVCMStepCount;
	int nVCMStepIdx;
	float nVCMMTFAFMaxValue;
	int nVCMStepIdxForMTFMax;

	//BYTE *pImgBuf;

} MTFAF;



// CCTSAFDlg dialog

class CCTSAFDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTSAFDlg)

public:
	CCTSAFDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCTSAFDlg();

	void LoadINI();
	void SaveINI();

	int m_DevIdx;
	BOOL SetAF(int DevIdx, WORD value);


	bool InitAF(int DevIdx, MTFAF &mtfaf, BOOL mode = 0);
	void TestToNext(MTFAF &mtfaf);
	int CalcMTFOfROI(MTFAF &mtfaf, unsigned char *pImgbuf, int w, int h, int roi_x, int roi_y, int roi_w, int roi_h, unsigned char bpp, unsigned char nBayerOrder);

	BYTE DoAF(int DevIdx, MTFAF &mtfaf);

	BOOL m_bDualAF;	
	MTFAF m_mtfaf[2];

	RECT m_ROI;


	float ratio;
	
	unsigned int m_nICNum;
	vector<CString> m_vecICList;

	CString m_strVCMIC[2];

	BOOL GetVCMICList();	
	BOOL VCMInit(int DevIdx);
	BOOL VCMPWDN(int DevIdx);

	BOOL WriteVCM(int DevIdx, WORD value);
	BOOL ReadVCM(int DevIdx, WORD &value);

// Dialog Data
	enum { IDD = IDD_DIALOG_AF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	CComboBox m_cbo_vcm[2];
	afx_msg void OnSelchangeComboVcmic0();
	afx_msg void OnSelchangeComboVcmic1();
	afx_msg void OnBnClickedButtonAf0();
	afx_msg void OnBnClickedButtonAf1();
	afx_msg void OnBnClickedButtonDualaf();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboRoi();
};
