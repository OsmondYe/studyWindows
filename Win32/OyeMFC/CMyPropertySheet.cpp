// CMyPropertySheet.cpp : implementation file
//

#include "pch.h"
#include "OyeMFC.h"
#include "CMyPropertySheet.h"


// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	my_init_and_build();
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	my_init_and_build();
}

CMyPropertySheet::~CMyPropertySheet()
{
}

void CMyPropertySheet::my_init_and_build()
{
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button

	AddPage(new CPropertyPage(IDD_DIALOG_BASE));
	AddPage(new CPropertyPage(IDD_ABOUTBOX));
	AddPage(&myPage);


}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMyPropertySheet message handlers
