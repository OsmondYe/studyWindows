#pragma once

#include <string>
#include <set>

namespace utils {

	inline bool iends_with(const std::wstring& m, const std::wstring& s) {
		if (m.size() < s.size()) {
			return false;
		}
		return std::equal(m.rbegin(), m.rbegin() + s.size(), s.rbegin(), s.rend(), [](wchar_t l, wchar_t r) {
			return std::tolower(l) == std::tolower(r);
			});

	}

	inline bool ibegin_with(const std::wstring& m, const std::wstring& s) {
		auto m_b = m.cbegin();
		auto s_b = s.cbegin();
		for (; m_b != m.cend() && s_b != s.cend(); m_b++, s_b++) {
			if (tolower(*m_b++) != tolower(*s_b++)) {
				return m_b == m.cbegin() ? false : true;
			}
		}
		return true;
	}


	inline bool iends_with(const std::wstring& m, const std::set<std::wstring>& ss) {
		for (auto& s : ss) {
			if (iends_with(m, s)) {
				return true;
			}
		}
		return false;
	}

	// s is a sub set of m
	inline bool iconatain(const std::wstring& m, const std::wstring& s) {
		if (m.size() < s.size()) {
			return false;
		}
		auto m_b = m.cbegin();
		auto s_b = s.cbegin();
		for (; s_b != s.cend(); m_b++, s_b++) {
			if (tolower(*m_b++) != tolower(*s_b++)) {
				return false;
			}
		}
		return true;
	}

	// file utils

	inline bool FileExists(const wchar_t* path)
	{
		DWORD dwAttrib = GetFileAttributes(path);
		return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	inline bool PathIsFolder(const wchar_t* path) {
		DWORD dwAttrib = GetFileAttributes(path);
		return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	inline void MakeSureFolderExist(const wchar_t* path) {
		// make sure path exist
		::SHCreateDirectoryEx(NULL, path, NULL);
	}
}
