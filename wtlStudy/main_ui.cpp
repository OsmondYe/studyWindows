#include "stdafx.h"
#include "main_ui.h"
#include <atlimage.h>

extern OyeFrameWnd* pMainWnd;


class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_WTLSTUDY_DIALOG };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
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

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

};

void OyeClientWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	::OutputDebugStringW(__FUNCTIONW__ L"\n");

	bool bControl = 0x8000 & GetKeyState(VK_CONTROL);
	bool bShift = 0x8000 & GetKeyState(VK_SHIFT);
	bool bTab  = 0x8000 & GetKeyState(VK_TAB);
	bool bEnter = 0x8000 & GetKeyState(VK_RETURN);
	{
		// output to debug
		RedrawWindow();
		CString str;
		str.Format(L"KeyDown :\n uChar=0x%-6x, nRepCnt=0x%-6x, nFlags=0x%-6x, Control=%d, Shift=%d" \
			"Tab=%d,Enter=%d",
			nChar, nRepCnt, nFlags,bControl, bShift, bTab, bEnter);
		CClientDC dc(m_hWnd);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOutW(0, 0, str, -1);
	}

	switch (nChar)
	{
	case VK_F5:		
		CaptureScreen(bControl ? true :false); //capture screen 
		break;
	case VK_F11:
		ToggleFullScreen();
		break;	
	case VK_ESCAPE:
		RedrawWindow();
		break;
	default:
		break;
	}

	SetMsgHandled(false);
}


void OyeClientWindow::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bControl = 0x8000 & GetKeyState(VK_CONTROL);
	bool bShift = 0x8000 & GetKeyState(VK_SHIFT);
	bool bTab = 0x8000 & GetKeyState(VK_TAB);
	bool bEnter = 0x8000 & GetKeyState(VK_RETURN);
	bool bAlt = (1 << 13)& nFlags;
	{
		// output to debug
		RedrawWindow();
		CString str;
		str.Format(L"SYSKeyDown :\n uChar=0x%-6x, nRepCnt=0x%-6x, nFlags=0x%-6x, Control=%d, Shift=%d" \
			"Tab=%d,Enter=%d,Alt=%d",
			nChar, nRepCnt, nFlags, bControl, bShift, bTab, bEnter, bAlt);
		CClientDC dc(m_hWnd);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOutW(0, 0, str, -1);
	}
	SetMsgHandled(false);
}



LRESULT OyeClientWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::OutputDebugStringW(__FUNCTIONW__ L"\n");
	CPaintDC dc(m_hWnd);

	CRect rc;
	GetClientRect(rc);
	CMemoryDC*  mdc=new CMemoryDC(dc, CRect(0,0,95,95));

	mdc->Draw3dRect(5, 5, 95, 95, RGB(255, 0, 0), RGB(0, 255, 0));

	mdc->TextOutW(0, 0, L"test");


	//dc.BitBlt(0, 0, rc.Width(), rc.Height(), mdc, 0, 0, SRCCOPY);

	CBrush myBrush;
	myBrush.CreatePatternBrush(mdc->m_bmp);

	dc.SelectBrush(myBrush);
	dc.PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);

	dc.Rectangle(10, 0, 110, 110);
	dc.Rectangle(550, 0, 600, 100);
	dc.Rectangle(950, 0, 1000, 100);


	delete mdc;


	return 0;
}

bool m_bTractInfo;

void OyeClientWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	::OutputDebugString(__FUNCTIONW__ L"\n");
	SetCapture();
	m_bTractInfo = true;
}



void OyeClientWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	::OutputDebugString(__FUNCTIONW__ L"\n");
	ReleaseCapture();
	m_bTractInfo = false;
}

void OyeClientWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	::OutputDebugString(__FUNCTIONW__ L"\n");
	if (!m_bTractInfo) {
		return;
	}
	CClientDC dc(m_hWnd);
		
	CSimpleArray< CString> strArray;

	// client mouse point 
	CString str;
	str.Format(L"Mouse Pos cur:\n x=%6d, y=%6d", point.x, point.y);
	strArray.Add(str);
	// screen mouse point
	ClientToScreen(&point);
	str.Format(L"Mouse Pos whole:\n x=%6d, y=%6d", point.x, point.y);
	strArray.Add(str);
	// bitspixel
	str.Format(L"GetDeviceCaps(BITSPIXEL)=%10d", GetDeviceCaps(dc, BITSPIXEL));
	strArray.Add(str);
	// bitmap planes
	str.Format(L"GetDeviceCaps(PLANES)=%10d", GetDeviceCaps(dc, PLANES));
	strArray.Add(str);


	//
	//  Primary Screen
	//

	// screen cx, width of primary display monitor
	str.Format(L"GetSystemMetrics(SM_CXSCREEN)=%10d", GetSystemMetrics( SM_CXSCREEN));
	strArray.Add(str);
	// screen cy, height of primary display monitor
	str.Format(L"GetSystemMetrics(SM_CYSCREEN)=%10d", GetSystemMetrics(SM_CYSCREEN));
	strArray.Add(str);
	// SM_CXFULLSCREEN 
	// The width of the client area for a full-screen window on the primary display monitor
	str.Format(L"GetSystemMetrics(SM_CXFULLSCREEN)=%10d", GetSystemMetrics(SM_CXFULLSCREEN));
	strArray.Add(str);
	// SM_CYFULLSCREEN, The height of the client area for a full-screen window on the primary display monitor,
	str.Format(L"GetSystemMetrics(SM_CYFULLSCREEN)=%10d", GetSystemMetrics(SM_CYFULLSCREEN));
	strArray.Add(str);




	//
	//  Virtual Screen 
	//

	// screen SM_XVIRTUALSCREEN
	str.Format(L"GetSystemMetrics(SM_XVIRTUALSCREEN)=%10d",
		GetSystemMetrics(SM_XVIRTUALSCREEN));
	strArray.Add(str);
	// screen SM_YVIRTUALSCREEN
	str.Format(L"GetSystemMetrics(SM_YVIRTUALSCREEN)=%10d",
		GetSystemMetrics(SM_YVIRTUALSCREEN));
	strArray.Add(str);


	// screen SM_CXVIRTUALSCREEN
	str.Format(L"GetSystemMetrics(SM_CXVIRTUALSCREEN)=%10d", 
		GetSystemMetrics(SM_CXVIRTUALSCREEN));
	strArray.Add(str);
	// screen SM_CYVIRTUALSCREEN
	str.Format(L"GetSystemMetrics(SM_CYVIRTUALSCREEN)=%10d", 
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
	strArray.Add(str);

	//Output
	RedrawWindow();
	dc.SetTextColor(RGB(255, 0, 0));
	dc.SetBkMode(TRANSPARENT);
	for (int i = 0; i < strArray.GetSize(); i++) {
		dc.TextOutW(0, i * 20, strArray[i], -1);
	}
}

