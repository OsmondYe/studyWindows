
// mainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "studyMFC.h"
#include "mainDlg.h"
#include "afxdialogex.h"

#include "util.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainDlg dialog



CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDYMFC_DIALOG, pParent), m_ui_bk_color(100,255,0,0), m_ui_bk_brush(m_ui_bk_color)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}




void changePicTransparent() {
	Gdiplus::Bitmap* im = Gdiplus::Bitmap::FromFile(L"D:\\allTestFile\\pics\\ccp.jpg");
	if (im == NULL) {
		return;
	}

	auto h = im->GetHeight();
	auto w = im->GetWidth();
	
	BYTE limite = 0xee;

	// change alpha
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			Gdiplus::Color c;
			im->GetPixel(i, j, &c);
			if (c.GetR() > limite&& c.GetG() > limite&& c.GetB() > limite) {
				im->SetPixel(i, j, Gdiplus::Color(0,0,0,0));
			}
			else {
				::OutputDebugString(L"");
			}
		}

	// resize;
	


	// return back
	CLSID clsid;
	GetEncoderClsid(L"image/png", &clsid);
	im->Save(L"D:\\allTestFile\\pics\\ccp2.png", &clsid);
}




BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	changePicTransparent();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		OnMyPaint(CPaintDC(this), rect);
	}
}



void CMainDlg::OnMyPaint(CPaintDC & dc, CRect & rc)
{
	Graphics g(dc);
	util::setCommonParam(g);

	int x_c = 100;
	int y_c = 100;
	int radius = 10;
	int diameter = radius * 2;

	// draw a pic with a circle 
	Bitmap circle(diameter, diameter);
	Gdiplus::Pen pen(Color(255, 0, 0, 0));
	Graphics g_c(&circle);
	util::setCommonParam(g_c);
	g_c.DrawEllipse(&pen, 0, 0, diameter, diameter);


	// using cachedbitmpa
	CachedBitmap cb(&circle, &g);

	g.FillRectangle(&m_ui_bk_brush, util::fromCRect(rc));
	// darw the circle into canvas
	for(int x=0; x<rc.Width(); x+=diameter)
		for (int y = 0; y < rc.Height(); y += diameter) {
			//g.DrawImage(&circle, x, y, diameter, diameter);
			g.DrawCachedBitmap(&cb, x, y);
			
		}


}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

