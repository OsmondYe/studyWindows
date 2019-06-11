#include "stdafx.h"
#include "watermark.h"
#include <functional>
#include <TlHelp32.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

namespace {

	ULONG_PTR gGidplusToken;
	Gdiplus::GdiplusStartupInput gGdipulsInput;

	// calculate the size art-text used
	SizeF GetTextBounds(const Font& font, const StringFormat& strFormat, const CString& szText)
	{
		GraphicsPath graphicsPathObj;
		FontFamily fontfamily;
		font.GetFamily(&fontfamily);
		graphicsPathObj.AddString(szText, -1, 
									&fontfamily, 
									font.GetStyle(), 
									font.GetSize(), 
									PointF(0, 0), &strFormat);
		RectF rcBound;
		graphicsPathObj.GetBounds(&rcBound);
		return SizeF(rcBound.Width, rcBound.Height);
	}

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

#ifndef MAXULONGLONG
#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#endif
	
	DWORD GetMainThreadID() {
		DWORD dwProcID = ::GetCurrentProcessId();
		DWORD dwMainThreadID = 0;
		ULONGLONG ullMinCreateTime = MAXULONGLONG;

		HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap != INVALID_HANDLE_VALUE) {
			THREADENTRY32 th32;
			th32.dwSize = sizeof(THREADENTRY32);
			BOOL bOK = TRUE;
			for (bOK = Thread32First(hThreadSnap, &th32); bOK;
				bOK = Thread32Next(hThreadSnap, &th32)) {
				if (th32.th32OwnerProcessID == dwProcID) {
					HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,
						TRUE, th32.th32ThreadID);
					if (hThread) {
						FILETIME afTimes[4] = { 0 };
						if (GetThreadTimes(hThread,
							&afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3])) {
							ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime,
								afTimes[0].dwHighDateTime);
							if (ullTest && ullTest < ullMinCreateTime) {
								ullMinCreateTime = ullTest;
								dwMainThreadID = th32.th32ThreadID; // let it be main... :)
							}
						}
						CloseHandle(hThread);
					}
				}
			}
			CloseHandle(hThreadSnap);
		}
		return dwMainThreadID;
	}
}

OverlayWnd::OverlayWnd(const std::wstring& overlay): strOverlay(overlay)
{
	Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);
	_PrepareOverly();	
}

OverlayWnd::~OverlayWnd()
{
	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
	}
}

