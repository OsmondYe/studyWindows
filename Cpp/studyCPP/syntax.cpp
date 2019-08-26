#include "pch.h"

/*
 bool _Has_unused_capacity() const noexcept
   const noexcept

 _Ty& back()
{	// return last element of mutable sequence
return (this->_Mylast()[-1]);
}

const _Ty& back() const
	{	// return last element of nonmutable sequence
		return (this->_Mylast()[-1]);
}

// 相同函数后面的const会引起重载吗

 static_cast dynamic_cast const_cast, reinterpret_cast

 std::addressof
 std::forward


 // Move ,右值引用
void push_back(int&& _val) {
	emplace_back(std::move(_val));
}

constexpr
constexpr allocator(const allocator&) noexcept = default;

*/


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

TEST(Syntax, Addressof) {
	// std::addressof
}

TEST(Syntax, Forward) {
	// std::forward
}


class sample_const {
public:
	sample_const() :_i(100) {}

	int& getI() {
		return _i;
	}

	const int& getI() const  {
		return _i;
	}

	int _i;
};


void tcout(const int& i) {
	cout << i;
}
// 成员函数返回引用时,最好分清楚const和非const引用
TEST(Syntax, MemFunConstOverride) {
	sample_const ss;
	int i = ss.getI();
	int& ii = ss.getI();
	const int& iii = ss.getI();
	tcout(ss.getI());
	cout << ii;

}

// some good template tech
