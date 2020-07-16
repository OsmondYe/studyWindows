#include "framework.h"
#include "debug.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>


using namespace std;


namespace {

	class LogStream {
	public:
		LogStream() {
			_fs.open(R"_(D:\allTestFile\log_analyze\log.log)_", ios::app | ios::out | ios::ate);
		}
		~LogStream() { _fs.close(); }

		wofstream& get() { return _fs; }

		wofstream _fs;
	};

	

	void WriteLog(const wstring& log) {
		// write to file	
		LogStream ls;
		auto& s = ls.get();

		s << log;

		if (log.back() != L'\n') {
			s << endl;
		}
		s << flush;

		//::OutputDebugStringW(log.c_str());
	}
}


inline wstring parseObject(POBJECT_ATTRIBUTES ObjectAttributes) {
	wstring rt;
	if (!ObjectAttributes) {
		rt += L"NULL";
		return rt;
	}
	if (ObjectAttributes->RootDirectory) {
		wstringstream wss;
		wss << showbase<<hex << (int)ObjectAttributes->RootDirectory;
		rt += L"RootDirectory: ";
		rt += wss.str();
		rt += L",\t";
	}
	else {
		rt += L"RootDirectory: NULL,\t ";
	}
	if (ObjectAttributes->ObjectName) {
		wstring s = ObjectAttributes->ObjectName->Buffer;
		rt += L"ObjectName: ";
		rt += s;
		rt += L",\t";
	}
	// parse attributes
	{
		rt += L"Attributes:{ ";
		ULONG att = ObjectAttributes->Attributes;
		if (att & OBJ_INHERIT) {
			rt += L"OBJ_INHERIT |";
		}
		if (att & OBJ_PERMANENT) {
			rt += L"OBJ_PERMANENT |";
		}
		if (att & OBJ_EXCLUSIVE) {
			rt += L"OBJ_EXCLUSIVE |";
		}
		if (att & OBJ_CASE_INSENSITIVE) {
			rt += L"OBJ_CASE_INSENSITIVE |";
		}
		if (att & OBJ_OPENIF) {
			rt += L"OBJ_OPENIF |";
		}
		if (att & OBJ_OPENLINK) {
			rt += L"OBJ_OPENLINK |";
		}
		if (att & OBJ_KERNEL_HANDLE) {
			rt += L"OBJ_KERNEL_HANDLE |";
		}
		if (att & OBJ_FORCE_ACCESS_CHECK) {
			rt += L"OBJ_FORCE_ACCESS_CHECK |";
		}
		if (att & OBJ_DONT_REPARSE) {
			rt += L"OBJ_DONT_REPARSE |";
		}
		if (att & OBJ_IGNORE_IMPERSONATED_DEVICEMAP) {
			rt += L"OBJ_IGNORE_IMPERSONATED_DEVICEMAP |";
		}
		if (att & OBJ_VALID_ATTRIBUTES) {
			rt += L"OBJ_VALID_ATTRIBUTES |";
		}

		rt += L"},\t";
	}
	


	return rt;
}

inline wstring parseDesiredAccess(ACCESS_MASK DesiredAccess) {
	wstring rt;
	if (DesiredAccess & DELETE) {
		rt += L"DELETE | ";
	}
	if (DesiredAccess & FILE_READ_DATA) {
		rt += L"FILE_READ_DATA | ";
	}
	if (DesiredAccess & FILE_READ_ATTRIBUTES) {
		rt += L"FILE_READ_ATTRIBUTES | ";
	}
	if (DesiredAccess & FILE_READ_EA) {
		rt += L"FILE_READ_EA | ";
	}
	if (DesiredAccess & READ_CONTROL) {
		rt += L"READ_CONTROL | ";
	}
	if (DesiredAccess & FILE_WRITE_DATA) {
		rt += L"FILE_WRITE_DATA | ";
	}
	if (DesiredAccess & FILE_WRITE_ATTRIBUTES) {
		rt += L"FILE_WRITE_ATTRIBUTES | ";
	}
	if (DesiredAccess & FILE_WRITE_EA) {
		rt += L"FILE_WRITE_EA | ";
	}
	if (DesiredAccess & FILE_APPEND_DATA) {
		rt += L"FILE_APPEND_DATA | ";
	}
	if (DesiredAccess & WRITE_DAC) {
		rt += L"WRITE_DAC | ";
	}
	if (DesiredAccess & WRITE_OWNER) {
		rt += L"WRITE_OWNER | ";
	}
	if (DesiredAccess & SYNCHRONIZE) {
		rt += L"SYNCHRONIZE | ";
	}	
	if (DesiredAccess & FILE_EXECUTE) {
		rt += L"FILE_EXECUTE | ";
	}

	// combination
	if (DesiredAccess & FILE_GENERIC_READ) {
		rt += L"FILE_GENERIC_READ | ";
	}
	if (DesiredAccess & FILE_GENERIC_WRITE) {
		rt += L"FILE_GENERIC_WRITE | ";
	}
	if (DesiredAccess & FILE_GENERIC_EXECUTE) {
		rt += L"FILE_GENERIC_EXECUTE | ";
	}

	// dir additional
	if (DesiredAccess & FILE_LIST_DIRECTORY) {  //Files in the directory can be listed.
		rt += L"FILE_LIST_DIRECTORY | ";
	}
	if (DesiredAccess & FILE_TRAVERSE) {  // 
		rt += L"FILE_TRAVERSE | ";
	}

	return rt;
}

