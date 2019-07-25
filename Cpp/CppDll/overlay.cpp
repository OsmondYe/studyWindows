#include "stdafx.h"
#include "overlay.h"
#include "watermark.h"


DLL_FUN_EXPORT void InitWaterMark(HWND targertWnd, wchar_t* strOverlay) {
	::OutputDebugStringW(L"InitWaterMark");
	std::wstring str;
	if (strOverlay == NULL ) {
		str=L"Default viewOverlay";
	}
	else {
		str = strOverlay;
	}
	ViewOverlyController::getInstance().Attach(targertWnd, str);
}

DLL_FUN_EXPORT void UpdateWaterMark(HWND targertWnd) {
	ViewOverlyController::getInstance().UpdateWatermark(targertWnd);
}

DLL_FUN_EXPORT void DeleteWaterMark(HWND targertWnd) {
	ViewOverlyController::getInstance().Detach(targertWnd);
	//ViewOverlyController::getInstance().Clear();
}