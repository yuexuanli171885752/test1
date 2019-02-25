#pragma once
#include "resource.h"

#include "cts/cts.h"
#include "utility.h"




// CCTSOSDlg dialog

class CCTSOSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTSOSDlg)

public:
	CCTSOSDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCTSOSDlg();

	CWnd *m_pParentWnd;

	BOOL m_bOSInited[2];
	CString m_strOSFile[2];
	LOGINFO m_OsLogInfo[2];

	BOOL m_bDualCamMode;

	void UpdateUI();
	void LoadINI();
	void SaveINI();

	BOOL InitOS(int DevIdx);
	void OSPrintf(int idx);
	void ShowCCMType(BYTE bRes, BYTE type);

	BOOL OS_SetConfigFromAPI(DWORD devInst);

// Dialog Data
	enum { IDD = IDD_DIALOG_OS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cbo_osfile0;
	CComboBox m_cbo_osfile1;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboOsfile0();
	afx_msg void OnSelchangeComboOsfile1();
	afx_msg void OnBnClickedButtonConfig0();
	afx_msg void OnBnClickedButtonConfig1();
	afx_msg void OnClickedButtonOstest0();
	afx_msg void OnClickedButtonOstest1();
	afx_msg void OnBnClickedButtonDualostest();
	CListCtrl m_list[2];	
	afx_msg void OnBnClickedButtonGetccmtype0();
	afx_msg void OnBnClickedButtonGetccmtype1();
	afx_msg void OnBnClickedButtonOssetconfig0();
	afx_msg void OnBnClickedButtonOssetconfig1();
	afx_msg void OnBnClickedButtonDualossetconfig();
};
