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

	//
	// for c
	//
	wstring forc{ L"123456789" };

	wchar_t* xx = new wchar_t[forc.size() + 1];  //size :: elements. not mem_size

	wcsncpy(xx, forc.c_str(), forc.size() + 1);

	delete[] xx;

	EXPECT_TRUE(xx != NULL);

}

TEST(String, DISABLED_EncodingPrefix) {
	// u8,u,U,L,R
	string str1 = "你好中国";
	//u8string str2 = u8"你好中国";  // u8-> utf8
	u16string str3 = u"你好中国";  // u ->  char16_t
	u32string str4 = U"你好中国";  // U -> char32_t
	wstring str5 = L"你好中国";   //  L -> wide string literal
	string str6 =R"rs(
this is a row string \n
you can use any char you want and to insert escapiing char with out using any \
)rs";
	wstring str7 = LR"rs(
this is a row string \n
you can use any char you want and to insert escapiing char with out using any \
)rs";

	wstring str8 = LR"(看看能否实现ABCabc123\n\t123\n456)";

	cout << str1 /*<< str2.c_str()*/<<str6 << endl;
	wcout << str5 << str7 << endl;
	wcout << str8 <<endl;

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
	w.erase(5, 4);  // move(e[5],e[5+4]);  // 如果不说明删除几个,默认s[5]后面的全给删除了
	w.erase(w.begin());  // erase( (it - begin),1)// only delete one 
	w.erase(w.begin() + 3, w.begin() + 5);  // earse( [frst-beg],[lst-frst]); return new frst 

	{
		string t{"abcdefg"};
		cout << t << endl;
		for (int i = 0; i < 7; ++i) {
			string t_i = t;
			t_i.erase(i,1);
			cout << t_i << endl;;
		}
	}

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
	wchar_t buf[10] = { 0 };
	w.copy(buf, 9);
	wcout <<L"string::copy to sent out contents to a outer's buf"<< buf << endl;

	//
	// resize
	//
	w.resize(10);
	//
	// swap
	//
	w.swap(e);

}
// string的 find 只有 pos,  string::npos 没找到
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

//
//    common application, 组合一些常见的应用和算法
//
TEST(String, ReverseStr) {
	wstring s = str;

	std::reverse(s.begin(), s.end());

	wcout << s;

	//for (; _First != _Last && _First != --_Last; ++_First)
	//{
	//	_STD iter_swap(_First, _Last);
	//}
}

TEST(String, ToUpperLower) {
	wstring w(str);
	std::transform(w.begin(), w.end(), w.begin(), std::toupper);
	wcout << L"std::toupper:" << w << endl;

	w.assign(str);
	std::transform(w.begin(), w.end(), w.begin(), std::tolower);
	wcout << L"std::tolower:" << w << endl;
}

//
//replace_all [c] to China;
//
TEST(String, ReplaceAll) {
	wstring w(L"there is [c] in [c], but [c] is not true [c]");
	wcout << w << endl;

	wstring pattern(L"[c]");   
	wstring replaced(L"China");  

	wcout << "pattern:" << pattern << endl;
	wcout << "replaced:" << replaced << endl;

	size_t pos = 0;
	
	while ( (pos = w.find(pattern, pos)) != wstring::npos) {
		w.replace(pos, pattern.length(), replaced);  // replace 只会影响当前的一段区间,所以pos+size就是接下来要搜索的起始点
		pos += replaced.size();
	}
	wcout << w << endl;	
}
// abc.txt ->  abc , .txt
TEST(String, SplitFileName) {
	string f("abt.txt");
	string base;
	string suffix;
	auto pos = f.find('.');
	if (pos != string::npos) {
		base = f.substr(0, pos);
		suffix = f.substr(pos + 1);
		cout << base << "   " << suffix << endl;
	}

}
// 识别每一个单词
TEST(String, ExtractWordInWords) {
	string sentence("China's railways\t are,;., expected to see a, travel rush. in the coming 10 days as many people");
	cout << "org:" << sentence<<endl;
	string delim(" \t;,.");

	size_t begIdx=0, endIdx=0;

	begIdx = sentence.find_first_not_of(delim, begIdx);
	while ((endIdx=sentence.find_first_of(delim,begIdx))!=string::npos)
	{
		cout << sentence.substr(begIdx, endIdx - begIdx) << endl;
		//cout << sentence.substr(begIdx, endIdx - begIdx) << endl;

		begIdx = sentence.find_first_not_of(delim, endIdx + 1);
	}
}

TEST(String, SlitSentence) {
	string sentence("China's railways\t are,;., expected to see a, travel rush. in the coming 10 days as many people");
	
	std::istringstream iss(sentence);
	vector<string> buf;
	string w;
	while (iss>>w)	{
		buf.push_back(w);
	}

	for_each(buf.begin(), buf.end(), [](string s) {cout << s << "\n"; });
}

TEST(String, UniqueCharInStr) {
	wstring s = str; s += str;
	
	wcout << s << endl;

	std::sort(s.begin(), s.end());
	//s.resize(std::unique(s.begin(), s.end())-s.begin());
	s.erase(std::unique(s.begin(), s.end()), s.end());
	wcout << s << endl;

}

TEST(String, IEqual) {
	std::vector<std::wstring> buf{
		L"WINWORD.EXE",
		L"POWERPNT.EXE",
		L"EXCEL.EXE"
	};
	std::wstring exe_name = L"winword.exe";

	bool rt= std::any_of(buf.begin(), buf.end(), [&exe_name](const std::wstring& str) {
		if (exe_name.empty() && str.empty()) {
			return true;
		}
		if (exe_name.length() != str.length()) {
			return false;
		}
		auto rt = std::mismatch(exe_name.begin(), exe_name.end(),
			str.begin(), str.end(), [](wchar_t l, wchar_t r) {
				return std::tolower(l) == std::tolower(r);
			});
		return rt.first == exe_name.end() && rt.second == str.end();
		});
	
	EXPECT_TRUE(rt);
}

TEST(String, Digit) {
	char p[] = "0x12";
	cerr <<"0x12"<<" "<< std::atoi("0x12") <<", since atoi can not support 0x"<< endl;


}

