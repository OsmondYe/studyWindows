#include "stdafx.h"

#include <boost/algorithm/algorithm.hpp>


#include <madCHook.h>
#include "hook.h"
#include <MsXml6.h>

#include "auto_disable.hpp"

BOOL  (WINAPI *BitBltNext)( HDC hdc,  int x,  int y,  int cx,  int cy, 
					HDC hdcSrc,  int x1,  int y1,  DWORD rop)=NULL;

BOOL  WINAPI MyBitBlt(HDC hdc, int x, int y, int cx, int cy,
	HDC hdcSrc, int x1, int y1, DWORD rop) {
	::OutputDebugString(__FUNCTIONW__ L"\n");

	RECT rc = { x1,y1,cx,cy };

	::DrawText(hdcSrc,
		L"Nextlabs SkyDRM Watermark Overlay", -1,
		&rc,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE
	);

	return BitBltNext(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}


DLL_FUN_EXPORT bool HookForScreenCapture() {
	//BitBlt()

	HookAPI("gdi32.dll", "BitBlt", MyBitBlt, (PVOID*)&BitBltNext);
	
	return true;
}