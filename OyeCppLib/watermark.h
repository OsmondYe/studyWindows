#pragma once
#include "stdafx.h"
#include "atlcrack.h"

typedef CWinTraits<
	WS_POPUP | WS_VISIBLE | WS_DISABLED,
	WS_EX_TOPMOST | WS_EX_LAYERED |	WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT
>OverlayWindowTraits;

class OverlayWnd :
	public CWindowImpl<OverlayWnd, CWindow, OverlayWindowTraits>
{
	friend class ViewOverlyController;
	OverlayWnd();
	~OverlayWnd();
	void UpdateOverlay(HWND target);

private:
	CMemoryDC* pmdc;
	void _DrawOverlay(HDC dc, LPRECT lpRect);
	void _PrepareOverly();
public:
	DECLARE_WND_CLASS_EX(L"NextlabsViewOverlay", 0, (HBRUSH)(COLOR_WINDOW + 1));
	BEGIN_MSG_MAP_EX(OyeClientWindow)		
	END_MSG_MAP()
};

// callback can not be used in std::bind, have to create a internal global var
class ViewOverlyController {
	OverlayWnd* _overlay;
	HHOOK _swhHook;
	CWindow _target;
	CWindow _targetTopMain;
public:
	ViewOverlyController();
	~ViewOverlyController();

public:
	void SetOverlyTarget(HWND target);
	void SetOverlyTargetOnTopLevel(
		const char* wndClassName, const char* caption=NULL);
	void UpdateOverlayText(wchar_t* text) {	 /*tbd*/ }

private:
	LRESULT OnMessageHook(int code, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK HookProxy(int code, WPARAM wParam, LPARAM lParam);
};



void DrawPrintWatermark_Test(HDC hdc);