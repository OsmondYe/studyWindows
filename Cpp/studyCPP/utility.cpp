#include "pch.h"
#include "helper.hpp"


#include <utility>

using namespace aux;

// std::pair
TEST(Utility, Pair) {
	// wrapper two obj into a one obj
	pair<vector<int>, string> a(getSorted(),"hello world");
	a.first = getSorted();
	a.second = "hello world";
	a = make_pair(getSorted(), "hello world");
	pair<vector<int>, string> b = make_pair(a.first, a.second);
	pair<vector<int>, string> c(a.first, a.second);
}

// std::tuple
TEST(Utility, Tuple) {
	// wrapper multi objs into one obj
}

// std::swap, 交换2个对象的值
TEST(Utility, Swap) {

}

//std::ref()
TEST(Utility, Ref) {
}

//std::ref()
TEST(Utility, Bind) {
}

// std::forward
/*
	配合move使用的,方便右值引用
	template<class _Ty>
	 constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) noexcept
	{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
	}

template<class _Ty>
	 constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept
	{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
	}
*/
TEST(Utility, Forward) {
	//std::forward(), 返回值一定是一个右值引用,static_cast<T&&>(t)
}


// std::move
TEST(Utility, Move) {
	//std::forward()
}