#pragma once
#include "stdafx.h"
#include "common_ui_components.h"

#include "atlcrack.h"

#include "watermark/watermark.h"

//typedef CWinTraits<WS_CHILD | WS_VISIBLE,
//					WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_ACCEPTFILES>									OyeMainWindowTraits;
typedef CWinTraits<WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE>	OyeMainWindowTraits;
class OyeMainWindow :
	public CWindowImpl<OyeMainWindow, CWindow, OyeMainWindowTraits>,
	public CPaintBkgnd<OyeMainWindow, RGB(0, 0,255 )>
{

public:
	DECLARE_WND_CLASS(L"OyeClientWindow");

	typedef CPaintBkgnd<OyeMainWindow, RGB(0, 0, 255)> CPaintBkgndBase;

	BEGIN_MSG_MAP(OyeMainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBUTTONDOWN)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_CHAR(OnChar)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)


		COMMAND_ID_HANDLER(IDM_FILE_ABOUT, OnAbout)
		CHAIN_MSG_MAP(CPaintBkgndBase)

	END_MSG_MAP()


	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	void OnChar(TCHAR chChar, UINT nRepCnt, UINT nFlags) {
		RedrawWindow();
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLBUTTONDOWN(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnRButtonDown(UINT nFlags, CPoint point);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};



typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS| WS_CLIPCHILDREN,
	WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE >
	OyeFrameWndTraits;
class OyeFrameWnd : 
	public CFrameWindowImpl<OyeFrameWnd,CWindow, OyeFrameWndTraits>,
	public CIdleHandler,
	public CMessageFilter,
	public CUpdateUI<OyeFrameWnd>,
	public CPaintBkgnd<OyeFrameWnd, RGB(255, 0, 0)>
{
private:
	OyeMainWindow client;
public:
	typedef CPaintBkgnd<OyeFrameWnd, RGB(255, 0, 0)> CPaintBkgndBase;
	typedef CFrameWindowImpl<OyeFrameWnd, CWindow, OyeFrameWndTraits> COyeFrameBase;
	 
	//IDR_MAINFRAME : some id with ICON, Menu, Accelerator
	DECLARE_FRAME_WND_CLASS_EX(L"OyeFrameWnd", IDR_FRAME, 
						CS_HREDRAW | CS_HREDRAW,
						(HBRUSH)(COLOR_WINDOW + 1))

	BEGIN_UPDATE_UI_MAP(OyeFrameWnd)
	END_UPDATE_UI_MAP()


	BEGIN_MSG_MAP(OyeFrameWnd)
		MSG_WM_CREATE(OnCreate)
		CHAIN_MSG_MAP(COyeFrameBase)
		CHAIN_MSG_MAP(CPaintBkgndBase)
	END_MSG_MAP()

public:
	// derived from CIdleHandler
	virtual  BOOL OnIdle() override;
	// derived from CMessageFilter
	virtual BOOL PreTranslateMessage(MSG* pMsg)override;
	int OnCreate(LPCREATESTRUCT lpCreateStruct) {
		::OutputDebugString(__FUNCTIONW__ L"\n");
			   
		m_hWndClient=client.Create(m_hWnd);
		return 0;
	}	
};






