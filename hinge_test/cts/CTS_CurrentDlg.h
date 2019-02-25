#pragma once

#include "resource.h"
// CCTS_CurrentDlg dialog
#include "utility.h"

#define TIMEEVENT_DYNAMICCURRENT 1
#define TIMEEVENT_STANDBYCURRENT 2


#define CURRENT_MIDDLERANGE_MAX 	8160
#define CURRENT_SMALLRANGE_MAX 	163.2

#define CURRENT_LARGERANGE 	0
#define CURRENT_MIDDLERANGE 	1
#define CURRENT_SMALLRANGE 	2
#define CURRENTRANGENUM 		3

#define CURRENT_CHANGERANGE_DELAY 0
#define CURRENT_READCONTINUE_DELAY 1



typedef struct _CURRENT
{	
	BOOL bTestMode; // 0: dynamic current, 1: standby current

	BOOL bCur[PWRNUM];
	float fCurLowSpec[PWRNUM];	 // uA	
	float fCurHighSpec[PWRNUM]; // uA			
	
	BYTE bValid[PWRNUM];
	float fCur[PWRNUM];	 // uA	

	// standby current
	BOOL bPWDN;
	BOOL bReset;
	BOOL bMCLKOff;
	BOOL bVCMPWDN;

	BOOL bHadGonetoStandbyMode;
	float fCurStandbySpec[PWRNUM];	 // uA	
	
	
	BOOL bCurResult[PWRNUM];	// 0: NG, 1: PASS, 2: overflow
	BOOL bCurrentTotalResult; // 0: NG, 1: PASS

		
}CURRENT;



class CCTS_CurrentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTS_CurrentDlg)

public:
	CCTS_CurrentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCTS_CurrentDlg();

	int m_DevIdx;
	CURRENT m_current;

	BOOL m_bDynamicCurrentTest;
	BOOL m_bStandbyCurrentTest;
	
	void LoadINI();
	void SaveINI();

	void GetCurrentParameter();
	void SetCurrentParameter();
	
	void UpdateResult();
	void EnableSpecUI(BOOL bEn);

	BOOL CalcDynamicCurrent(CURRENT &current);
	BOOL CalcStandbyCurrent(CURRENT &current);

	BOOL SetVCMDW9718SPWDN();
	
// Dialog Data
	enum { IDD = IDD_DIALOG_CURRENT };

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonDynamiccurrent();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_static_result[PWRNUM];
	CStatic m_static_resultall;
	CEdit m_edt_pwr[PWRNUM];
	afx_msg void OnBnClickedButtonStandbycurrenttest();
};
