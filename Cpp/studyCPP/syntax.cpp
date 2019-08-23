#include "pch.h"

TEST(Syntax, InitializerList) {
	// require constructor : vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
	// compile convert { 1,2,3,4,5,6 } ->  initializer_list<int>;
	vector<int> aa{ 1,2,3,4,5,6 };

	for_each(aa.begin(), aa.end(), [](int& i) {std::cout << i; });

	vector<std::pair<int, string>> ll{ {1,"haha"},{2,"haha"} };

	ll.size();
}

TEST(Syntax, RVauleReference) {
	//int& a = int(); //cannot convert from 'int' to 'int &'


	int&&b = int();  // var b, refer to a const value
	int&&c = 70;

	cout <<&b<<"\t"<< b<< "\t" << &c<<"\t"<<c;

	cout << "the combination of rvalue references and lvalue references is just what is needed to easily code move semantics\n";
}


TEST(Syntax, Move) {
	// move constructor
	// move assignment operator
	// std::move
	// if ojb is movable, so it isnt copyable
}

TEST(Syntax, Ref) {
	// std::ref
}

// some good template tech
