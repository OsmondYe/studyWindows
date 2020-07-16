#pragma once
#include <Windows.h>

#include <pshpack8.h>
typedef struct _THREAD_DATA {

    //
    // API locks
    //
    bool file_NtCreateFile_lock;
    //BOOLEAN file_NtClose_lock;


} THREAD_DATA;
#include <poppack.h>

