#include "stdafx.h"

//use DISABLED_ prefix to filter out 

TEST(Windows, DISABLED_ShellOperation) {
	const wchar_t* exe = L"D:\\OyeProject\\CSharp\\rmd-windows\\SkydrmLocal\\Debug\\SkydrmLocal.exe";
	const wchar_t* cmd = L"-share c:\\aaa.txt";
	::ShellExecuteW(NULL, L"open", exe, cmd, NULL, SW_SHOW);
}

TEST(Windows, DISABLED_RegOperation) {
	HKEY root = HKEY_CURRENT_USER;
	const wchar_t* parent = L"Software\\NextLabs\\SkyDRM\\LocalApp";
	HKEY hParent;

	if (ERROR_SUCCESS != RegOpenKeyExW(root, parent, 0, KEY_READ, &hParent))
	{
		return;
	}

	DWORD value_type;
	byte* value_buffer;
	DWORD value_length;

	// get length first
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"Executable", NULL, &value_type, NULL, &value_length)) {
		RegCloseKey(hParent);
		return;
	}

	value_buffer = new byte[value_length + 2];
	// get value;
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"Executable", NULL, &value_type, value_buffer, &value_length)) {
		RegCloseKey(hParent);
		return;
	}
	// close 
	if (ERROR_SUCCESS != RegCloseKey(hParent)) {
		return;
	}
	std::wstring path((wchar_t*)value_buffer);
	// set value to out param
	EXPECT_FALSE(path.empty());

}