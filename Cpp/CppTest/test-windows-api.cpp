#include "stdafx.h"
#include "oye_handy_tools.hpp"
//use DISABLED_ prefix to filter out 

TEST(Windows, VectorAsBuf) {
	std::vector<wchar_t> buf(1024,0);// buf[1024]={0};
	int len = ::GetModuleFileNameW(NULL, buf.data(), 1024);
	int buflen=buf.size();// size==0;
	buf.resize(len);
	std::wstring path(buf.begin(), buf.end());
	wcout << path;
	//
	// either is ok
	//
	wstring buf2(1024, 0);
	len=::GetModuleFileNameW(NULL, (wchar_t*)buf2.data(), 1024);
	buf2.resize(len);

	wcout << buf2;
}

TEST(Windows, DISABLED_ShellOperation) {
	const wchar_t* exe = L"D:\\OyeProject\\CSharp\\rmd-windows\\SkydrmLocal\\Debug\\SkydrmLocal.exe";
	const wchar_t* cmd = L"-share c:\\aaa.txt";
	::ShellExecuteW(NULL, L"open", exe, cmd, NULL, SW_SHOW);
}


TEST(Windows, DynamicDLLLoad) {
	const wchar_t* dll = L"WINSPOOL.DRV";
	const char* fun_name = "EnumJobNamedProperties";

	auto h = ::LoadLibraryW(dll);
	EXPECT_TRUE(h != NULL);
	if (h == NULL) {
		cout << "null loadlibrary" << endl;
		return;
	}
	auto fun = ::GetProcAddress(h, fun_name);

	EXPECT_TRUE(fun != NULL);
}


