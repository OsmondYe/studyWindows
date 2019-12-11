#include "pch.h"
#include <madCHook.h>


#ifdef  _WIN64
int b64bit = true;
#pragma comment(lib,"madCHook64mt.lib")
#else 
int b64bit = false;
#pragma comment(lib,"madCHook32.lib")
#endif //  _WIN64


using namespace std;


string hook_help_message() {
	ostringstream oss;
	oss << "Inject tools, inject specific dll into running processes\n"
		<< "only " << (b64bit ? 64 : 32) << "bit dll can be supported\n"
		<< "useage: oyetools {-pid id | -pname name} {dll_path}"
		<< endl;
	return oss.str();
}


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
		if (!_tcsicmp(pe.szExeFile, process_name)) {
			rt.insert(get_process(pe.th32ProcessID));
		}
	}
	CloseHandle(hSnapshot);
	return rt;
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
			cerr << "can not perform inject dll, pid is:" << ::GetProcessId(i) << endl;
			continue;
		}
		else {
			cout << "inject ok for pid:" << ::GetProcessId(i) << endl;
		}
	}
	return;
}

