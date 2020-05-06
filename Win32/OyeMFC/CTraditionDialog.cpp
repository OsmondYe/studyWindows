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
	, m_folder_path(_T(""))
	, is_ws_clipchildren(FALSE)
{

}

CTraditionDialog::~CTraditionDialog()
{
}

void CTraditionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_test_button);
	//  DDX_Control(pDX, IDC_EDIT_FOLDER, m_folder_name);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_folder_path);
	DDX_Check(pDX, IDC_CHECK_WS_CLIPCHILDREN, is_ws_clipchildren);
}


BEGIN_MESSAGE_MAP(CTraditionDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTraditionDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CLEAR_FOLDER, &CTraditionDialog::OnBnClickedClearFolder)
	ON_BN_CLICKED(IDC_TEST_WS_CLIPCHILDREN, &CTraditionDialog::OnBnClickedTestWsClipchildren)
//	ON_WM_PAINT()
ON_BN_CLICKED(IDC_BUTTON4, &CTraditionDialog::OnBnClickedButton4)
ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CTraditionDialog message handlers


BOOL CTraditionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableToolTips();
	EnableTrackingToolTips();

	
	// Tool tip
	m_tool_tip.CreateEx(this, TTS_ALWAYSTIP | TTS_NOPREFIX, WS_EX_TOPMOST);
	m_tool_tip.Activate(true);
	m_tool_tip.SetWindowPos(&wndTopMost, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE );
	
	
	m_tool_tip.AddTool(&m_test_button,L"this is a button by call m_tool_tip.AddTool");

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

static const int HotKeyID = 0x0100;

void CTraditionDialog::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default
	if (nHotKeyId == HotKeyID) {
		//this->SetFocus();
		CenterWindow(CWnd::GetDesktopWindow());
		// need to check the source code
		ActivateTopParent();
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CTraditionDialog::OnBnClickedButton1()
{
	m_tool_tip.ShowWindow(SW_SHOW);

	auto x = RegisterHotKey(m_hWnd, HotKeyID, MOD_CONTROL | MOD_SHIFT, 0x50);

	//m_test_button.ShowWindow(SW_HIDE);
}


INT_PTR CTraditionDialog::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::OnToolHitTest(point, pTI);
}

// Demo: how to clear a folder's all child files
void CTraditionDialog::OnBnClickedClearFolder()
{
	UpdateData();

	using namespace ATL;
	CComPtr<IFileOperation> spFO;

	if (FAILED(spFO.CoCreateInstance(__uuidof(FileOperation)))) {
		return;
	}
	spFO->SetOperationFlags(FOF_NO_UI);
	// clear folder contents
	CComPtr<IShellItem> spItem;
	CComPtr<IEnumShellItems> spItems;
	if (FAILED(::SHCreateItemFromParsingName(m_folder_path, NULL, __uuidof(IShellItem), (void**)&spItem))) {
		return;
	}
	if (FAILED(spItem->BindToHandler(NULL, BHID_EnumItems, _uuidof(IEnumShellItems), (void**)&spItems))) {
		return;
	}
	if (FAILED(spFO->DeleteItems(spItems))) {
		return;
	}
	spFO->PerformOperations();
}


// Window Style:  WS_CLIPCHILDREN
//  - 绘制时,子窗口显示效果不受影响,但实测必须是CClientDC才行
void CTraditionDialog::OnBnClickedTestWsClipchildren()
{
	UpdateData();
	if (is_ws_clipchildren) {
		ModifyStyle(0, WS_CLIPCHILDREN);		//add
	}
	else {
		ModifyStyle(WS_CLIPCHILDREN,0);		//remove
	}
	// 全部重绘

	//Invalidate();
	CRect rc;
	GetClientRect(rc);
	//CWindowDC dc(this);
	CClientDC dc(this);

	CString str = L"Hello World";
	CSize size = dc.GetTextExtent(str);

	for (int x = 0; x < rc.Width(); x += size.cx + 10) {
		for (int y = 0; y < rc.Height(); y += size.cy + 10) {
			dc.TextOutW(x, y, L"hello world");
		}
	}

	// 这三个兄弟到底啥区别
	//Invalidate();
	//RedrawWindow();
	//InvalidateRect(rc);
}


//void CTraditionDialog::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: Add your message handler code here
//					   // Do not call CDialogEx::OnPaint() for painting messages
//}

bool bTimerSet = false;

void CALLBACK MyTimer(HWND hwnd, UINT, UINT_PTR, DWORD) {
	CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));
	//CClientDC dc(CWnd::FromHandle(hwnd));

	CRect rc(100, 100, 500, 500);
	
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format(_T("%T,  %A, %B %d, %Y"));
	dc.Draw3dRect(rc, RGB(255, 0, 0), RGB(0, 255, 0));
	//dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 255));
	dc.DrawText(s, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CTraditionDialog::OnBnClickedButton4()
{
	if (!bTimerSet) {
		bTimerSet = true;
		SetTimer(1, 200, MyTimer);
	}
	else {
		bTimerSet = false;
		KillTimer(1);
	}
	// TODO: Add your control notification handler code here
}



