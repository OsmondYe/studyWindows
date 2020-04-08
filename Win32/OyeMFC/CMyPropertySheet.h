#pragma once

#include "CMyPropertyPage.h"

// CMyPropertySheet

class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

public:
	CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CMyPropertySheet();

private:
	void my_init_and_build();

	CMyPropertyPage  myPage;

protected:
	DECLARE_MESSAGE_MAP()
};


