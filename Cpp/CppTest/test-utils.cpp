#include "stdafx.h"
#include "Monitor.h"
#include <chrono>
#include <ctime>
#include <locale>
#include <regex>

#include "oye_handy_tools.hpp"
#include "oye_str.hpp"



//use DISABLED_ prefix to filter out 
using namespace std;


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
