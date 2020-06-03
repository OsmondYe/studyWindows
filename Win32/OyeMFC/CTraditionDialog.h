#pragma once


// CTraditionDialog dialog

class CTraditionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTraditionDialog)
	int gCounter{ 0 };
public:
	CTraditionDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTraditionDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRADITION_WIN32_BOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_test_button;
	CToolTipCtrl  m_tool_tip;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
//	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedButton1();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
//	CEdit m_folder_name;
	afx_msg void OnBnClickedClearFolder();
	CString m_folder_path;
	afx_msg void OnBnClickedTestWsClipchildren();
//	afx_msg void OnPaint();
	BOOL is_ws_clipchildren;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedButtonHashkeyStudy();
	afx_msg void OnBnClickedBtnAutoinc();
};
