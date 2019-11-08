// using WTL for easy demo
#include "stdafx.h"
#include <dwmapi.h>
//
// watermark lib here, you need watermark.h cand watermark.cpp
//
#include "watermark.h"
#include "MainDialog.h"


using namespace std;

// atl&wtl required
CAppModule _Module;


// gdi+ global
ULONG_PTR gGidplusToken = 0;
Gdiplus::GdiplusStartupInput gGdipulsInput;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls
	_Module.Init(NULL, hInstance);
	Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);

	MainDialog main;
	main.DoModal(NULL);

	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
	}	   	 
	::CoUninitialize();

	return 0;
}



