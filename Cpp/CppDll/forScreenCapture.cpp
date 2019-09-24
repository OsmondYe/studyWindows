#include "stdafx.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/algorithm.hpp>

#include <madCHook.h>
#include "hook.h"
#include <MsXml6.h>
#include "watermark.h"
#include "auto_disable.hpp"



HWND targetWnd = (HWND)-2;


BOOL  (WINAPI *BitBltNext)( HDC hdc,  int x,  int y,  int cx,  int cy, 
					HDC hdcSrc,  int x1,  int y1,  DWORD rop)=NULL;

BOOL  WINAPI MyBitBlt(HDC hdc, int x, int y, int cx, int cy,
	HDC hdcSrc, int x1, int y1, DWORD rop) {

	ViewOverlyController::getInstance();

	if (cx != GetSystemMetrics(78) || cy != GetSystemMetrics(79)) {
		return BitBltNext(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
	// SRCPAINT | CAPTUREBLT
	if (rop != 0x40CC0020) {
		return BitBltNext(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
	if (x1 != GetSystemMetrics(76) || y1 != GetSystemMetrics(77)) {
		return BitBltNext(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}


	::OutputDebugString(__FUNCTIONW__ L"\n");
	RECT rc = { x1,y1,cx,cy };
	DrawViewWatermark_Test(hdcSrc, L"count=1\nTesting anti-ScreenCapture to draw this string\nSkyDRM by Nextlabs. inc", &rc);
	return BitBltNext(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}


/*
Helpers::CreateWindowExW(
			 0x88u,                             // topmost and toolwnd
			 wndClassName,
			 0i64,
			 0x80000000,                        // WS_POPUP
			 this2->x,
			 this2->y,
			 this2->cxSize,
			 this2->cySize,
			 0i64,
			 0i64,
			 (HMENU)this2->HINSTANCE,
			 (HINSTANCE)this2,
			 &vars0);
*/

HWND(WINAPI *CreateWindowExWNext)(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,
	DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,
	HINSTANCE hInstance,LPVOID lpParam)=NULL;


HWND WINAPI MyCreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
	HINSTANCE hInstance, LPVOID lpParam) {


	if (dwExStyle != 0x88u || dwStyle!= 0x80000000 || lpWindowName!=NULL) {
		return CreateWindowExWNext(dwExStyle, lpClassName, lpWindowName, dwStyle,
			X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}
	if (lpClassName == NULL) {
		return CreateWindowExWNext(dwExStyle, lpClassName, lpWindowName, dwStyle,
			X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}



	std::wstring className = L"Microsoft-Windows-SnipperCaptureForm";
	if (!boost::istarts_with(lpClassName, className)) {
		return CreateWindowExWNext(dwExStyle, lpClassName, lpWindowName, dwStyle,
			X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}


	::OutputDebugString(__FUNCTIONW__ L"\n");
	HWND hMyHWD= CreateWindowExWNext(dwExStyle, lpClassName, lpWindowName, dwStyle,
		X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	// attach overlay on it
	//OverlayConfigBuilder ocb;
	//ocb.SetFontColor(100, 255, 0, 100)
	//	.SetFontRotation(20)
	//	.SetFontSize(20)
	//	.SetString(L"count=2\nTesting anti-ScreenCapture to draw this string\nSkyDRM by Nextlabs. inc");

	//ViewOverlyController::getInstance().Attach(hMyHWD,ocb.Build());

	targetWnd = hMyHWD;

	return hMyHWD;
}

HDC (WINAPI *GetDCNext)(HWND hWnd)=NULL;

HDC WINAPI MyGetDC(HWND hWnd) {
	if (hWnd != targetWnd) {
		return GetDCNext(hWnd);
	}
	::OutputDebugString(__FUNCTIONW__ L"\n");
	HDC hdc= GetDCNext(hWnd);
	RECT rc = { 0,0,GetSystemMetrics(78),GetSystemMetrics(79) };
	DrawViewWatermark_Test(hdc, L"count=1\nTesting anti-ScreenCapture to draw this string\nSkyDRM by Nextlabs. inc", &rc);
	return hdc;
}


BOOL (WINAPI *EndPaintNext)(HWND hWnd,CONST PAINTSTRUCT* lpPaint)=NULL;
BOOL WINAPI MyEndPaint(HWND hWnd, CONST PAINTSTRUCT* lpPaint) {
	if (hWnd != targetWnd) {
		return EndPaintNext(hWnd,lpPaint);
	}
	::OutputDebugString(__FUNCTIONW__ L"\n");
	RECT rc = { 0,0,GetSystemMetrics(78),GetSystemMetrics(79) };
	DrawViewWatermark_Test(lpPaint->hdc, L"Test anti-ScreenCapture\n中文:测试反截屏水印\nSkyDRM WindowRMX\nNextlabs. inc\n", &rc);
	return EndPaintNext(hWnd, lpPaint);
}



DLL_FUN_EXPORT bool HookForScreenCapture() {
	//BitBlt()

	//HookAPI("gdi32.dll", "BitBlt", MyBitBlt, (PVOID*)&BitBltNext);
	
	HookAPI("User32.dll", "CreateWindowExW", MyCreateWindowExW, (PVOID*)&CreateWindowExWNext);
	//HookAPI("User32.dll", "GetDC", MyGetDC, (PVOID*)& GetDCNext);
	HookAPI("User32.dll", "EndPaint", MyEndPaint, (PVOID*)& EndPaintNext);

	//::GetDC()

	//EndPaint()

	return true;
}