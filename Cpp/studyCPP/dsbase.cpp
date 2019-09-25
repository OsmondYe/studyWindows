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

// 还有问题,理论上都没有搞明白
void perm(const string& cs, string& pfx) {
	if (cs.empty()) { cout <<pfx<< endl; return; }

	for (int i = 0; i < cs.length(); ++i) {
		char c = cs[i];
		string cs_i = cs; 
		cs_i.erase(i,1);
		perm(cs_i, pfx+=c);
	}
}

void perm2(string& cs, int k, int m) // 1 based, not 0
{
	if (k == m) {
		cout << cs << endl;
		return;
	}

	for (int i = k; i <= m; ++i) {
		std::swap(cs[k], cs[i]);
		perm2(cs, k + 1, m);
		std::swap(cs[k], cs[i]);
	}
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

	string s{ "ab" };

	string pfx;
	perm(s,pfx);
	//perm2(s,0,s.length()-1); // 
}