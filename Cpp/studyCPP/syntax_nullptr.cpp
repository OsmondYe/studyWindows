#include "pch.h"

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

}