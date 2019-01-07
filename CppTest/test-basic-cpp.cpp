#include "stdafx.h"

#include <cctype>

using namespace std;


template<typename T>
class BasicAllocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;


	BasicAllocator() throw() {};
	BasicAllocator(const BasicAllocator& other) throw() {};

	template<typename U>
	BasicAllocator(const BasicAllocator<U>& other) throw() {};

	template<typename U>
	BasicAllocator& operator = (const BasicAllocator<U>& other) { return *this; }
	BasicAllocator<T>& operator = (const BasicAllocator& other) { return *this; }
	~BasicAllocator() {}

	pointer address(reference value) const { return &value; }
	const_pointer address(const_reference value) const { return &value; }

	pointer allocate(size_type n, const void* hint = 0) { return static_cast<pointer> (::operator new (n * sizeof(value_type))); }
	void deallocate(void* ptr, size_type n) { ::operator delete (static_cast<T*> (ptr)); }

	template<typename U, typename... Args>
	void construct(U* ptr, Args&&  ... args) { ::new (static_cast<void*> (ptr)) U(std::forward<Args>(args)...); }
	void construct(pointer ptr, const T& val) { new (static_cast<T*> (ptr)) T(val); }

	template<typename U>
	void destroy(U* ptr) { ptr->~U(); }
	void destroy(pointer ptr) { ptr->~T(); }

	size_type max_size() const { return 0x4000; } /**return std::size_t(-1);**/

	template<typename U>
	struct rebind
	{
		typedef BasicAllocator<U> other;
	};
};


// traits with case-insensitive eq:
struct custom_traits : std::char_traits<char> {

	static bool eq(char c, char d) { 		
		return std::tolower(c) == std::tolower(d); 
	}

	static const char* find(const char* s, std::size_t n, char c)
	{
		while (n-- && (!eq(*s, c))) 
			++s; 

		return s;
	}	
};


struct custom_traits2 : custom_traits {
	 static  int compare(
		 const char * const first,
		const char * const second, 
		 const size_t count) noexcept 
	{	
		 return _strnicmp(first, second, count);
	}
};


struct MyTraits : public char_traits<char> {
	 static  int compare(
		 const char * const l,
		const char * const r, const size_t c) noexcept // strengthened
	{	
		 return _strcmpi(l, r);
	}
};


//use DISABLED_ prefix to filter out 
TEST(CppLanguage, CppBasicStringStudy) {
	// basic_string<> code research:
	{
		std::basic_string<char, char_traits<char>, allocator<char>> str(1,'c');
		

		str = "Hello world Hello world Hello world";
		char a = str[6];

		const char* p = "China China China China China China";

		str.insert(6, p, std::strlen(p));

		auto aaa = str.substr(6, 0x23);

		auto index=str.find('l', 0);


	}

	{
		int i = 12; {
			int i = 7;

			int c = i;
		}
		basic_string<char, MyTraits, allocator<char>> s1(" C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Enterprise\\Common7\\Packages\\Debugger\\Visualizers\\stl.natvis(751,10): Successfully parsed expression");
		auto b = s1.begin();
		auto e = s1.end();

		std::sort(b, e);


		for (int i = 0; i < s1.size(); i++) {
			s1[i] = std::toupper(s1[i]);
		}

		std::transform(s1.begin(), s1.end(), s1.begin(), std::tolower);

		

		std::random_shuffle(b, e);

		EXPECT_TRUE(std::all_of(b, e, [](auto c) {return c < 'w'; }));

		std::remove_if(b, e, [](auto c) {return c > 'i'; });


		EXPECT_TRUE(true);
	}

	return;

	{
		/*
		<数据,数据的逻辑模型,数据的物理模型> + 模型能实现的功能
		<Elem,Traits_Elem,Allocator_Elem>  + class String
		*/
		std::string aCharString("Hello World");
		std::basic_string<char, char_traits<char>, allocator<char>>   aCharString2("Hello world");
		aCharString2 = "Hello worldHello worldHello worldHello worldHello worldHello worldHello worldHello worldHello worldHello world";

		std::wstring	aWCharString(L"Hello World");
		std::basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>> aWCharString2(L"Hello World");
	}


	//
	// Extend : case insensitive
	//

	//basic
	{
		std::basic_string<char, custom_traits, allocator<char>> ci_str1("HELLO WORLD");
		std::basic_string<char, custom_traits, allocator<char>> ci_str2("hello world");


		EXPECT_EQ(ci_str1.find('o'), 4);
		EXPECT_NE(ci_str1.find('e'), ci_str1.npos);

		EXPECT_TRUE(ci_str1 == ci_str2);
	}

	// fixed 
	{
		std::basic_string<char, custom_traits2, allocator<char>> ci_str1("HELLO WORLD");
		std::basic_string<char, custom_traits2, allocator<char>> ci_str2("hello world");

		EXPECT_EQ(ci_str1.find('o'), 4);
		EXPECT_NE(ci_str1.find('e'), ci_str1.npos);

		EXPECT_TRUE(ci_str1 == ci_str2);
		EXPECT_FALSE(ci_str1 > ci_str2);
		EXPECT_FALSE(ci_str1 < ci_str2);
	}
	

	// for replace
	{
		std::basic_string<char, char_traits<char>, allocator<char>>  str;

		str = "Hello world";

		
		str.replace(6,5, "china_hangzhou",std::strlen("china_hangzhou"));


	}

	// with algorithm
	{
		std::basic_string<char, char_traits<char>, allocator<char>>  str = "Hello world";

		// to upper
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
		// to lower
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);

		// how many 'o'
		int c = std::count_if(str.begin(), str.end(), [](auto& c) {return c == 'o'; });
		EXPECT_EQ(2, c);

		// find world
		

	
		// equals
		const char* p = "HeLLO world";
		bool bEqual = std::equal(str.begin(), str.end(), p);
		
		EXPECT_TRUE(bEqual);

		bEqual = std::equal(str.begin(), str.end(), p, [](auto& a, auto& b) {return custom_traits::eq(a,b); });

		EXPECT_TRUE(bEqual);

		// sort
		std::sort(str.begin(), str.end());


		EXPECT_TRUE(true);
	}
	EXPECT_TRUE(true);

	
}


TEST(CppLanguage, String) {
//TEST(CppLanguage, DISABLED_String) {
	// str has 54 chars
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

