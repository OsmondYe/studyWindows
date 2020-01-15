#include "pch.h"
#include "helper.hpp"

using namespace aux;

// keyword:  decltype;
/*
transform(cbegin(aa), cend(aa), std::inserter(keys, keys.begin()), [](decltype(aa)::value_type const& pair) {
	return pair.first;
});


	template<class... _Valty>
		decltype(auto) emplace_back(_Valty&&... _Val)

*/

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


TEST(Template, base) {
	aux::println("template basic");
}

// function-pointer as the parameter of the tempalte
// generic callback
template<typename T, void (*f)(T & v)>
void my_foreach(T array[], unsigned size) {
	for (size_t i = 0; i < size; i++)
	{
		f(array[i]);
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