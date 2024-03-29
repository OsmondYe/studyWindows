﻿#include "pch.h"

/*
CreateFileW
GetFileSizeEx
SetFilePointerEx
SetEndOfFile
ReadFile
WriteFile
FlushFileBuffers
CancelSynchronosIO
//------------overlapped-------------
HasOverlappedIOComplete
CancelIO
*/

using namespace std;

void create_file(wstring path) {
	// good place study CreateFileW
	DWORD dwDesiredAccess = GENERIC_READ | 
							GENERIC_WRITE | 
							GENERIC_EXECUTE | 
							GENERIC_ALL;
	DWORD dwShareMode = FILE_SHARE_READ | 
						FILE_SHARE_WRITE | 
						FILE_SHARE_DELETE;
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };  //allow inheritance by child process
	DWORD dwCreateDisposition = 
		CREATE_NEW |				// error if exsit
		CREATE_ALWAYS |				// overwrite if exist
		OPEN_EXISTING | 
		OPEN_ALWAYS | 
		TRUNCATE_EXISTING;			// not exsit -> error,    exsit -> truncate 
	DWORD dwFlagAndAttributes =
		FILE_FLAG_NO_BUFFERING |			// 驱动不要做缓存
		FILE_FLAG_SEQUENTIAL_SCAN |			// 给驱动缓存暗示，顺序读取，所以会多读一些
		FILE_FLAG_RANDOM_ACCESS |			// 给驱动暗示， 这个文件要随机读取，因此预取意义不大
		FILE_FLAG_WRITE_THROUGH |			// 给驱动暗示，不要进行写缓存
		FILE_FLAG_DELETE_ON_CLOSE |			// 释放就自动帮忙删除
		FILE_FLAG_OVERLAPPED |				// Asynchronous IO
		FILE_FLAG_BACKUP_SEMANTICS |
		FILE_FLAG_FIRST_PIPE_INSTANCE |
		FILE_FLAG_OPEN_NO_RECALL |
		FILE_FLAG_OPEN_REPARSE_POINT |
		FILE_FLAG_OPEN_REQUIRING_OPLOCK |
		FILE_FLAG_POSIX_SEMANTICS |
		FILE_FLAG_SESSION_AWARE |
		/////////////////for attributes///////////////////
		FILE_ATTRIBUTE_ARCHIVE |
		FILE_ATTRIBUTE_ENCRYPTED |
		FILE_ATTRIBUTE_HIDDEN |			//by default, hiden file
		FILE_ATTRIBUTE_NORMAL |			//normal file
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED |
		FILE_ATTRIBUTE_READONLY |
		FILE_ATTRIBUTE_SYSTEM |
		FILE_ATTRIBUTE_TEMPORARY;

	// Common use:
	DWORD dwTMP = FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;

	//::CreateFileW(path.c_str(), dwDesiredAccess, dwShareMode,&sa,dwCreateDisposition,)

	//GetFileAttributes()
}