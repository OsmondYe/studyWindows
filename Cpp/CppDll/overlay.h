#pragma once

#include<Windows.h>
#define DLL_FUN_EXPORT extern"C" __declspec(dllexport)


DLL_FUN_EXPORT void InitWaterMark(HWND targertWnd, wchar_t* strOverlay);

DLL_FUN_EXPORT void UpdateWaterMark(HWND targertWnd);

DLL_FUN_EXPORT void DeleteWaterMark(HWND targertWnd);
