// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "OyeMFC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_BTN_QUERY, &CMainFrame::OnBtnQuery)
	ON_COMMAND(ID_SYSLINK2, &CMainFrame::OnSyslink2)
	ON_COMMAND(ID_MYONEDRIVE, &CMainFrame::OnMyonedrive)
	ON_COMMAND(ID_RMDWIN, &CMainFrame::OnRmdWindow)
	ON_COMMAND(ID_Dialog_base, &CMainFrame::OnDialogbase)
	ON_COMMAND(ID_CHECK_MODEL_DIALOG, &CMainFrame::OnCheckModelDialog)
	ON_COMMAND(ID_DLG_COMMON, &CMainFrame::OnDialogCommon)
	ON_COMMAND(ID_CFontDialog, &CMainFrame::OnCfontdialog)
	ON_COMMAND(ID_CFileDialog, &CMainFrame::OnCfiledialog)
	ON_COMMAND(ID_CColorDialog, &CMainFrame::OnCcolordialog)
	ON_COMMAND(ID_CPrintDialog, &CMainFrame::OnCprintdialog)
	ON_COMMAND(ID_MyDialog, &CMainFrame::OnMydialog)
	ON_COMMAND(ID_CHECK_EnableMDITabbedGroups, &CMainFrame::OnCheckEnablemditabbedgroups)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MODEL_DIALOG, &CMainFrame::OnUpdateCheckModelDialog)
	ON_UPDATE_COMMAND_UI(ID_CHECK_EnableMDITabbedGroups, &CMainFrame::OnUpdateCheckEnablemditabbedgroups)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::init_status_bar()
{

	CString strTitlePane1;
	CString strTitlePane2;
	strTitlePane1.LoadString(IDS_STATUS_PANE1);
	strTitlePane2.LoadString(IDS_STATUS_PANE2);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	m_wndStatusBar.Create(this);
}

void CMainFrame::init_ribbon_bar()
{
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	m_wndRibbonBar.SetWindows7Look(FALSE);
	
}

void CMainFrame::init_docking()
{
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
}

void CMainFrame::init_style()
{
	// set the visual manager and style based on persisted value
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);
	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	init_style();
	init_docking();
	
	init_status_bar();
	init_ribbon_bar();

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.lpszName = L"CMainFrame::MainFrm.cpp";

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnBtnQuery()
{
	MessageBox(L"OnBtnQuery");
	// TODO: Add your command handler code here
}

void CMainFrame::OnSyslink2()
{
	CMFCRibbonLinkCtrl* pelem = (CMFCRibbonLinkCtrl*)m_wndRibbonBar.FindByID(ID_SYSLINK2);
	pelem->OpenLink();
}

void CMainFrame::OnMyonedrive()
{
	CMFCRibbonLinkCtrl* pelem = (CMFCRibbonLinkCtrl*)m_wndRibbonBar.FindByID(ID_MYONEDRIVE);
	pelem->OpenLink();
}

void CMainFrame::OnRmdWindow()
{
	CMFCRibbonLinkCtrl* pelem = (CMFCRibbonLinkCtrl*)m_wndRibbonBar.FindByID(ID_RMDWIN);
	pelem->OpenLink();
}

void CMainFrame::OnDialogbase()
{
	if (is_model_dialog) {
		// model
		CDialog dia(IDD_DIALOG_BASE);
		dia.DoModal();
	}
	else {
		// modeless
		CDialog* pdia2 = new CDialog();
		pdia2->Create(IDD_DIALOG_BASE);
		pdia2->ShowWindow(SW_NORMAL);
	}
}

void CMainFrame::OnCheckModelDialog()
{
	is_model_dialog = is_model_dialog ? false : true;
}

void CMainFrame::OnUpdateCheckModelDialog(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(is_model_dialog);
}

void CMainFrame::OnDialogCommon()
{
	CCommonDialog* p = new CFontDialog();
	p->DoModal();
	

	// TODO: Add your command handler code here
}

void CMainFrame::OnCfontdialog()
{
	// TODO: Add your command handler code here
	CFontDialog x;
	x.DoModal();
}

void CMainFrame::OnCfiledialog()
{
	CFileDialog x(true);
	x.DoModal();
}

void CMainFrame::OnCcolordialog()
{
	CColorDialog x;
	x.DoModal();
}

void CMainFrame::OnCprintdialog()
{
	CPrintDialog x(true);
	x.DoModal();


	CPrintDialogEx x2;
	x2.DoModal();

}

void CMainFrame::OnMydialog()
{
	CDialogEx x(IDD_DIALOG_BASE);

	CPngImage img;
	img.Load(IDB_CAPTURE);

	x.SetBackgroundImage(img);

	x.DoModal();
}

void CMainFrame::OnCheckEnablemditabbedgroups()
{
	is_enable_mdi_tabbed_groups = is_enable_mdi_tabbed_groups ? false : true;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(is_enable_mdi_tabbed_groups, mdiTabParams);
}

void CMainFrame::OnUpdateCheckEnablemditabbedgroups(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(is_enable_mdi_tabbed_groups);
}
