#include "pch.h"
#include "helper.hpp"

using namespace std;
using namespace aux;

/*
Range [beg,end)  -- half-open range
Multi_range  < [b1,e1), b2>  确保range2有效长度至少和r1相同


Manipulating  {移除 | 重排 | 修改}
Manipulating  {移除 | 重排 | 修改}
predicate :  bool fun(T);


Nonmodifying
Modifying
Removing
Mutating   -- assigning or swapping
Sorting
sorted-range
numeric

*/
TEST(Algorithm, ForEach) {
	// [b:e)  void f(*iter); 
	// return f();  this could be a class with functor obejct
	vector<int> v = getSorted();
	output(v);

	for_each(v.begin(), v.end(), [](int& i) {i++;}); // lamda 内参数为 int& ， 有可能改变元素的值
	output(v);

	struct Sum {
		Sum() :sum(0) {}
		int sum;
		void operator()(int& i) { sum += i; }
	};

	Sum s = for_each(v.begin(), v.end(), Sum());
	cout << "Sum s=" << s.sum<<endl;
	
}

TEST(Algorithm, Predicate) {
	// all_of  [b:e)  all(f(x)) return true;
	// any_of    has(f(x)) return true
	// none_of  !all_of
	vector<int> v = getSorted();

	cout << boolalpha;
	cout << "all_of i>101:" << all_of(v.begin(), v.end(), [](int& i) {return i > 101; }) << endl;
	cout << "any_of is 50"<< any_of(v.begin(), v.end(), [](int& i) {return i ==50; }) << endl;
	cout << "none_of is 101" << none_of(v.begin(), v.end(), [](int& i) {return i == 101; }) << endl;
}

TEST(Algorithm, Count) {
	// [b:e) if(f(x)) count++; return count
	auto v = getSorted();
	cout << "count 10:" << count(v.begin(), v.end(), 10) << endl;
	cout << "cout 20<x<50:" << count_if(v.begin(), v.end(), [](int& i) {return i > 20 && i < 50; }) << endl;
}

TEST(Algorithm, Find) {
	auto v = getSorted();
	decltype(v) v2 = { 55,56,57,58 };
	// find return it when found, or last iter- if not
	/*	for (; _First != _Last; ++_First)
			if (*_First == _Val)
				break;
		return (_First);
	*/
	cout << "find 90 is:" << *(std::find(v.begin(), v.end(), 90)) << endl;
	// if(Pred(*it)) return it;
	std::find_if(v.begin(), v.end(), [](int& i) {return i > 20; });
	// if (!Pred(*it)) return it;
	std::find_if_not(v.begin(), v.end(), [](int& i) {return i < 20; });

	/*
	V区间中搜索_第一个存在于死亡名单上的人
	for (; first != last; ++first) {
		for (ForwardIt it = s_first; it != s_last; ++it) {
			if (p(*first, *it)) {
				return first;
			}
		}
	}
	*/
	cout << "find_first_of: the first man that appeared in the black-list" << endl;
	cout << *(std::find_first_of(v.begin(), v.end(), v2.begin(), v2.end())) << endl;
	cout << *(std::find_first_of(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i == j + 12; })) << endl;
	// find end, last 
	cout << "find_end: better to name as search_end, last occurrence of the v2 in v\n";
	
	auto iter=std::find_end(v.begin(), v.end(), v2.begin(), v2.end());
	if (iter != v.end()) {
		cout << "result of find_end\n";
		for_each(iter, iter + v2.size(), [](int i) {cout << i << " "; });
	}

	// adjacent_find :
	cout << "std::adjacent_find: two consecutive identical elemetns";
	//std::adjacent_find(v.cbegin(), v.cend());
}


TEST(Algorithm, MaxMin) {

	cout <<"max(1,1):"<< std::max(1, 1)<<endl;

	auto v = getSorted();

	cout << std::max(v[5], v[6], [](int i, int j) {return i < j; }) << endl
		<< std::min(v[5], v[6], [](int i, int j) {return i < j; }) << endl
		<< *max_element(v.begin(), v.end(), less<int>()) << endl
		<< *min_element(v.begin(), v.end(), less<int>()) << endl;

	cout << "max_element:" << *std::max_element(v.begin(), v.end()) << endl;
	cout << "min_element:" << *std::min_element(v.begin(), v.end()) << endl;

	auto rt = minmax_element(v.begin(), v.end());
	cout << "in minmax_element,max is:" << *rt.second << "min is:" << *rt.first << endl;
}

