#include "stdafx.h"
#include "watermark.h"

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")


#ifndef USING_GDI_PLUS
#define USING_GDI_PLUS

extern ULONG_PTR gGidplusToken;
extern Gdiplus::GdiplusStartupInput gGdipulsInput;

#endif // ! USING_GDI_PLUS




namespace {
	SizeF GetTextBounds(const Font& font, const StringFormat& strFormat, const CString& szText)
	{
		GraphicsPath graphicsPathObj;
		FontFamily fontfamily;
		font.GetFamily(&fontfamily);
		graphicsPathObj.AddString(szText, -1, 
									&fontfamily, 
									font.GetStyle(), 
									font.GetSize(), 
									PointF(0, 0), &strFormat);
		RectF rcBound;
		graphicsPathObj.GetBounds(&rcBound);
		return SizeF(rcBound.Width, rcBound.Height);
	}
}

OverlayChildWnd::OverlayChildWnd()
{
#ifdef USING_GDI_PLUS
	Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);
#endif // USING_GDI_PLUS
	
}

OverlayChildWnd::~OverlayChildWnd()
{
#ifdef USING_GDI_PLUS
	if (gGidplusToken != NULL) {
		Gdiplus::GdiplusShutdown(gGidplusToken);
	}
#endif // USING_GDI_PLUS
}

CRect OldTarget(-1, -1, -1, -1);

void OverlayChildWnd::UpdateOverlay(HWND target)
{
	
	CRect ScreenRC = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	CRect targetRC;
	if (target == NULL) {
		targetRC = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else {
		::GetWindowRect(target, &targetRC);
	}

	if (OldTarget.EqualRect(targetRC)) {
		return;
	}
	else {
		OldTarget = targetRC;
	}

	// make layered wnd always covered the targert Wnd
	MoveWindow(targetRC);

	// target ==0 ,get screen dc
	HDC hdcTarget = ::GetDC(NULL);
	if (hdcTarget == NULL) {
		return;
	}		
	CMemoryDC hMemScreen(hdcTarget, targetRC);
	if (hMemScreen.m_hDC == NULL) {
		return;
	}
	// draw Overlay in TargetDC
	_DrawOverlay(hMemScreen, targetRC);


	BLENDFUNCTION blend = { AC_SRC_OVER ,0,100,AC_SRC_ALPHA };
	//CPoint p(targetRC.left, targetRC.right);
	CPoint p(0, 0);
	CPoint dstpt(targetRC.left, targetRC.top);
	CSize s(targetRC.Width(), targetRC.Height());
	//CSize s(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	// draw in Screen, but always get target wnd's region info
	::UpdateLayeredWindow(m_hWnd, ::GetWindowDC(target), &dstpt, &s, hMemScreen,
		&p, NULL, &blend, ULW_ALPHA);
	::ReleaseDC(NULL, hdcTarget);

}


void OverlayChildWnd::_DrawOverlay(HDC dcScreen, LPRECT lpRestrictDrawingRect)
{
	if (dcScreen == NULL) {
		return;
	}

	CString str(L"[Chard_Cao] Nextlabs View Overlay -- V3 Investigation");
	CRect rc(lpRestrictDrawingRect);

	// using gdi+
	{
		Gdiplus::Graphics g(dcScreen);
		g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		Gdiplus::Pen myPen(Gdiplus::Color(255, 100, 255, 100),10);
		Gdiplus::FontFamily fontfamily(L"Arial");
		Gdiplus::Font myFont(&fontfamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::RectF myRect(0, 0, rc.Width(), rc.Height());
		Gdiplus::StringFormat format;
		Gdiplus::SolidBrush brush(Gdiplus::Color(250, 255, 0, 255));
		Gdiplus::Region myRegion(myRect);

		// text rectangle
		g.DrawRectangle(&myPen, Gdiplus::RectF(
			rc.left+10, rc.top,
			rc.Width()-20, rc.Height()));

		// how to play, can be used only one
		Gdiplus::SizeF surface(rc.Width(),rc.Height());
		Gdiplus::SizeF text = GetTextBounds(myFont, format, str);
		text.Width+= 60; 
		text.Height+= 120;


		g.SetClip(&myRegion);
		g.RotateTransform(-5);
		g.TranslateTransform(rc.left, rc.top);
		for (int i = 0; i < surface.Width; i += text.Width) {
			for (int j = 0; j < surface.Height; j += text.Height) {
				g.DrawString(str, -1, &myFont, 
					PointF(i, j), &format, &brush);
			}
		}
		
		// test output
		//g.DrawString(L"(basePoint[0,0])rc.left, rc.top", -1, &myFont, PointF(0, 0), &format, &brush);
		//g.DrawString(L"PointF(0,40)", -1, &myFont, PointF(0,40), &format, &brush);
		//g.DrawString(L"PointF(200, 40)", -1, &myFont, PointF(600, 40), &format, &brush);
		//g.DrawString(str+"PointF(0, 200)", -1, &myFont, PointF(0, 200), &format, &brush);
		//g.DrawString(str, -1, &myFont, PointF(0,rc.Height()-80 ), &format, &brush);
		
		//g.TranslateTransform(0,0);

	}

}


int OverlayChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

void OverlayChildWnd::OnPaint(CDCHandle) {
	::OutputDebugString(__FUNCTIONW__ L"\n");
	CPaintDC dc(this->m_hWnd);

	//
	// begin paint
	//
	// maybe need to resize it selt
	//CRect newSize;
	//GetParentClientSize(&newSize);
	//this->ResizeClient(newSize.Width(), newSize.Height(), false);

	// after move  
	//drawRect(dc);

	//_DrawOverlay(dc);

	//
	// end paint
	//

}

