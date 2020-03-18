#include "pch.h"

/*
constexpr is a new keyword
	- apply to object, like const
		- known value when compiler time
		- compiling time, no this var, using its value instead
	- apply to function, this is a new concept
		- give compiler a hint, 
			if can cal the value, 
				don't using by calling a func, 
				use a value instead
*/

TEST(Syntax, ConstExpr) {

	int x = 12;

	const int y = x;

	// y = 12; error, const value can not be modified

	//constexpr int z = x;  // -> error, constexpr obj must be a known value for compiler time

	constexpr int z = 0xabcd;

	int dd = x + z;
	/*
	compile time, now z,using 0xabcd instead

		mov         eax,dword ptr [x] 
		add         eax,0ABCDh  
		mov         dword ptr [dd],eax
	*/

	aux::println(dd);
}

namespace {
	constexpr int doubleInf(const int x) {
		return x * 2;
	}
}

TEST(Syntax, ConstExprInFunc) {
	constexpr int a = doubleInf(12);  //  mov dword ptr [a], 18h   , without calling function
	aux::println(a); 
	aux::println(doubleInf(a));

	constexpr int b = doubleInf(a);  // b is constexpr,
	const int c = doubleInf(a);  //will call the fun, since c is not constexpr
	aux::println(b);


	// error -> param must be a compiler known value
	//constexpr int d = doubleInf(::time(NULL));  
}

TEST(Syntax, ConstExprInStl) {
	cout << "Constexpr ctor in stl's unique_ptr\n";
}