TEST(Algorithm, Search) {
	// std::search_n()
	// _n 意思： 具有n个连续具备此特征
	vector<int> v = { 1,2,3,4,5,6,6,6,6,7,8,9,10 };
	// 这个区间里面是否有4个连续的6
	bool rt = std::search_n(v.begin(), v.end(), 4, 6) != v.end();
	EXPECT_TRUE(rt);

	//std::search()
	// 查找第一个子空间, first sub_range
	// 2 个区间, 是否有相等的子区间,如果有返回其地址
	vector<int> v2 = { 5,6,6 };  // search 中的 v2区间内所有值,可以在v的区间内找到

	auto iter = std::search(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i == j; });
	if (iter != v.end()) {
		for_each(iter, v.end(), [](int i) {cout << i << " "; });
	}

	// 判断v 是否存在 4个连续偶数
	bool dummy[4] = { true,true,true,true };
	iter = std::search(v.begin(), v.end(), dummy, dummy + 4, [](int i, bool j) {return j == ((i % 2) == 0); });
	if (iter != v.end()) {
		for_each(iter, v.end(), [](int i) {cout << i << " "; });
	}
}



//
//  Mutating 
//

TEST(Algorithm, Copy) {
	/*
	for (; _First != _Last; ++_Dest, (void)++_First)
		{
		*_Dest = *_First;
		}
	*/
	auto v1 = getSorted(10);
	cout << "before:"; output(v1);
	auto v2 = v1; v2.clear();
	// using iterator insert adapter to impl copy
	//std::copy(v1.begin(), v1.end(), v2.begin());  // error, since v2 is empty, convert it iterator as a insert_insetror
	std::copy(v1.begin(), v1.end(), std::inserter(v2, v2.begin()));
	cout << "v2, using std::inserter:"; output(v2);

	std::copy(v1.begin(), v1.end(), std::back_inserter(v2));// or std::front_inserter
	cout << "v2, using std::back_inserter:"; output(v2);


	//std::copy_if
	std::copy_if(v1.begin(), v1.end(), std::inserter(v2, v2.end()), [](int i) {return i > 5; });
	output(v2);

	// std::copy_backward,  从后往前进行复制
	vector<string> vs{ "hello","this","is","an","example" };
	deque<string> ll(vs.begin(),vs.end());
	std::copy_backward(vs.begin(), vs.end(), ll.end());
}


TEST(Algorithm, Transform) {
	/*
		for (; _UFirst != _ULast; ++_UFirst, (void)++_UDest)
		{
		*_UDest = _Func(*_UFirst);
		}
	*/
	auto v1 = getSorted(10);
	cout << "before:"; output(v1);
	std::transform(v1.begin(), v1.end(), v1.begin(), [](int i) {return i + 1; });
	cout << "after:"; output(v1);

	// 应用, toupper, tolower
	string a{ "haha_hehe_heihei" }; 
	println(a);
	transform(a.begin(), a.end(), a.begin(), std::toupper);
	println(a);

	/*
		for (; _UFirst1 != _ULast1; ++_UFirst1, (void)++_UFirst2, ++_UDest)
		{
		*_UDest = _Func(*_UFirst1, *_UFirst2);
		}
	*/
	auto v2 = getSorted(10);
	auto v3 = v2;

	decltype(v3) v4;
	// v2 ,v3 两个序列对应值相加，结果送v4
	transform(v2.begin(), v2.end(), v3.begin(), back_inserter(v4), [](int i, int j) {return i + j; });
	output(v4);

	// 应用,计算区间内的间差,
	// 错位相减,各间差组成区间
	v1 = getSorted();
	v4.clear();
	std::transform(v1.begin(), v1.end() - 1, v1.begin() + 1,back_inserter(v4), 
		[](int i, int j) {return j - i; }
	);
	output(v4);
}


TEST(Algorithm, Fill) {
	auto v = vector<int>(10);
	fill(v.begin(), v.end(), 12);
	output(v);
	fill_n(v.begin(), 2, 100);  // count 2, value =100;
	output(v);
	
	// app:
	fill_n(ostream_iterator<char>(cout), 10, '-');
	fill_n(ostream_iterator<char>(cout), 10, '=');
}


TEST(Algorithm, Swap) {
	auto v = getSorted(100);
	// obj swap
	std::swap(*v.begin(), *(v.begin() + 1));
	// rang swap
	std::swap_ranges(v.begin() + 10, v.begin() + 20, v.begin() + 20);
	// iter swap  , using two iter, swap its pointed value
	std::iter_swap(v.end() - 1, v.end() - 2);

	output(v);
}


TEST(Algorithm, Sort) {

	auto gv = getRandom(1000);
	std::sort(gv.begin(), gv.end());
	//output(v);
}

