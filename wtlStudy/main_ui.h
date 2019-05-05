#pragma once
#include "stdafx.h"
#include "common_ui_components.h"
#include "atlcrack.h"
#include "watermark.h"
#include "oye_cpp.h"

typedef CWinTraits<WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE>	OyeClientWindowTraits;
class OyeClientWindow :
	public CWindowImpl<OyeClientWindow, CWindow, OyeClientWindowTraits>,
	public CPaintBkgnd<OyeClientWindow, RGB(0, 0,255 )>
{
public:
	bool isFullScreen();
	void ToggleFullScreen();


public:// Message

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnLButtonDown(UINT nFlags, CPoint point);

	void OnLButtonUp(UINT nFlags, CPoint point);

	void OnMouseMove(UINT nFlags, CPoint point);

	void OnRButtonDown(UINT nFlags, CPoint point);

public:
	DECLARE_WND_CLASS(L"OyeClientWindow");

	typedef CPaintBkgnd<OyeClientWindow, RGB(0, 0, 255)> CPaintBkgndBase;

	BEGIN_MSG_MAP_EX(OyeClientWindow)
		// wnd common msg
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		// Mouse
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		// keyboard
		//MSG_WM_CHAR(OnChar)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_SYSKEYDOWN(OnSysKeyDown)

		
		CHAIN_MSG_MAP(CPaintBkgndBase)

	END_MSG_MAP()


private:

	void CaptureScreen(bool bvirtualScreen=false);
};



typedef CWinTraits<
	WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS| WS_CLIPCHILDREN,
	WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE 
	>OyeFrameWndTraits;
class OyeFrameWnd : 
	public CFrameWindowImpl<OyeFrameWnd,CWindow, OyeFrameWndTraits>,
	public CUpdateUI<OyeFrameWnd>,
	public CMessageFilter,
	public CIdleHandler,
	public CPaintBkgnd<OyeFrameWnd, RGB(255, 0, 0)>
{
private:
	OyeClientWindow m_Client;
public:
	typedef CPaintBkgnd<OyeFrameWnd, RGB(255, 0, 0)> CPaintBkgndBase;
	typedef CFrameWindowImpl<OyeFrameWnd, CWindow, OyeFrameWndTraits> COyeFrameBase;
	 
	//IDR_MAINFRAME : some id with ICON, Menu, Accelerator
	DECLARE_FRAME_WND_CLASS_EX(L"OyeFrameWnd", IDR_FRAME,
					CS_HREDRAW | CS_HREDRAW,
					(HBRUSH)(COLOR_WINDOW + 1));


	BEGIN_UPDATE_UI_MAP(OyeFrameWnd)
		UPDATE_ELEMENT(ID_FUNCS_FULLSCREEN,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FUNCS_CAPTURESCREEN, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()


	BEGIN_MSG_MAP_EX(OyeFrameWnd)
		MSG_WM_CREATE(OnCreate)
		//MSG_WM_COMMAND()
		//com
		COMMAND_ID_HANDLER_EX(ID_FUNCS_FULLSCREEN,OnScreen)
		COMMAND_ID_HANDLER_EX(ID_FUNCS_CAPTURESCREEN, OnCaptureScreen)
		COMMAND_ID_HANDLER_EX(IDM_FILE_ABOUT, OnAbout)
		CHAIN_MSG_MAP(COyeFrameBase)
		CHAIN_MSG_MAP(CPaintBkgndBase)
		CHAIN_MSG_MAP(CUpdateUI<OyeFrameWnd>)
	END_MSG_MAP()

public:
	// derived from CIdleHandler
	virtual  BOOL OnIdle() override{
		UIUpdateMenuBar();
		return false;
	}
	// derived from CMessageFilter	 
	virtual BOOL PreTranslateMessage(MSG * pMsg) override {
		if (COyeFrameBase::PreTranslateMessage(pMsg)) {
			return true;
		}
		//return m_Client.pre;
		// some times in futrue, give child wnd a chance to handle 
		return false;
	}

	int OnCreate(LPCREATESTRUCT lpCreateStruct);


	void OnAbout(UINT uNotifyCode, int nID, CWindow wndCtl);


	void OnScreen(UINT uNotifyCode, int nID, CWindow wndCtl) {
		DBGFUNC
		m_Client.ToggleFullScreen();
	}

	void OnCaptureScreen(UINT uNotifyCode, int nID, CWindow wndCtl) {
		DBGFUNC
		CaptureWholeScreens();

	}

};






