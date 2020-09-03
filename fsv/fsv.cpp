/*
	File Virtualization
	For Single File, for first release

	using TLS to setup locks

	History:

*/

#include "framework.h"
#include "config.h"
#include "fsv.h"
#include "tls.h"
#include "ds.h"
#include "pf.h"
#include <string>
#include <map>
#include <shlobj_core.h>

#include "utils.hpp"


#pragma comment(lib,"ntdll.lib")

#ifdef _DEBUG
#include "debug.h"
#endif // _DEBUG



#ifndef  _WIN64
#define WOW64_FS_REDIR
#endif ! _WIN64


// header supplyments from ntddk
namespace {
	// FileBasicInformation
	typedef struct _FILE_BASIC_INFORMATION {
		LARGE_INTEGER CreationTime;
		LARGE_INTEGER LastAccessTime;
		LARGE_INTEGER LastWriteTime;
		LARGE_INTEGER ChangeTime;
		ULONG FileAttributes;
	} FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

	// FileNetworkOpenInformation
	typedef struct _FILE_NETWORK_OPEN_INFORMATION {
		LARGE_INTEGER CreationTime;
		LARGE_INTEGER LastAccessTime;
		LARGE_INTEGER LastWriteTime;
		LARGE_INTEGER ChangeTime;
		LARGE_INTEGER AllocationSize;
		LARGE_INTEGER EndOfFile;
		ULONG FileAttributes;
	} FILE_NETWORK_OPEN_INFORMATION, * PFILE_NETWORK_OPEN_INFORMATION;

};


// hook at high level to filterout noisy
namespace {
	typedef UINT(WINAPI* Sig_GetDriveTypeW)(LPCWSTR lpRootPathName);

	typedef HRESULT(__stdcall* Sig_QISearch)(void* that, void* pqit, const IID* const riid, void** ppv);


	typedef BOOL(WINAPI* Sig_WriteFile)(
		_In_ HANDLE hFile,
		_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
		_In_ DWORD nNumberOfBytesToWrite,
		_Out_opt_ LPDWORD lpNumberOfBytesWritten,
		_Inout_opt_ LPOVERLAPPED lpOverlapped
		);


	typedef BOOL(WINAPI* Sig_WriteFileEx)(
		_In_ HANDLE hFile,
		_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
		_In_ DWORD nNumberOfBytesToWrite,
		_Inout_ LPOVERLAPPED lpOverlapped,
		_In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
		);


	typedef BOOL (WINAPI* Sig_SetFileSecurityW)(LPCWSTR lpFileName,
		_In_ SECURITY_INFORMATION SecurityInformation,
		_In_ PSECURITY_DESCRIPTOR pSecurityDescriptor
	);

}


typedef NTSTATUS(NTAPI* Sig_NtCreateFile)(
	PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength
	);

typedef NTSTATUS(NTAPI* Sig_NtOpenFile)(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG ShareAccess,
	IN ULONG OpenOptions
	);


typedef NTSTATUS(NTAPI* Sig_NtQueryAttributesFile)(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_BASIC_INFORMATION* FileInformation);

typedef NTSTATUS(NTAPI* Sig_NtQueryFullAttributesFile)(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_NETWORK_OPEN_INFORMATION* FileInformation);





NTSTATUS NTAPI Hooked_NtOpenFile(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG ShareAccess,
	IN ULONG OpenOptions
);



NTSTATUS NTAPI Hooked_NtCreateFile(
	PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength
);


NTSTATUS NTAPI Hooked_NtQueryAttributesFile(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_BASIC_INFORMATION* FileInformation);

NTSTATUS NTAPI Hooked_NtQueryFullAttributesFile(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_NETWORK_OPEN_INFORMATION* FileInformation);

UINT WINAPI Hooked_GetDriveTypeW(LPCWSTR lpRootPathName);
HRESULT __stdcall Hooked_QISearch(void* that, void* pqit, const IID* const riid, void** ppv);

