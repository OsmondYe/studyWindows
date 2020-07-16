#include "stdafx.h"
#include <winternl.h>
#include <shlobj_core.h>
#include "oye_handy_tools.hpp"
//use DISABLED_ prefix to filter out 

#pragma comment(lib, "ntdll.lib")




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



VOID NTAPI RtlInitUnicodeString(PUNICODE_STRING DestinationString,PCWSTR SourceString);

bool PathIsFolder(const wchar_t* path) {
	DWORD dwAttrib = GetFileAttributes(path);
	return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


TEST(Windows, UnicodeString) {
	UNICODE_STRING   aStr = { 0 };
	//#pragma comment(lib, "ntdll.lib")
	RtlInitUnicodeString(&aStr, LR"_(\??\C:\Users\oye\AppData\Local\Microsoft\Office\16.0\Floodgate\Word.CampaignStates.json)_");

	wcout << aStr.Buffer << endl;

	bool isFolder = PathIsFolder(LR"_(\??\c:\aaaaa\)_");

	// change the path, and detoured into another path, begin with D:\Nextlabs_Jail\;

	wstring Jail_Path = LR"_(D:\Nextlabs_Jail\)_";

	wstring org_path = aStr.Buffer;

	// modify algo
	{
		if (org_path[5] == L':') {
			//org_path[5] = L'\\';
			org_path.erase(5,1);
		}
		org_path.insert(4, Jail_Path);
		Jail_Path = org_path;
	}

	wcout << Jail_Path << endl;

	wstring folder = org_path.substr(0, org_path.find_last_of(L'\\'));

	// make sure path exist
	::SHCreateDirectoryEx(NULL, Jail_Path.c_str(), NULL);
}

