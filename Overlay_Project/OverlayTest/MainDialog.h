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

	void OnClose() {
		KillTimer(1);
		EndDialog(0);
	}

	void OnTimer(UINT_PTR nIDEvent) {OnTimer_Mouse();}

	void OnTimer_Mouse();

	void OnDrawBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnClearBtnDown(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnShowColorEffect(BYTE a, BYTE r, BYTE g, BYTE b);

private:

	void FillBuilder(OverlayConfigBuilder& builder);

	inline wstring GetSelFontName() {
		wstring font_name;
		int curSel = m_wList.GetCurSel();
		font_name.reserve(m_wList.GetTextLen(curSel));
		m_wList.GetText(curSel, (wchar_t*)font_name.c_str());
		return std::wstring(font_name.c_str());
	}

	void ShowWatermarkDemo();

	void CreatePngImageWithWaterMark();

	inline Gdiplus::Color GetFontColor() {return Gdiplus::Color(m_FontColorA, m_FontColorR, m_FontColorG, m_FontColorB);}


protected:
	CEdit m_wEdit;
	CEdit m_wMousePos;
	CListBox m_wList;
	CStatic m_wFontDemo;
	CTrackBarCtrl m_ColorA;
	CTrackBarCtrl m_ColorR;
	CTrackBarCtrl m_ColorG;
	CTrackBarCtrl m_ColorB;

	// DDE
	CString m_EditContent;
	int m_FontSize;
	int m_FontRotate;

	// Color{A,R,G,B}
	int m_FontColorA;
	int m_FontColorR;
	int m_FontColorG;
	int m_FontColorB;
public:
	BEGIN_MSG_MAP_EX(MainDialog)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_TIMER(OnTimer)

		COMMAND_HANDLER_EX(IDC_BUTTON1, BN_CLICKED, OnDrawBtnDown)
		COMMAND_HANDLER_EX(IDC_BUTTON2, BN_CLICKED, OnClearBtnDown)
		COMMAND_HANDLER_EX(IDC_FONTLIST, LBN_DBLCLK, OnListDBClick)

		MSG_WM_HSCROLL(OnHScroll)

		END_MSG_MAP()

	// ddx
	BEGIN_DDX_MAP(MainDialog)
		DDX_TEXT(IDC_MYEDIT, m_EditContent)
		DDX_INT(IDC_FONTSIZE, m_FontSize)
		DDX_INT(IDC_FONTROTATE, m_FontRotate)
	END_DDX_MAP()
	
};