BOOL WINAPI Hooked_WriteFile(
	_In_ HANDLE hFile,
	_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
	_In_ DWORD nNumberOfBytesToWrite,
	_Out_opt_ LPDWORD lpNumberOfBytesWritten,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
);


BOOL WINAPI  Hooked_WriteFileEx(
	_In_ HANDLE hFile,
	_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
	_In_ DWORD nNumberOfBytesToWrite,
	_Inout_ LPOVERLAPPED lpOverlapped,
	_In_ LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);


BOOL WINAPI Hooked_SetFileSecurityW(LPCWSTR lpFileName,
	_In_ SECURITY_INFORMATION SecurityInformation,
	_In_ PSECURITY_DESCRIPTOR pSecurityDescriptor
);


Sig_NtCreateFile Hooked_NtCreateFile_Next = NULL;
Sig_NtOpenFile Hooked_NtOpenFile_Next = NULL;
Sig_NtQueryAttributesFile Hooked_NtQueryAttributesFile_Next = NULL;
Sig_NtQueryFullAttributesFile Hooked_NtQueryFullAttributesFile_Next = NULL;
Sig_GetDriveTypeW Hooked_GetDriveTypeW_Next = NULL;
Sig_QISearch Hooked_QISearch_Next = NULL;
Sig_WriteFile Hooked_WriteFile_Next = NULL;
Sig_WriteFileEx Hooked_WriteFileEx_Next = NULL;
Sig_SetFileSecurityW Hooked_SetFileSecurityW_Next = NULL;


namespace utils {

	static const std::wstring JailPath{ FV_JAIL_PATH };

	// build jail path from org path
	inline void config_jail_path(std::wstring& jail, const std::wstring& org) {
		// org path has been in Jail 
		if (utils::iconatain(org, JailPath)) {
			jail = org;
			return;
		}
		// create full jail path
		std::wstring  new_path = org;
		if (new_path[5] == L':') {
			//org_path[5] = L'\\';
			new_path.erase(5, 1);
		}
		new_path.insert(4, jail);
		jail = new_path;

	}

	inline void copy_file_wrapper(const wchar_t* org, const wchar_t* dst) {
		if (!::CopyFile(org, dst,false)) {
			auto err = ::GetLastError();
			debug_string_with_LastError(L"call CopyFile failed.",err);
		}
	}
}

namespace mem {

	void* mem_alloc(ULONG size) {
		void* ptr;
		ptr = ::VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		return ptr;
	}

	void mem_free(void* ptr) {
		if (!::VirtualFree(ptr, 0, MEM_RELEASE)) {
			// very bad situation
			::RaiseException(STATUS_ACCESS_VIOLATION, EXCEPTION_NONCONTINUABLE_EXCEPTION, 0, NULL);
			::ExitProcess(-1);
		}
	}

	void new_object_attribute(OBJECT_ATTRIBUTES& new_one, OBJECT_ATTRIBUTES* old_template, const std::wstring& new_path) {

		new_one.Length = sizeof(OBJECT_ATTRIBUTES);
		new_one.Attributes = old_template->Attributes;
		new_one.RootDirectory = old_template->RootDirectory;
		new_one.SecurityDescriptor = old_template->SecurityDescriptor;
		new_one.SecurityQualityOfService = old_template->SecurityQualityOfService;

		// will trigger mem-leak
		UNICODE_STRING* new_path_unistr = (UNICODE_STRING*)mem_alloc(sizeof(UNICODE_STRING));
		RtlInitUnicodeString(new_path_unistr, new_path.c_str());
		new_one.ObjectName = new_path_unistr;
	}
}

namespace tls {
	THREAD_DATA* getTlsData() {
		THREAD_DATA* data;
		data = (THREAD_DATA*)tls_get_data();
		if (data == nullptr) {
			// give a new one
			data = (THREAD_DATA*)mem::mem_alloc(sizeof(THREAD_DATA));
			if (tls_set_data(data)) {
				return data;
			}
			else {
				return nullptr;
			}
		}
		return data;
	}
}


