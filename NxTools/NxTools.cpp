// NxTools.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"


#ifdef  _WIN64
int b64bit = true;
#else 
int b64bit = false;
#endif //  _WIN64


#include <madCHook.h>

#pragma comment(lib,"madCHook64mt.lib")
//#pragma comment(lib,"libvcruntime.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")



using namespace std;




HANDLE get_process(int process_id) {
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
}

HANDLE get_process(const wchar_t* process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return INVALID_HANDLE_VALUE;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
		if (!_tcsicmp(pe.szExeFile, process_name)) {
			CloseHandle(hSnapshot);
			return get_process(pe.th32ProcessID);
		}
	}
	return INVALID_HANDLE_VALUE;
}

std::set<HANDLE> get_processes(const wchar_t* process_name)
{
	std::set<HANDLE> rt;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		cerr << "Can not call CreateToolhelp32Snapshot to get process snapshot, check your permission" << endl;
		abort();
		return rt;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
		if (!_tcsicmp(pe.szExeFile,process_name)) {
			rt.insert(get_process(pe.th32ProcessID));
		}
	}
	CloseHandle(hSnapshot);
	return rt;
}




void display_help() {
	cout << "Inject tools, inject specific dll into running processes" << endl;
	cout << "only " << (b64bit ? 64 : 32) << "bit dll can be supported" << endl;
	cout << "useage: nxtool {-pid id | -pname name} {dll_path}" << endl;

}


void inject_dll(int pid, const string& dll_path) {

	HANDLE hTargetProcess = get_process(pid);
	if (INVALID_HANDLE_VALUE == hTargetProcess) {
		cerr << "Can not find the process which pid is " << pid << endl;
		return;
	}

	if (!InjectLibraryA(dll_path.c_str(), hTargetProcess)) {
		cerr << "can not perform inject dll" << endl;
		return;
	}
	else {
		cout << "inject ok for pid:" << pid << endl;
	}
	return;

}

void inject_dll(const string& pname, const string& dll_path) {
	std::wstring wpname(pname.begin(), pname.end());
	auto s = get_processes(wpname.c_str());	
	if (s.empty()) {
		cerr << "Can not find any the process which pname is " << pname << endl;
		abort();
		return;
	}

	for (auto i : s) {
		if (!InjectLibraryA(dll_path.c_str(), i)) {
			cerr << "can not perform inject dll, pid is:" <<::GetProcessId(i)<< endl;
			continue;
		}
		else {
			cout<< "inject ok for pid:"<< ::GetProcessId(i) << endl;
		}
	}
	return;

}


int main(int argc,char* argv[])
{
	if (argc <= 1) {
		display_help();
		return 0;
	}

	int pid = -1;
	std::string pname;
	std::string dll_path;
	for (int i = 1; i < argc; ++i) {
		if (0==_stricmp("-pid", argv[i])) {
			pid =  std::atoi(argv[++i]);
			continue;
		}
		if (0 == _stricmp("-pname", argv[i])) {
			pname.assign(argv[++i]);
			continue;
		}
		dll_path = argv[i];
	}

	if (pid == -1 && pname.empty()) {
		display_help();
		return 0;
	}

	if (dll_path.empty()) {
		display_help();
		return 0;
	}

	if (pid != -1) {
		inject_dll(pid, dll_path);
		return 0;
	}

	if (!pname.empty()) {
		inject_dll(pname, dll_path);
		return 0;
	}

	return 0;
}
