#pragma once


// ShellWindow

class ShellWindow : public CWnd
{
	DECLARE_DYNAMIC(ShellWindow)

public:
	ShellWindow();
	virtual ~ShellWindow();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
};


