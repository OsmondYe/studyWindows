// using WTL for easy demo
#include "stdafx.h"
#include <string>
#include <vector>
#include "resource.h"

using namespace std;

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

class TestMain : public CDialogImpl<TestMain> {
public:
	enum {IDD= IDD_ABOUTBOX};

	CEdit m_wEdit;
	CListBox m_wList;
	CStatic m_wFontDemo;

	BEGIN_MSG_MAP_EX(TestMain)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		COMMAND_HANDLER_EX(IDC_FONTLIST, LBN_DBLCLK, OnListDBClick)
	
	END_MSG_MAP()

	void OnListDBClick(UINT uNotifyCode, int nID, CWindow wndCtl) {

		wstring demo(L"This is the demo string for the specified font");

		
		wchar_t buf[0x20];
		m_wList.GetText(m_wList.GetCurSel(), buf);
		wstring str(buf);
		m_wFontDemo.SetWindowTextW(str.c_str());

		CRect rc;
		m_wFontDemo.GetWindowRect(&rc);

		using namespace Gdiplus;
		
		Graphics g(m_wFontDemo.GetDC());
		FontFamily font_family(buf);
		Font font(&font_family, 14, FontStyle::FontStyleBold, Unit::UnitPixel);
		SolidBrush brush(Color(100, 100, 100, 100));
		RectF layout(0, 0, rc.Width(), rc.Height());
		StringFormat string_format;
		
		g.DrawString(demo.c_str(), -1, &font, layout, &string_format, &brush);

	}


	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
		CenterWindow(GetParent());
		// Associate:
		m_wEdit.Attach(GetDlgItem(IDC_MYEDIT));
		m_wList.Attach(GetDlgItem(IDC_FONTLIST));
		m_wFontDemo.Attach(GetDlgItem(IDC_FONTDEMOSTR));

		m_wEdit.AppendText(L"this is a demo");

		EnumGDIPlugInstalledFont();

		m_wFontDemo.SetWindowTextW(L"this is test demo");

		return True;
	}

	wstring EnumGDIPlugInstalledFont() {
		wstring rt;
		Gdiplus::InstalledFontCollection ifc;

		auto count = ifc.GetFamilyCount();
		int actualFound = 0;

		Gdiplus::FontFamily* buf = new Gdiplus::FontFamily[count];	
		ifc.GetFamilies(count, buf, &actualFound);
		for (int i = 0; i < actualFound; i++) {
			wchar_t name[0x20] = { 0 };
			buf[i].GetFamilyName(name);
			m_wEdit.AppendText(name);
			m_wEdit.AppendText(L"\r\n");

			m_wList.AddString(name);
		}
		
		delete[] buf;
		return rt;
	}

	BOOL OnEraseBkgnd(CDCHandle dc) {
		CRect rc;
		GetClientRect(&rc);
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));
		CBrush old = dc.SelectBrush(brush);
		dc.PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		return true;

	}

	void OnClose() {
		DestroyWindow();
	}

	void OnDestroy() {
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



