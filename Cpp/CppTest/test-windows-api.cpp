#include "stdafx.h"

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


TEST(Windows, DISABLED_COM) {

	size_t size = sizeof(VARIANT);
	EXPECT_EQ(size, 0x10);

}


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

TEST(Windows, DISABLED_RegOperation2) {
	HKEY root = HKEY_CURRENT_USER;
	const wchar_t* parent = L"Software\\AAA\\BBB\\CCC\\DDD\\EEE\\FFF\\GGG";
	DWORD rt = 0;
	HKEY result = 0;

	rt=RegCreateKey(root, parent, &result);

	rt=RegSetValueW(result, L"null", 1, parent, wcslen(parent));



	static const wchar_t* hSKYDRM = L"Software\\Nextlabs\\SkyDRM\\LocalApp";

	wchar_t skydrm_cmd[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;

	if (ERROR_SUCCESS != ::RegGetValueW(HKEY_CURRENT_USER, hSKYDRM, L"Executable", RRF_RT_REG_SZ, NULL, skydrm_cmd, &size)) {
		rt = E_UNEXPECTED;
	}


	// cat str with format "%path%" -view "%1"
	wchar_t nxl_cmd[MAX_PATH] = { 0 };
	::swprintf_s(nxl_cmd,L"\"%s\" -view \"%%1\"", skydrm_cmd);


	EXPECT_TRUE(1);



}