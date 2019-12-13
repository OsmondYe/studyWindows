#include "stdafx.h"

//use DISABLED_ prefix to filter out 

TEST(Windows, VectorAsBuf) {
	std::vector<wchar_t> buf(1024,0);// buf[1024]={0};
	int len = ::GetModuleFileNameW(NULL, buf.data(), 1024);
	int buflen=buf.size();// size==0;
	buf.resize(len);
	std::wstring path(buf.begin(), buf.end());
	wcout << path;
	//
	// either is ok
	//
	wstring buf2(1024, 0);
	len=::GetModuleFileNameW(NULL, (wchar_t*)buf2.data(), 1024);
	buf2.resize(len);

	wcout << buf2;
}



TEST(Windows, DISABLED_ShellOperation) {
	const wchar_t* exe = L"D:\\OyeProject\\CSharp\\rmd-windows\\SkydrmLocal\\Debug\\SkydrmLocal.exe";
	const wchar_t* cmd = L"-share c:\\aaa.txt";
	::ShellExecuteW(NULL, L"open", exe, cmd, NULL, SW_SHOW);
}

TEST(Windows, DISABLED_RegOperation) {
	HKEY root = HKEY_CURRENT_USER;
	const wchar_t* parent = L"Software\\NextLabs\\SkyDRM\\LocalApp";
	HKEY hParent;

	if (ERROR_SUCCESS != RegOpenKeyExW(root, parent, 0, KEY_READ, &hParent))
	{
		return;
	}

	DWORD value_type;
	byte* value_buffer;
	DWORD value_length;

	// get length first
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"Executable", NULL, &value_type, NULL, &value_length)) {
		RegCloseKey(hParent);
		return;
	}

	value_buffer = new byte[value_length + 2];
	// get value;
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"Executable", NULL, &value_type, value_buffer, &value_length)) {
		RegCloseKey(hParent);
		return;
	}
	// close 
	if (ERROR_SUCCESS != RegCloseKey(hParent)) {
		return;
	}
	std::wstring path((wchar_t*)value_buffer);
	// set value to out param
	EXPECT_FALSE(path.empty());
}

TEST(Windows, DISABLED_RegOperation2) {
	HKEY root = HKEY_CURRENT_USER;
	const wchar_t* parent = L"Software\\AAA\\BBB\\CCC\\DDD\\EEE\\FFF\\GGG";
	DWORD rt = 0;
	HKEY result = 0;

	rt=RegCreateKey(root, parent, &result);

	rt=RegSetValueW(result, L"null", 1, parent, wcslen(parent));

	static const wchar_t* hSKYDRM = L"Software\\Nextlabs\\SkyDRM\\LocalApp";

	wchar_t skydrm_cmd[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;

	if (ERROR_SUCCESS != ::RegGetValueW(HKEY_CURRENT_USER, hSKYDRM, L"Executable", RRF_RT_REG_SZ, NULL, skydrm_cmd, &size)) {
		rt = E_UNEXPECTED;
	}


	// cat str with format "%path%" -view "%1"
	wchar_t nxl_cmd[MAX_PATH] = { 0 };
	::swprintf_s(nxl_cmd,L"\"%s\" -view \"%%1\"", skydrm_cmd);


	EXPECT_TRUE(1);

}



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



std::set<std::wstring> get_targets_from_regsitry() {
	std::set<std::wstring> ps;
	HKEY root = HKEY_CURRENT_USER;
	const wchar_t* parent = LR"(Software\NextLabs\SkyDRM\OSRmx)";
	DWORD rt = 0;
	HKEY result = 0;
	HKEY hParent;
	if (ERROR_SUCCESS != RegOpenKeyExW(root, parent, 0, KEY_READ, &hParent))
	{
		return std::move(ps);
	}

	DWORD value_type;
	DWORD value_length = 0;

	// get length first
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"target", NULL, &value_type, NULL, &value_length) || value_length == 0) {
		RegCloseKey(hParent);
		return std::move(ps);
	}

	// get value
	std::wstring str((size_t)value_length + 1, 0);
	if (ERROR_SUCCESS != RegQueryValueExW(hParent, L"target", NULL, &value_type, (LPBYTE)str.data(), &value_length)) {
		RegCloseKey(hParent);
		return std::move(ps);
	}
	RegCloseKey(hParent);

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);	
	
	// spilt by ; and each into set
	size_t beg = 0;
	size_t end = 0;
	while ((end = str.find(';', beg)) != str.npos) {
		auto token = str.substr(beg, end - beg);
		if (!token.empty())
			ps.insert(token);
		beg = end + 1;
	}

	return std::move(ps);
}


class registry {
public: 
	class param {
		friend class registry;
	public:
		param(std::wstring path) :sub_key(path), access_right(KEY_READ), root(HKEY_CURRENT_USER), open_key(NULL){}
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
			out_value.resize(length/2);
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
		return ERROR_SUCCESS==::RegOpenKeyExW(p.root, p.sub_key.c_str(), NULL, p.access_right, &p.open_key);
	}
	inline std::uint32_t _buflen(param& p, const std::wstring& name) {
		DWORD length = -1;
		DWORD type;
		::RegQueryValueExW(p.open_key, name.c_str(), NULL, &type, NULL, &length);
		return length;
		
	}
	inline bool _get(param& p, const std::wstring& name, std::uint8_t* buf,std::uint32_t buf_len) {
		return ERROR_SUCCESS == ::RegQueryValueExW(p.open_key, name.c_str(), NULL, NULL, buf, (DWORD*)&buf_len);
	}

};


TEST(Windows, OSRMX) {
	
	registry::param p(LR"(Software\NextLabs\SkyDRM\OSRMX\watermark)");
	registry r;

	wstring font_name,text;
	r.get(p, L"font_name", font_name);	
	r.get(p, L"text", text);

	std::uint32_t size = 0;

	r.get(p, L"font_size", size);
	r.get(p, L"font_style", size);
	r.get(p, L"text_rotation", size);
	r.get(p, L"text_color", size);



	registry::param p2(LR"(Software\NextLabs\SkyDRM\OSRMX\target)");

	wstring list;

	r.get(p2, L"", list);
}