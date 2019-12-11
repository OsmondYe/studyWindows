#include "stdafx.h"

#include <mutex>
#include <cctype>
#include "watermark.h"
#include <cmath>  // using sin cos
#include <functional>
#include <TlHelp32.h>
#include <gdiplus.h>


using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

#ifndef MAXULONGLONG
#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#endif

using namespace std;



namespace {

	ULONG_PTR gGidplusToken;
	Gdiplus::GdiplusStartupInput gGdipulsInput;

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}
	// calculate the size art-text used
	Gdiplus::SizeF CalcTextSizeF(const Font& font, const Gdiplus::StringFormat& strFormat, const CString& szText)
	{
		Gdiplus::GraphicsPath graphicsPathObj;
		Gdiplus::FontFamily fontfamily;
		font.GetFamily(&fontfamily);
		graphicsPathObj.AddString(szText, -1,&fontfamily,font.GetStyle(),font.GetSize(),PointF(0, 0), &strFormat);
		Gdiplus::RectF rcBound;
		graphicsPathObj.GetBounds(&rcBound);
		return Gdiplus::SizeF(rcBound.Width, rcBound.Height);
	}
	
	Gdiplus::SizeF CalcTextSizeF(
		const Gdiplus::Graphics& drawing_surface,
		const CString& szText,
		const Gdiplus::StringFormat& strFormat,
		const Font& font)
	{
		Gdiplus::RectF rcBound;
		drawing_surface.MeasureString(szText, -1, &font, Gdiplus::PointF(0, 0), &strFormat, &rcBound);
		return Gdiplus::SizeF(rcBound.Width, rcBound.Height);
	}

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

	std::vector<std::wstring> GetInstalledFonts()
	{
		Gdiplus::InstalledFontCollection ifc;
		std::vector<std::wstring> rt;
		auto count = ifc.GetFamilyCount();
		int actualFound = 0;

		Gdiplus::FontFamily* buf = new Gdiplus::FontFamily[count];
		ifc.GetFamilies(count, buf, &actualFound);
		for (int i = 0; i < actualFound; i++) {
			wchar_t name[0x20] = { 0 };
			buf[i].GetFamilyName(name);
			rt.push_back(name);
		}

		delete[] buf;
		return rt;
	}

	bool iequal(const std::wstring& l, const std::wstring& r) {
		if (l.size() != r.size()) {
			return false;
		}

		return std::equal(l.begin(), l.end(), r.begin(), r.end(), [](wchar_t i, wchar_t j) {
			return std::tolower(i) == std::tolower(j);
		});

	}

	class MsgAntiReenter {
	public:
		MsgAntiReenter() {
			if (InitializeCriticalSectionAndSpinCount(&cs, 0x80004000) == FALSE) {
				InitializeCriticalSection(&cs);
			}
		}
		~MsgAntiReenter() {
			DeleteCriticalSection(&cs);
		}
		void thread_enable(void) {
			EnterCriticalSection(&cs);
			disabled_thread[GetCurrentThreadId()]--;
			LeaveCriticalSection(&cs);
		}

		void thread_disable(void) {
			DWORD tid = GetCurrentThreadId();
			EnterCriticalSection(&cs);
			if (disabled_thread.find(tid) == disabled_thread.end()) {
				disabled_thread[tid] = 0;
			}
			disabled_thread[tid]++;
			LeaveCriticalSection(&cs);
		}

		bool is_thread_disabled(void) {
			bool result = false;
			DWORD tid = GetCurrentThreadId();
			EnterCriticalSection(&cs);
			if (disabled_thread.find(tid) != disabled_thread.end()) {
				if (disabled_thread[tid] > 0) {
					result = true;
				}
			}
			LeaveCriticalSection(&cs);
			return result;
		}		

	private:
		CRITICAL_SECTION     cs;
		std::map<DWORD, int>  disabled_thread;
	};
	class MsgAntiRenter_Control {
	public:
		MsgAntiRenter_Control(MsgAntiReenter& mar) :_mar(mar) { mar.thread_disable(); }
		~MsgAntiRenter_Control() { _mar.thread_enable(); }
	private:
		MsgAntiReenter& _mar;
	};


	MsgAntiReenter g_mar;

} // end anonymous namespace

namespace gdi {

	vector<wstring> GetInstalledFonts()
	{
		Gdiplus::InstalledFontCollection ifc;
		vector<wstring> rt;
		auto count = ifc.GetFamilyCount();
		int actualFound = 0;

		Gdiplus::FontFamily* buf = new Gdiplus::FontFamily[count];
		ifc.GetFamilies(count, buf, &actualFound);
		for (int i = 0; i < actualFound; i++) {
			wchar_t name[0x20] = { 0 };
			buf[i].GetFamilyName(name);
			rt.push_back(name);
		}

		delete[] buf;
		return rt;
	}