//
//  Numeric
//
TEST(Algorithm, Iota) {	// 小增量的意思, 针对c++那就是 ++i 每次都会自增
	auto v = vector<int>(100);
	// [b:e)  *it=++i;
	iota(v.begin(), v.end(), 0);
	output(v);
}

TEST(Algorithm, Accumulate) {
	// [_Val = _Reduce_op(_Val, *_UFirst))  return _Val
	auto v = getSorted(100);
	cout << accumulate(v.begin(), v.end(), 0, [](int b, int c) {return b + c; });
}

TEST(Algorithm, InnerProduct) {
	// [s1)*[s2)init = op1(std::move(init), op2(*first1, *first2));
	auto v = getSorted(10);
	cout << inner_product(v.begin(), v.end(), v.begin(), 0)<<endl;
}

TEST(Algorithm, AdjacentDifference) {
	auto v = getSorted(10);
	for_each(v.begin(), v.end(), [](int& i) {return i *= i; });

	auto v2 = vector<int>(v.size());
	//[b:e) ->  vaule=op(b,e) -> (e-b)
	adjacent_difference(v.begin(), v.end(), v2.begin(), [](auto first, auto second) {return second + first; });
	output(v);
	output(v2);
}

TEST(Algorithm, PartialSum) {
	auto v = getSorted(10);
	auto v2 = vector<int>(v.size());
	//[b:e) ->  o1=b1, o2=b1+b2,o3=b1+b2+b3 ...
	partial_sum(v.begin(), v.end(), v2.begin(), [](const  int& first, const int& second) {return second + first; });
	output(v);
	output(v2);
}


//
//  Comparision
//

TEST(Algorithm, Equal) {
	//
	//  // compare [b1, e1) to [b2, ...) using _Pred
	//
	auto v = getSorted(10);
	auto v_ = v;
	// [b1:e1) == [b2:e2)
	cout<<equal(v.begin(), v.end(), v_.begin(), v_.end(), [](int i, int j) {return i == j; });


	auto v1 = vector<int>{ 1,2,3,4,5 };
	auto v2 = vector<int>{ 2,3,4,5,6 };
	// 2 sets of elems are same
	std::equal(v1.begin() + 1, v1.end(), v2.begin(), v2.end() - 1);

	// 忽略大小写比较
	string s1{ "china" }, s2{ "ChinA" };
	cout << s1 << "==" << s2 << "?" << std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(),
		[](char i, char j) {return ::tolower(i) == tolower(j);
		}) << endl;


}


TEST(Algorithm, Mismatch) {
	auto v1 = vector<int>{ 1,2,3,4,5 };
	auto v2 = vector<int>{ 2,3,4,5,6 };
	//std::mismatch()
	auto pair = std::mismatch(v1.begin() + 1, v1.end() - 1, v2.begin());
	if (pair.first == v1.end()) {
		cout << "no mismatch";
	}
	cout << *pair.first << " " << *pair.second;

}

TEST(Algorithm, Partition) {
	// partition 就是根据一个规则进行分割,符合的放前面
	auto v = aux::getSorted(10);
	auto is_even = [](int i) {return i % 2 == 0; };

	cout << "before parition:";
	aux::output(v);
	std::partition(v.begin(), v.end(), is_even);
	cout << "after partiton:"; output(v);
}


TEST(Algorithm, Lexicographic) {
	auto v = getSorted(10);
	auto v2 = v;
	// [b1:e1) == [b2:e2)
	// may triger exception when you dont use > or < , for example, use ==, will triger exception
	// if i<j then j must > i , if not , tirgger exception
	cout<<lexicographical_compare(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i < j; });
}

TEST(Algorithm, IsPermutation) {
	// 不考虑顺序,2个区间数据相同, 不同的排列组合而已
	// 比 Equal要高级一点
	cout << "TBD, I don't understand the algo for is_permutation";
	auto v = getSorted(10);
	cout << "v:";
	aux::output(v);
	auto v2 = v;
	//std::shuffle(v2.begin(),v2.end(),std::)
	aux::shuffle(v2);
	cout << "v2:";
	aux::output(v2);

	cout << "std::isPermutation:" << std::is_permutation(v.begin(), v.end(), v2.begin()) << endl;
}



//
//   Sort Operation
//

TEST(Algorithm, IsSorted) {
	auto v = getSorted(10);
	bool b = std::is_sorted(v.begin(), v.end());
	cout << "is_sorted:" << b;
}

TEST(Algorithm, IsSortedUntil) {
	auto v = getSorted(10);
	auto it = std::is_sorted_until(v.begin(), v.end());
}