namespace sys {
	// org -> jail
	std::map<std::wstring, std::wstring> path_map;

	inline void record_map(const std::wstring& org, const std::wstring& jail) {
		path_map[org] = jail;
	}

	// 'in map' means org file path has been detoured
	inline bool is_in_map(const std::wstring& org) {
		return path_map.count(org) != 0;
	}

	inline void remove_item_in_map(const std::wstring& org, const std::wstring& jail) {
		if (is_in_map(org)) {
			path_map.erase(org);
		}
	}

	CRITICAL_SECTION gcs;
	inline void init_cs() {
		::InitializeCriticalSection(&gcs);
	}
}


SV_API bool _stdcall init_file_virtualization();


NTSTATUS NTAPI Hooked_NtOpenFile(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG ShareAccess,
	IN ULONG OpenOptions
) {
	NTSTATUS status;
	THREAD_DATA* tls;
	std::wstring jail_path = utils::JailPath;
	const std::wstring org_path = ObjectAttributes->ObjectName->Buffer;
	OBJECT_ATTRIBUTES  new_obj{ 0 };


	tls = tls::getTlsData();
	// anti-reentrance
	if (tls->thread_lock || tls->file_NtOpenFile_lock) {
		return Hooked_NtOpenFile_Next(FileHandle,DesiredAccess, ObjectAttributes, IoStatusBlock,ShareAccess, OpenOptions);
	}

	tls->file_NtOpenFile_lock = true;
	tls->thread_lock = true;
	do {
		debug_string(L"=======begin Hooked_NtOpenFile=======\n");
		{// debug
			std::wstring msg;
			msg += L"org_path:  "; msg += org_path;  msg += L"\n";
			debug_string(msg.c_str());
		}
		// for open docx
		
		if (utils::iends_with(org_path, { {L".docx"}, {L".tmp"}})) {
			debug_param_ntopenfile(DesiredAccess, ObjectAttributes, ShareAccess, OpenOptions);
			::DebugBreak();
		}

		if (OpenOptions == FILE_OPEN_REPARSE_POINT) {
			debug_param_ntopenfile(DesiredAccess, ObjectAttributes, ShareAccess, OpenOptions);
			::DebugBreak(); // I donot know why open only have this option
		}

		// filter first
		if (pf::Filter_Out_By_Params_NtOpenFile(DesiredAccess, ObjectAttributes, ShareAccess,OpenOptions)) 
		{
			debug_string(L"Match filter police, direct call next\n");
			status = Hooked_NtOpenFile_Next(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
			break;
		}


		// create jail path
		utils::config_jail_path(jail_path, org_path);

		{// debug	
			std::wstring msg;
			msg += L"jail_path: "; msg += jail_path; msg += L"\n";
			debug_string(msg.c_str());
		}

		// extend: how to extend the object_attributes
		mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);

		// org_path has not been recorded, we need to create the detour one
		if (!sys::is_in_map(org_path)) {

			std::wstring folder = jail_path.substr(0, jail_path.find_last_of(L'\\'));
			utils::MakeSureFolderExist(folder.c_str());

			// check if file exsit
			if (utils::FileExists(org_path.c_str())) {
				// copy to jail path
				debug_string(L"copy org file to jail");
				utils::copy_file_wrapper(org_path.c_str(), jail_path.c_str());
			}
			else {
				debug_string(L"org path has not been detoured, and org path is not exsit in org intent");
			}
		}
		else {
			debug_string(L"org path has been detoured");
		}

		// redirect the org path to the jail path
		debug_string(L"call Hooked_NtOpenFile_Next\n");
		debug_param_ntopenfile(DesiredAccess, &new_obj, ShareAccess, OpenOptions);

		status = Hooked_NtOpenFile_Next(FileHandle,
			DesiredAccess, &new_obj, IoStatusBlock,
			ShareAccess, OpenOptions);


		if (NT_SUCCESS(status)) {

			if ((DesiredAccess & DELETE) && (OpenOptions & FILE_NON_DIRECTORY_FILE)) {
				// since the file have been deleted, remove it from map
				debug_string(L"call ok, remove_item_in_map, org_path ->jail_path\n");
				sys::remove_item_in_map(org_path, jail_path);
			}
			else {
				// record map, org_path ->jail_path;
				debug_string(L"call ok, record map, org_path ->jail_path\n");
				sys::record_map(org_path, jail_path);
			}

			

			// if open for delete,  delete the file in map
			// DesiredAccess: { DELETE    OpenOptions: { FILE_NON_DIRECTORY_FILE


		}
		else {
			debug_string_with_NTSTATUS(L"!!!Notice!!!,Failed calling NtOpenFile", status);
		}

	} while (false);
	
	debug_string(L"=======end Hooked_NtOpenFile=======\n\n\n\n");
	// finish
	tls->file_NtOpenFile_lock = false;
	tls->thread_lock = false;

	return status;

}


