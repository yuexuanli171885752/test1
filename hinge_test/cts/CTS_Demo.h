
// CTS_Demo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCTS_DemoApp:
// See CTS_Demo.cpp for the implementation of this class
//

class CCTS_DemoApp : public CWinApp
{
public:
	CCTS_DemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCTS_DemoApp theApp;