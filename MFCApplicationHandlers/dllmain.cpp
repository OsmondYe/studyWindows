// dllmain.cpp : Implementation of DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "MFCApplicationHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CMFCApplicationHandlersModule _AtlModule;

class CMFCApplicationHandlersApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMFCApplicationHandlersApp, CWinApp)
END_MESSAGE_MAP()

CMFCApplicationHandlersApp theApp;

BOOL CMFCApplicationHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;
	return CWinApp::InitInstance();
}

int CMFCApplicationHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
