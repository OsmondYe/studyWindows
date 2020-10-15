
#include "stdafx.h"
#include <WinUser.h>




BOOL CALLBACK Callback_EnumTopWnd(HWND wnd, LPARAM obj) {
	if (::IsWindow(wnd) && ::IsWindowVisible(wnd)) {
		std::set<HWND>* hs = (std::set<HWND>*)obj;
		hs->insert(wnd);
	}
	return true;
}

void extractIcon(const std::set<HWND>& hs) {
	//for (auto h : hs) {
	//		HICON icon = (HICON)::GetClassLongW(h, GCLP_HICON);
	//		Gdiplus::Bitmap bm(icon);
	//		CLSID png;
	//		GetEncoderClsid(L"image/png", &png);
	//		wstring name = std::to_wstring((int)h) + L".png";
	//		bm.Save(name.c_str(), &png);
	//}
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


void* Printer_Enum(DWORD flag, wchar_t* name, DWORD level, DWORD& nCount) {
	DWORD needed=0;
	nCount = 0;

	::EnumPrinters(flag, name,level, NULL, 0, &needed, &nCount);
	if (needed == 0) {
		return 0;
	}
	BYTE* pPrintInfo = new BYTE[needed];

	::EnumPrintersW(flag, name, level, pPrintInfo, needed, &needed, &nCount);
	return pPrintInfo;
}


TEST(User32, PrinterRelated) {
	wchar_t buf[0x1000] = { 0 };

	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_NAME | PRINTER_ENUM_SHARED | PRINTER_ENUM_NETWORK | PRINTER_ENUM_REMOTE | PRINTER_ENUM_CATEGORY_ALL;
	DWORD flags_local = PRINTER_ENUM_LOCAL;
	DWORD flags_network = PRINTER_ENUM_NETWORK;
	DWORD flags_remote = PRINTER_ENUM_REMOTE;
	DWORD flags_all = PRINTER_ENUM_CATEGORY_ALL;
	
	DWORD nPrinters = 0;

	std::vector<DWORD> fs { flags_local ,flags_network ,flags_remote ,flags_all };

	for (auto f : fs) {

		PRINTER_INFO_1* p1 = (PRINTER_INFO_1*)Printer_Enum(f, NULL, 1, nPrinters);
		for (size_t i = 0; i < nPrinters; i++)
		{
			auto x = p1[i];
			wcout << x.pName << endl;
		}
		PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)Printer_Enum(f, NULL, 2, nPrinters);
		for (size_t i = 0; i < nPrinters; i++)
		{
			auto x = p2[i];
			wcout << x.pPrinterName << endl;
		}
		PRINTER_INFO_3* p3 = (PRINTER_INFO_3*)Printer_Enum(f, NULL, 3, nPrinters);
		for (size_t i = 0; i < nPrinters; i++)
		{
			auto x = p3[i];
			wcout << x.pSecurityDescriptor << endl;
		}
		PRINTER_INFO_4* p4 = (PRINTER_INFO_4*)Printer_Enum(f, NULL, 4, nPrinters);
		for (size_t i = 0; i < nPrinters; i++)
		{
			auto x = p4[i];
			wcout << x.pPrinterName << endl;
		}
		PRINTER_INFO_5* p5 = (PRINTER_INFO_5*)Printer_Enum(f, NULL, 5, nPrinters);
		for (size_t i = 0; i < nPrinters; i++)
		{
			auto x = p5[i];
			wcout << x.pPortName << endl;
		}
	}


	PRINTDLGW xxx = { 0 };
	xxx.lStructSize = sizeof(xxx);
	xxx.Flags = PD_PRINTTOFILE | PD_RETURNDC;
	::PrintDlg(&xxx);

	
}


