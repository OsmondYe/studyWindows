// OyeMFC.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OyeMFC.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COyeApp

BEGIN_MESSAGE_MAP(COyeApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COyeApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &COyeApp::OnFileNew)
	ON_COMMAND(ID_APP_EXIT, &COyeApp::OnAppExit)
END_MESSAGE_MAP()

COyeApp::COyeApp() noexcept :m_hMDIAccel(NULL), m_hMDIMenu(NULL)
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	SetAppID(_T("SkyCode.OyeTool.1"));
}

COyeApp theApp;

// COyeApp initialization

BOOL COyeApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	// Initialize OLE libraries
	AfxOleInit();
	AfxEnableControlContainer();
	EnableTaskbarInteraction();
	AfxInitRichEdit2();
	SetRegistryKey(_T("SkyCode"));
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// create the main windows here
	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_OyeMFCTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));

	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWNORMAL);
	pFrame->UpdateWindow();

	return TRUE;
}

int COyeApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != nullptr)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != nullptr)
		FreeResource(m_hMDIAccel);

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// COyeApp message handlers

void COyeApp::OnFileNew()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	pFrame->LockWindowUpdate();
	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_OyeMFCTYPE, m_hMDIMenu, m_hMDIAccel);
	pFrame->UnlockWindowUpdate();
}



// App command to run the dialog
void COyeApp::OnAppAbout()
{
	CDialogEx aboutDlg(IDD_ABOUTBOX);
	aboutDlg.DoModal();
}

// COyeApp customization load/save methods

void COyeApp::PreLoadState()
{
	CString strName;
	strName.LoadString(IDS_EDIT_MENU);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COyeApp::LoadCustomState()
{
}

void COyeApp::SaveCustomState()
{
}


BOOL COyeApp::PreTranslateMessage(MSG* pMsg)
{
	if(m_pMainWnd&& ::TranslateAccelerator(m_pMainWnd->m_hWnd, m_hMDIAccel, pMsg)){
	// osmond, add here to support acceletator 
		return true;
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}


void COyeApp::OnAppExit()
{
	// TODO: Add your command handler code here
	this->m_pMainWnd->DestroyWindow();
}
