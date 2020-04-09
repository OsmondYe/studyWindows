// CMyTaskDialogDemo.cpp : implementation file
//

#include "pch.h"
#include "OyeMFC.h"
#include "CMyTaskDialogDemo.h"
#include "afxdialogex.h"


// for task_dialog  vista above feature

#include<afxtaskdialog.h>


// CMyTaskDialogDemo dialog

IMPLEMENT_DYNAMIC(CMyTaskDialogDemo, CDialogEx)

CMyTaskDialogDemo::CMyTaskDialogDemo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYTASK_DIALOG, pParent)
{

}

CMyTaskDialogDemo::~CMyTaskDialogDemo()
{
}

void CMyTaskDialogDemo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyTaskDialogDemo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyTaskDialogDemo::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyTaskDialogDemo message handlers

void CMyTaskDialogDemo::OnBnClickedButton1()
{
	CTaskDialog dlg(
		L"this is the content ,TaskDialog is Vista above feature",
		L"what's the main instruction mean?",
		L"this is the Title",
		TDCBF_YES_BUTTON | TDCBF_NO_BUTTON,
		TDF_ENABLE_HYPERLINKS | TDF_ALLOW_DIALOG_CANCELLATION	,	
		L"what's the footer mean?"
	);

	dlg.SetMainIcon( AfxGetApp()->LoadIconW(IDR_MAINFRAME));

	dlg.DoModal();
}
