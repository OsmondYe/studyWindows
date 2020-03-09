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