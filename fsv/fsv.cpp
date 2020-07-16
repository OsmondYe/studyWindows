/*
    File Virtualization
    For Single File, for first release 

    using TLS to setup locks

    History:

        
*/

#include "framework.h"
#include "fsv.h"
#include "tls.h"
#include "ds.h"
#include "pf.h"
#include <string>
#include <map>
#include <shlobj_core.h>


#pragma comment(lib,"ntdll.lib")

#ifdef _DEBUG
#include "debug.h"
#endif // _DEBUG



#ifndef  _WIN64
#define WOW64_FS_REDIR
#endif ! _WIN64



typedef NTSTATUS (NTAPI *Sig_NtCreateFile)(
     PHANDLE FileHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     PIO_STATUS_BLOCK IoStatusBlock,
     PLARGE_INTEGER AllocationSize ,
     ULONG FileAttributes,
     ULONG ShareAccess,
     ULONG CreateDisposition,
     ULONG CreateOptions,
     PVOID EaBuffer ,
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


Sig_NtCreateFile Hooked_NtCreateFile_Next = NULL;
Sig_NtOpenFile Hooked_NtOpenFile_Next = NULL;






//
// Data and Param Analyzing  for NtCreateFile
//

inline bool isGenericeRead(ACCESS_MASK desiredAccess) {
    return desiredAccess == FILE_GENERIC_READ;
}


namespace util {
    inline bool FileExists(const wchar_t* path)
    {
        DWORD dwAttrib = GetFileAttributes(path);
        return ( (dwAttrib != INVALID_FILE_ATTRIBUTES) &&!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    inline bool PathIsFolder(const wchar_t* path) {
        DWORD dwAttrib = GetFileAttributes(path);
        return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    inline void MakeSureFolderExist(const wchar_t* path) {
       // make sure path exist
       ::SHCreateDirectoryEx(NULL, path, NULL);
    }
    // build jail path from org path
    inline void config_jail_path(std::wstring& jail, const std::wstring& org) {
        // create full jail path
        std::wstring  new_path = org;
        if (new_path[5] == L':') {
            //org_path[5] = L'\\';
            new_path.erase(5, 1);
        }
        new_path.insert(4, jail);
        jail = new_path;

    }
}

namespace mem {

    void* mem_alloc(ULONG size) {
        void* ptr;
        ptr = ::VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
        return ptr;
    }

    void mem_free(void* ptr) {
        if(!::VirtualFree(ptr, 0, MEM_RELEASE)){
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

    THREAD_DATA*  getTlsData() {
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
    inline bool is_in_map(const std::wstring& org) {
        return path_map.count(org) != 0;
    }

    CRITICAL_SECTION gcs;

    inline void init_cs() {
        ::InitializeCriticalSection(&gcs);
    }
    

}


SV_API bool _stdcall init_file_virtualization() {
    // hook ntcreatefile / ntopenfile
    if (!HookAPI("ntdll", "NtCreateFile", Hooked_NtCreateFile, (void**)&Hooked_NtCreateFile_Next, 0)) {
        return false;
    }
    if (!HookAPI("ntdll", "NtOpenFile", Hooked_NtOpenFile, (void**)&Hooked_NtOpenFile_Next, 0)) {
        return false;
    }


    return true;
}


NTSTATUS NTAPI Hooked_NtOpenFile(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
) {

    return Hooked_NtCreateFile_Next(FileHandle,
        DesiredAccess, ObjectAttributes, IoStatusBlock,
        NULL, 0, ShareAccess, FILE_OPEN, OpenOptions, NULL, 0
    );

}





NTSTATUS NTAPI Hooked_NtCreateFile(
    PHANDLE FileHandle,ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,    ULONG CreateOptions,
    PVOID EaBuffer,ULONG EaLength
)
{
    NTSTATUS status;
    THREAD_DATA* tls;

    tls = tls::getTlsData();
    // anti-reentrance
    if (tls->file_NtCreateFile_lock) {

        return Hooked_NtCreateFile_Next(FileHandle,
            DesiredAccess, ObjectAttributes, IoStatusBlock,
            AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
        );
    }    

    tls->file_NtCreateFile_lock = true;

    debug_string(L"=======begin Hooked_NtCreateFile=======");

   
    // create jail path
    std::wstring jail_path = LR"_(D:\Nextlabs_Jail\)_";
    const std::wstring org_path = ObjectAttributes->ObjectName->Buffer;
    
    std::wstring msg = L"ObjectName: ";
    msg += org_path;
    debug_string(msg.c_str());

    do
    {
        //
        // filter first
        //
        if (util::PathIsFolder(org_path.c_str()) ||
            pf::Filter_Out_By_Params_NtCreateFile(DesiredAccess, ObjectAttributes, FileAttributes,
            ShareAccess, CreateDisposition, CreateOptions)
            ) {

            debug_string(L"match fitler, direct call next");
            
            status = Hooked_NtCreateFile_Next(FileHandle,
                DesiredAccess, ObjectAttributes, IoStatusBlock,
                AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
            );
            break;
        }

        // create full jail path
        util::config_jail_path(jail_path, org_path);     

        // detour it 
        OBJECT_ATTRIBUTES  new_obj{ 0 };

        // org_path has been detoured
        if (sys::is_in_map(org_path)) {
            mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);
        }
        else {
            // check if file exsit
            if (util::FileExists(org_path.c_str())) {
                // copy to jail path
                std::wstring folder = jail_path.substr(0, jail_path.find_last_of(L'\\'));
                util::MakeSureFolderExist(folder.c_str());
                ::CopyFile(org_path.c_str(), jail_path.c_str(), false);
            }
            else {
                // make sure path exist in jail
                std::wstring folder = jail_path.substr(0, jail_path.find_last_of(L'\\'));
                util::MakeSureFolderExist(folder.c_str());
            }

            mem::new_object_attribute(new_obj, ObjectAttributes, jail_path);
        }

        //
        if (!(ShareAccess & FILE_SHARE_READ)) {
            ShareAccess = (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
        }


        // redirect the org path to the jail path
        debug_string(L"call Hooked_NtCreateFile_Next");
        status = Hooked_NtCreateFile_Next(FileHandle,
            DesiredAccess, &new_obj, IoStatusBlock,
            AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength
        );

        if (NT_SUCCESS(status)) {
            // record map, org_path ->jail_path;
            debug_string(L"call ok, record map, org_path ->jail_path");
            sys::record_map(org_path, jail_path);
        }
        else {
            debug_string_with_NTSTATUS(L"call failed",status);
        }


        debug_param_ntcreatefile(DesiredAccess, ObjectAttributes, FileAttributes, ShareAccess, CreateDisposition, CreateOptions);

    } while (false);
    

    debug_string(L"\n\n=======end Hooked_NtCreateFile=======\n\n\n\n");
    // finish
    tls->file_NtCreateFile_lock = false;
    
    return status;
}

