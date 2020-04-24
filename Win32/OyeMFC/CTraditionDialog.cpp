// CTraditionDialog.cpp : implementation file
//

#include "pch.h"
#include "OyeMFC.h"
#include "CTraditionDialog.h"
#include "afxdialogex.h"


// CTraditionDialog dialog

IMPLEMENT_DYNAMIC(CTraditionDialog, CDialogEx)

CTraditionDialog::CTraditionDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRADITION_WIN32_BOX, pParent)
{

}

CTraditionDialog::~CTraditionDialog()
{
}

void CTraditionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_test_button);
}


BEGIN_MESSAGE_MAP(CTraditionDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTraditionDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTraditionDialog message handlers


BOOL CTraditionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableToolTips();
	EnableTrackingToolTips();

	m_test_button.SetWindowTextW(L"give you a new button name");
	//m_test_button.SetState(true);
	
	
	// Tool tip
	m_tool_tip.CreateEx(this, TTS_ALWAYSTIP | TTS_NOPREFIX, WS_EX_TOPMOST);
	m_tool_tip.Activate(true);
	m_tool_tip.SetWindowPos(&wndTopMost, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE );
	
	
	m_tool_tip.AddTool(&m_test_button,L"this is a button");

	TOOLINFO ti;
	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	ti.hwnd = m_hWnd;
	ti.uId = (int)m_hWnd;
	ti.hinst = AfxGetApp()->m_hInstance;
	ti.lpszText = L"this is good";	
	GetClientRect(&ti.rect);

	m_tool_tip.SetToolInfo(&ti);

	//m_tool_tip.Activate(true);  // tooltip must call RelayEvent??????


	// TODO:  Add your specialized creation code here
	//AnimateWindow(500, AW_SLIDE | AW_HOR_NEGATIVE);
	//FlashWindowEx(FLASHW_ALL, 10, 1000);

	return TRUE;  
				  
}


BOOL CTraditionDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_LBUTTONUP ||
		pMsg->message == WM_MOUSEMOVE)
	{
		m_tool_tip.Activate(true);
		m_tool_tip.RelayEvent(pMsg);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTraditionDialog::OnClose()
{
	AnimateWindow(500, AW_HIDE | AW_HOR_POSITIVE);
	CDialogEx::OnClose();
}


void CTraditionDialog::OnBnClickedButton1()
{
	m_tool_tip.ShowWindow(SW_SHOW);

	//m_test_button.ShowWindow(SW_HIDE);
}
