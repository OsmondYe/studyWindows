// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <atlbase.h>
#include <atlapp.h>
#include "hook.h"

#include "watermark.h"

#pragma unmanaged



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
		::OutputDebugString(L"DLL_PROCESS_ATTACH");
		HookForScreenCapture();
		//_CrtDbgBreak();
		//_beginthread(Thread2, NULL, NULL);
		
		
		break;
    case DLL_PROCESS_DETACH:
		::OutputDebugString(L"DLL_PROCESS_DETACH");
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}

