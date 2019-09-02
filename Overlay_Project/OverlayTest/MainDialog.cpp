#include "stdafx.h"
#include "watermark.h"
#include "MainDialog.h"
#include <cmath>


BOOL MainDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	CenterWindow(GetParent());
	// Associate:
	m_wEdit.Attach(GetDlgItem(IDC_MYEDIT));
	m_wList.Attach(GetDlgItem(IDC_FONTLIST));
	m_wFontDemo.Attach(GetDlgItem(IDC_FONTDEMOSTR));

	m_ColorA.Attach(GetDlgItem(IDC_SLIDE_COLOR_A));
	m_ColorR.Attach(GetDlgItem(IDC_SLIDE_COLOR_R));
	m_ColorG.Attach(GetDlgItem(IDC_SLIDE_COLOR_G));
	m_ColorB.Attach(GetDlgItem(IDC_SLIDE_COLOR_B));



	m_FontColorA = 150;
	m_FontColorR = 100;
	m_FontColorG = 0;
	m_FontColorB = 100;

	auto fin = gdi::GetInstalledFonts();
	for (auto& i : fin) {
		m_wList.AddString(i.c_str());
	}

	m_wList.SetCurSel(1);


	m_ColorA.SetRange(0, 255); m_ColorA.SetPos(m_FontColorA);
	m_ColorR.SetRange(0, 255); m_ColorR.SetPos(m_FontColorR);
	m_ColorG.SetRange(0, 255); m_ColorG.SetPos(m_FontColorG);
	m_ColorB.SetRange(0, 255); m_ColorB.SetPos(m_FontColorB);


	m_EditContent = L"Watermark by Nextlabs-CDC\r\n缩放拉伸最大最小显示隐藏\r\nOsmond.ye@Nextlabs.com";
	m_FontSize = 30;
	m_FontRotate = -20;

	DoDataExchange(false);

	return True;
}


void MainDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar) {
	bool changed = false;
	if (pScrollBar.m_hWnd == m_ColorA.m_hWnd) {
		m_FontColorA = m_ColorA.GetPos();
		changed = true;
	}
	if (pScrollBar.m_hWnd == m_ColorR.m_hWnd) {
		m_FontColorR = m_ColorR.GetPos();
		changed = true;
	}
	if (pScrollBar.m_hWnd == m_ColorG.m_hWnd) {
		m_FontColorG = m_ColorG.GetPos();
		changed = true;
	}
	if (pScrollBar.m_hWnd == m_ColorB.m_hWnd) {
		m_FontColorB = m_ColorB.GetPos();
		changed = true;
	}
	if (changed) {
		//ShowWatermarkDemo();
		OnShowColorEffect(m_FontColorA, m_FontColorR, m_FontColorG, m_FontColorB);
	}
}



void MainDialog::OnDrawBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	OnClearBtnDown(uNotifyCode, nID, wndCtl);
	ShowWatermarkDemo();
}

void MainDialog::OnClearBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CRect rc;
	CWindow desktop(::GetDesktopWindow());
	//desktop.GetWindowRect(&rc);
	//desktop.RedrawWindow(&rc,0, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_ERASE);

	::RedrawWindow(NULL,NULL,NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_ERASE);

}

void MainDialog::OnShowColorEffect(BYTE a, BYTE r, BYTE g, BYTE b)
{
	CWindow effect(GetDlgItem(IDC_COLOR_EFFECT));

	BYTE base = -1;

	Gdiplus::Color c(a, r, g, b), c_vert(a^base,r^base,g^base,b^base);

	Gdiplus::Graphics gg(effect);

	CString  str;
	str.Format(L"Color:\nA:%d\nR:%d\nG:%d\nB:%d", a, r, g, b);

	CRect rc;
	effect.GetWindowRect(&rc);

	Gdiplus::RectF layout(0, 0, rc.Width(), rc.Height());
	Gdiplus::SolidBrush brush(c), strBrush(c_vert);
	Gdiplus::StringFormat sf;
	sf.SetAlignment(Gdiplus::StringAlignment::StringAlignmentNear);
	sf.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
	gg.FillRectangle(&brush, layout);
	gg.DrawString(str.GetString(), -1, 
		&Gdiplus::Font( Gdiplus::FontFamily::GenericSansSerif(),14),
		layout,
		&sf,
		&strBrush);

}


void MainDialog::FillBuilder(OverlayConfigBuilder & builder)
{
	DoDataExchange(true);
	wstring demostr(m_EditContent);
	wstring font_name = GetSelFontName();
	int fontSize = m_FontSize;
	int colorA = m_FontColorA;
	int colorB = m_FontColorB;
	int colorR = m_FontColorR;
	int colorG = m_FontColorG;
	int rotate = m_FontRotate;


	builder.SetFontColor(colorA, colorR, colorG, colorB)
		.SetFontName(font_name)
		.SetString(demostr)
		.SetFontSize(fontSize)
		.SetFontRotation(rotate);


}


