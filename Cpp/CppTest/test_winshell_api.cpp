#include "stdafx.h"
#include <shlwapi.h>
#include <shlobj_core.h>


using namespace std;


TEST(Shell32, GetSpecialFolder) {

	wchar_t tmp[0x100] = { 0 };

	if (FAILED(::SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, tmp))) {

	}

	std::wstring path(tmp);

	wcout << "Local_Path:" << path.c_str() << endl;


}

TEST(Shell32, PathAppend) {
	wchar_t tmp[0x100] = { LR"_(C:\Users\oye\AppData\Local)_" };

	::PathAppendW(tmp, LR"_(NextLabs\SkyDRM\tmp\)_");

	wcout << "cated path:" << tmp << endl;

	if (!::PathFileExistsW(tmp)) {
		// Make sure path chain exist
	}

}

// make sure path exist
TEST(Shell32, CreateFolderMultiPart) {

	wstring path = LR"_(D:\allTestFile\AutoRecovery\a\b\c\e)_";

	::SHCreateDirectoryEx(NULL, path.c_str(), NULL);
}


TEST(Shell32, Nxl_GetUserTmpFolder) {

	wchar_t tmp[0x100] = { 0 };

	EXPECT_TRUE(
		SUCCEEDED(::SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, tmp))
	);

	// append  

	::PathAppendW(tmp, LR"_(NextLabs\SkyDRM\tmp\)_");

	// give it an guid to make it unique by current calling
	GUID guid = { 0 };
	::CoCreateGuid(&guid);

	OLECHAR* guidStr = NULL;
	::StringFromCLSID(guid, &guidStr);

	::PathAppendW(tmp, guidStr);

	::CoTaskMemFree(guidStr);


	::SHCreateDirectoryEx(NULL, tmp, NULL);


	EXPECT_TRUE(
		::PathIsDirectoryW(tmp)
	);


}


TEST(Shell32, Extract_File_Folder) {

	// give a existed file,  extract it parent folder
	wchar_t tmp[0x100] = { LR"_("D:\allTestFile\nxl_file_test\ppt.ppt")_" };


	wchar_t* file_name = ::PathFindFileNameW(tmp);

	std::wstring fn = file_name;
	wcout << L"File Name:" << fn.c_str() << endl;

	PathRemoveFileSpec(tmp);

	wcout << L"path:" << tmp << endl;

}

TEST(Shell32, Extract_File_Folder2) {
	wchar_t tmp[0x100] = { LR"_("D:\allTestFile\nxl_file_test\ppt.ppt")_" };

	wstring path = tmp;

	while ('\\' != path.back()) {
		path.pop_back();
	}

	wcout << path << endl;

}