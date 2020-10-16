#include "stdafx.h"



TEST(System, LockScreen) {
	::LockWorkStation();
}


TEST(System, LogOff) {

	DWORD flag =
		EWX_LOGOFF	  |     // clear this session
		EWX_POWEROFF  |     // restart	must have the SE_SHUTDOWN_NAME privilege.
		EWX_REBOOT	  |		
		EWX_SHUTDOWN  
	;

	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);


	::ExitWindowsEx(EWX_POWEROFF, 0);
}

TEST(System, Information) {

	wchar_t name[255] = { 0 };
	DWORD len = 255;
	::GetComputerNameW(name, &len);
	wprintf(L"Computer name:%s\n", name);
	::GetUserNameW(name, &len);
	wprintf(L"User name:%s\n", name);
	::GetSystemDirectoryW(name, 255);			//C:\Windows\system32
	wprintf(L"SystemDir name:%s\n", name);
	::GetWindowsDirectoryW(name, 255);			//C:\Windows
	wprintf(L"WindowsDir name:%s\n", name);

	// Mem Status
	MEMORYSTATUS mem = { 0 };
	mem.dwLength = sizeof(mem);

	::GlobalMemoryStatus(&mem);

	wprintf(L"Total Memory: %d\n", mem.dwTotalPhys / 1024 / 1024 + 1);
	wprintf(L"Avail Memory: %d\n", mem.dwAvailPhys / 1024 / 1024 + 1);
	wprintf(L"Virul Memory: %d\n", mem.dwTotalVirtual / 1024 / 1024 + 1);
	wprintf(L"AViru Memory: %d\n", mem.dwAvailVirtual / 1024 / 1024 + 1);
	wprintf(L"Page  Memory: %d\t\t current size of the committed memory limit \n", mem.dwTotalPageFile / 1024 / 1024 + 1);
	wprintf(L"APage Memory: %d\t\t maximum amount of memory the current process can commi\n", mem.dwAvailPageFile / 1024 / 1024 + 1);

}