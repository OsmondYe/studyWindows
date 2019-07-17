#include "stdafx.h"
#include "watermark.h"



ViewOverlyController* p;



DLL_FUN_EXPORT void InitWaterMark(HWND targertWnd) {
	::OutputDebugStringW(L"InitWaterMark");
	p = new ViewOverlyController(L"Test Watermark by Nextalbs SkyDRM");
	p->SetOverlyTarget(targertWnd);
}

DLL_FUN_EXPORT void UpdateWaterMark(HWND targertWnd) {
	p->UpdateWatermark(targertWnd);
}


DLL_FUN_EXPORT void DeleteWaterMark() {
	if (p != NULL) {
		delete p;
		p = NULL;
	}
}