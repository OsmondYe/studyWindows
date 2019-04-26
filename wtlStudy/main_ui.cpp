#include "stdafx.h"
#include "main_ui.h"




class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_WTLSTUDY_DIALOG };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();
		return TRUE;    // let the system set the focus
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}

};

LRESULT OyeMainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::OutputDebugStringW(__FUNCTIONW__ L"\n");
	std::wstring hello(L"LRESULT OyeMainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)");

	CPaintDC dc(this->m_hWnd);
	RECT      rc;
	GetClientRect(&rc);
	dc.DrawTextW(hello.c_str(), hello.size(), &rc,  DT_BOTTOM| DT_SINGLELINE |DT_CENTER);

	return 0;
}

LRESULT OyeMainWindow::OnLBUTTONDOWN(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::OutputDebugString(__FUNCTIONW__ L"\n");
	//CPaintDC dc(this->m_hWnd);
	CClientDC dc(this->m_hWnd);
	//CWindowDC dc(this->m_hWnd);
	CRect      rc;	
	std::wstring hello(L"LRESULT OnDraw(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)");
	GetClientRect(&rc);
	dc.DrawTextW(hello.c_str(), hello.size(), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.TextOutW(0, 0, hello.c_str(), hello.size());

	// Draw a rectangle at (100,100) with dimensions (100,200);
	dc.Rectangle(100, 100, 200, 300);

	// Draw an ellipse inside the previous rectangle.
	dc.Ellipse(200, 300, 600, 400);



	return 0;
}

void OyeMainWindow::OnRButtonDown(UINT nFlags, CPoint point) {
	::OutputDebugString(__FUNCTIONW__ L"\n");
	RedrawWindow();
}

 LRESULT OyeMainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::DestroyWindow(m_hWnd);
	return 0;
}

 LRESULT OyeMainWindow::OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostQuitMessage(0);
	return 0;
}

 LRESULT OyeMainWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

 // derived from CIdleHandler

  BOOL OyeFrameWnd::OnIdle() {
	 ::OutputDebugString(__FUNCTIONW__ L"\n");
	 return false;
 }

 // derived from CMessageFilter

  BOOL OyeFrameWnd::PreTranslateMessage(MSG * pMsg) {
	 ::OutputDebugString(__FUNCTIONW__ L"\n");
	 return false;
 }
