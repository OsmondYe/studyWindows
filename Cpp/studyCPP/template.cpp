#include "pch.h"
using namespace aux;
/*
Keyword:
	instantiation;		-> compiler time, to produce the true fucntion according to acutal arguement passing to template function(T_F)
	decution			-> which acutal arguement passing to T_F		
	specialization      -> using tempalte to create the real class/function
						   explicit
						   partial 
	ODR					-> one-definition rule
*/

/*
#  for decltype
transform(cbegin(aa), cend(aa), std::inserter(keys, keys.begin()), [](decltype(aa)::value_type const& pair) {
	return pair.first;
});


	template<class... _Valty>
		decltype(auto) emplace_back(_Valty&&... _Val)

*/


template <typename T>
inline const T& my_max(const T& a, const T& b) {
	return a > b ? a : b;
}

template <typename RT,typename T1, typename T2 >
inline  const RT& my_max2(const T1& a, const T2& b) {
	return a > b ? a : b;
}

template <typename RT, typename T1, typename T2 >
inline   RT my_max3(const T1& a, const T2& b) {
	return a > b ? a : b;
}

TEST(Template, GenericMax) {
	println(my_max(1, 2));
	println(my_max(1.1, 2.2));
	println(my_max(string("strst"), string("hello")));
	//println(my_max(1, 2.2));  error
	println(my_max<double>(1, 2.2));
	println(my_max(static_cast<double>(1), 2.2));
	//println(my_max("str", "hello"));
	//println(my_max('c', 2));
	auto x = my_max2<double>(4.1, 2);
	println(my_max2<double>( 4.1, 2));
	println(my_max3<double>( 4.1, 2));
}

template<typename T>
class my_stack {
public:
	std::vector<T> elems;

	my_stack() {
		elems.clear();
	}

	void push(const T& t) {
		elems.push_back(t);
	}
	T top(); // in outside, declaration, must use full name
	void pop() {
		elems.pop_back();
	}
	// some must usingt full type
	my_stack(const my_stack<T>&) = delete;
	my_stack<T>&  operator = (const my_stack<T>&) = delete;
};

template<typename T>
T my_stack<T>::top() {
	return elems.back();
}

typedef my_stack<int> myIntStack;

TEST(Template, ClassDefine) {
	my_stack<double> ss;

	ss.push(1.2);
	ss.push(1.222);
	println(ss.top());
	ss.pop();
	println(ss.top());
	ss.pop();
	// using typedef
	myIntStack s;
	s.push(1);
	println(s.top());
	s.pop();
}
template<>
class my_stack<std::string> {   // tell compiler, if string, using this specialization version
public:
	std::deque<std::string> elems;  // elements

	void push(const string& s) { elems.push_back(s); }
	void pop() { elems.pop_back(); }
	std::string top() { return elems.back(); }
};

TEST(Template, Specialization) {
	
	my_stack<string> ss;

	ss.push("hello");
	ss.push("hello22");
	println(ss.top());ss.pop();
	println(ss.top());ss.pop();
}


TEST(Template, LocalSpecialization) {

	println("base:\t   template<typename T1, typename T2> class Myclass");
	println("Local1:\t   template<typename T> class Myclass<T,T>");
	println("Local2:\t   template<typename T> class Myclass<T,int>");
	println("Local3:\t   template<typename T1, typename T2> class Myclass<T1*,T2*>");
}


template<typename T, typename Container = std::vector<T> >
class my_stack2 {
public:
	void push(const T& t) {elems.push_back(t);}
	T top(); // in outside, declaration, must use full name
	void pop() {elems.pop_back();}

	// using
	Container elems;
};


TEST(Template, DefaultTemplateAcutalArgurment) {
	my_stack2<int> s;
}

template<typename T, int size>
class my_stack3 {
public:
	T elem[size];
	
};

TEST(Template, NoneTypeTemplateArgurment) {

	my_stack3<int, 3> ss;

	ss.elem[0] = 1;
	ss.elem[1] = 1;
	ss.elem[2] = 1;

}


// - unified data-struct
template<typename T = int>
struct my_node {
	T elem;
	my_node* next;

	my_node(T e) :elem(e), next(NULL) {}

};

// array with fixed len 
template<typename T = int, unsigned size = 10>
class my_array {
	T elem[size];
public:
	T& operator[](unsigned i) {
		return elem[i];
	}
};

