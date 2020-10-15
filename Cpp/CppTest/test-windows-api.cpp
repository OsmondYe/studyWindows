#include "stdafx.h"
#include <winternl.h>
#include <shlobj_core.h>
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






//TEST(Windows, FCreateFile) {
//
////#define GENERIC_READ                     (0x80000000L)
////#define GENERIC_WRITE                    (0x40000000L)
////#define GENERIC_EXECUTE                  (0x20000000L)
////#define GENERIC_ALL                      (0x10000000L)
//
////#define CREATE_NEW          1
////#define CREATE_ALWAYS       2
////#define OPEN_EXISTING       3
////#define OPEN_ALWAYS         4
////#define TRUNCATE_EXISTING   5
//
////#define FILE_SHARE_READ                 0x00000001  
////#define FILE_SHARE_WRITE                0x00000002  
////#define FILE_SHARE_DELETE               0x00000004 
//
//	::CreateFileW(replaced_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//}


//TEST(Windows, FReplaceFile) {
//
//	auto rt = ::ReplaceFileW(
//		replaced_file,
//		replacement_file,
//		backup_file,
//		2,
//		0,
//		0
//	);
//
//	EXPECT_EQ(rt , TRUE);
//
//}


TEST(Windows, FClearFolder) {
	SHFILEOPSTRUCTW file_op = {
	NULL,
	FO_DELETE,
	LR"_(C:\Users\oye\Nextlabs_Jail)_",
	L"",
	FOF_NOCONFIRMATION |
	FOF_NOERRORUI |
	FOF_SILENT,
	false,
	0,
	L"" };

	::SHFileOperationW(&file_op);

}