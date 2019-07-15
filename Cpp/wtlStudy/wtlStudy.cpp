#include "stdafx.h"
#include "resource.h"

#include "main_ui.h"

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")



#ifndef USING_GDI_PLUS
#define USING_GDI_PLUS

ULONG_PTR gGidplusToken = 0;
Gdiplus::GdiplusStartupInput gGdipulsInput;

#endif // !USING_GDI_PLUS





CAppModule _Module;

OyeFrameWnd* pMainWnd = NULL;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE /*hPrevInstance*/,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	_Module.Init(NULL, hInstance);
	
#ifdef USING_GDI_PLUS
	Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);
#endif // USING_GDI_PLUS



	int i = sizeof(MENUITEMINFO);

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	

	// Main wnd
	pMainWnd= new OyeFrameWnd();
	pMainWnd->CreateEx();
	//auto rgn=::CreateEllipticRgn(50, 50, 200, 400);
	//pMainWnd->SetWindowRgn(rgn);
	pMainWnd->ShowWindow(nCmdShow);
	
	// Overly Wnd
	//pWnd =new OverlayWnd();
	//CRect rc(0, 0, 1000, 400);
	//pWnd->Create(NULL,rc);
	//
	//pWnd->ShowWindow(SW_SHOW);
	//pWnd->UpdateWindow();
	//// set ovlery wnd target on man wnd
	//pWnd->UpdateOverlay(pMainWnd->m_hWnd);

	//ViewOverlyController c;
	//c.SetOverlyTarget(*pMainWnd);
	
	int nRet = theLoop.Run();


#ifdef USING_GDI_PLUS
	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
	}
#endif // USING_GDI_PLUS



	_Module.RemoveMessageLoop();
	_Module.Term();
	::CoUninitialize();


    return 0;
}