NTSTATUS NTAPI Hooked_NtCreateFile(
	PHANDLE FileHandle, ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition, ULONG CreateOptions,
	PVOID EaBuffer, ULONG EaLength)
{
	NTSTATUS status;
	THREAD_DATA* tls;
	std::wstring jail_path = utils::JailPath;
	const std::wstring org_path = ObjectAttributes->ObjectName->Buffer;
	OBJECT_ATTRIBUTES  new_obj{ 0 };

	
	tls = tls::getTlsData();
	// anti-reentrance
	if (tls->thread_lock || tls->file_NtCreateFile_lock) {

		return Hooked_NtCreateFile_Next(FileHandle,
			DesiredAccess, ObjectAttributes, IoStatusBlock,
			AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
		);
	}

	tls->file_NtCreateFile_lock = true;
	tls->thread_lock = true;
	do
	{
		debug_string(L"=======begin Hooked_NtCreateFile=======\n");
		{// debug
			std::wstring msg;
			msg += L"org_path:  "; msg += org_path;  msg += L"\n";
			debug_string(msg.c_str());
		}

		if (utils::iends_with(org_path, { {L".docx"}, {L".tmp"} })) {
			::DebugBreak();
		}
		// filter first
		if (pf::Filter_Out_By_Params_NtCreateFile(DesiredAccess, ObjectAttributes, FileAttributes,
				ShareAccess, CreateDisposition, CreateOptions)
			) {

			debug_string(L"Match filter police, direct call next\n");
			status = Hooked_NtCreateFile_Next(FileHandle,
				DesiredAccess, ObjectAttributes, IoStatusBlock,
				AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
			);
			break;
		}


		// create jail path
		utils::config_jail_path(jail_path, org_path);
		{// debug
			std::wstring msg;
			msg += L"jail_path: "; msg += jail_path; msg += L"\n";
			debug_string(msg.c_str());
		}

		// extend: how to extend the object_attributes
		mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);				

		// org_path has not been recorded, we need to create the detour one
		if (!sys::is_in_map(org_path)) {

			// add new feature, client want to create a folder
			if ((CreateDisposition == FILE_CREATE) && (CreateOptions & FILE_DIRECTORY_FILE)) {
				debug_string(L"user want to create a folder, make it in jail");
			}
			else {
				// make sure path exist in jail
				std::wstring folder = jail_path.substr(0, jail_path.find_last_of(L'\\'));
				utils::MakeSureFolderExist(folder.c_str());
				// check if file exsit
				if (utils::FileExists(org_path.c_str())) {
					// copy to jail path
					debug_string(L"copy org file to jail");
					utils::copy_file_wrapper(org_path.c_str(), jail_path.c_str());
				}
				else {
					debug_string(L"org path has not been detoured, and org path is not exsit in org intent");
				}
			}
		}
		else {
			debug_string(L"org path has been detoured");
		}


		// redirect the org path to the jail path
		debug_string(L"call Hooked_NtCreateFile_Next\n");
		debug_param_ntcreatefile(DesiredAccess, &new_obj, FileAttributes, ShareAccess, CreateDisposition, CreateOptions);
		
		status = Hooked_NtCreateFile_Next(FileHandle,
			DesiredAccess, &new_obj, IoStatusBlock,
			AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
		);

		if (NT_SUCCESS(status)) {
			// record map, org_path ->jail_path;
			debug_string(L"succeed call, record map[org_path ->jail_path]\n");
			sys::record_map(org_path, jail_path);
		}
		else {
			debug_string_with_NTSTATUS(L"!!!Notice!!!,Failed calling NtCreateFile", status);
		}


	} while (false);


	debug_string(L"=======end Hooked_NtCreateFile=======\n\n\n\n");
	// finish
	tls->file_NtCreateFile_lock = false;
	tls->thread_lock = false;

	return status;
}



