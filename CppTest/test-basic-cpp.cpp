#include "stdafx.h"

class A {
public:
	int getInt() const {
		return 1;
	}
};

class B :public A
{
public:
	B() {
		printf("gg");
	}
	int GetAAA() const {
		printf("ggg");
		return getInt();
	}
};


using namespace std;


// traits with case-insensitive eq:
struct custom_traits : std::char_traits<char> {
	static bool eq(char c, char d) { 
		return std::tolower(c) == std::tolower(d); 
	}
	// some (non-conforming) implementations of basic_string::find call this instead of eq:
	static const char* find(const char* s, std::size_t n, char c)
	{
		while (n-- && (!eq(*s, c))) ++s; return s;
	}
};




//use DISABLED_ prefix to filter out 
TEST(CppLanguage, CppBasicStringStudy) {
	// basic_string<> code research:
/*
<数据,数据的逻辑模型,数据的物理模型> + 模型能实现的功能
<Elem,Traits_Elem,Allocator_Elem>  + class String
*/
	std::string aCharString("Hello World");
	std::basic_string<char, char_traits<char>, allocator<char>>   aCharString2("Hello world");

	std::wstring	aWCharString(L"Hello World");
	std::basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>> aWCharString2(L"Hello World");


	std::basic_string<char, custom_traits, allocator<char>> case_insensitive_str1("HELLO WORLD");
	std::basic_string<char, custom_traits, allocator<char>> case_insensitive_str2("hello world");

}


TEST(CppLanguage, String) {
//TEST(CppLanguage, DISABLED_String) {
	// str has 54 chars
	B b;
	b.GetAAA();

	B *pb = new B;


	const wchar_t str[] = L"The Quick Brown Fox Jumps Over The Lazy Dog,1234567890";
	// sizeof(str)==110,    55chars,(within '\0')

	EXPECT_EQ(54, sizeof(str)/sizeof(wchar_t));

	wcout << L"String:" << str << "\tLen:" << wcslen(str) << endl;

	wstring w(str);

	wcout << L"Use wstring:" << w.c_str() << L"\tSize:" << w.size() << endl;


	

}



//TEST(CppLanguage, StringStream) {
TEST(CppLanguage, DISABLED_StringStream) {
//TEST(CppLanguage, StringStream) {
	std::string w;
	std::stringstream ss;
	std::istringstream iss;


	//iss << "this is" << 10 << " " << 11.233 << " " << 'c' << " " << endl;

	cout << ss.str();
}


#pragma region MyRegion

// 测试目的:  函数返回一个对象, 编译器会在函数调用处将 返回值当做 函数的一个隐式参数 传入,
// 普通函数  rcx, [rtObj]  
// 类成员函数   rcx, this; rdx, [rtObj]

std::list<string> rtList() {
	std::list<string> ll;

	ll.push_back(string("hello"));
	ll.push_back(string("world"));

	return ll;
}

class Mytest {
public:
	std::list<string> rtList() {
		std::list<string> ll;

		ll.push_back(string("hello"));
		ll.push_back(string("world"));

		return ll;
	}
};


TEST(CppLanguage, ReturtObj) {


	auto a = rtList();
	Mytest t;
	auto b = t.rtList();


	EXPECT_FALSE(a.empty());

}
#pragma endregion

