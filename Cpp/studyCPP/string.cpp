#include "pch.h"

using namespace std;

// str has 54 chars, without \0
static const wchar_t str[] = L"The Quick Brown Fox Jumps Over The Lazy Dog,1234567890";

TEST(String, Basic) {
	
	wcout << L"String:" << str << "\tLen:" << wcslen(str) << endl;
	cout << "sizeof(str)" << sizeof(str) << endl;
	// sizeof(str)==110,    55chars,(within '\0')
	EXPECT_EQ(54+1, sizeof(str) / sizeof(wchar_t));

	wstring w(str);
	wcout << L"Use wstring:" << w.c_str() << L"\tSize:" << w.size() << endl;

	// 如何确保使用迭代器时, [beg end) 是同一个对象区间?
	//_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "string iterators incompatible (e.g. point to different string instances)");

}

TEST(String, Iter) {
	wstring w(str);

	auto iter = w.cbegin() + 1;

	w.resize(1000);

	auto iter2 = w.cbegin() + 10;

	wstring w2(iter, iter2);

}

TEST(String, Assign) {
	/*
	 1 iter beg, end
	 2 count, char    ->  100 , 'w'
	 3 str            ->  cpoy and store str
	 4 str, len
	 5 wstring,
	 6 wstring, begin, count
	 7 Initialize_list
	 8 wstring&&  move
	*/

	wstring w(str);
	w.assign(str, 10); // only copy first 10 elem

	wstring tmp(L"tmp_tmp_tmp_tmp_tmp_tmp_");

	w.assign(tmp);
	w.assign(std::move(tmp));


	wcout << w;
}