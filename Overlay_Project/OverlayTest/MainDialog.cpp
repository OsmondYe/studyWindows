#include "stdafx.h"
#include "MainDialog.h"

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
		ShowWatermarkDemo();
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

	m_FontColorA = 50;
	m_FontColorR = 50;
	m_FontColorG = 50;
	m_FontColorB = 50;

	EnumGDIPlugInstalledFont();
	m_wList.SetCurSel(1);


	m_ColorA.SetRange(0, 255); m_ColorA.SetPos(m_FontColorA);
	m_ColorR.SetRange(0, 255); m_ColorR.SetPos(m_FontColorR);
	m_ColorG.SetRange(0, 255); m_ColorG.SetPos(m_FontColorG);
	m_ColorB.SetRange(0, 255); m_ColorB.SetPos(m_FontColorB);


	m_EditContent=L"This is test watermark by Nextlabs-CDC\n测试窗口水印-缩放拉伸最大最小显示隐藏";
	m_FontSize = 20;

	DoDataExchange(false);

	return True;
}

BOOL MainDialog::OnEraseBkgnd(CDCHandle dc) {
	CRect rc;
	GetClientRect(&rc);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	CBrush old = dc.SelectBrush(brush);
	dc.PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
	return true;
}


void MainDialog::ShowWatermarkDemo() {
	
	DoDataExchange(true);

	wstring demostr(m_EditContent);	
	wstring font_name = GetSelFontName();

	m_wFontDemo.RedrawWindow();

	using namespace Gdiplus;

	CRect rc;
	m_wFontDemo.GetWindowRect(&rc);
	Graphics g(m_wFontDemo.GetDC());
	FontFamily font_family(font_name.c_str());
	Font font(&font_family, m_FontSize, FontStyle::FontStyleBold, Unit::UnitPixel);
	SolidBrush brush(Color(m_FontColorA, m_FontColorR, m_FontColorG, m_FontColorB));
	RectF layout(0, 0, rc.Width(), rc.Height());
	StringFormat string_format;

	g.DrawString(demostr.c_str(), -1, &font, layout, &string_format, &brush);
	
}



