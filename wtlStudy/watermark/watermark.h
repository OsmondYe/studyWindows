#pragma once
#include "stdafx.h"
#include "atlcrack.h"

// stabled
//typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_MAXIMIZE | WS_CLIPSIBLINGS | WS_DISABLED
//	,WS_EX_TRANSPARENT | WS_EX_TOPMOST >OverlayWindowTraits;

#define WM_OVERLAY (WM_USER +1)

typedef CWinTraits
<	WS_POPUP | WS_VISIBLE | WS_DISABLED,
	WS_EX_TOPMOST | WS_EX_LAYERED | 
	WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT
//	| WS_EX_TRANSPARENT 
>OverlayWindowTraits;

class OverlayChildWnd :
	public CWindowImpl<OverlayChildWnd, CWindow, OverlayWindowTraits>
{
public:
	OverlayChildWnd();
	~OverlayChildWnd();

	void UpdateOverlay(HWND target);

private:
	inline bool GetParentClientSize(LPRECT lpRect) {
		return this->GetParent().GetClientRect(lpRect);
	}

	void _DrawOverlay(HDC dc, LPRECT lpRect);


public:	// WM_XXX
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnPaint(CDCHandle);

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		// 屏蔽背景绘制
		return TRUE;
	}

public:
	DECLARE_WND_CLASS_EX(L"NextlabsViewOverlay", 0, (HBRUSH)(COLOR_WINDOW + 1));

	BEGIN_MSG_MAP_EX(OyeClientWindow)
		//MSG_WM_SHOWWINDOW(OnShowWindow)
		/*MSG_WM_ERASEBKGND(OnEraseBkgnd)*/
		//MSG_WM_CREATE(OnCreate)
		//MSG_WM_PAINT(OnPaint)
		//MSG_WM_NCPAINT(OnNcPaint)
		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		END_MSG_MAP()
};

