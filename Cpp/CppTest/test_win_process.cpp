#include "stdafx.h"



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


}