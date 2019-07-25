#pragma once
#include "stdafx.h"
#include "atlcrack.h"

typedef 
CWinTraits<	WS_POPUP | WS_VISIBLE | WS_DISABLED,
			WS_EX_TOPMOST | WS_EX_LAYERED |	WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT
		  >OverlayWindowTraits;

/*
	with WS_EX_LAYERED
	with WS_EX_TOOLWINDOW, floating and convering the target windows,

	Notice:
		- using controller to change this wnd's size and make it fined convered with target wnd
		- size of tar_wnd changing, changing this simultaneously
			   
*/
class OverlayWindow : public CWindowImpl<OverlayWindow, CWindow, OverlayWindowTraits>
{
	friend class ViewOverlyController;
public:
	OverlayWindow(){}
	~OverlayWindow() {
		if (IsWindow()) {
			HideWnd();
			this->DestroyWindow();
		}
		
	};
private:
	inline void SetOverlay(const std::wstring& ol) { strOverlay = ol; }
	inline void Init(const std::wstring& ol) {
		SetOverlay(ol);
		Create(NULL);
		_PrepareOverly(); }
	void UpdateOverlay(HWND target);
	inline void HideWnd() { this->ShowWindow(SW_HIDE); }
	CMemoryDC* pmdc;
	void _DrawOverlay(HDC dc, LPRECT lpRect);
	void _PrepareOverly();

public: // section for wnd_reg and msg_handler
	DECLARE_WND_CLASS_EX(L"NextlabsViewOverlay", 0, (HBRUSH)(COLOR_WINDOW + 1));
	// don't need handle msg, using win_default_msg_handler is enough
	BEGIN_MSG_MAP_EX(OverlayWindow)
	END_MSG_MAP()
private:
	std::wstring strOverlay;
};



/*
Notice:
	callback can't be used in std::bind, create a internal global var instead

*/
class ViewOverlyController {
	HHOOK _swhHook;
	// targetDocWnd -> OverlayWnd
	std::map<HWND, std::shared_ptr<OverlayWindow> > _wnds;


public:
	static ViewOverlyController& getInstance();

private:
	ViewOverlyController():_swhHook(NULL) {};
	~ViewOverlyController();

public:
	void Attach(HWND target, const std::wstring& overlay, int tid = 0);
	void Detach(HWND target);
	void Clear();	// clear all watermarks

	void SetOverlyTarget(HWND target);
	inline void SetOverlyTargetOnTopLevel(const char* wndClassName, const char* caption = NULL) {
		HWND target = ::FindWindowA(wndClassName, caption);
		if (target == NULL) {
			return;
		}
		SetOverlyTarget(target);
	}
	/*void UpdateOverlayText(const wchar_t* text) { _overlay->SetOverlay(text); }
	
	}*/
	void UpdateWatermark(HWND target);

private:
	LRESULT OnMessageHook(int code, WPARAM wParam, LPARAM lParam);

private:
	static ViewOverlyController* sgIns;
	static std::recursive_mutex sgRMutex;
	static LRESULT CALLBACK HookProxy(int code, WPARAM wParam, LPARAM lParam);
};





/*
For print overlay, it's easy, only to hook well-known print api

*/
void DrawPrintWatermark_Test(HDC hdc, std::wstring overlay);