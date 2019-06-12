#include "pch.h"

using namespace std;

TEST(String, Basic) {

	// str has 54 chars, without \0
	const wchar_t str[] = L"The Quick Brown Fox Jumps Over The Lazy Dog,1234567890";
	
	// sizeof(str)==110,    55chars,(within '\0')
	EXPECT_EQ(54, sizeof(str) / sizeof(wchar_t));

	wcout << L"String:" << str << "\tLen:" << wcslen(str) << endl;

	wstring w(str);

	wcout << L"Use wstring:" << w.c_str() << L"\tSize:" << w.size() << endl;

}