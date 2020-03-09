#include "pch.h"

using namespace std;


TEST(Syntax, TemplateFucDeduction) {
/*
template func with auto deduction:
	- template<typename T> void f(ParamType param);
		- determine typeof expr in f(expr);
		- ParamType  vs  T
		- {
			ptr&ref for (plain old data)
			universal ref
			none-ptr & none-ref
			}
	- for ptr & ref
		- template<typename T>   void f(T& param);
		  int x=0;		  const int cx=x;		  const int& rx =x;

		  f(x);		T int,			ParamType -> int&
		  f(cx);    T const int		ParamType -> cosnt int&
		  f(rx);	T cosnt int&    ParamType -> const int&
		- template<typename T>   void f(const T& param);
		  int x=0;		  const int cx=x;		  const int& rx =x;

		  f(x);		T int			ParamType -> const int&
		  f(cx);    T int			ParamType -> cosnt int&
		  f(rx);	T int			ParamType -> const int&
		- template<typename T>   void f(T* param)
		  int x=0;
		  const int* px =&x;

		  f(&x);	T int, ParamType -> int*;
		  f(px);    T int, ParamType -> cosnt int*
	- for unviersal ref
		- template <typename T>  void f(T&& param)
		  int x=0;		  const int cx=x;		  const int& rx =x;

		  f(x);		x: lv,		T int&				ParamType -> int&
		  f(cx);	cx:lv,		T const int&		ParamType -> const int&
		  f(rx);	rx:lv,		T const int&		ParamType -> const int&
		  f(27);	27:rv,		T int,				ParamType -> int&&
	- for none-ptr & none-ref, (pass by value)
		- template<typename T>   void f(T param)  , param is a new var,
		  int x=0;		  const int cx=x;		  const int& rx =x;

		  f(x);		T,ParamType -> int
		  f(cx);	T,ParamType -> int
		  f(rx);	T,ParamType -> int

*/
	cout << "see comments above" << endl;
}


namespace forArrayParam {
	template < typename T , std::size_t N>
	constexpr std::size_t arraySize(T(&)[N]) noexcept {
		return N;
	}
}

TEST(Syntax, ArrayParam) {
/*
	degration form arrary to pointer

	- const char name[] = "osmond.ye";	//   name: const char[10]  (9 + '\0'  is 10)
	- const char* pname = name;			//	 panme:  cosnt char*    (degradation from array to const char*)

	- template <typename T>  void f(T param)
		f(name)    T -> const char*
	- template <typename T>  void f(T& param)
		f(name)   T  -> const char[10]
*/
	const char name[] = "osmond.ye";
	cout << forArrayParam::arraySize(name) << endl;
}

TEST(Syntax, Auto) {
/*
 like deduction in template T and ParamType,
	auto is T
	but outer need to use ParamType, like const T,  const T& , T&, T*
*/
	auto x = 27;			// int
	const auto cx = x;		// const int
	const auto& rx = x;		// const int&

	cout << &x << endl;
	cout << &cx << endl;
	cout << &rx << endl;  
	cout << "&x ==&rx" << endl;

	auto&& uref1 = x;		// int&
	auto&& uref2 = cx;		// const int&
	auto&& uref3 = 27;		// int&&

	const char name[] = "osmond.ye";

	auto arr1 = name;  // const char*
	auto& arr2 = name; // cosnt char (&)[10]

	auto x3 = { 27 };  // x3 = std::initalizer_list<int>   ***
	auto x4{ 27 };


	unordered_map<string, int> m{
		{"123",123},
		{"456",456},
		{"789",789},
	};

	for (const pair<string, int>& p : m) {
		cout << p.first << p.second << endl;  // each time will call ~pair<>
	}
	for (const pair<const string, int>& p : m) {
		cout << p.first << p.second << endl;
	}

	// auto will deduct the right tyep pair<const string,int> 
	for (const auto& p : m) {
		cout << p.first << p.second << endl;

	}

	// hiden proxcy type cannot work well with auto
	cout << "hiden proxcy type cannot work well with auto\n";
}


TEST(Syntax, Decltype) {

	const int x = 27;

	auto y = x;				// y: int
	decltype(auto) z = x;   // z: const int  , using decltype(auto) to extract exact type

	cout << typeid(x).name() << endl;
	cout << typeid(z).name() << endl;

}