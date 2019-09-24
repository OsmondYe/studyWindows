#include "stdafx.h"
#include "overlay.h"
#include "watermark.h"

#include <madCHook.h>
#include "auto_disable.hpp"


DLL_FUN_EXPORT void InitWaterMark(HWND targertWnd, wchar_t* strOverlay) {
	::OutputDebugStringW(L"InitWaterMark");
	//std::wstring str;
	//if (strOverlay == NULL ) {
	//	str=L"Default viewOverlay";
	//}
	//else {
	//	str = strOverlay;
	//}
	//ViewOverlyController::getInstance().Attach(targertWnd, str);
	//TestPrintOverlayHook(str);
}

DLL_FUN_EXPORT void UpdateWaterMark(HWND targertWnd) {
	ViewOverlyController::getInstance().UpdateWatermark(targertWnd);
}

DLL_FUN_EXPORT void DeleteWaterMark(HWND targertWnd) {
	ViewOverlyController::getInstance().Detach(targertWnd);
	//ViewOverlyController::getInstance().Clear();
}





std::wstring PrintOverlay;

int (WINAPI *MyEndDocNext)(HDC hdc) = NULL;
int WINAPI MyEndDoc(HDC hdc) {
	DrawPrintWatermark_Test(hdc, PrintOverlay);
	return MyEndDocNext(hdc);
}

 int (WINAPI *MyEndPageNext)( HDC hdc) = NULL;
 int WINAPI MyEndPage(HDC hdc) {
	 DrawPrintWatermark_Test(hdc, PrintOverlay);
	 return MyEndPageNext(hdc);
 }


void TestPrintOverlayHook(const std::wstring& strOverlay) {
	PrintOverlay = strOverlay;
	::HookAPI("gdi32.dll", "EndDoc", MyEndDoc, (PVOID*)&MyEndDocNext);
	::HookAPI("gdi32.dll", "EndPage", MyEndPage, (PVOID*)&MyEndPageNext);
}