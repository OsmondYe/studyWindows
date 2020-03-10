#include "pch.h"

// Uniform Initialization  and Initializer List

// narrowing :  will degrade value's precision

// class data member can using {} to give a default value;

namespace syntax_init {
	class Widget {
	public:
		Widget() {}
		Widget(int x, int y) {}

		Widget(const Widget& rh) {
			x_ = rh.x_;
			y_ = rh.y_;
		}

		Widget(std::initializer_list<int> il) {
			cout << "trying to analyze std::initializer_list<int> il\n";
			for (auto beg = il.begin(); beg != il.end(); ++beg) {
				cout << *beg << endl;
			}
		}

		Widget& operator = (const  Widget& rh) {
			x_ = rh.x_;
			y_ = rh.y_;
			return *this;
		}

		int x_{ 0x123456 };   // give x a defualt value , c11
		int y_{ 0xabcdef };
	};
}


using namespace syntax_init;

TEST(Syntax, InitVar) {
	cout << "{} will be called Uniform Initialization\n";
	// using {} as many as i can 

	int x2{ 12 };
	char c{ 'd' };


	Widget w;		// call default ctor
	Widget x = w;   // call copy-ctor
	x = w;			// call operator = 


	Widget w2{};  // call default ctor to build w2;

	Widget w3();  // fuck off, w3 is a function type
	cout << typeid(w3).name() << endl;


//
// 一致初始化
//
	int values[]{ 1,2,3,4,5,6 };
	vector<int> v{ 2,3,4,5,6,76 };
	vector<string> cities{ "changan","xian","beijing","tianjing" };
	int j{};  //  j=0;
	int* ppp{}; // ppp=nullptr;


}

TEST(Syntax, Narrowing) {
	int x1 = 5.3;  // ok, but  x1 form double to int  -> narrowing;
	//int x2{ 5.3 };  // error: required narrowing conversion from 'double' to 'int'
	cout << "int x2{ 5.3 };  // error: required narrowing conversion from 'double' to 'int'" << endl;

	//char x3{ 999999 }; //error C2397: conversion from 'int' to 'char' requires a narrowing conversion
	cout << "char x3{ 999999 }; //error C2397: conversion from 'int' to 'char' requires a narrowing conversion\n";
}



TEST(Syntax, InitializerList) {
	/*
template<class _Elem>	class initializer_list{
public:
	typedef _Elem value_type;	typedef const _Elem& reference;
	typedef const _Elem& const_reference;	typedef size_t size_type;
	typedef const _Elem* iterator;	typedef const _Elem* const_iterator;

	constexpr initializer_list() noexcept
		: _First(nullptr), _Last(nullptr){	}

	constexpr initializer_list(const _Elem *_First_arg,
		const _Elem *_Last_arg) noexcept
		: _First(_First_arg), _Last(_Last_arg)
		{	// construct with pointers
		}
	 constexpr const _Elem * begin() const noexcept
		{		return (_First);	}

	 constexpr const _Elem * end() const noexcept
		{	return (_Last);	}

	 constexpr size_t size() const noexcept
		{	return (static_cast<size_t>(_Last - _First));}
private:
	const _Elem *_First;
	const _Elem *_Last;
	};

Require compile to convert {1,2,3,4,5} ->  initializer_list object -> some thing like a range

*/


// require constructor : vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
// compile convert { 1,2,3,4,5,6 } -> [buffer] ->  initializer_list<int>;	
	vector<int> aa{ 1,2,3,4,5,6 };
	for_each(aa.begin(), aa.end(), [](int& i) {std::cout << i; });

	for (auto i : { 1,2,3,4,5,6 }) {
		cout << i << ' ';
	}

	vector<std::pair<int, string>> ll{ {1,"haha"},{2,"haha"} };
	ll.size();


	// test self define InitializerList-Constructor
	Widget w{ 1,2,3,4,5,6 };
}
