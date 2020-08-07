#pragma once



// param filter
namespace pf {

    bool Filter_Out_By_ObjectAttributes(POBJECT_ATTRIBUTES ObjectAttributes);

    bool Filter_Out_By_Params_NtCreateFile(
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        ULONG FileAttributes,
        ULONG ShareAccess,
        ULONG CreateDisposition,
        ULONG CreateOptions);

    bool Filter_Out_By_Params_NtOpenFile(
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        ULONG ShareAccess,
        ULONG OpenOptions);

}