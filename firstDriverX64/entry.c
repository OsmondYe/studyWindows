#include <ntddk.h>

#define POOL_TAG_REGISTRY 'geRO'


/* dbgprint
	%x		hex
	%wZ		&UnicodeString
*/ 


/*
	Memory 
	ExAllocatePoolWithTag  ExFreePoolWithTag

*/


//
// A structure representing the instance information associated with
// a particular device
//
#pragma pack(push,8)

typedef struct _GLOBAL_DATA {
	BOOLEAN		bDbgBrk;
	ULONG		os_major;
	ULONG		os_minor;
	ULONG		os_build;
}GLOBAL_DATA;

#pragma pack(pop)



GLOBAL_DATA global = { 0 };


NTSTATUS InitGlobalData();

VOID DriverUnload(PDRIVER_OBJECT driver)
{
	DbgPrint("hello: Our driver is unloading...\r\n");
}


NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING reg_path)
{
	DbgPrint("hello Driver, Osmond\n");

	InitGlobalData();

	if(global.bDbgBrk){
		DbgBreakPoint();
	}

	driver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

NTSTATUS InitGlobalData() {
	/* load keys from registry
	*	\Registry\Machine\Software\oye\driver
	*/
	global.bDbgBrk = FALSE;
	// for bDbgBrk;
	NTSTATUS rt = STATUS_SUCCESS;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Oye\\Driver");
	UNICODE_STRING ValueDbgBrk = RTL_CONSTANT_STRING(L"allow_debug_break");
	OBJECT_ATTRIBUTES KeyObj = { 0 };
	InitializeObjectAttributes(&KeyObj, &KeyPath, OBJ_CASE_INSENSITIVE, NULL, NULL);
	HANDLE Key;
	KEY_VALUE_PARTIAL_INFORMATION KeyFake;
	PKEY_VALUE_PARTIAL_INFORMATION pKeyValue;
	LONG KeyValue_Length;


	rt = ZwOpenKey(&Key, KEY_QUERY_VALUE, &KeyObj);
	if (!NT_SUCCESS(rt)) {
		DbgPrint("[odrv], open key failed %wZ, error: 0x%x\n",&KeyPath,rt);
		return rt;
	}

	rt = ZwQueryValueKey(Key, &ValueDbgBrk, KeyValuePartialInformation, &KeyFake, sizeof(KeyFake), &KeyValue_Length);
	if (!NT_SUCCESS(rt) && rt != STATUS_BUFFER_OVERFLOW && rt != STATUS_BUFFER_TOO_SMALL) {
		// unexpected error;
		DbgPrint("[odrv],call ZwQueryValueKey failed, error: 0x%x\n", rt);
		return rt;
	}
	
	// allocate memory
	pKeyValue = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(PagedPool, KeyValue_Length, POOL_TAG_REGISTRY);
	if (pKeyValue == NULL) {
		rt = STATUS_INSUFFICIENT_RESOURCES;
		DbgPrint("[odrv], call ExAllowcatePoolWithTag failed, error: 0x%x\n", rt);
		return rt;
	}

	// call again
	rt = ZwQueryValueKey(Key, &ValueDbgBrk, KeyValuePartialInformation, pKeyValue, KeyValue_Length, &KeyValue_Length);
	if (!NT_SUCCESS(rt)) {
		// unexpected error;
		DbgPrint("[odrv],call ZwQueryValueKey failed, error: 0x%x\n", rt);
		return rt;
	}

	// check the value;
	ULONG value = *(ULONG*)pKeyValue->Data;

	global.bDbgBrk = value;


	// release
	if (Key) {
		ZwClose(Key);
	}
	if (pKeyValue) {
		ExFreePoolWithTag(pKeyValue, POOL_TAG_REGISTRY);
	}


	// get version
	OSVERSIONINFOW osv = { 0 };
	osv.dwOSVersionInfoSize = sizeof(osv);

	RtlGetVersion(&osv);

	global.os_build = osv.dwBuildNumber;
	global.os_major = osv.dwMajorVersion;
	global.os_minor = osv.dwMinorVersion;


	return rt;
}