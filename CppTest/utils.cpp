#include "stdafx.h"
#include "utils.h"



bool win::is_file_exist(const char * file)
{
	DWORD dwAttrib = GetFileAttributesA(file);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::is_file_exist(const wchar_t * file)
{
	DWORD dwAttrib = GetFileAttributesW(file);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::is_dir_exist(const char * dir)
{
	DWORD dwAttrib = GetFileAttributesA(dir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::is_dir_exist(const wchar_t * dir)
{
	DWORD dwAttrib = GetFileAttributesW(dir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::is_path_exist(const char * dir)
{
	return INVALID_FILE_ATTRIBUTES != GetFileAttributesA(dir);
}

bool win::is_path_exist(const wchar_t * dir)
{
	return INVALID_FILE_ATTRIBUTES != GetFileAttributesW(dir);
}

HANDLE win::get_process(int process_id) {
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
}

HANDLE win::get_process(const wchar_t * process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return INVALID_HANDLE_VALUE;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
		if (!_tcscmp(pe.szExeFile, process_name)) {
			CloseHandle(hSnapshot);
			return get_process(pe.th32ProcessID);
		}
	}
	return INVALID_HANDLE_VALUE;
}


	