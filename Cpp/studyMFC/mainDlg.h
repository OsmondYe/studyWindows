
// mainDlg.h : header file
//

#pragma once


// CMainDlg dialog
class CMainDlg : public CDialogEx
{
// Construction
public:
	CMainDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDYMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual void OnMyPaint(CPaintDC& dc, CRect& rc);



private:
	Gdiplus::Color m_ui_bk_color;
	SolidBrush m_ui_bk_brush;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