inline wstring parseFileAttributes(ULONG FileAttributes) {
	wstring rt;
	if (FileAttributes & FILE_ATTRIBUTE_READONLY) {
		rt += L"FILE_ATTRIBUTE_READONLY | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_HIDDEN) {
		rt += L"FILE_ATTRIBUTE_HIDDEN | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_SYSTEM) {
		rt += L"FILE_ATTRIBUTE_SYSTEM | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		rt += L"FILE_ATTRIBUTE_DIRECTORY | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
		rt += L"FILE_ATTRIBUTE_ARCHIVE | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_DEVICE) {
		rt += L"FILE_ATTRIBUTE_DEVICE | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_NORMAL) {
		rt += L"FILE_ATTRIBUTE_NORMAL | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_TEMPORARY) {
		rt += L"FILE_ATTRIBUTE_TEMPORARY | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_SPARSE_FILE) {
		rt += L"FILE_ATTRIBUTE_SPARSE_FILE | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
		rt += L"FILE_ATTRIBUTE_REPARSE_POINT | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_COMPRESSED) {
		rt += L"FILE_ATTRIBUTE_COMPRESSED | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_OFFLINE) {
		rt += L"FILE_ATTRIBUTE_OFFLINE | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) {
		rt += L"FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
		rt += L"FILE_ATTRIBUTE_ENCRYPTED | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM) {
		rt += L"FILE_ATTRIBUTE_INTEGRITY_STREAM | ";
	}	
	if (FileAttributes & FILE_ATTRIBUTE_VIRTUAL) {
		rt += L"FILE_ATTRIBUTE_VIRTUAL | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA) {
		rt += L"FILE_ATTRIBUTE_NO_SCRUB_DATA | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_EA) {
		rt += L"FILE_ATTRIBUTE_EA | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_PINNED) {
		rt += L"FILE_ATTRIBUTE_PINNED | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_UNPINNED) {
		rt += L"FILE_ATTRIBUTE_UNPINNED | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN) {
		rt += L"FILE_ATTRIBUTE_RECALL_ON_OPEN | ";
	}
	if (FileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) {
		rt += L"FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS | ";
	}
	return rt;
}

inline wstring parseShareAccess(ULONG ShareAccess) {
	wstring rt;
	if (ShareAccess& FILE_SHARE_READ) {
		rt += L"FILE_SHARE_READ | ";
	}
	if (ShareAccess & FILE_SHARE_WRITE) {
		rt += L"FILE_SHARE_WRITE | ";
	}
	if (ShareAccess & FILE_SHARE_DELETE) {
		rt += L"FILE_SHARE_DELETE | ";
	}

	return rt;
}

inline wstring parseCreateDispositon(ULONG CreateDispositon) {
	wstring rt;
	if (CreateDispositon & FILE_SUPERSEDE) {
		rt += L"FILE_SUPERSEDE | ";
	}
	if (CreateDispositon & FILE_CREATE) {
		rt += L"FILE_CREATE | ";
	}
	if (CreateDispositon & FILE_OPEN) {
		rt += L"FILE_OPEN | ";
	}
	if (CreateDispositon & FILE_OPEN_IF) {
		rt += L"FILE_OPEN_IF | ";
	}
	if (CreateDispositon & FILE_OVERWRITE) {
		rt += L"FILE_OVERWRITE | ";
	}
	if (CreateDispositon & FILE_OVERWRITE_IF) {
		rt += L"FILE_OVERWRITE_IF | ";
	}
	
	return rt;
}

