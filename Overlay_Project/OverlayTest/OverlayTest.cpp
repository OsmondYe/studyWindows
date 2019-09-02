// using WTL for easy demo
#include "stdafx.h"

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

	//
	//
	//
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);



	MainDialog main;
	main.Create(NULL);

	//
	// attach watermark on main
	//

	OverlayConfigBuilder builder;
	builder
		.SetString(L"Watermark Test\nNextlabs SkyDRM Product\n这是中文测试,信领达(杭州)软件有限公司\n~!@#$%^&*(){}[];:,.<>")
		.SetFontSize(20)
		.SetFontName(L"Microsfot YaHei UI Light")
		//.SetFontName(L"Arial")
		.SetFontTransparency(50)
		.SetFontColor(255, 0, 0)
		.SetFontStyle(OverlayConfig::FontStyle::FS_BoldItalic)
		.SetTextAlignment(OverlayConfig::TextAlignment::TA_Centre)
		.SetLineAlignment(OverlayConfig::TextAlignment::TA_Left)
		.SetFontRotation(-20);


	ViewOverlyController::getInstance().Attach(main.m_hWnd, builder.Build()	);

	main.UpdateWindow();
	main.ShowWindow(SW_NORMAL);


	int nRet = theLoop.Run();

	main.Detach();

	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
	}	   	 
	_Module.RemoveMessageLoop();
	_Module.Term();
	::CoUninitialize();


	return 0;
}



