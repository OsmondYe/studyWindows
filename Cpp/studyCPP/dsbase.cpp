#include "pch.h"
#include "helper.hpp"



//recursion:  f(n)=n!  
// 到20!就不行了
unsigned long long factorial(int n) {
	if (n <= 1) {return 1;}			// base component, 递归出口
	return n * factorial(n - 1);	// recursive component 朝着递归出口方向前进
}

unsigned long long factorial_sum(int n) {
	if (n == 0) return 0;
	return n + factorial_sum(n - 1);
}

unsigned long long fibonacci(int n) {
	if (n == 0) return 0;
	if (n == 1)  return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

int gcd(int x, int y) {
	if (y == 0) return x;
	return gcd(y, x % y);
}


TEST(DSBase, basic) {
	// cacl n!
	for (int i = 0; i < 20; i++)
	{
		cout << i << "!=" << factorial(i) << endl;
	}
	for (int i = 0; i < 100; i++)
	{
		cout << "factorial_sum" << i << "=" << factorial_sum(i) << endl;
	}
	// calc fabonacci
	for (int i = 0; i < 20; i++)
	{
		cout <<"fabonacci"<< i << "=" << fibonacci(i) << endl;
	}

	// gcd
//for (size_t i = 0; i < 101; i++)
//{
//	for (size_t j = 0; j < 101; j++)
//	{
//		cout << i << " " << j << "=" << gcd(i, j)<<endl;
//	}
//}
}

// divide and conquer
template<class T>
bool minmax(T a[], int n, int& indexOfMin, int& indexOfMax)
{// Locate min and max elements in a[0:n-1].
 // Return false if less than one element.
	if (n < 1) return false;

	if (n == 1)
	{
		indexOfMin = indexOfMax = 0;
		return true;
	}

	// n > 1
	int s = 1;              // start point for loop
	if (n % 2 == 1)  // n is odd
		indexOfMin = indexOfMax = 0;
	else
	{// n is even, compare first pair
		if (a[0] > a[1])
		{
			indexOfMin = 1;
			indexOfMax = 0;
		}
		else
		{
			indexOfMin = 0;
			indexOfMax = 1;
		}
		s = 2;
	}

	// compare remaining pairs
	for (int i = s; i < n; i += 2)
	{
		// find larger of a[i] and a[i + 1], then compare larger one
		// with a[p.second] and smaller one with a[p.first]
		if (a[i] > a[i + 1])
		{
			if (a[i] > a[indexOfMax])
				indexOfMax = i;
			if (a[i + 1] < a[indexOfMin])
				indexOfMin = i + 1;
		}
		else
		{
			if (a[i + 1] > a[indexOfMax])
				indexOfMax = i + 1;
			if (a[i] < a[indexOfMin])
				indexOfMin = i;
		}
	}

	return true;
}

TEST(DSBase, myMinMax) {
	int a[] = { 3, 2, 4, 1, 6, 9, 8, 7, 5, 0 };
	int n = 10, theMin, theMax;

	// output the array elements
	cout << "a[0:9] = ";
	copy(a, a + n, ostream_iterator<int>(cout, " "));
	cout << endl;

	// locate min and max
	minmax(a, n, theMin, theMax);

	// output locations and values
	cout << "The min element is at " << theMin << endl
		<< "Its value is " << a[theMin] << endl;
	cout << "The max element is at " << theMax << endl
		<< "Its value is " << a[theMax] << endl;
}



template<typename T>
void permutation(T l[], int beg, int end) {
	if(beg+1 ==end){
		std::copy(l, l + end, ostream_iterator<T>(cout, "")); 
		cout << endl;
		return;
	}
	for (int i = beg; i < end; ++i) {
		std::swap(l[beg], l[i]);
		permutation(l, beg+1, end);
		std::swap(l[beg], l[i]);
	}
}



TEST(DSBase, Perm) {

	char ss[] ={ "abcde" };
	permutation(ss, 0, 4);
}