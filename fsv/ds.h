#pragma once
#include <Windows.h>

typedef struct _THREAD_DATA {

    //
    // name buffers:  first index is for true name, second for copy name
    //

    WCHAR* name_buffer[2][NAME_BUFFER_DEPTH];
    ULONG name_buffer_len[2][NAME_BUFFER_DEPTH];
    int depth;

    //
    // locks
    //

    BOOLEAN key_NtCreateKey_lock;

    BOOLEAN file_NtCreateFile_lock;
    BOOLEAN file_NtClose_lock;
    BOOLEAN file_GetCurDir_lock;

    BOOLEAN ipc_KnownDlls_lock;

    BOOLEAN obj_NtQueryObject_lock;

    //
    // file module
    //

    ULONG file_dont_strip_write_access;

} THREAD_DATA;


