#include "pch.h"

/*
Compile means
	- determine code segment
	- determing data block
	- organize code and date

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

void arg_value(string str) {
	cout << str;
}

void arg_value_ref(string& str) {
	cout << str;
}

void arg_value_cref(const string& str) {
	cout << str;
}

void arg_value_rvref(string&& str) {   // str is a l_value
	cout << str;
}

void arg_value_crvref(const string&& str) {   // str is a l_value
	cout << str;
}

TEST(Syntax, RVauleReference) {
	//int& x = 12; //cannot convert from 'int' to 'int &'

	int base = 12;
	int& a = base;
	cout << "same address for a and base\n";
	cout << a << '\t' << &a << '\t' << &base << endl; // same address for a and base
	
	////////////

	int&&b = 12;  // var b, refer to a const value
	cout << &b << "\t" << b << endl;  // b is a var, point to the int 12


	cout << "the combination of rvalue references and lvalue references is just what is needed to easily code move semantics\n";

	cout << " RBF is the base of  move and forward semantices\n";

	std::string&& str = "this is good";  //"this is good" (07FF7576A8BB0h) build as string and set to str, str is a stack var

	arg_value(str);  // will call basic_string(const basic_string& _Right)  to gen a str var
	arg_value_ref(str);
	arg_value_cref(str);
	//arg_value_rvref(str); // error rv_ref can not be bound to lv;
	arg_value_rvref(std::move(str));
	arg_value_crvref(std::move(str));
	cout << str;


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