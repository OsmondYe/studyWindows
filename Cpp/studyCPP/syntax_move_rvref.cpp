#include "pch.h"

/*
	move semantic and right-vaule-reference
*/

namespace {
	const int len = 10000;
	class BigData {
	public :
		int* p;
		BigData() :p(new int[len]) {}

		BigData(const BigData& rh) :BigData() {
			for (int i = 0; i < len; i++) {
				p[i] = rh.p[i];
			}
		}

		BigData& operator=(const BigData& rh) {
			if (this == &rh) {
				return *this;
			}
			delete[]p;
			p = new int[len];
			for (int i = 0; i < len; i++) {
				p[i] = rh.p[i];
			}
		}
		~BigData() {if (p) {delete[] p;}}
	};

	BigData getData() {
		BigData rt;
		rt.p[0] = 1;
		rt.p[1] = 2;
		rt.p[2] = 3;
		return rt;     //copy rt's data to outer,  rt will de-ctor here
	}

	class BigData2 {
	public:
		int* p;
		BigData2() :p(new int[len]) {}

		BigData2(const BigData2& rh) :BigData2() {
			for (int i = 0; i < len; i++) {
				p[i] = rh.p[i];
			}
		}

		BigData2& operator=(const BigData2& rh) {
			if (this == &rh) {
				return *this;
			}
			delete[]p;
			p = new int[len];
			for (int i = 0; i < len; i++) {
				p[i] = rh.p[i];
			}
		}
		~BigData2() {
			if (p) {
				delete[] p;
			}
		}

		// mov
		BigData2(BigData2&& rh) :p(0){
			swap(p, rh.p);  // l <-> r,  and l value will not be used anymore
		}

		BigData2& operator=(BigData2&& rh) {
			if (this == &rh) {
				return *this;
			}
			swap(p, rh.p); // l <-> r,  and l value will not be used anymore
		}

	};

	BigData2 getData2() {
		BigData2 rt;
		rt.p[0] = 1;
		rt.p[1] = 2;
		rt.p[2] = 3;
		return rt;     //this version get move-semantic optimize than previous one
	}


	class LRConfused : public BigData2 {
	public:
		LRConfused() :BigData2() {}

		LRConfused(const LRConfused& rh) : BigData2(rh) {};
		LRConfused(LRConfused&& rh) : BigData2(rh) {}; //: BigData2(rh) got problme, will call BigData2(const BigData2& rh)

	};
}

TEST(Syntax, RVauleReferenceProblemIndroduce) {
	BigData b;
	b.p[10] = 5;
	BigData a =b;  // copy ctor?? how to optimize? 
	
	BigData c;
	c = b;		// equal operator?? how to optimize?

	BigData d = getData(); // problem here in getData();
	cout << "getData has a temp obj, and copy obj to d, then, obj will call deconstructor\n";
	cout << "[[optimization direction]] ->  move temp obj to outer, and do not need to call deconsturcotr, outer'will call it\n";
	cout << " right value ref give me a right to modify some right-value, especially in function's return value\n";

	cout << "Give Bigdata move-consturctor and move assignment\n";


	BigData2 e = getData2();  // much faster than getData();



}

TEST(Syntax, RVauleReferenceProblemIndroduceNoRVO) {
	for (size_t i = 0; i < len; i++)
	{
		auto x = getData();
	}
}

TEST(Syntax, RVauleReferenceProblemIndroduceRVO) {
	for (size_t i = 0; i < len; i++)
	{
		auto x = getData2();
	}
}

TEST(Syntax, RVauleReferenceProblemIndroduceLRConfused) {

	LRConfused a;
	LRConfused b = a;             // using copy-ctor   
	LRConfused c = std::move(a);  // using move_ctor
}

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

/*
perfect forwarding 
*/
namespace {
	template <typename F>
	struct reverse_bind {
		F f_;
		reverse_bind(F const& f) : f_(f) {}

		template<typename T1, typename T2>
		void operator()(T1 t1, T2 t2) {
			f_(t2, t1);
		}
	};

}