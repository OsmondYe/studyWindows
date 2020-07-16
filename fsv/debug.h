#pragma once




void debug_param_ntcreatefile(
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions);



void debug_string(const wchar_t* str);

void debug_string_with_NTSTATUS(const wchar_t* str, NTSTATUS status);