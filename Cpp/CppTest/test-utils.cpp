#include "stdafx.h"
#include "utils.h"
#include "Monitor.h"
#include <chrono>
#include <ctime>
#include <locale>
#include <regex>

#include "oye_handy_tools.hpp"
#include "oye_str.hpp"



//use DISABLED_ prefix to filter out 
using namespace std;


class OverlayText {

public:
	OverlayText() { _default(); }
	OverlayText(const std::wstring& text) :_text(text) { _default(); }

	inline void InsertReplacableToken(const std::wstring& key, const std::wstring& value) {
		_tokens[key] = value;
	}

	std::wstring GetReplacedText() {
		std::wstring rt = _text;
		std::for_each(_tokens.cbegin(), _tokens.cend(), [&rt](const std::pair<std::wstring, std::wstring>& pair) {

			if (pair.first.empty() || pair.second.empty()) {
				return;
			}
			auto k = pair.first;
			auto v = pair.second;

			k.replace(k.find(L"$"),1, L"\\$");
			k.replace(k.find(L"("),1, L"\\(");
			k.replace(k.find(L")"),1, L"\\)");

			std::wregex re(k,std::regex::icase);
			rt=std::regex_replace(rt, re, v);
		});
		return rt;

	}

private:
	void _default() {
		const static wchar_t* USER = L"$(User)";
		const static wchar_t* EMAIL = L"$(Email)";
		const static wchar_t* HOST = L"$(Host)";
		const static wchar_t* IP = L"$(IP)";
		const static wchar_t* BREAK = L"$(Break)";  //  to \n
		const static wchar_t* DATE = L"$(DATE)";    //  "YYYY-MM-DD"
		const static wchar_t* TIME = L"$(TIME)";	//  "HH:mm:ss"	  

		// Cur 
		_tokens[BREAK] = L"\n";
		_tokens[DATE] = _get_date();
		_tokens[TIME] = _get_time();
	}

	inline std::wstring _get_date() {
		std::wstringstream ss;
		auto time = std::time(nullptr);		
		ss<< std::put_time(std::localtime(&time), L"%Y-%m-%d");
		return ss.str();
	}

	inline std::wstring _get_time() {
		std::wstringstream ss;
		auto time = std::time(nullptr);
		ss << std::put_time(std::localtime(&time), L"%H:%M:%S");
		return ss.str();
	}
	inline std::wstring _get_datetime() {
		std::wstringstream ss;
		auto time = std::time(nullptr);
		ss << std::put_time(std::localtime(&time), L"%Y-%m-%d %H:%M:%S");
		return ss.str();
	}

private:
	std::wstring _text;
	// i.e.  $(User) ->  Osmond.Ye
	std::map<std::wstring, std::wstring> _tokens;
};


TEST(Utils, DISABLED_OverlayText) {
	OverlayText tc(L"$(User)$(BREAk)$(Email)$(Host)$(IP)--$(break)--$(date)--$(time)");
	wcout << tc.GetReplacedText();
}


TEST(Utils, DISABLED_sha1){
	std::string str = "The Quick Brown Fox Jumps Over The Lazy Dog,the quick brown fox jumps over the lazy dog";
	std::string exp_rt("fdf652d252642deafaebf1786c17d13d604180aa");

	std::vector<unsigned char> fix_buf;
	const size_t fix_len = 20;
	fix_buf.reserve(fix_len);
	win::crypt::sha1((const unsigned char*)str.c_str(), str.length(), fix_buf.data());
	std::string rt = win::str::from_digits(fix_buf.data(), fix_len);
	
	EXPECT_STRCASEEQ(rt.data(), exp_rt.data());

}
void DeleteAllFiles(std::wstring path)
{
	std::vector<std::wstring::value_type> doubleNullTerminatedPath;
	std::copy(path.begin(), path.end(), std::back_inserter(doubleNullTerminatedPath));
	doubleNullTerminatedPath.push_back(L'\0');
	doubleNullTerminatedPath.push_back(L'\0');

	SHFILEOPSTRUCTW fileOperation;
	fileOperation.wFunc = FO_DELETE;
	fileOperation.pFrom = &doubleNullTerminatedPath[0];
	fileOperation.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION | FOF_SILENT;

	int result = ::SHFileOperationW(&fileOperation);
	if (result != 0) {
		throw std::runtime_error("Could not delete directory");
	}
}

TEST(Utils, DISABLED_EnumFolder) {
	using namespace boost::filesystem;
	try {
		const wchar_t* p = L"D:\\allTestFile\\AutoRecovery";
		if (!is_directory(p)) {
			return;
		}
		std::vector<directory_entry> v; // To save the file names in a vector.
			
		copy(recursive_directory_iterator(p), recursive_directory_iterator(), back_inserter(v));
		std::wcout << p << L" is a directory containing:\n";

		for (std::vector<directory_entry>::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			std::cout << (*it).path().string() << endl;
		}
		
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}

}

inline std::wstring getURLEncoderName(const std::wstring path) {
	wchar_t buf[MAX_PATH] = { 0 };
	DWORD len = MAX_PATH;

	::InternetCanonicalizeUrlW(path.c_str(), buf, &len, 0);

	return std::wstring(buf, len);
}