NTSTATUS NTAPI Hooked_NtQueryAttributesFile(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_BASIC_INFORMATION* FileInformation)
{
	NTSTATUS status;
	THREAD_DATA* tls;

	tls = tls::getTlsData();
	// anti-reentrance
	if (tls->thread_lock || tls->file_NtQueryAttributesFile_lock) {
		return Hooked_NtQueryAttributesFile_Next(ObjectAttributes, FileInformation);
	}
	tls->file_NtQueryAttributesFile_lock = true;
	//
	// main logic
	//
	debug_string(L"=======begin Hooked_NtQueryAttributesFile=======\n");

	// create jail path
	std::wstring jail_path = utils::JailPath;
	const std::wstring org_path = ObjectAttributes->ObjectName->Buffer;

	{// debug string
		std::wstring msg = L"ObjectName: ";
		msg += org_path;
		debug_string(msg.c_str());
	}
	do {
		// org_path has been detoured
		if (sys::is_in_map(org_path)) {
			// detour it 
			OBJECT_ATTRIBUTES  new_obj{ 0 };
			// create full jail path
			utils::config_jail_path(jail_path, org_path);
			mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);
			status = Hooked_NtQueryAttributesFile_Next(&new_obj, FileInformation);
		}
		else {
			status = Hooked_NtQueryAttributesFile_Next(ObjectAttributes, FileInformation);
		}
	} while (0);

	debug_string(L"=======end NtQueryAttributesFile=======\n\n");
	//
	// exit
	//
	tls->file_NtQueryAttributesFile_lock = false;

	return status;
}

NTSTATUS NTAPI Hooked_NtQueryFullAttributesFile(
	OBJECT_ATTRIBUTES* ObjectAttributes,
	FILE_NETWORK_OPEN_INFORMATION* FileInformation)
{
	NTSTATUS status;
	THREAD_DATA* tls;

	tls = tls::getTlsData();
	// anti-reentrance
	if (tls->thread_lock || tls->file_NtQueryFullAttributesFile_lock) {
		return Hooked_NtQueryFullAttributesFile_Next(ObjectAttributes, FileInformation);
	}
	tls->file_NtQueryFullAttributesFile_lock = true;
	//
	// main logic
	//
	debug_string(L"=======begin Hooked_NtQueryFullAttributesFile=======\n");

	// create jail path
	std::wstring jail_path = utils::JailPath;
	const std::wstring org_path = ObjectAttributes->ObjectName->Buffer;

	std::wstring msg = L"ObjectName: ";
	msg += org_path; 
	debug_string(msg.c_str());

	do {
		// org_path has been detoured
		if (sys::is_in_map(org_path)) {
			// detour it 
			OBJECT_ATTRIBUTES  new_obj{ 0 };
			// create full jail path
			utils::config_jail_path(jail_path, org_path);
			mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);
			status = Hooked_NtQueryFullAttributesFile_Next(&new_obj, FileInformation);
		}
		else {
			status = Hooked_NtQueryFullAttributesFile_Next(ObjectAttributes, FileInformation);
		}
	} while (0);


	debug_string(L"=======end NtQueryFullAttributesFile=======\n\n\n\n");
	//
	// exit
	//
	tls->file_NtQueryFullAttributesFile_lock = false;

	return status;
}


