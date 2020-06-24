#pragma once

//
// process module
//
class Module {
public:
	Module(HMODULE hm = NULL) :m_hm(hm) {
		std::vector<char> buf(255, 0);
		buf.resize(GetModuleFileNameA(m_hm, (char*)buf.data(), 255));
		m_path.assign(buf.begin(), buf.end());

		std::vector<wchar_t> wbuf(255, 0);
		wbuf.resize(GetModuleFileNameW(m_hm, (wchar_t*)wbuf.data(), 255));
		m_wpath.assign(wbuf.begin(), wbuf.end());
	}

	inline const std::string GetPathA() { return m_path; }
	inline const std::wstring GetPathW() { return m_wpath; }
	inline std::string GetNameA() { return m_path.substr(m_path.find_last_of("\\/") + 1); }
	inline std::wstring GetNameW() { return m_wpath.substr(m_wpath.find_last_of(L"\\/") + 1); }

private:
	HMODULE m_hm;
	std::string m_path;
	std::wstring m_wpath;
};


//
//  window
//


//
//  registry
//


class registry {
public:
	class param {
		friend class registry;
	public:
		param(std::wstring path) :sub_key(path), access_right(KEY_READ), root(HKEY_CURRENT_USER), open_key(NULL) {}
	private:
		HKEY root; //HKEY_CLASSES_ROOT,HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE
		std::wstring sub_key;
		REGSAM access_right;
		HKEY open_key;
		class close_guard {
			friend class registry;
			HKEY _open_key;
			close_guard(HKEY open_key) :_open_key(open_key) {}
			~close_guard() { ::RegCloseKey(_open_key); }
		};
	};

	bool get(param& p, const std::wstring& name, std::wstring& out_value) {
		if (!_open(p)) {
			return false;
		}
		param::close_guard gurad(p.open_key);
		std::uint32_t length = _buflen(p, name);
		if (0 > length) {
			return false;
		}
		else if (0 == length) {
			out_value.clear();
			return true;
		}
		else {
			out_value.resize(length / 2);
		}

		if (!_get(p, name, (std::uint8_t*)out_value.data(), length)) {
			return false;
		}
		// str trim
		if (out_value.back() == '\0') {
			out_value.pop_back();
		}
		return true;

	}

	bool get(param& p, const std::wstring& name, std::uint32_t& out_value) {
		if (!_open(p)) {
			return false;
		}
		param::close_guard gurad(p.open_key);
		std::uint32_t length = _buflen(p, name);
		if (-1 == length || length != sizeof(out_value)) {
			return false;
		}
		return _get(p, name, (std::uint8_t*) & out_value, sizeof(out_value));
	}
private:
	inline bool _open(param& p) {
		return ERROR_SUCCESS == ::RegOpenKeyExW(p.root, p.sub_key.c_str(), NULL, p.access_right, &p.open_key);
	}
	inline std::uint32_t _buflen(param& p, const std::wstring& name) {
		DWORD length = -1;
		DWORD type;
		::RegQueryValueExW(p.open_key, name.c_str(), NULL, &type, NULL, &length);
		return length;

	}
	inline bool _get(param& p, const std::wstring& name, std::uint8_t* buf, std::uint32_t buf_len) {
		return ERROR_SUCCESS == ::RegQueryValueExW(p.open_key, name.c_str(), NULL, NULL, buf, (DWORD*)&buf_len);
	}

};

