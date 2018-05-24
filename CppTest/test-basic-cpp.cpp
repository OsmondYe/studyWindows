#include "stdafx.h"

//use DISABLED_ prefix to filter out 

//TEST(CppLanguage, String) {
TEST(CppLanguage, DISABLED_String) {
	// str has 54 chars
	const wchar_t str[] = L"The Quick Brown Fox Jumps Over The Lazy Dog,1234567890";
	// sizeof(str)==110,    55chars,(within '\0')

	EXPECT_EQ(54, sizeof(str));

	wcout << L"String:" << str << "\tLen:" << wcslen(str) << endl;

	wstring w(str);

	wcout << L"Use wstring:" << w.c_str() << L"\tSize:" << w.size() << endl;

}

//TEST(CppLanguage, StringStream) {
TEST(CppLanguage, DISABLED_StringStream) {
	std::string w;
	std::stringstream ss;

	ss <<"this is" << 10 << " " << 11.233 << " " << 'c' << " " << endl;	
	cout << ss.str();
}