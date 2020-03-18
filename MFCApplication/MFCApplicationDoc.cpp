#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication.h"
#endif

#include "MFCApplicationDoc.h"
#include "SrvrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplicationDoc

IMPLEMENT_DYNCREATE(CMFCApplicationDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CMFCApplicationDoc, COleServerDoc)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CMFCApplicationDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CMFCApplicationDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMFCApplicationDoc, COleServerDoc)
END_DISPATCH_MAP()

// Note: we add support for IID_IMFCApplication to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {faa7acdb-56e2-45eb-a130-11f9ff12a57f}
static const IID IID_IMFCApplication =
{0xfaa7acdb,0x56e2,0x45eb,{0xa1,0x30,0x11,0xf9,0xff,0x12,0xa5,0x7f}};

BEGIN_INTERFACE_MAP(CMFCApplicationDoc, COleServerDoc)
	INTERFACE_PART(CMFCApplicationDoc, IID_IMFCApplication, Dispatch)
END_INTERFACE_MAP()


// CMFCApplicationDoc construction/destruction

CMFCApplicationDoc::CMFCApplicationDoc() noexcept
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CMFCApplicationDoc::~CMFCApplicationDoc()
{
	AfxOleUnlockApp();
}

BOOL CMFCApplicationDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CMFCApplicationDoc server implementation

COleServerItem* CMFCApplicationDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CMFCApplicationSrvrItem* pItem = new CMFCApplicationSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}




// CMFCApplicationDoc serialization

void CMFCApplicationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCApplicationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCApplicationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCApplicationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplicationDoc diagnostics

#ifdef _DEBUG
void CMFCApplicationDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CMFCApplicationDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG


// CMFCApplicationDoc commands