	bool SaveFileAsBitmap(Gdiplus::Image * image, std::wstring path)
	{

		CLSID clsid;
		GetEncoderClsid(L"image/bmp", &clsid);

		image->Save(path.c_str(), &clsid);

		return false;
	}

	Gdiplus::PointF CaculateRotated(Gdiplus::PointF& org, int angle)
	{
		static const double PI = std::acos(-1);
		Gdiplus::PointF rt;

		double radians = angle * PI / 180;

		rt.X = org.X*std::cos(radians) - org.Y*std::sin(radians);
		rt.Y = org.X*std::sin(radians) + org.Y*std::cos(radians);

		return rt;
	}

	Gdiplus::RectF CaculateOutbound(Gdiplus::PointF(&org)[4])
	{

		vector<Gdiplus::REAL> Xs, Ys;
		for (int i = 0; i < 4; i++) {
			Xs.push_back(org[i].X);
			Ys.push_back(org[i].Y);
		}

		std::sort(Xs.begin(), Xs.end());
		std::sort(Ys.begin(), Ys.end());

		Gdiplus::REAL width = Xs.back() - Xs.front();
		Gdiplus::REAL height = Ys.back() - Ys.front();

		return Gdiplus::RectF(Xs.front(), Ys.front(), width, height);
	}

	Gdiplus::RectF CalculateMinimumEnclosingRectAfterRotate(const Gdiplus::SizeF & size, int rotate)
	{
		PointF org[4] = {
			{0,0},{0,size.Height},{size.Width, size.Height},  {size.Width, 0}
		};

		PointF org_r[4];
		for (int i = 0; i < 4; i++) {
			org_r[i] = gdi::CaculateRotated(org[i], rotate);
		}

		return gdi::CaculateOutbound(org_r);
	}
}


