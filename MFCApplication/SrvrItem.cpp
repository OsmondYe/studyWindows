// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SrvrItem.cpp : implementation of the CMFCApplicationSrvrItem class
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication.h"

#include "MFCApplicationDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplicationSrvrItem implementation

IMPLEMENT_DYNAMIC(CMFCApplicationSrvrItem, COleServerItem)

CMFCApplicationSrvrItem::CMFCApplicationSrvrItem(CMFCApplicationDoc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO: add one-time construction code here
	//  (eg, adding additional clipboard formats to the item's data source)
}

CMFCApplicationSrvrItem::~CMFCApplicationSrvrItem()
{
	// TODO: add cleanup code here
}

void CMFCApplicationSrvrItem::Serialize(CArchive& ar)
{
	// CMFCApplicationSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CMFCApplicationDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
			pDoc->Serialize(ar);
	}
}

BOOL CMFCApplicationSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Most applications, like this one, only handle drawing the content
	//  aspect of the item.  If you wish to support other aspects, such
	//  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
	//  implementation of OnGetExtent should be modified to handle the
	//  additional aspect(s).

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CMFCApplicationSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  The default implementation
	//  here simply returns a hard-coded number of units.

	// TODO: replace this arbitrary size

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC units

	return TRUE;
}

BOOL CMFCApplicationSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);

	// TODO: set mapping mode and extent
	//  (The extent is usually the same as the size returned from OnGetExtent)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO: add drawing code here.  Optionally, fill in the HIMETRIC extent.
	//  All drawing takes place in the metafile device context (pDC).

	return TRUE;
}


// CMFCApplicationSrvrItem diagnostics

#ifdef _DEBUG
void CMFCApplicationSrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CMFCApplicationSrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

