// CMyPropertyPage.cpp : implementation file
//

#include "pch.h"
#include "OyeMFC.h"
#include "CMyPropertyPage.h"
#include "afxdialogex.h"


// CMyPropertyPage dialog

IMPLEMENT_DYNAMIC(CMyPropertyPage, CPropertyPage)

CMyPropertyPage::CMyPropertyPage()
	: CPropertyPage(IDD_MYPROPERTY_PAGE)
	, m_path(_T(""))
{

}

CMyPropertyPage::~CMyPropertyPage()
{
}

void CMyPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BITMAP_SHOW, m_pic);
	DDX_Text(pDX, IDC_EDIT1, m_path);
}


BEGIN_MESSAGE_MAP(CMyPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyPropertyPage::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyPropertyPage message handlers


void CMyPropertyPage::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog open(true, L"png", L"*.png",
		OFN_HIDEREADONLY);
	if (open.DoModal() == IDOK) {
		m_path = open.GetPathName();

		CPngImage img;
		img.LoadFromFile(m_path);
		m_pic.SetBitmap(img);
		
		UpdateData(FALSE);
	}
}


BOOL CMyPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Init some controls and values

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
