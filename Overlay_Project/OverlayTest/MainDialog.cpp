#include "stdafx.h"
#include "MainDialog.h"
#include "gdi_util.h"


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
	m_FontColorR = 200;
	m_FontColorG = 50;
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


	m_EditContent=L"Watermark by Nextlabs-CDC\r\n缩放拉伸最大最小显示隐藏\r\nOsmond.ye@Nextlabs.com";
	m_FontSize = 20;
	m_FontRotate = 30;

	DoDataExchange(false);

	return True;
}

void MainDialog::OnDrawBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	ShowWatermarkDemo();
}

void MainDialog::OnClearBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CWindow desktop(::GetDesktopWindow());
	desktop.SendMessageW(WM_SETTINGCHANGE);
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

void MainDialog::ShowWatermarkDemo() {
	//DoDataExchange(true);

	//wstring demostr(m_EditContent);	
	//wstring font_name = GetSelFontName();

	//m_wFontDemo.RedrawWindow();
	//
	//CRect rc;
	//m_wFontDemo.GetWindowRect(&rc);


	//using namespace Gdiplus;
	//Gdiplus::Graphics g(m_wFontDemo.GetDC());
	//FontFamily font_family(font_name.c_str());
	//Font font(&font_family, m_FontSize, FontStyle::FontStyleBold, Unit::UnitPixel);
	//SolidBrush brush(Color(m_FontColorA, m_FontColorR, m_FontColorG, m_FontColorB));
	//Gdiplus::RectF layout(0, 0, rc.Width(), rc.Height());
	//StringFormat string_format;

	//Pen pen(Color(255, 0, 0));

	//g.DrawRectangle(&pen, layout);

	//// calc needed layout

	//Gdiplus::RectF outLayout;
	//g.MeasureString(demostr.c_str(), -1, &font, layout, &outLayout);
	//g.DrawRectangle(&pen, outLayout);



	////REAL X = outLayout.Width / 2, Y = outLayout.Height / 2;
	//g.RotateTransform(m_FontRotate);
	//Gdiplus::RectF outLayout2;
	//g.MeasureString(demostr.c_str(), -1, &font, layout, &outLayout2);
	//g.DrawRectangle(&pen, outLayout2);


	//g.DrawString(demostr.c_str(), -1, &font, layout, &string_format, &brush);



	CreatePngImageWithWaterMark();
	
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

	desktop.RedrawWindow();
	Graphics g(desktop);

	// set params
	Color font_color(colorA, colorR, colorG, colorB);
	Pen pen(font_color);
	SolidBrush brush(font_color);
	Gdiplus::FontFamily font_family(font_name.c_str());
	Gdiplus::Font font(&font_family, fontSize, FontStyle::FontStyleBold, Unit::UnitPixel);
	Gdiplus::RectF layout(0, 0, rc.Width(), rc.Height());
	// calc str outbound rect
	Gdiplus::RectF strRect;
	g.MeasureString(demostr.c_str(), -1, &font, PointF(0,0), &strRect);

	REAL imSize = std::max<REAL>(strRect.Width, strRect.Height)*2;

	// how to determine rotated outBound size

	Gdiplus::Bitmap* bk = NULL;

	Gdiplus::Bitmap image(imSize, imSize);
	{
		Graphics g2(&image);
		{
			// Draw coordinate
			// X
			g2.DrawLine(&pen, PointF(0, imSize / 2), PointF(imSize, imSize / 2));
			// Y
			g2.DrawLine(&pen, PointF(imSize / 2, 0), PointF(imSize / 2, imSize));
		}

		

		g2.TranslateTransform(imSize / 2, imSize / 2);
		

		//
		// for debug uesed
		// 
		Pen pen(Color(255, 255, 0, 0));
		// Draw a line after rotate

		PointF org[4] = {
			{0,0},{0,strRect.Height},{strRect.Width, strRect.Height},  {strRect.Width, 0}
		};

		PointF org_r[4];
		for (int i = 0; i < 4; i++) {
			org_r[i] = gdi::CaculateRotated(org[i], rotate);
		}
			
		g2.DrawLines(&pen, org, 4);
		g2.DrawLines(&pen, org_r, 4);


		RectF myRect = gdi::CaculateOutbound(org_r);
		g2.DrawRectangle(&pen, myRect);

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
			Gdiplus::RectF(0,0, strRect.Width, strRect.Height),
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
		if (rotate == 70) {
			//gdi::SaveFileAsBitmap(bk, L"d:\\allTestFile\\aaaa.bmp");
		}
	}



	// since g is the demoWnd, let TextrueBrush to fill this rect
	


	Gdiplus::TextureBrush tbrush(bk);
	
	g.FillRectangle(&tbrush, layout);

	if (bk != NULL) {
		delete bk;
	}


	// how to tell watermark to update its overlay?

}


