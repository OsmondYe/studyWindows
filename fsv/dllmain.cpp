/* 
sv : system visualization
    file visulization
    registry visualization


    hook at ntdll.dll as user_level tiny one
        NtCreatefile
        NtOpenFile

History:



*/
#include "framework.h"

#include "tls.h"

// using printf, sprintf 
#pragma comment(lib, "legacy_stdio_definitions.lib")




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        tls_init();
        return true;
    case DLL_THREAD_ATTACH:
        
    case DLL_THREAD_DETACH:
        // call tls_freedata() here
        return true;
    case DLL_PROCESS_DETACH:
        tls_deinit();
        return true;
    }
    return TRUE;
}

