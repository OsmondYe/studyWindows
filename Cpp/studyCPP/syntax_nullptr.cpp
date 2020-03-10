#include "pch.h"

#include <cstddef>


/*
for vs defined:

#ifdef __cplusplus
	namespace std
	{
		typedef decltype(__nullptr) nullptr_t;
	}

	using ::std::nullptr_t;
#endif


- std::nullptr_t is the type of the null pointer literal, nullptr. 
- It is a distinct type that is 
	- not itself a pointer type 
	or 
	- a pointer to member type.

*/

namespace syntax_nullptr {
	void f(int) { cout << __FUNCSIG__ << endl; }
	void f(bool) { cout << __FUNCSIG__ << endl; }
	void f(void*) { cout << __FUNCSIG__ << endl; }

	int fp(void*) { cout << __FUNCSIG__ << endl; return 1; }

	using MuxGuard = std::lock_guard<std::mutex>;   // c11 typedef
	template< typename FuncType, typename MuxType, typename PtrType>
	auto lockAndCall(FuncType& func, MuxType& mutex, PtrType ptr) -> decltype(func(ptr))
	{
		MuxGuard g(mutex);
		return func(ptr);
	}


	void fp_c11(int*) { cout << __FUNCSIG__ << endl; }
	void fp_c11(double*) { cout << __FUNCSIG__ << endl; }
	void fp_c11(std::nullptr_t nullp) { cout << __FUNCSIG__ << endl;  }
};

using namespace syntax_nullptr;

TEST(Syntax, NullPtr) {
	f(0);		// f(int)
	f(NULL);	// f(int)
	f(nullptr); // f(void*)
	//
	std::nullptr_t x{};
	f(x);

	//
	std::mutex f1m, f2m, f3m;

	//auto r1 = lockAndCall(fp, f1m, 0);		// 0 is int, not ptr
	//auto r2 = lockAndCall(fp, f2m, NULL);		// NULL is int, not ptr
	auto r3 = lockAndCall(fp, f3m, nullptr);

	int* ip(NULL);
	double* dp(NULL);
	fp_c11(ip);
	fp_c11(dp);
	fp_c11(nullptr);
	//fp_c11(NULL);  // error: more than one instance of overloaded fucntion fp_cpp matchs the arguement list
	//fp_c11(0);     // error: more than one instance of overloaded fucntion fp_cpp matchs the arguement list

}