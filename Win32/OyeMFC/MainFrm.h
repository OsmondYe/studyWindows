#pragma once

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	bool is_model_dialog = false;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBtnQuery();
	afx_msg void OnSyslink2();
	afx_msg void OnMyonedrive();
	afx_msg void OnRmdWindow();
	afx_msg void OnDialogbase();
	afx_msg void OnCheckModelDialog();
	afx_msg void OnUpdateCheckModelDialog(CCmdUI* pCmdUI);
	afx_msg void OnDialogCommon();
	afx_msg void OnCfontdialog();
	afx_msg void OnCfiledialog();
	afx_msg void OnCcolordialog();
	afx_msg void OnCprintdialog();
	afx_msg void OnMydialog();
};


