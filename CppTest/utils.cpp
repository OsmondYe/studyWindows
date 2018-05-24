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
