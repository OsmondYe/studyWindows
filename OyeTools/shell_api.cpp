#include "pch.h"

#include <Shlobj.h>


// CSIDL : constant special id list
//map<int, string> win32_csidl{
//	CSIDL_DESKTOP
//	CSIDL_INTERNET
//	CSIDL_PROGRAMS
//	CSIDL_CONTROLS
//	CSIDL_PRINTERS
//	CSIDL_PERSONAL
//	CSIDL_FAVORITES
//	CSIDL_STARTUP
//	CSIDL_RECENT
//	CSIDL_SENDTO
//	CSIDL_BITBUCKET
//	CSIDL_STARTMENU
//	CSIDL_MYDOCUMENTS
//	CSIDL_MYMUSIC
//	CSIDL_MYVIDEO
//	CSIDL_DESKTOPDIRECTORY
//	CSIDL_DRIVES
//	CSIDL_NETWORK
//	CSIDL_NETHOOD
//	CSIDL_FONTS
//	CSIDL_TEMPLATES
//	CSIDL_COMMON_STARTMENU
//	CSIDL_COMMON_PROGRAMS
//	CSIDL_COMMON_STARTUP
//	CSIDL_COMMON_DESKTOPDIRECTORY
//	CSIDL_APPDATA
//	CSIDL_PRINTHOOD
//}

void shell_api_test() {
	for (int i = 0; i < 0x20; i++) {
		wchar_t path[255] = { 0 };
		if (SUCCEEDED(::SHGetFolderPathW(NULL, i, 0, 0, path))) {
			wcout<<path<<endl;
		}
	}
}


void lookup_english_word_in_browser(const std::string& word) {

	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	std::string url = "http://dict.youdao.com/w/";
	url += word;
	SHELLEXECUTEINFOA data{ 0 };
	data.cbSize = sizeof(SHELLEXECUTEINFOW);
	data.lpFile = url.c_str();
	data.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
	data.nShow = SW_SHOWNORMAL;
	
	::ShellExecuteExA(&data);
}

