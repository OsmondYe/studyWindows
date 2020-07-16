#include "framework.h"
#include "pf.h"
#include <string>
#include <set>
#include <algorithm>

using namespace std;

/*
Characteristic code:
- folder : CreateOptions & FILE_DIRECTORY_FILE
- device : \??\nxrmdrv  \??\pipe\nxrmservice,  ,\DEVICE\ ,\??\SrpDevice
- 

- path:{
	system_path {  C:\Program Files   C:\Program Files (x86)  C:\Windows  }
	suffix { .dll  }
}
	
*/

namespace {
	auto lamba_ignore_case_compare_w = [](wchar_t l, wchar_t r) { return std::toupper(l) == std::toupper(r); };
	
	inline bool icontain(const wstring& m, const wstring& s) {
		if (m.size() < s.size()) {
			return false;
		}
		return m.end() != std::search(m.begin(), m.end(), s.begin(), s.end(), lamba_ignore_case_compare_w);
	}

	inline bool iends_with(const wstring& m, const wstring& s) {
		if (m.size() < s.size()) {
			return false;
		}
		return std::equal(m.rbegin(), m.rbegin()+s.size(), s.rbegin(), s.rend(), [](wchar_t l, wchar_t r) {
			return std::tolower(l) == std::tolower(r);
			});

	}

	inline bool iends_with(const wstring& m, const set<wstring>& ss) {
		for (auto& s : ss) {
			if (iends_with(m, s)) {
				return true;
			}
		}
		return false;
	}


	inline bool is_qfn_begins(const wstring& m) {
		if (m.size() < 5) {
			return false;
		}
		if (m[0] == L'\\' &&
			m[1] == L'?' &&
			m[2] == L'?' &&
			m[3] == L'\\' &&
			m[5] == L':'
			) {
			return true;
		}
		return false;
	}

	inline bool is_valid_ObjectAttributes(POBJECT_ATTRIBUTES ObjectAttributes) {
		if (!ObjectAttributes) {
			return false;
		}
		if (!ObjectAttributes->ObjectName) {
			return false;
		}

		if (ObjectAttributes->ObjectName->Length <= 8) {
			// invalid 
			return false;
		}

		return true;
	}

	inline bool is_matched_str_ObjectAttributes(POBJECT_ATTRIBUTES ObjectAttributes) {

		PUNICODE_STRING  puniStr = ObjectAttributes->ObjectName;

		wstring path = puniStr->Buffer;
		int len = puniStr->Length;
		
		// if  match suffix .dll
		static set<wstring> suffix = {
			{L".dll"},{L"exe"}, 
			{L".db"},{L".db-journal"},{L".db-wal"},{L".db.session"},{L".db.session-journal"}, {L".db-shm"}
		};
		
		if (iends_with(path, suffix)) {
			// match dll
			return true;
		}
		
		// if valid disk path
		if (!is_qfn_begins(path)) {
			return true;
		}
		else {
			// Window and Program folder
			static wstring sfWindows = { L":\\Windows" };
			static wstring sfProgramm = { L":\\Program Files" };
			static wstring sfProgramme2 = { L":\\Program Files (x86)" };

			if (icontain(path, sfWindows) || icontain(path, sfProgramm) || icontain(path, sfProgramme2)) {
				return true;
			}
		}

		return false;
	}

}


// filter out readonly, directory system folder, device, ntwork,
bool pf::Filter_Out_By_Params_NtCreateFile(ACCESS_MASK DesiredAccess, 
	POBJECT_ATTRIBUTES ObjectAttributes, 
	ULONG FileAttributes, 
	ULONG ShareAccess, 
	ULONG CreateDisposition, 
	ULONG CreateOptions)
{
	if (!is_valid_ObjectAttributes(ObjectAttributes)) {
		return true;
	}

	// folder will be ingored
	if (CreateOptions & FILE_DIRECTORY_FILE) {
		return true;
	}

	if (!(CreateOptions & FILE_NON_DIRECTORY_FILE)) {
		// no this, is not a file 
		return true;
	}

	if (CreateOptions == FILE_SYNCHRONOUS_IO_ALERT) {
		// SkyDRM, Tenant.history,Instance.history,
		return true;
	}

	if (CreateOptions == (FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT)) {
		//ObjectName: \??\D:\,
		return true;
	}




	// for match str will be ingored
	if (is_matched_str_ObjectAttributes(ObjectAttributes)) {
		return true;
	}

	return false;
}
