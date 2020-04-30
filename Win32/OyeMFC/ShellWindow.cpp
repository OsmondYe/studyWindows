// ShellWindow.cpp : implementation file
//

#include "pch.h"
#include "OyeMFC.h"
#include "ShellWindow.h"


// ShellWindow

IMPLEMENT_DYNAMIC(ShellWindow, CWnd)

ShellWindow::ShellWindow()
{

}

ShellWindow::~ShellWindow()
{
}


BEGIN_MESSAGE_MAP(ShellWindow, CWnd)
END_MESSAGE_MAP()



// ShellWindow message handlers




BOOL ShellWindow::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: Add your specialized code here and/or call the base class
	CString strClassName;
	if (lpszClassName == NULL)
	{
		strClassName = GetGlobalData()->RegisterWindowClass(_T("Oye:ShellWindow"));
	}
	else {
		strClassName = lpszClassName;
	}
	
	dwStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CWnd::CreateEx(dwExStyle, strClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}
