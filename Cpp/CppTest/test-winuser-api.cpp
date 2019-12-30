#include "stdafx.h"
// user32.dll
#include <WinUser.h>

BOOL CALLBACK Callback_EnumTopWnd(HWND wnd, LPARAM obj) {
	if (::IsWindow(wnd) && ::IsWindowVisible(wnd)) {
		std::set<HWND>* hs = (std::set<HWND>*)obj;
		hs->insert(wnd);
	}
	return true;
}

void extractIcon(const std::set<HWND>& hs) {
	for (auto h : hs) {
			HICON icon = (HICON)::GetClassLongW(h, GCLP_HICON);
			Gdiplus::Bitmap bm(icon);
			CLSID png;
			GetEncoderClsid(L"image/png", &png);
			wstring name = std::to_wstring((int)h) + L".png";
			bm.Save(name.c_str(), &png);
	}
}

TEST(User32, DISABLE_RetriveWindowIcon) {
	// using EnumWindows + GetClassLong + Gdi_plus_bitmap
	std::set<HWND> hs;
	::EnumWindows(Callback_EnumTopWnd, (LPARAM)&hs);
	cout << hs.size() << endl;
	extractIcon(hs);

}

TEST(User32, WndThreadProcessID) {
	std::set<HWND> hs;
	::EnumWindows(Callback_EnumTopWnd, (LPARAM)&hs);
	for (auto h : hs) {
		DWORD pid = -1;
		DWORD tid = ::GetWindowThreadProcessId(h, &pid);
		cout << hex << showbase << "hwnd:" << (DWORD)h << "\tpid:" << pid << "\ttid:" << tid << endl;
	}
}

TEST(User32, DrawWindowRect) {

}