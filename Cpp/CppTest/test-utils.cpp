#include "stdafx.h"
#include "utils.h"
#include "Monitor.h"
#include <chrono>
//use DISABLED_ prefix to filter out 
using namespace std;


TEST(Utils,sha1){

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

TEST(Utils, EnumFolder) {
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


TEST(Utils, FileMonitor) {

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