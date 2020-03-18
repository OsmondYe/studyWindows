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

// SrvrItem.h : interface of the CMFCApplicationSrvrItem class
//

#pragma once

class CMFCApplicationSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CMFCApplicationSrvrItem)

// Constructors
public:
	CMFCApplicationSrvrItem(CMFCApplicationDoc* pContainerDoc);

// Attributes
	CMFCApplicationDoc* GetDocument() const
		{ return reinterpret_cast<CMFCApplicationDoc*>(COleServerItem::GetDocument()); }

// Overrides
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// Implementation
public:
	~CMFCApplicationSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

