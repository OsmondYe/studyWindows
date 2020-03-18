#include "pch.h"

// Iterator adapter
// this cpp file mainly focused on
// std::begin, std::end
//
#include <iterator>

// Functor adpater
// adapter for functor object
//
#include <functional>

// Container adapter
// 
//
#include <stack>
#include <queue>

using namespace std;

TEST(Adapter, Basic) {
	// test data
	int ia[] = { 1,2,3,4,5 };
	int* beg = ia;
	int* end = ia + 5;

	auto beg2 = std::begin(ia);
	/*template<class _Ty,
	size_t _Size>
	_NODISCARD constexpr _Ty *end(_Ty (&_Array)[_Size]) noexcept
	{	// get end of array
	return (_Array + _Size);
	}*/
	auto end2 = std::end(ia);
	
	std::ostream_iterator<int> out(cout, " ");
	// override operator =, and set it as cout<<12;
	out = 12;
	cout << endl;

	std::copy(beg, end, out);
	cout << endl;
	// each i= (i+3)*2;
	for_each(beg, end, [](int& i) {i = (i + 3) * 2; });
	for_each(beg, end, [](int& i) {cout << i<<" " ; });
	
}