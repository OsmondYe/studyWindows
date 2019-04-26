#pragma once

#ifndef DLL_FUN_EXPORT
#define DLL_FUN_EXPORT extern"C" __declspec(dllexport)
#endif 


DLL_FUN_EXPORT bool HookIXmlHttpRequestInterface();



DLL_FUN_EXPORT bool HookForScreenCapture();