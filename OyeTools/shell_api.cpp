#include "pch.h"


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