// function-pointer as the parameter of the tempalte
// generic callback
template<typename T, void (*Func)(T & v)>
void my_foreach(T array[], unsigned size) {
	for (size_t i = 0; i < size; i++)
	{
		Func(array[i]);
	}
}

void my_add(int& i) {
	i++;
}

void my_print(int& i) {
	cout << i<<" ";
}

TEST(Template, FunctionAsParam) {
	aux::println("using function pointer as tempalte parameter");
	int a[] = { 1,2,3,4,5 };
	my_foreach<int, my_add>(a, 5);
	my_foreach<int, my_print>(a, 5);
}

template<typename Container>
void my_print_all(const Container& c) {
	typename Container::const_iterator pos;  // if you want to use Container's sub type, you must using typename keyword

	for (pos = c.cbegin(); pos != c.cend(); ++pos) {
		cout << *pos << " ";
	}
	cout << endl;
}

TEST(Template, TypeNameInTemmplateTemplate) {
	vector<int> ss{ 1,2,3,4,5 };
	my_print_all(ss);
}


template<typename T, int val>
T addVal(const T& x) {
	return x + val;
}

TEST(Template, ConstTemmplateParam) {
	vector<int> x{ 1,2,3,4,5 };
	std::transform(x.begin(), x.end(), x.begin(), addVal<int, 50>);
	my_print_all(x);
}


template<typename T, typename Container = std::vector<T> >
class my_stack4 {
public:
	void push(const T& t) { elems.push_back(t); }
	T top() { return elems.back(); }
	void pop() { elems.pop_back(); }

	// forceing T2 converting to T
	template<typename T2, typename Container = std::vector<T2> >
	void operator=(const my_stack4<T2>& r) {
		if ((void*)this == (void*)&r) {
			return;
		}
		this->elems.clear();
		for (auto i : r.elems) {
			this->elems.push_back((T)i);
		}
	}

	// using
	Container elems;
};

TEST(Template, ClassMemFunUsingTemplate) {
	my_stack4<int> s1;
	my_stack4<double>s2;

	s2.push(1.1);
	s2.push(2.1);
	s2.push(3.1);
	// different elem type can merge together 
	s1 = s2;
	my_print_all(s1.elems);
	
}

/*
	pointer and reference as template parameter

	// must be a const value in compile time
*/


template<int* p>   // require p must be a const, so global value is good
struct my_wrapper {
	int get() { return *p; }
	void set(int i) { *p = i; }
};


int global_int = 12;

TEST(Template, PtrRefAsParam) {
	println("pointer and reference as template parameter");

	my_wrapper<&global_int>  wrapper;
	wrapper.set(45);
	cout << wrapper.get();
}


/*
	class member function as tempalte parameter
*/

class my_class {
public:
	int i_;
	my_class(int i) :i_(i) {}
	int add(int i) { return i_ += i; }
	int sub(int i) { return i_ -= i; }
	int mul(int i) { return i_ *= i; }
	int div(int i) { return i_ /= i; }
};

typedef int (my_class::* my_calss_mem_fun)(int);

template<my_calss_mem_fun func>
void fun(my_class& obj, int param) {
	 (obj.*func)(param);
}

TEST(Template, MemFunAsParam) {
	println("class member function as tempalte parameter");

	my_class cc(12);

	fun<&my_class::add>(cc, 12);
}

/*
	the other template parameter as template parameter

	only class can be the template template parameter
	
*/

template< template<typename TT> class Func, typename T>
void my_foreach2(T array[], unsigned size) {

	Func<T> func;

	for (size_t i = 0; i < size; i++)
	{
		func(array[i]);
	}

}
template<typename T>
struct my_add2 {
	void operator()(T& i) { i++; }
};

TEST(Template, TempalteTemplateParamAsParam) {
	println("template template parameter as the template parameter");

	vector<int> v = getSorted(10);

	// my_add2 is a tempalte class, but we make it as my_foreach2's param
	my_foreach2 < my_add2 > (v.data(), 10);
}

/*
	Specialized case in template function
*/

template<int i=100>
void my_specialized_fun() {
	my_specialized_fun<i-1>();
	cout << i << " ";
}

template<> 
void my_specialized_fun<0>() {
	cout << "0 ";
}

TEST(Template, SpecializedCase) {
	my_specialized_fun(); cout << endl;
}



TEST(Template, Concept) {
	cout << "Concept is a new idea to decor Template T parameter\n";
	cout << "For example, how can I konw my class object can be used for std::min()\n";
}