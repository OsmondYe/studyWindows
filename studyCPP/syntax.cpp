#include "pch.h"

TEST(Syntax, RVauleReference) {
	//int& a = int(); //cannot convert from 'int' to 'int &'


	int&&b = int();  // var b, refer to a const value
	int&&c = 70;

	cout <<&b<<"\t"<< b<< "\t" << &c<<"\t"<<c;

	cout << "the combination of rvalue references and lvalue references is just what is needed to easily code move semantics\n";

}