void OverlayWindow::UpdateOverlaySizePosStatus(HWND target)
{
	
	CRect targetRC;
	if (target == NULL) {
		// user physical device may changed ,you get it each tiem
		targetRC = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else {
		::GetWindowRect(target, &targetRC);
	}

	if (OldTarget.EqualRect(targetRC)) {
		OutputDebugStringA("same CRect, return directly");
		return;
	}
	else 
	{
		OldTarget = targetRC;
		std::ostringstream oss;
		oss << "new size:[" << targetRC.left << "," << targetRC.top << "," << targetRC.right << "," << targetRC.bottom << "]\n";
		OutputDebugStringA(oss.str().c_str());
	}

	targetRC.DeflateRect(_config.GetDisplayOffset());
	// make layered wnd always covered the targert Wnd
	MoveWindow(targetRC,false);

	BLENDFUNCTION blend = { AC_SRC_OVER ,0,0xFF,AC_SRC_ALPHA };
	//CPoint p(targetRC.left, targetRC.right);
	CPoint zero_pt(0, 0);
	CPoint dstpt = targetRC.TopLeft();
	CSize  s = targetRC.Size();
	//CSize s(targetRC.Width(), targetRC.Height());
	
	// draw in Screen, but always get target wnd's region info
	if (!::UpdateLayeredWindow(
		this->m_hWnd,   // this wnd must specifying WS_EX_LAYERED 
		NULL,
		&dstpt, &s,   // [Pos,+Size] new m_hWnd's posiziton
		*pmdc,&zero_pt,   // src dc and {left,top}
		NULL,&blend, ULW_ALPHA)  // using alpha blend,
		) {
		// error occured
		auto err = ::GetLastError();
		std::string strErr = "Faied call UpdateLayeredWindow,error is ";
		strErr += std::to_string(err);
		::OutputDebugStringA(strErr.c_str());
	}
	else {
		std::ostringstream oss;
		oss << "UpdateLayeredWindow,dst[" << dstpt.x << "," << dstpt.y << "],size[" << s.cx << "," << s.cy << "]\n";
		OutputDebugStringA(oss.str().c_str());
	}
	//::BringWindowToTop(this->m_hWnd);
}

Gdiplus::Bitmap * OverlayWindow::_GetOverlayBitmap(const Gdiplus::Graphics& drawing_surface)
{
	CString overlay_str(_config.GetString().c_str());	
	Gdiplus::FontFamily fontfamily(_config.GetFontName().c_str());
	Gdiplus::Font font(&fontfamily, _config.GetFontSize(),_config.GetGdiFontStyle(), Gdiplus::UnitPixel);
	Gdiplus::SolidBrush brush(_config.GetFontColor());
	Gdiplus::StringFormat str_format;
	str_format.SetAlignment(_config.GetGdiTextAlignment());
	str_format.SetLineAlignment(_config.GetGdiLineAlignment());
	Gdiplus::SizeF str_size = CalcTextSizeF(drawing_surface, overlay_str,str_format,font);
	Gdiplus::RectF str_enclosing_rect = gdi::CalculateMinimumEnclosingRectAfterRotate(str_size, _config.GetFontRotation());

	Gdiplus::REAL surface_size = 2 * std::ceil(std::hypot(str_size.Width, str_size.Height));

	Gdiplus::Bitmap surface(surface_size, surface_size, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&surface);
	// make a good quality
	g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	// set centre point as the base point
	g.TranslateTransform(surface_size / 2, surface_size / 2);
	g.RotateTransform(_config.GetFontRotation());
	// set string
	g.DrawString(overlay_str.GetString(), -1, &font,
		Gdiplus::RectF(0, 0, str_size.Width, str_size.Height),
		&str_format,&brush);
	g.ResetTransform();
	g.Flush();

	// since drawing org_point is the centre, 
	Gdiplus::RectF absolute_layout = str_enclosing_rect;
	absolute_layout.Offset(surface_size / 2, surface_size / 2);


	// request bitmap is the partly clone with absolute_layout
	return surface.Clone(absolute_layout, PixelFormat32bppARGB);
}

Gdiplus::Bitmap * OverlayWindow::_GetOverlayBitmapFromFile(const std::wstring & path)
{
	Gdiplus::Bitmap* im = Gdiplus::Bitmap::FromFile(path.c_str());
	if (im == NULL) {
		return im;
	}

	auto h = im->GetHeight();
	auto w = im->GetWidth();

	// change alpha
	for(int i=0;i<h;i++)
		for (int j = 0; j < w; j++) {
			Gdiplus::Color c;
			im->GetPixel(i, j, &c);	
			if (c.GetAlpha()) {
				Gdiplus::Color c_(Gdiplus::Color::MakeARGB(20, c.GetR(), c.GetG(), c.GetB()));
				im->SetPixel(i, j, c_);
			}

		}
	return im;
}

void OverlayWindow::_DrawOverlay(HDC dcScreen, LPRECT lpRestrictDrawingRect)
{
	if (dcScreen == NULL) {
		return;
	}
	CRect rc(lpRestrictDrawingRect);
	Gdiplus::RectF surface(0,0,rc.Width(), rc.Height());		
	Gdiplus::Graphics g(dcScreen);
	// make a good quality
	g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	
	//Gdiplus::Bitmap* bk = _GetOverlayBitmapFromFile(L"D:\\allTestFile\\pics\\keng.png");
	//Gdiplus::Bitmap* bk = _GetOverlayBitmapFromFile(L"D:\\allTestFile\\pics\\webwxgeticon.jpg");
	// lower -back ground
	Gdiplus::Bitmap* bk = _GetOverlayBitmapFromFile(L"D:\\allTestFile\\pics\\ccp2.png");
	if (bk != NULL) {
		Gdiplus::TextureBrush brush(bk,Gdiplus::WrapModeTile);
		//brush.RotateTransform(_config.GetFontRotation());  // for picture using rotatTransfrom
		g.FillRectangle(&brush, surface);
	}
	// middle background
	Gdiplus::Bitmap* bk2 = _GetOverlayBitmap(g);
	if (bk2 != NULL) {
		Gdiplus::TextureBrush brush2(bk2, Gdiplus::WrapModeTile);
		g.FillRectangle(&brush2, surface);
		delete bk;
	}
}


void OverlayWindow::_PrepareOverly()
{
	// Get Whole Screen pixels
	CRect ScreenRC = { 
		GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN)+100,
		GetSystemMetrics(SM_CYVIRTUALSCREEN)+100 };

	// Get a large surface to draw overlay
	CDC dc=::GetDC(NULL);
	pmdc = new CMemoryDC(dc, ScreenRC);
	_DrawOverlay(*pmdc, ScreenRC);
}




//
//  classController
//
//

ViewOverlyController* ViewOverlyController::sgIns = NULL;
std::recursive_mutex ViewOverlyController::sgRMutex;
#ifdef ViewOverlyControllerScopeGurad
	#error this is Impossible
#endif // ViewOverlyControllerScopeGurad
#define ViewOverlyControllerScopeGurad std::lock_guard<std::recursive_mutex> g(sgRMutex)

ViewOverlyController & ViewOverlyController::getInstance()
{
	ViewOverlyControllerScopeGurad;
	if (sgIns == NULL) {
		// init gdi++
		Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);

		sgIns = new ViewOverlyController();
	}
	return *sgIns;
	
}


ViewOverlyController::~ViewOverlyController()
{
	if (_swhHook != NULL) {
		::UnhookWindowsHookEx(_swhHook);
	}
	// deinit gdi++
	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
		gGidplusToken = NULL;
	}

}

