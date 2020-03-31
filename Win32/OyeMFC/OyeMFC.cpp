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
END_MESSAGE_MAP()


// COyeApp construction

COyeApp::COyeApp() noexcept
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

	SetAppID(_T("SkyCode.OyeTool.1"));

}

// The one and only COyeApp object

COyeApp theApp;


// COyeApp initialization

BOOL COyeApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_OyeMFCTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_OyeMFCTYPE));




	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
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

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void COyeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COyeApp customization load/save methods

void COyeApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COyeApp::LoadCustomState()
{
}

void COyeApp::SaveCustomState()
{
}

// COyeApp message handlers



