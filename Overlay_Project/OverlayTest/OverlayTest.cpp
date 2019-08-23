// using WTL for easy demo
#include "stdafx.h"
#include "resource.h"


#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")


//
// watermark lib here, you need watermark.h cand watermark.cpp
//
#include "watermark.h"


ULONG_PTR gGidplusToken = 0;
Gdiplus::GdiplusStartupInput gGdipulsInput;

CAppModule _Module;


class TestMain : public CWindowImpl<TestMain,CWindow, CFrameWinTraits>{
	BEGIN_MSG_MAP_EX(TestMain)
		MSG_WM_CLOSE(OnDestroy)
		MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()

	void OnDestroy() {
		DestroyWindow();
		::PostQuitMessage(0);
	}

};

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



	TestMain main;
	main.Create(NULL);

	//
	// attach watermark on main
	//

	ViewOverlyController::getInstance().Attach(main.m_hWnd, 
			L"This is test watermark by Nextlabs-CDC\n测试窗口水印-缩放拉伸最大最小显示隐藏");
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
