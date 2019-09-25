
// studyMFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CApp:
// See studyMFC.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	CApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	ULONG_PTR gGidplusToken;
	Gdiplus::GdiplusStartupInput gGdipulsInput;
};

extern CApp theApp;