void MainDialog::ShowWatermarkDemo() {
	
	// update watermark
	ViewOverlyController::getInstance().Detach(this->m_hWnd);
	

	OverlayConfigBuilder builder;
	this->FillBuilder(builder);
	ViewOverlyController::getInstance().Attach(this->m_hWnd, builder.Build());

	//this->UpdateWindow();


	// print effect into screen
	//for(int i=0;i<100000;i++)   //debug if memory leak
	//CreatePngImageWithWaterMark();

	
}



void MainDialog::CreatePngImageWithWaterMark()
{
	// prepare UI parmas
	DoDataExchange(true);
	wstring demostr(m_EditContent);
	wstring font_name = GetSelFontName();
	int fontSize = m_FontSize;
	int colorA = m_FontColorA;
	int colorB = m_FontColorB;
	int colorR = m_FontColorR;
	int colorG = m_FontColorG;
	int rotate = m_FontRotate;
	CRect rc;
	//m_wFontDemo.GetWindowRect(&rc);

	CWindow desktop(::GetDesktopWindow());
	desktop.GetWindowRect(&rc);
	// bind with desktop
	using namespace Gdiplus;

	desktop.RedrawWindow(&rc);
	Graphics g(desktop);

	// set params
	Gdiplus::Color font_color = GetFontColor();
	Pen pen(Gdiplus::Color::Black);
	SolidBrush brush(font_color);
	Gdiplus::FontFamily font_family(font_name.c_str());
	Gdiplus::Font font(&font_family, fontSize, FontStyle::FontStyleBoldItalic, Unit::UnitPixel);
	Gdiplus::RectF layout(0, 0, rc.Width(), rc.Height());
	// calc str outbound rect
	Gdiplus::RectF str_org_rect;
	g.MeasureString(demostr.c_str(), -1, &font, PointF(0,0), &str_org_rect);

	REAL imSize =  2* std::ceil(std::hypot(str_org_rect.Width, str_org_rect.Height));

	// how to determine rotated outBound size
	Gdiplus::Bitmap* bk = NULL;
	Gdiplus::Bitmap image(imSize, imSize);
	{
		Graphics g2(&image);
		{
			// Draw coordinate
			// X
			//g2.DrawLine(&pen, PointF(0, imSize / 2), PointF(imSize, imSize / 2));
			// Y
			//g2.DrawLine(&pen, PointF(imSize / 2, 0), PointF(imSize / 2, imSize));
		}
			
		// set centre point as the base point
		g2.TranslateTransform(imSize / 2, imSize / 2);
		

		//
		// for debug uesed
		// 
		Pen pen(Gdiplus::Color::Black);
		// Draw a line after rotate

		PointF org[4] = {
			{0,0},{0,str_org_rect.Height},{str_org_rect.Width, str_org_rect.Height},  {str_org_rect.Width, 0}
		};

		PointF org_r[4];
		for (int i = 0; i < 4; i++) {
			org_r[i] = gdi::CaculateRotated(org[i], rotate);
		}
			
		//g2.DrawLines(&pen, org, 4);
		//g2.DrawLines(&pen, org_r, 4);


		RectF myRect = gdi::CaculateOutbound(org_r);
		//g2.DrawRectangle(&pen, myRect);

		// amend myRect
		myRect.Offset(imSize / 2, imSize / 2);

		//
		// end for debug
		//
		
		g2.RotateTransform(rotate);
		Gdiplus::StringFormat string_format;
		string_format.SetAlignment(StringAlignment::StringAlignmentCenter);
		string_format.SetLineAlignment(StringAlignment::StringAlignmentCenter);
		
		// set string
		g2.DrawString(demostr.c_str(), -1, &font,
			Gdiplus::RectF(0,0, str_org_rect.Width, str_org_rect.Height),
			&string_format,
			&brush);

		g2.ResetTransform();



		g2.Flush();

		bk = image.Clone(myRect, PixelFormat32bppARGB);
		if (bk == NULL) {
			//gdi::SaveFileAsBitmap(&image, L"d:\\allTestFile\\bbb.bmp");
			return;
		}
		// save to disk
		/*if (rotate == -2) {
			gdi::SaveFileAsBitmap(bk, L"d:\\allTestFile\\aaaa.bmp");
		}*/
	}

	// since g is the demoWnd, let TextrueBrush to fill this rect

	Gdiplus::TextureBrush tbrush(bk);
	
	g.FillRectangle(&tbrush, layout);

	if (bk != NULL) {
		delete bk;
	}

}




