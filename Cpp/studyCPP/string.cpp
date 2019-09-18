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
	wcout << L"sizeof(wstring):" << sizeof(wstring) << endl;   // 40   5*8   container_proxy*, Bx_union{buf[16],ptr}, size, capacity;
	// 如何确保使用迭代器时, [beg end) 是同一个对象区间?
	//_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "string iterators incompatible (e.g. point to different string instances)");


	//
	//  Capacity
	//
	EXPECT_TRUE(w.empty() == false);
	EXPECT_TRUE(w.size() == w.length());
	EXPECT_TRUE(w.capacity() > 0);
	w.reserve(100);
}

TEST(String, Advanced) {
	EXPECT_TRUE(sizeof(wstring) == sizeof(int*) * 4);
}

TEST(String, Access) {
	// at [] front back data c_str
	wstring w(str);

	auto c = w.at(0);
	auto c2 = w[0];
	EXPECT_TRUE(c == c2);
	auto c3 = w.front();  // return *begin()
	EXPECT_TRUE(c == c3);

	auto c4 = w.back();   // return *(end()-1);
	w.back() = 'H';
	w.front() = 'W';

	wcout << w.data();
	wcout << w.c_str();

	EXPECT_TRUE(w.data() == w.c_str());

	wcout << w << endl;


	//
	//  get this obj's mem_allocator
	//
	auto alloc = w.get_allocator();

}

TEST(String, Iter) {
	// begin/cbegin  end/cend,  rbegin/crbegin, rend/crend
	wstring w(str);

	auto iter = w.cbegin() + 1;

	w.resize(1000);

	auto iter2 = w.cbegin() + 10;

	// will throw out,  since iterator is invalid
	//wstring w2(iter, iter2);
}



TEST(String, AssignOperEqual) {
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

	//
	// test for operator =
	// 
	w = str;
	w = { 'h','e','l','l','o' };
	w = 'h';
	w = wstring(L"that is that");
	w = L"NULL for NULL";
}


TEST(String, Operations) {
	wstring w(str);
	// clear
	w.clear();
	EXPECT_TRUE(w.empty() == true);

	w = L"d1-d2-d3-d4-d5-d6";
	//
	//  insert,   1 based index, 2 based on iter
	//
	w.insert(0, 4, 'T');   // index, count, char
	w.insert(12, L"Hello China");// index ,str
	w.insert(15, L"Hello World", 5);// index, str, strlen
	w.insert(1, wstring(L"this is good"));// index, wstring
	// using it, after insertion , it may invalid, so api return it for the pos of first inserted char
	auto it = w.begin() + 6;
	it = w.insert(it, 'T');
	it = w.insert(w.begin() + 5, 10, 'T');
	it = w.insert(w.begin(), w.begin(), w.end());
	it = w.insert(w.begin() + 3, { 'h','e','l','l','o' });

	//
	//  erase  1 index based 2 iter based
	//
	wstring e(L"Hello world");
	e.erase(5); // only e[0,5] left 
	w.erase(5, 4);  // move(e[5],e[5+4]);
	w.erase(w.begin());  // erase( (it - begin),1)// only delete one 
	w.erase(w.begin() + 3, w.begin() + 5);  // earse( [frst-beg],[lst-frst]); return new frst 

	//
	// push_back pop_back
	//
	e.push_back('h');
	e.pop_back();
	
	//
	// append
	//
	w.clear();
	w
		.append(10, 'c')
		.append(L"hello")
		.append(L"hello", 3)
		.append(e, 0, 5)
		.append(e.end()-1,e.end())
		.append(wstring(L"China"))
	;

	//
	// compare
	//
	w.compare(L"china");
	w.compare(wstring(L"china"));
	//
	// replace
	//
	w = str;
	w.replace(10, 5, L"red");
	w.replace(w.begin(), w.begin() + 3, 1, 'A');

	//
	// substr
	//
	w.substr(10);

	//
	// copy
	//
	//w.copy() // sent out

	//
	// resize
	//
	w.resize(10);
	//
	// swap
	//
	w.swap(e);

}

TEST(String, Search) {
	wstring w(str);
	//
	// find
	//
	auto pos=w.find('F', 5); // find F from w[5]
	pos = w.find(L"Dog");
	pos = w.find(wstring(L"Brown"));
	//
	// rfind
	//
	pos = w.rfind('F',w.length());

	//
	// find_first_of,  find_first_not_of,
	//

	//
	// find_last_of, find_last_not_of
	//
}


TEST(String, Mutate) {
	wstring w(str);
	std::transform(w.begin(), w.end(), w.begin(), std::toupper);
	wcout << L"cctype std::toupper:" << w << endl;

	w.assign(str);
	std::transform(w.begin(), w.end(), w.begin(), std::tolower);
	wcout << L"cctype std::tolower:" << w << endl;

	// copy into a new str
	w.assign(str);
	wstring w2;
	//w2.reserve(w.length()+1); // wrong! reserve只是修改了capacity, w2的length依旧没有改变
	w2.assign(w.length(), 0);
	std::transform(w.begin(), w.end(), w2.begin(), std::tolower);
	wcout << L"w2:" << w2 << endl;

	// method 2, using iterator adapter
	w2.clear();
	std::transform(w.begin(), w.end(), std::back_inserter(w2), std::tolower);
}

TEST(String, ReplaceAll) {
	wstring w(L"there is [c] in [c], but [c] is not true [c]");
	wcout << w << endl;

	//
	//replace_all [c] to China;
	//
	wstring pattern(L"[c]");
	wstring replaced(L"China");

	size_t pos = 0;
	
	while ( (pos = w.find(pattern, pos)) != wstring::npos) {
		w.replace(pos, pattern.length(), replaced);
		pos += replaced.size();
	}
	wcout << w << endl;	
}