inline wstring parseCreateOptions(ULONG CreateOptions) {
	wstring rt;
	if (CreateOptions & FILE_DIRECTORY_FILE) {
		rt += L"FILE_DIRECTORY_FILE | ";
	}
	if (CreateOptions & FILE_NON_DIRECTORY_FILE) {
		rt += L"FILE_NON_DIRECTORY_FILE | ";
	}
	if (CreateOptions & FILE_WRITE_THROUGH) {
		rt += L"FILE_WRITE_THROUGH | ";
	}
	if (CreateOptions & FILE_SEQUENTIAL_ONLY) {
		rt += L"FILE_SEQUENTIAL_ONLY | ";
	}
	if (CreateOptions & FILE_RANDOM_ACCESS) {
		rt += L"FILE_RANDOM_ACCESS | ";
	}
	if (CreateOptions & FILE_NO_INTERMEDIATE_BUFFERING) {
		rt += L"FILE_NO_INTERMEDIATE_BUFFERING | ";
	}
	if (CreateOptions & FILE_SYNCHRONOUS_IO_ALERT) {
		rt += L"FILE_SYNCHRONOUS_IO_ALERT | ";
	}
	if (CreateOptions & FILE_SYNCHRONOUS_IO_NONALERT) {
		rt += L"FILE_SYNCHRONOUS_IO_NONALERT | ";
	}
	if (CreateOptions & FILE_CREATE_TREE_CONNECTION) {
		rt += L"FILE_CREATE_TREE_CONNECTION | ";
	}
	if (CreateOptions & FILE_NO_EA_KNOWLEDGE) {
		rt += L"FILE_NO_EA_KNOWLEDGE | ";
	}
	if (CreateOptions & FILE_OPEN_REPARSE_POINT) {
		rt += L"FILE_OPEN_REPARSE_POINT | ";
	}
	if (CreateOptions & FILE_DELETE_ON_CLOSE) {
		rt += L"FILE_DELETE_ON_CLOSE | ";
	}
	if (CreateOptions & FILE_OPEN_BY_FILE_ID) {
		rt += L"FILE_OPEN_BY_FILE_ID | ";
	}
	if (CreateOptions & FILE_OPEN_FOR_BACKUP_INTENT) {
		rt += L"FILE_OPEN_FOR_BACKUP_INTENT | ";
	}
	if (CreateOptions & FILE_RESERVE_OPFILTER) {
		rt += L"FILE_RESERVE_OPFILTER | ";
	}
	if (CreateOptions & FILE_OPEN_REQUIRING_OPLOCK) {
		rt += L"FILE_RESERVE_OPFILTER | ";
	}
	if (CreateOptions & FILE_COMPLETE_IF_OPLOCKED) {
		rt += L"FILE_RESERVE_OPFILTER | ";
	}

	return rt;
}

void debug_param_ntcreatefile(ACCESS_MASK DesiredAccess, 
	POBJECT_ATTRIBUTES ObjectAttributes, 
	ULONG FileAttributes, 
	ULONG ShareAccess, 
	ULONG CreateDisposition, 
	ULONG CreateOptions)
{
	wstring msg;
	
	msg += L"---->>>>>for NtCreateFile:\n";

	msg += L"\tObject: { ";
	msg += parseObject(ObjectAttributes);
	msg += L" }\n";

	msg += L"\tDesiredAccess: { ";
	msg += parseDesiredAccess(DesiredAccess);
	msg += L" }\n";

	msg += L"\tFileAttributes: { ";
	msg += parseFileAttributes(FileAttributes);
	msg += L" }\n";

	msg += L"\tShareAccess: { ";
	msg += parseShareAccess(ShareAccess);
	msg += L" }\n";

	msg += L"\tCreateDisposition: { ";
	msg += parseCreateDispositon(CreateDisposition);
	msg += L" }\n";

	msg += L"\tCreateOptions: { ";
	msg += parseCreateOptions(CreateOptions);
	msg += L" }\n";


	msg += L"----end>>>for NtCreateFile:\n";


	WriteLog(msg);
}

void debug_string(const wchar_t* str)
{
	WriteLog(str);
}

void debug_string_with_NTSTATUS(const wchar_t* str, NTSTATUS status)
{
	wstring msg = str;
	wstringstream wss;
	
	msg += L",status=0x";
	wss << hex << status;
	msg += wss.str();

	WriteLog(msg);
}