CRect OldTarget(-1, -1, -1, -1);
void OverlayWnd::UpdateOverlay(HWND target)
{
	
	CRect targetRC;
	if (target == NULL) {
		targetRC = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else {
		::GetWindowRect(target, &targetRC);
	}

	if (OldTarget.EqualRect(targetRC)) {
		return;
	}
	else {
		OldTarget = targetRC;
	}

	// make layered wnd always covered the targert Wnd
	MoveWindow(targetRC);

	BLENDFUNCTION blend = { AC_SRC_OVER ,0,100,AC_SRC_ALPHA };
	//CPoint p(targetRC.left, targetRC.right);
	CPoint p(0, 0);
	CPoint dstpt(targetRC.left, targetRC.top);
	CSize s(targetRC.Width(), targetRC.Height());
	
	// draw in Screen, but always get target wnd's region info
	::UpdateLayeredWindow(*this, 
		NULL, 
		&dstpt, &s, *pmdc,
		&dstpt, NULL,
		&blend, ULW_ALPHA);
}


void OverlayWnd::_DrawOverlay(HDC dcScreen, LPRECT lpRestrictDrawingRect)
{
	if (dcScreen == NULL) {
		return;
	}

	CString str(strOverlay.c_str());
	if (str.IsEmpty()) {
		str = L"Nextlabs SkyDRM Overlay(NULL)";
	}
	CRect rc(lpRestrictDrawingRect);

	// using gdi+
	{
		Gdiplus::Graphics g(dcScreen);
		g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		// config res
		Gdiplus::Pen myPen(Gdiplus::Color(255, 100, 255, 100),10);
		Gdiplus::FontFamily fontfamily(L"Arial");
		Gdiplus::Font myFont(&fontfamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::StringFormat format;
		Gdiplus::SolidBrush brush(Gdiplus::Color(150, 255, 0, 255));
			
		// how to play, can be used only one
		Gdiplus::SizeF surface(rc.Width(),rc.Height());
		Gdiplus::SizeF text = GetTextBounds(myFont, format, str);
		text.Width+= 60; 
		text.Height+= 120;
		// draw
		g.RotateTransform(-5);
		g.TranslateTransform(rc.left, rc.top);
		for (int i = 0; i < surface.Width; i += text.Width) {
			for (int j = 0; j < surface.Height; j += text.Height) {
				g.DrawString(str, -1, &myFont, 
					PointF(i, j), &format, &brush);
			}
		}
	}

}

void OverlayWnd::_PrepareOverly()
{
	// Get Whole Screen pixels
	CRect ScreenRC = { 
		GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN) };

	// Get a larget surface to draw overlay
	CDC dc=::GetDC(NULL);
	pmdc = new CMemoryDC(dc, ScreenRC);
	_DrawOverlay(*pmdc, ScreenRC);
}


//
//  classController
//
//  using ::SetWindowsHookEx to monitor window sizing msg
//
static ViewOverlyController* pVOC = NULL;

ViewOverlyController::ViewOverlyController(const std::wstring& overlay)
	: _overlay(NULL), _swhHook(NULL)
{
	_overlay = new OverlayWnd(overlay);
	_overlay->Create(NULL);

	pVOC = this;
}

ViewOverlyController::~ViewOverlyController()
{
	_overlay->HideWnd();
	if (_swhHook != NULL) {
		::UnhookWindowsHookEx(_swhHook);
	}
}

void ViewOverlyController::SetOverlyTarget(HWND target)
{
	_target = target;
	_targetTopMain = _target.GetTopLevelParent();

	_swhHook = ::SetWindowsHookEx(WH_CALLWNDPROCRET,
		ViewOverlyController::HookProxy,
		NULL,
		//::GetMainThreadID()
		::GetCurrentThreadId()
	);

	if (_swhHook == NULL) {
		throw new std::exception("failed, call SetWindowsHookEx");
	}
		
}

void ViewOverlyController::SetOverlyTargetOnTopLevel(
	const char * wndClassName, 
	const char * caption)
{
	HWND target=::FindWindowA(wndClassName, caption);
	if (target == NULL) {
		return;
	}
	SetOverlyTarget(target);
	
		
}



LRESULT ViewOverlyController::OnMessageHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0 || lParam == 0) {
		::CallNextHookEx(_swhHook, code, wParam, lParam);
	}

	CWPRETSTRUCT* p = (CWPRETSTRUCT*)lParam;

	// may be main window moving
	UINT msg = p->message;
	HWND t = p->hwnd;
	if (t != _target && t != _targetTopMain) {
		return ::CallNextHookEx(_swhHook, code, wParam, lParam);
	}
	if (msg == WM_MOVING || msg == WM_MOVE ||
		msg == WM_WINDOWPOSCHANGING || msg == WM_WINDOWPOSCHANGED ||
		msg == WM_SHOWWINDOW
		) {
		if (_overlay != NULL) {
			_overlay->UpdateOverlay(_target);
		}
	}
	return ::CallNextHookEx(_swhHook, code, wParam, lParam);

}

LRESULT ViewOverlyController::HookProxy(int code, WPARAM wParam, LPARAM lParam)
{	
	return pVOC->OnMessageHook(code, wParam, lParam);
}


void DrawPrintWatermark_Test(HDC hdc,std::wstring overlay)
{
	if (hdc == NULL) {
		return;
	}
	int dwdeviceWidth = ::GetDeviceCaps(hdc, PHYSICALWIDTH);	//the width of the physical page, in device units
	int dwdeviceHigh = ::GetDeviceCaps(hdc, PHYSICALHEIGHT);	//the height of the physical page, in device units
	int dwLogicX = ::GetDeviceCaps(hdc, LOGPIXELSX);
	int dwLogicY = ::GetDeviceCaps(hdc, LOGPIXELSY);

	float fPaperWidth = (float)dwdeviceWidth / dwLogicX;
	float fPaperHigh = (float)dwdeviceHigh / dwLogicY;

	CString str(overlay.c_str());
	if (str.IsEmpty()) {
		str = L"Nextlabs SkyDRM Overlay(NULL)";
	}
	Graphics g(hdc);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	//g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	// config res
	Gdiplus::Pen myPen(Gdiplus::Color(255, 100, 255, 100), 10);
	Gdiplus::FontFamily fontfamily(L"Arial");
	Gdiplus::Font myFont(&fontfamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::StringFormat format;
	Gdiplus::SolidBrush brush(Gdiplus::Color(150, 255, 0, 255));

	// how to play, can be used only one
	Gdiplus::SizeF surface(dwdeviceWidth, dwdeviceHigh);
	Gdiplus::SizeF text = GetTextBounds(myFont, format, str);
	text.Width += 60;
	text.Height += 120;
	// draw
	g.RotateTransform(-5);
	for (int i = 0; i < surface.Width; i += text.Width) {
		for (int j = 0; j < surface.Height; j += text.Height) {
			g.DrawString(str, -1, &myFont,
				PointF(i, j), &format, &brush);
		}
	}


}
