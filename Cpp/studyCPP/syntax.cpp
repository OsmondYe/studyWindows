#include "pch.h"

/*

noexcept  函数不抛出异常
constexpr int square(int x){return x*x;}  在编译时直接帮square的结果作为const

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

	for (auto i : { 1,2,3,4,5,6 }) {
		cout << i << ' ';
	}

	vector<std::pair<int, string>> ll{ {1,"haha"},{2,"haha"} };
	ll.size();

	//
	// 一致初始化
	//
	int values[]{ 1,2,3,4,5,6 };
	vector<int> v{ 2,3,4,5,6,76 };
	vector<string> cities{ "changan","xian","beijing","tianjing" };
	int j{};  //  j=0;
	int* ppp{}; // ppp=nullptr;



}


TEST(Syntax, NULLPTR) {
	void* p = nullptr;
	/*
		void f(int)
		void f(void*)
		
		if f(p) ?
	*/

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

	// 如果一个值,以后不再使用了,可以用move来表明移动走了

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

// 不定参模板
namespace vt {
	//作为递归调用出口,必须提供
	template<typename T>
	void print(const T& arg) {
		cout << arg << endl;
	}
	// 会被解释成递归调用
	template <typename T, typename ... Types>
	void print(const T& first, const Types& ... args) {
		// handle first
		print<T>(first);
		// handle others
		print(args ...);
	}
	/*
	  print(1, 12.34, "hello world", 'c');
		print 会被编译成多个函数
			print<int,double,char*,char>
			print<double,char*,char>
			print<char*,char>
			print<char>
	*/
}
TEST(Syntax, Template_Variadic) {

	vt::print(1, 12.34, "hello world", 'c');
	/*
		print 会被编译成多个函数
			print<int,double,char*,char> 
			print<double,char*,char> 
			print<char*,char> 
			print<char> 
	*/
}


// using可以应用于模板的简写行驶了
template<typename T> 
using ThisIsMyVec= std::vector<T>;
TEST(Syntax, Template_Alias) {
	ThisIsMyVec<int> v{ 1,2,3,4,5,6 };
	vt::print(v[0], v[1], v[2]);
}

// keyword decytype(var) 可以让编译器找到这个变量的类型
TEST(Syntax, Template_Decltype) {
	vector<int> v{ 1,2,3,4,5,6,7,8,9 };
	decltype(v)::const_iterator cit = v.cbegin();
}


/* 模板支持新的声明方法, 自动推倒需要的返回值类型
template<typename T1, typename T2>
auto add(T1 x, T2 y) -> decltype(x + y);

add 模板函数的具体返回值取决于 x+y后的类型  c11
*/
TEST(Syntax,TemplateNewFunctionDeclaration) {

}

// 扩展了 enum
enum class Salutation : char {
	mr,
	ms,
	co,
	none
};

TEST(Syntax, ScoptedEnumeration) {
	Salutation s = Salutation::mr;
	s = Salutation::ms;
	s = (Salutation)12;
}


// 类支持重载 new 运算符
TEST(Syntax, Override_New) {
	class ttt {
	public:
		void* operator new(size_t size) {
			std::cout << "some one call me" << std::endl;
			return malloc(size);
		}
		int a;
		int b;
	};

	class td :public ttt {
	public:
		int c;
	};

	ttt* a = new ttt();
	td* b = new td();

	cout << "using this, derived calss will call base class's operator new to apply memory" << endl;
	cout << "so , override new is a good method to detact memory leak" << endl;

}