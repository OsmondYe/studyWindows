#include "pch.h"

/*
	move semantic and right-vaule-reference
*/
void arg_value(string str) {	cout << str;}
void arg_value_ref(string& str) {	cout << str;}
void arg_value_cref(const string& str) {	cout << str;}
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

	int&& b = 12;  // var b, refer to a const value
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

}



namespace mrvref {
	class Widget  {

		using DataType = std::vector<int>;

	public:
		Widget() :data_({ 1,2,3,4,5 }) {}
		// give doWord two version
		DataType& doWork() &
		{
			return data_;
		}

		DataType doWork() &&   // compiler will modifty  DataType doWork(std::vector<int>& returnValue)
		{
			return std::move(data_);  // compiler will modifty returnValue.vector(vector&& data)  // move construct
		}

		DataType data_;
	};

	Widget makeWidget() {
		return Widget();
	}
}


TEST(Syntax, Move2) {
	using namespace mrvref;	

	Widget w;

	auto x1 = w.doWork();  // x1 will call vector(const vector& _Right)
	
	auto x2 = makeWidget().doWork();  // Widget().doWork(x2)  vector(vector&& _Right)

	// same as x2
	auto x3 = Widget().doWork();  // Widget().doWork(x3)

	for (auto i : x2) {
		cout << i;
	}

}