#pragma once


// CMyTaskDialogDemo dialog

class CMyTaskDialogDemo : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTaskDialogDemo)

public:
	CMyTaskDialogDemo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMyTaskDialogDemo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