void OyeClientWindow::OnRButtonDown(UINT nFlags, CPoint point) {
	::OutputDebugString(__FUNCTIONW__ L"\n");
	RedrawWindow();
}

 void OyeClientWindow::CaptureScreen(bool bvirtualScreen)
 {
	 CRect      rc;
	 GetClientRect(&rc);
	 
	 CClientDC dc(m_hWnd);
	 CDC wdc = ::GetWindowDC(NULL);

	 CRect    srcRc(0, 0,
		 GetSystemMetrics(bvirtualScreen ? SM_CXVIRTUALSCREEN : SM_CXSCREEN),
		 GetSystemMetrics(bvirtualScreen ? SM_CYVIRTUALSCREEN : SM_CYSCREEN)

	 );


	 //dc.BitBlt(0, 0, rc.Width(), rc.Height(), wdc, 0, 0, SRCCOPY);
	 //dc.BitBlt(0, 0, 20, 20, wdc, 0, 0, SRCCOPY);
	 dc.StretchBlt(0, 0, rc.Width(), rc.Height(),
		 wdc,
		 0, 0,
		 GetSystemMetrics(bvirtualScreen?SM_CXVIRTUALSCREEN: SM_CXSCREEN ),
		 GetSystemMetrics(bvirtualScreen?SM_CYVIRTUALSCREEN: SM_CYSCREEN ),
		 SRCCOPY
	 );

	 // save to disk
	 CatureImageBy(wdc, srcRc);
 }



 bool OyeClientWindow::isFullScreen()
 {
	 return !(this->GetStyle() & WS_CHILD);
 }

 void OyeClientWindow::ToggleFullScreen()
 {
	 if (isFullScreen()) {
		 //restore
		 if (pMainWnd != NULL) {

			 ModifyStyle(WS_POPUP, WS_CHILD);
			 ModifyStyleEx(0, WS_EX_CLIENTEDGE);
			 SetParent(pMainWnd->m_hWnd);

			 pMainWnd->FlashWindow(true);
			 pMainWnd->SetFocus();
		}
	 }
	 else {
		 // in to full screen mode
		 ModifyStyle(WS_CHILD, WS_POPUP);
		 ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
		 SetParent(NULL);
		 MoveWindow(GetVirtualScreenRect());
		 FlashWindow(true);
		 SetFocus();
	 }
	
 }

 void OyeClientWindow::DrawSth(CString str)
 {	 
	 
	 static auto chooseColour = []()->COLORREF {
		 static COLORREF c = 0;
		 static COLORREF f = RGB(255, 255, 255);
		 COLORREF rt = c++;
		 if (c == f) {
			 c=0;
		 }		 
		 return rt;
	 };

	 
	 this->RedrawWindow();

	 	 
	 CClientDC dc(m_hWnd);
	 CRect rc;
	 GetClientRect(rc);
	 CMemoryDC mdc(dc, rc);
	 mdc.BitBlt(0, 0, rc.Width(), rc.Height(), dc, 0, 0, PATCOPY);

	 //mdc.BitBlt(0, 0, rc.Width(), rc.Height(), dc, 0, 0, SRCCOPY);
	 
	 mdc.SetBkMode(TRANSPARENT);
	 for(int col=0;col<10;col++)
	 for (int i = 0; i < 30; i++ ) {
		CString s = str;
		COLORREF color = chooseColour();
		mdc.SetTextColor(color);
		s.AppendFormat(L"0x%06X", color);
		mdc.TextOutW(col*120, i*20,  s);
	 } 
 }

 void OyeClientWindow::DrawColourRect()
 {
	 CClientDC dc(m_hWnd);
	 CRect rc;
	 GetClientRect(rc);

	 CMemoryDC mdc(dc, rc);

	 RGBEnum color;


	 int delta = 1;
	 const int count = 4;
	 int cur = 0;

		 for (size_t j = 0; j < rc.Height(); j+= delta)
	 {
	 for (size_t i = 0; i < rc.Width(); i+= delta)
		 {
			 //dc.SetDCPenColor(color);
			 //dc.SetDCBrushColor(color);			 			 
			 //dc.FillSolidRect(i, j, i + delta, j + delta, color);
		 if (cur++ == 4) {
			 cur = 0;
			 color++;
		 }
		 mdc.SetPixel(j,i, color.base);
			 //color++;
			 //color =RGB(rand()%255, rand() % 255, rand() % 255);
			 //dc.Rectangle);
			 //dc.TextOutW(i, j, L"w", -1);
		 }
	 }
 }

 


 //
 //  for oye Frame work
 //

 void OyeFrameWnd::OnAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
 {
	 CAboutDlg dlg;
	 dlg.DoModal();
 }

 void OyeFrameWnd::OnClipboard(UINT uNotifyCode, int nID, CWindow wndCtl)
 {
	 switch (nID)
	 {
	 case ID_CLIPBOARD_COPY:
		 //::MessageBox(m_hWnd, L"ID_CLIPBOARD_COPY", L"", MB_OK);
		 break;
	 case ID_CLIPBOARD_CUT:
		 //::MessageBox(m_hWnd, L"ID_CLIPBOARD_CUT", L"", MB_OK);
		 break;
	 case ID_CLIPBOARD_PASTE:
		 OnPaste();
		 return;
	 default:
		 break;
	 }
	 //
	 CString str = L"this is a test";
	 int len = (str.GetLength() + 1) * 2;
	 HGLOBAL h = ::GlobalAlloc(GPTR, len);
	 ::memcpy(h, str, len);	 
	 ::OpenClipboard(m_hWnd);
	 ::EmptyClipboard();
	 ::SetClipboardData(CF_UNICODETEXT, h);
	 ::CloseClipboard();
		 
 }

 void OyeFrameWnd::OnPaste()
 {
	 if (!::OpenClipboard(m_hWnd)) {
		 return;
	 }
	 HANDLE h = ::GetClipboardData(CF_UNICODETEXT);
	 if (NULL == h) {
		 return;
	 }
	 int size=::GlobalSize(h);
	 CString s((wchar_t*)h);
	 ::CloseClipboard();

	 ::MessageBox(m_hWnd, s, s, 0);

 }
 
 void OyeFrameWnd::OnColourful(UINT uNotifyCode, int nID, CWindow wndCtl)
 {
	 static bool bset = false;
	 if (!bset) {
		 bset = true;
		 SetTimer(1, 50);
	 }
	 else {
		 bset = false;
		 KillTimer(1);
	 }
 }

 void OyeFrameWnd::OnDrawColourRect(UINT uNotifyCode, int nID, CWindow wndCtl)
 {
	 m_Client.DrawColourRect();
 }

 void OyeFrameWnd::OnTimer(UINT_PTR nIDEvent)
 {
	 if (nIDEvent == 1) {
		 m_Client.DrawSth("c:");
	 }
 }

 void OyeFrameWnd::OnScramble(UINT uNotifyCode, int nID, CWindow wndCtl)
 {
	 // 利用内存dc当做暂存区域，然后把屏幕分成100份，随机打乱
	 const int NUM = 300;

	 HWND hscreen= GetDesktopWindow();
	 if (!::LockWindowUpdate(hscreen)) {
		 return;
	 }


	 CDC dc = ::GetDCEx(hscreen, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
	 auto cx = GetSystemMetrics(SM_CXSCREEN) / 10;
	 auto cy = GetSystemMetrics(SM_CYSCREEN) / 10;

	 CRect rc(0, 0, cx, cy);

	 CMemoryDC mdc(dc, rc);
	 int iKeep[NUM][4];
	 int i, j, x1, y1, x2, y2;
	 for (i = 0; i < 2; i++)
		 for (j = 0; j < NUM; j++)
		 {
			 if (i == 0)
			 {
				 iKeep[j][0] = x1 = cx * (rand() % 10);
				 iKeep[j][1] = y1 = cy * (rand() % 10);
				 iKeep[j][2] = x2 = cx * (rand() % 10);
				 iKeep[j][3] = y2 = cy * (rand() % 10);
			 }
			 else
			 {
				 x1 = iKeep[NUM - 1 - j][0];
				 y1 = iKeep[NUM - 1 - j][1];
				 x2 = iKeep[NUM - 1 - j][2];
				 y2 = iKeep[NUM - 1 - j][3];
			 }
			 BitBlt(mdc, 0, 0, cx, cy, dc, x1, y1, SRCCOPY);
			 BitBlt(dc, x1, y1, cx, cy, dc, x2, y2, SRCCOPY);
			 BitBlt(dc, x2, y2, cx, cy, mdc, 0, 0, SRCCOPY);

			 Sleep(1);
		 }
	LockWindowUpdate(NULL);



 }



 int OyeFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	 ::OutputDebugString(__FUNCTIONW__ L"\n");

	 //
	 m_hAccel = WTL::AtlLoadAccelerators(IDR_FRAME);
	 // client
	 m_hWndClient = m_Client.Create(m_hWnd);

	 // statusbar	
	 CreateSimpleStatusBar();
	 m_StatusBar.Attach(m_hWndStatusBar);
	 int barCX[3]={ 100,200,-1 };
	 m_StatusBar.SetParts(3, barCX);
	 m_StatusBar.SetText(0, L"status bar 0");
	 m_StatusBar.SetText(1, L"status bar 1");
	 m_StatusBar.SetText(2, L"status bar 2");	 

	 //must Register msg-filtering and idle
	 CMessageLoop* pLoop = _Module.GetMessageLoop();
	 pLoop->AddMessageFilter(this);
	 pLoop->AddIdleHandler(this);


	 UIEnable(ID_FUNCS_FULLSCREEN, true);
	 UIEnable(ID_FUNCS_CAPTURESCREEN, true);

	/* BITMAP bp = { 1,2,3,4,5,6,(void*)0x123456789abcdef };
	 
	 int a = sizeof(BITMAP);
	 int b = sizeof(DIBSECTION);*/

	 //CImageList

	 //::GetWindowLongW(NULL,GWLP_HWNDPARENT)

	 return 0;
 }