/*--------------------------------------------------------------
// For Speed Up
--------------------------------------------------------------*/
UINT __stdcall Hooked_GetDriveTypeW(LPCWSTR lpRootPathName)
{
	THREAD_DATA* tls = tls::getTlsData();
	tls->thread_lock = true;

	UINT rt = Hooked_GetDriveTypeW_Next(lpRootPathName);

	tls->thread_lock = false;

	return rt;

}

HRESULT __stdcall Hooked_QISearch(void* that, void* pqit, const IID* const riid, void** ppv)
{
	HRESULT rt = E_NOTIMPL;
	THREAD_DATA* tls = tls::getTlsData();
	tls->thread_lock = true;

	rt = Hooked_QISearch_Next(that, pqit, riid, ppv);

	tls->thread_lock = false;

	return rt;
}

BOOL __stdcall Hooked_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	BOOL rt = false;
	THREAD_DATA* tls = tls::getTlsData();
	tls->thread_lock = true;

	rt = Hooked_WriteFile_Next(hFile, lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);

	tls->thread_lock = false;

	return rt;
}

BOOL __stdcall Hooked_WriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	BOOL rt = false;
	THREAD_DATA* tls = tls::getTlsData();
	tls->thread_lock = true;

	rt = Hooked_WriteFileEx_Next(hFile,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);

	tls->thread_lock = false;

	return rt;
}

/*--------------------------------------------------------------
// For Igored Directly
--------------------------------------------------------------*/
BOOL __stdcall Hooked_SetFileSecurityW(LPCWSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	return TRUE;
}





bool _stdcall init_file_virtualization() {

	// clear Nextlabs_jack 

	SHFILEOPSTRUCTW file_op = {
	NULL,
	FO_DELETE,
	LR"_(C:\Users\oye\Nextlabs_Jail)_",
	L"",
	FOF_NOCONFIRMATION |
	FOF_NOERRORUI |
	FOF_SILENT,
	false,
	0,
	L"" };

	::SHFileOperationW(&file_op);

	// hook ntcreatefile / ntopenfile
	CollectHooks();
	if (!HookAPI("ntdll", "NtCreateFile", Hooked_NtCreateFile, (void**)&Hooked_NtCreateFile_Next, 0)) {
		return false;
	}
	if (!HookAPI("ntdll", "NtOpenFile", Hooked_NtOpenFile, (void**)&Hooked_NtOpenFile_Next, 0)) {
		return false;
	}

	if (!HookAPI("ntdll", "NtQueryAttributesFile", Hooked_NtQueryAttributesFile, (void**)&Hooked_NtQueryAttributesFile_Next, 0)) {
		return false;
	}

	if (!HookAPI("ntdll", "NtQueryFullAttributesFile", Hooked_NtQueryFullAttributesFile, (void**)&Hooked_NtQueryFullAttributesFile_Next, 0)) {
		return false;
	}


	// for noisy filter helper:  to hook some highlevel api to set the lock and getthrough
	{
		if (!HookAPI("Kernelbase", "GetDriveTypeW", Hooked_GetDriveTypeW, (void**)&Hooked_GetDriveTypeW_Next, 0)) {
			return false;
		}

		if (!HookAPI("Kernelbase", "QISearch", Hooked_QISearch, (void**)&Hooked_QISearch_Next, 0)) {
			return false;
		}
		if (!HookAPI("Kernelbase", "WriteFile", Hooked_WriteFile, (void**)&Hooked_WriteFile_Next, 0)) {
			return false;
		}
		if (!HookAPI("Kernelbase", "WriteFileEx", Hooked_WriteFileEx, (void**)&Hooked_WriteFileEx_Next, 0)) {
			return false;
		}
	}

	// ignored
	{
		if (!HookAPI("Kernelbase", "SetFileSecurityW", Hooked_SetFileSecurityW, (void**)&Hooked_SetFileSecurityW_Next, 0)) {
			return false;
		}
	}


	FlushHooks();



	return true;
}