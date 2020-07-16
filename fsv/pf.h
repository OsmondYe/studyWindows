#pragma once



// param filter
namespace pf {  
    bool Filter_Out_By_Params_NtCreateFile(
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        ULONG FileAttributes,
        ULONG ShareAccess,
        ULONG CreateDisposition,
        ULONG CreateOptions);

}