#pragma once




void debug_param_ntcreatefile(
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions);

void debug_param_ntopenfile(
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    ULONG ShareAccess,
    ULONG OpenOptions
);



void debug_string(const wchar_t* str);

void debug_string_with_NTSTATUS(const wchar_t* str, NTSTATUS status);

void debug_string_with_LastError(const wchar_t* str, DWORD lasterror);