void ViewOverlyController::Attach(HWND target, const OverlayConfig& config, int tid)
{
	ViewOverlyControllerScopeGurad;

	if (_wnds.find(target) != _wnds.end()) {
		// has got
		if (_wnds[target]->_config != config) {
			_wnds[target]->SetOverlay(config);
		}
	}
	else {
		std::shared_ptr<OverlayWindow> spWnd(new OverlayWindow());
		spWnd->Init(config,target);
		_wnds[target] = spWnd;
		SetOverlyTarget(target);
	}
}

void ViewOverlyController::Detach(HWND target)
{
	ViewOverlyControllerScopeGurad;

	if (_wnds.find(target) != _wnds.end()) {
		_wnds[target]->HideWnd();
		_wnds.erase(target);
	}
}

void ViewOverlyController::Clear()
{
	ViewOverlyControllerScopeGurad;
	_wnds.clear();
}

void ViewOverlyController::SetOverlyTarget(HWND target)
{
	ViewOverlyControllerScopeGurad;

	if (_swhHook == NULL) {
		// call this function on UI thread
		_swhHook = ::SetWindowsHookEx(WH_CALLWNDPROCRET,	// after wnd had processed the message
			ViewOverlyController::HookProxy,
			NULL,
			//::GetMainThreadID()
			::GetCurrentThreadId()
		);

		if (_swhHook == NULL) {
			throw new std::exception("failed, call SetWindowsHookEx");
		}
	}

	if (_wnds.find(target) != _wnds.end()) {
		_wnds[target]->UpdateOverlaySizePosStatus(target);
	}
}

void ViewOverlyController::UpdateWatermark(HWND target) {
	ViewOverlyControllerScopeGurad;
	if (_wnds.find(target) != _wnds.end()) {
		_wnds[target]->UpdateOverlaySizePosStatus(target);
	}
}

LRESULT ViewOverlyController::OnMessageHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0 || lParam == 0) {
		return ::CallNextHookEx(_swhHook, code, wParam, lParam);
	}

	if (g_mar.is_thread_disabled()) {
		return ::CallNextHookEx(_swhHook, code, wParam, lParam);
	}
	MsgAntiRenter_Control auto_control(g_mar);

	CWPRETSTRUCT* p = (CWPRETSTRUCT*)lParam;
	UINT msg = p->message;
	HWND t = p->hwnd;

	ViewOverlyControllerScopeGurad;

	if(_wnds.empty() || _wnds.find(t) == _wnds.end()){
		return ::CallNextHookEx(_swhHook, code, wParam, lParam);
	}

	switch (msg)
	{
	case WM_MOVE:
	case WM_MOVING:
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED:
	case WM_SHOWWINDOW:
	//case WM_SIZE:
	//case WM_SIZING:
	case WM_SYSCOMMAND:
	{
		_wnds[t]->UpdateOverlaySizePosStatus(t);
		break;
	}
	case WM_DESTROY:
	{
		// target wnd wants destory tell to destory overlay wnd
		_wnds[t]->HideWnd();
		_wnds.erase(t);
		break;
	}
	default:
		break;
	}
	
	return ::CallNextHookEx(_swhHook, code, wParam, lParam);

}

LRESULT ViewOverlyController::HookProxy(int code, WPARAM wParam, LPARAM lParam)
{	

	return getInstance().OnMessageHook(code, wParam, lParam);
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
	Gdiplus::SizeF text = CalcTextSizeF(myFont, format, str);
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




//
//  for config of overlay
// 

std::wstring OverlayConfigBuilder::GetDefaultFontName()
{
	// using Gdipuls provided default 
	std::vector<wchar_t> buf(0x30, 0);
	Gdiplus::FontFamily::GenericSansSerif()->GetFamilyName(buf.data(), LANG_NEUTRAL);
	return std::wstring(buf.data());
}

bool OverlayConfigBuilder::IsFontNameSupported(const std::wstring & font_name)
{
	auto cont = GetInstalledFonts();
	for (auto& i : cont) {
		if (iequal(i, font_name)) {
			return true;
		}
	}
	return false;
}

bool OverlayConfig::IsSameConfig(const OverlayConfig & rh)
{
	if (this == &rh) {
		return true;
	}
	// check all 
	if (this->m_str != rh.m_str) {return false;}
	if (this->m_font_size != rh.m_font_size) {return false;}
	if (this->m_font_rotation != rh.m_font_rotation) { return false; }
	if (this->m_font_color_A != rh.m_font_color_A) {return false;}
	if (this->m_font_color_R != rh.m_font_color_R) {return false;}
	if (this->m_font_color_G != rh.m_font_color_G) {return false;}
	if (this->m_font_color_B != rh.m_font_color_B) {return false;}
	if (this->m_font_style!=rh.m_font_style) { return false; }
	if (!iequal(this->m_font_name, rh.m_font_name)) { return false; }
	if (this->m_display_offset != rh.m_display_offset) { return false; }

	return true;
}
