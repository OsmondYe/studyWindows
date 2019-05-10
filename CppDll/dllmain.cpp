// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <atlbase.h>
#include <atlapp.h>
#include "hook.h"

#include "watermark.h"

#pragma unmanaged

ViewOverlyController* p;

void __cdecl Thread2(void*) {
	p = new ViewOverlyController();
	p->SetOverlyTargetOnTopLevel(
		"HwndWrapper[Viewer.exe;;b590c697-bc9d-4ffe-9ae4-45940df9428d]",
		NULL);
	int bRet;
	MSG msg;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hModule);
		//HookIXmlHttpRequestInterface();
		//HookForScreenCapture();
		::OutputDebugString(L"DLL_PROCESS_ATTACH");
		//_CrtDbgBreak();
		_beginthread(Thread2, NULL, NULL);
		
		
		break;
    case DLL_PROCESS_DETACH:
		::OutputDebugString(L"DLL_PROCESS_DETACH");
		delete p;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}

