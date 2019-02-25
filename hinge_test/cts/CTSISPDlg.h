#pragma once
#include "resource.h"
#include "cts/cts.h"
#include "utility.h"

// CCTSISPDlg dialog

class CCTSISPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTSISPDlg)

public:
	CCTSISPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCTSISPDlg();

	CWnd *m_pParent;

	int m_DevIdx;
	BOOL m_bDoTogether;

	BOOL m_bAWBON;
	BOOL m_bAEON;
	BOOL m_bLSCON;

	BYTE DoAWBOnSensor(DWORD devInst, tagAWBREG AWBReg, BOOL bEn, BOOL bKeepWBAfterAWBOFF, int nWidth, int nHeight, float fRoiRatio);
	BYTE DoAWBOnCTS(DWORD devInst, BOOL bEn, BOOL bKeepWBAfterAWBOFF, int nWidth, int nHeight, float fRoiRatio);
	BYTE DoAE(int nDevID, tagREG reg[], int Ytarget, int nWidth, int nHeight, float fRoiRatio);

// Dialog Data
	enum { IDD = IDD_DIALOG_ISP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnAwb();
	afx_msg void OnBnClickedBtnAe();
	afx_msg void OnBnClickedButtonLsccalibration();
	afx_msg void OnBnClickedButtonLsc();
	afx_msg void OnBnClickedButtonLscload();
	afx_msg void OnBnClickedButtonLscsave();
	virtual BOOL OnInitDialog();
};
