#pragma once


// CMyPropertyPage dialog

class CMyPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPropertyPage)

public:
	CMyPropertyPage();
	virtual ~CMyPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPROPERTY_PAGE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CStatic m_pic;
	CString m_path;
	virtual BOOL OnInitDialog();
};