TEST(Utils, DISABLED_FileMonitor) {

	using namespace boost::filesystem;
	//wcout <<  << endl;
	//::MessageBox(NULL, getURLEncoderName(L"aBook1 - Copy-2019-07-30-09-49-27").c_str(), 0, 0);
	//wcout << getURLEncoderName(L"aBook1  ,. 中文测试啊啊啊- Copy-2019-07-30-09-49-27") << endl;
	//::MessageBox(NULL, getURLEncoderName(L"aBook1  ,. 中文测试啊啊啊- Copy-2019-07-30-09-49-27").c_str(), 0, 0);

	const wchar_t* Path = L"D:\\allTestFile\\AutoRecovery\\aBook1  ,. 中文测试啊啊啊- Copy-2019-07-30-09-49-27.xlsx";

	path p(Path);
	
	wstring name( p.filename().generic_wstring());
	wstring stem(p.stem().generic_wstring());
	wstring stemEncode = getURLEncoderName(stem);


	DirMonitor m;
	
	auto lambda= [](const std::wstring& path) {
		using namespace boost::filesystem;
		wcout << L"----callback noticed!!-----" << endl;
		std::vector<directory_entry> v; // To save the file names in a vector.
		std::copy(recursive_directory_iterator(path), recursive_directory_iterator(), back_inserter(v));
		for (std::vector<directory_entry>::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			try {
				wstring p = (*it).path().generic_wstring();
				wcout << L"remove file:" << p << endl;
				if (is_regular_file(*it)) {
					wcout << "is file,so using win32 to del" << endl;
					// remove file readonly and hiden attr;
					::SetFileAttributesW(p.c_str(), ::GetFileAttributesW(p.c_str()) & (~FILE_ATTRIBUTE_HIDDEN) & (~FILE_ATTRIBUTE_READONLY));
					if (!::DeleteFileW(p.c_str())) {
						wcout << L"del failed" << endl;
					}
				}
				if (is_directory(*it)) {
					if (!::RemoveDirectoryW(p.c_str())) {
						wcout << L"remove dir failed" << endl;
					}
				}
				//boost::filesystem::remove_all((*it));
			}
			catch (std::exception& e) {
				cout << e.what() << endl;
			}
		}

	};

	m.AddDir(L"C:\\Users\\oye\\AppData\\Roaming\\Microsoft\\Excel", lambda);
	m.AddDir(L"C:\\Users\\oye\\AppData\\Roaming\\Microsoft\\Excel", lambda);
	m.AddDir(L"D:\\allTestFile\\AutoRecovery\\EXCEL", lambda);
	m.StartWork();


	//std::this_thread::sleep_for(std::chrono::seconds(10));


	//m.MarkStop();
	m.Join();

	
}

TEST(Utils, Process) {
	Module m;
	cout << m.GetPathA()<<endl;
	wcout << m.GetPathW() << endl;
	cout << m.GetNameA() << endl;
	wcout << m.GetNameW() << endl;
	
	Module n(::LoadLibrary(L"kernel32.dll"));
	cout << n.GetPathA() << endl;
	wcout << n.GetPathW() << endl;
	cout << n.GetNameA() << endl;
	wcout << n.GetNameW() << endl;


	string l{ "hello" };

	string r{ "hello" };

	auto i = (l.compare(r) == 0);
	cout << i;

}

// f(x) :  how many ways in total when you in x layer(s)
// f(x) = f(x-2) + f(x-1);
// f(0) =0;
// f(1) = 1;
// f(2) = 2;
 long long  func_x(int x) {
	vector<long long> dp(x+1, 0);

	dp[0] = 0;	dp[1] = 1;	dp[2] = 2;
	if (x <= 2) {
		return dp[x];
	}
	for (int i = 3; i <= x; i++) {
		dp[i] = dp[i - 1] + dp[i - 2];
		cout << "f(" << i << ")=\t" << dp[i] << endl;
	}
	return dp[x];
}

TEST(Utils, Str) {

	int x = 100;
	cout << "f(100)=\t" << func_x(x) << endl;


	// reserve
	string str{ "12345Hello" };
	//EXPECT_STREQ(str.c_str(), oye_str::reserve(oye_str::reserve(str)).c_str());

	{// iequal
		string s1{ "hello" };
		string s2{ "Hello" };
		EXPECT_TRUE(oye_str::iequal(s1, s2));

		string s3, s4;
		EXPECT_TRUE(oye_str::iequal(s3, s4));
	}

	{// icontain
		EXPECT_TRUE(oye_str::icontain(str, ""));
		EXPECT_TRUE(oye_str::icontain(str, "5"));
		EXPECT_TRUE(oye_str::icontain(str, "123"));
		EXPECT_TRUE(oye_str::icontain(str, "hello"));
		EXPECT_TRUE(!oye_str::icontain(str, "china"));
	}

	{// trim kinds

		string s = "    china    ";

		cout << oye_str::trim_left_copy(s) << endl;
		cout << oye_str::trim_right_copy(s) << endl;
		cout << oye_str::trim_copy(s) << endl;


		string s2 = "*&^china^&*";

		auto lamda = [](char c) {
			set<char> ss = { '&','*','^' };
			return ss.count(c) != 0;
		};

		cout << oye_str::trim_left_copy_if(s2, lamda) << endl;
		cout << oye_str::trim_right_copy_if(s2, lamda) << endl;
		cout << oye_str::trim_copy_if(s2, lamda) << endl;

		string path = R"_('"c:\abc\ddd\ccc\eee.txt"')_";

		auto l = [](char c) {  return c == '\'' || c == '\"'; };
		cout << oye_str::trim_copy_if(path, l) << endl;
		
	}
}