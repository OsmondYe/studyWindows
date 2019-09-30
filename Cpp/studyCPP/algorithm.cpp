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
	// find return it when found, or last iter- if not
	/*	for (; _First != _Last; ++_First)
			if (*_First == _Val)
				break;
		return (_First);
	*/
	auto v = getSorted();
	cout << "find 90 is:" << *(std::find(v.begin(), v.end(), 90)) << endl;
	// if(Pred(*it)) return it;
	std::find_if(v.begin(), v.end(), [](int& i) {return i > 20; });
	//if (!Pred(*it)) return it;
	std::find_if_not(v.begin(), v.end(), [](int& i) {return i < 20; });
	// [b1:e1), [b2:e2) find first *p1==*p2   return p1
	// in 2 sequence, first match 1 seq
	auto v2 = getRandom();
	cout << *(std::find_first_of(v.begin(), v.end(), v2.begin(), v2.end())) << endl;
	cout << *(std::find_first_of(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i = j + 12; })) << endl;
	// find end, last 
	cout << "tbd find end\n";
	//std::find_end(v.begin(), v.end(), v2.begin(), v2.end());
}

TEST(Algorithm, EqualMismatch) {
	auto v1 = vector<int>{ 1,2,3,4,5 };
	auto v2 = vector<int>{ 2,3,4,5,6 };
	std::equal(v1.begin() + 1, v1.end(), v2.begin(),v2.end() - 1);
	//std::mismatch()
	auto pair = std::mismatch(v1.begin()+1, v1.end() - 1, v2.begin());
	cout << *pair.first << " " << *pair.second;

}

TEST(Algorithm, Search) {
	//std::search()
}

TEST(Algorithm, MaxMin) {
	auto v = getSorted();
	cout << std::max(v[5], v[6], [](int i, int j) {return i < j; }) << endl
		<< std::min(v[5], v[6], [](int i, int j) {return i < j; }) << endl
		<< *max_element(v.begin(), v.end(), less<int>()) << endl
		<< *min_element(v.begin(), v.end(), less<int>()) << endl;

	auto rt = minmax_element(v.begin(), v.end());
}


//
//  Mutating 
//
TEST(Algorithm, Fill) {
	auto v = vector<int>(10);
	fill(v.begin(), v.end(), 12);
	output(v);
	fill_n(v.begin(), 2, 100);  // count 2, value =100;
	output(v);
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
	transform(v2.begin(), v2.end(), v3.begin(),  back_inserter(v4), [](int i, int j) {return i + j; });
	output(v4);

}

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


	//std::copy(v1.begin(), v1.end(), v1.end()-1);
	std::copy_if(v1.begin(), v1.end(), std::inserter(v2, v2.end()), [](int i) {return i > 5; });
	output(v2);

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
	adjacent_difference(v.begin(), v.end(), v2.begin(), [](int& first, int& second) {return second + first; });
	output(v);
	output(v2);
}

TEST(Algorithm, PartialSum) {
	auto v = getSorted(10);
	auto v2 = vector<int>(v.size());
	//[b:e) ->  o1=b1, o2=b1+b2,o3=b1+b2+b3 ...
	partial_sum(v.begin(), v.end(), v2.begin(), [](int& first, int& second) {return second + first; });
	output(v);
	output(v2);
}


//
//  Comparision
//

TEST(Algorithm, Equal) {
	auto v = getSorted(10);
	auto v2 = v;
	// [b1:e1) == [b2:e2)
	cout<<equal(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i == j; });
}

TEST(Algorithm, Lexicographic) {
	auto v = getSorted(10);
	auto v2 = v;
	// [b1:e1) == [b2:e2)
	// may triger exception when you dont use > or < , for example, use ==, will triger exception
	// if i<j then j must > i , if not , tirgger exception
	cout<<lexicographical_compare(v.begin(), v.end(), v2.begin(), v2.end(), [](int i, int j) {return i < j; });
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


