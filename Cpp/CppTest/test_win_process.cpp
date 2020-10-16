#include "stdafx.h"

#include <tlhelp32.h>  // CreateToolHelp32Snapshot


TEST(Process, Create) {
	STARTUPINFO si; //进程启动时需要初始化的结构
	PROCESS_INFORMATION pi; //进程启动后的有关信息
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.wShowWindow = SW_SHOW;//这里设置窗口为显示，SW_HIDE为隐藏窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	CreateProcess(LR"_(C:\Windows\notepad.exe)_",
		NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
}

TEST(Process, ShellOperation) {
	//
	// open webpage
	//
	const wchar_t* web = L"www.baidu.com";

	::ShellExecuteW(
		NULL,		// NULL window
		L"open",    // verb
		web,		// lp File 
		NULL,		// NULL param 
		NULL,		// NULL dir
		SW_SHOW     // defualt window show state
	);
}


TEST(Process, Enum) {
	//<tlhelp32.h>
	DWORD flag = TH32CS_INHERIT |		//  handle can be inherited
		TH32CS_SNAPALL |				//   ==  TH32CS_SNAPHEAPLIST |  TH32CS_SNAPMODULE TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD
		TH32CS_SNAPHEAPLIST |			//  heaps  in pID
		TH32CS_SNAPMODULE |				//  
		TH32CS_SNAPMODULE32 |
		TH32CS_SNAPPROCESS |			//  include all proc , in Process32First
		TH32CS_SNAPTHREAD				//  include all thread, in Thread32First;
		;	
	DWORD th32ProcessID = 0; // current process

	// enum all process

	auto hProceSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32W pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32W);
	if (::Process32FirstW(hProceSnap,&pe32)) {
		do {
			wprintf(L"pid:%d,\tppid:%d,\tthreadCnt:%d,\tpPath:%s\n",
				pe32.th32ProcessID,
				pe32.th32ParentProcessID,
				pe32.cntThreads,
				pe32.szExeFile
			);
		} while (Process32NextW(hProceSnap, &pe32));
	}

}

TEST(Process, AdjustDebugRights) {

	bool rt = false;
	
	auto p = ::GetCurrentProcess();
	HANDLE hProcToken = NULL;

	rt = ::OpenProcessToken(p, TOKEN_ALL_ACCESS, &hProcToken);

	{
		// give SE_DEBUG_NAME, privilege
		// #define SE_DEBUG_NAME    TEXT("SeDebugPrivilege")
		BOOL bEnablePrivilege = TRUE;
		TOKEN_PRIVILEGES tp;
		LUID luid;
		rt = ::LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &luid);
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;

		rt =::AdjustTokenPrivileges(hProcToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	}


}

TEST(Process, Terminate) {

	HANDLE hTargetProcessHandle;
	DWORD dwDesiredAccess = PROCESS_ALL_ACCESS;
	DWORD dwRargetProcessId = 0000;
	hTargetProcessHandle = ::OpenProcess(dwDesiredAccess, NULL, dwRargetProcessId);

	::TerminateProcess(hTargetProcessHandle, 0);

}