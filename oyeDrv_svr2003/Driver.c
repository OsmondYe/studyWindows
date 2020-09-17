
#include <ntddk.h>
#include <initguid.h>

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID(GUID_DEVINTERFACE_oyeDrvsvr2003,
    0x9fed3ea1, 0xb4ae, 0x4841, 0xbc, 0xc5, 0x44, 0xb8, 0xa6, 0x84, 0x94, 0x59);


EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;

EXTERN_C_END

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#endif

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
   
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("hello driver");

    return STATUS_UNSUCCESSFUL;
}
