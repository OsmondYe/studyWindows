#pragma once
#include <Windows.h>

#include <pshpack8.h>
typedef struct _THREAD_DATA {

    bool thread_lock;

    //
    // API locks
    //
    bool file_NtCreateFile_lock;
    bool file_NtOpenFile_lock;

    bool file_NtQueryAttributesFile_lock;
    bool file_NtQueryFullAttributesFile_lock;


} THREAD_DATA;
#include <poppack.h>

