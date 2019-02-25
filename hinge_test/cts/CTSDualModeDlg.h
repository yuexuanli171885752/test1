#pragma once

#include "resource.h"
#include "cts/cts.h"
#include "utility.h"

// CCTSDualModeDlg dialog

class CCTSDualModeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTSDualModeDlg)

public:
	CCTSDualModeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCTSDualModeDlg();

	CWnd *m_pParent;

// Dialog Data
	enum { IDD = IDD_DIALOG_DUALMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_cbo_devdual[2];
	afx_msg void OnBnClickedButtonEnforcedualmode();
	virtual BOOL OnInitDialog();
};
