#include "pch.h"




/*

value_type{char, wchar_t, int32, int64}

_ALLOC_MASK = sizeof (value_type) <= 1 ? 15
	: sizeof (value_type) <= 2 ? 7
	: sizeof (value_type) <= 4 ? 3
	: sizeof (value_type) <= 8 ? 1 : 0

return (pointer_traits<pointer>::pointer_to(**this));

const auto _Mycont =123;

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
	/*
template<class _Elem>	class initializer_list{
public:
	typedef _Elem value_type;	typedef const _Elem& reference;
	typedef const _Elem& const_reference;	typedef size_t size_type;
	typedef const _Elem* iterator;	typedef const _Elem* const_iterator;

	constexpr initializer_list() noexcept
		: _First(nullptr), _Last(nullptr){	}

	constexpr initializer_list(const _Elem *_First_arg,
		const _Elem *_Last_arg) noexcept
		: _First(_First_arg), _Last(_Last_arg)
		{	// construct with pointers
		}
	 constexpr const _Elem * begin() const noexcept
		{		return (_First);	}

	 constexpr const _Elem * end() const noexcept
		{	return (_Last);	}

	 constexpr size_t size() const noexcept
		{	return (static_cast<size_t>(_Last - _First));}
private:
	const _Elem *_First;
	const _Elem *_Last;
	};

Require compile to convert {1,2,3,4,5} ->  initializer_list object -> some thing like a range

*/
	// require constructor : vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
	// compile convert { 1,2,3,4,5,6 } -> [buffer] ->  initializer_list<int>;	
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
	// move assignment operator  对于一个类而言,2个都需要有
	// std::move
	// if ojb is movable, so it isnt copyable
	expe::ExperimentObj a ;
	{
		expe::ExperimentObj b = std::move(a);
	}

	expe::ExperimentObj c;
	c = std::move(a);

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

/*
	using iterator_category = random_access_iterator_tag;
	using value_type = typename _Mystr::value_type;
	using difference_type = typename _Mystr::difference_type;
	using pointer = typename _Mystr::const_pointer;
	using reference = const value_type&;
*/