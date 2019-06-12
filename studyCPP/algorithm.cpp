#include "pch.h"
#include "helper.hpp"
//TEST(CppAlgorithm, Basic)

using namespace std;
using namespace aux;

TEST(Algorithm, Basic) {
	// total is 80  using iterator as a range, beg ,end
	//[b:e)
}

struct Sum {
	Sum() :sum(0) {}
	int sum;
	void operator()(int& i) { sum += i; }
};

TEST(Algorithm, ForEach) {
	// [b:e)  void f(x); 
	// return f();  this could be a class with functor obejct
	vector<int> v = getSorted();
	output(v);

	for_each(v.begin(), v.end(), [](int& i) {i++;});
	output(v);

	Sum s = for_each(v.begin(), v.end(), Sum());
	cout << "Sum s=" << s.sum<<endl;
}

TEST(Algorithem, Predicate) {
	// all_of  [b:e) f(x)
	// any_of
	// none_of
	vector<int> v = getSorted();

	cout << boolalpha;
	cout << "all_of i>101:" << all_of(v.begin(), v.end(), [](int& i) {return i > 101; }) << endl;
	cout << "any_of is 50"<< any_of(v.begin(), v.end(), [](int& i) {return i ==50; }) << endl;
	cout << "none_of is 101" << none_of(v.begin(), v.end(), [](int& i) {return i == 101; }) << endl;
}

TEST(Algorithem, Count) {
	// [b:e) if(f(x)) count++; return count
	auto v = getSorted();
	cout << "count 10:" << count(v.begin(), v.end(), 10) << endl;
	cout << "cout 20<x<50:" << count_if(v.begin(), v.end(), [](int& i) {return i > 20 && i < 50; }) << endl;

}