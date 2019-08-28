#pragma once

using namespace std;

class MainDialog : public CDialogImpl<MainDialog>,
					public CWinDataExchange<MainDialog>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	void OnListDBClick(UINT uNotifyCode, int nID, CWindow wndCtl) {
		ShowWatermarkDemo();
	}

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

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
			m_wList.AddString(name);
		}

		delete[] buf;
		return rt;
	}

	BOOL OnEraseBkgnd(CDCHandle dc);

	void OnClose() {
		DestroyWindow();
	}

	void OnDestroy() {
		::PostQuitMessage(0);
	}


private:

	inline wstring GetSelFontName() {
		wstring font_name;
		int curSel = m_wList.GetCurSel();
		font_name.reserve(m_wList.GetTextLen(curSel));
		m_wList.GetText(curSel, (wchar_t*)font_name.c_str());
		return std::move(font_name);
	}

	void ShowWatermarkDemo();

protected:
	CEdit m_wEdit;
	CListBox m_wList;
	CStatic m_wFontDemo;
	CTrackBarCtrl m_ColorA;
	CTrackBarCtrl m_ColorR;
	CTrackBarCtrl m_ColorG;
	CTrackBarCtrl m_ColorB;

	// DDE
	CString m_EditContent;
	int m_FontSize;

	// Color{A,R,G,B}
	int m_FontColorA;
	int m_FontColorR;
	int m_FontColorG;
	int m_FontColorB;
public:
	BEGIN_MSG_MAP_EX(MainDialog)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_DESTROY(OnDestroy)
		//MSG_WM_ERASEBKGND(OnEraseBkgnd)
		COMMAND_HANDLER_EX(IDC_FONTLIST, LBN_DBLCLK, OnListDBClick)
		MSG_WM_HSCROLL(OnHScroll)

		END_MSG_MAP()

	// ddx
	BEGIN_DDX_MAP(MainDialog)
		//DDX_CONTROL(IDC_MYEDIT, m_wEdit)
		DDX_TEXT(IDC_MYEDIT, m_EditContent)
		DDX_INT(IDC_FONTSIZE, m_FontSize)
	END_DDX_MAP()
};




