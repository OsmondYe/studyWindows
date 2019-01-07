#pragma once


int __cdecl f1(int a, int b, int c, int d) {
	int local1 = a;
	int local2 = b;
	int local3 = c;
	int local4 = d;
	int total = a + b + c + d + local1 + local2 + local3 + local4;
	return total;
}

int _stdcall f2(int a, int b, int c, int d) {
	int total = a + b + c + d;
	return total;
}

int __fastcall f4(int a, int b, int c, int d) {
	int total = a + b + c + d;
	return total;
}

// for c++ virtual function

class BaseNonImprotant {
	int _a; int _b; int _c; int _d;

public:
	BaseNonImprotant() :_a(1),_b(2),_c(3),_d(4){

	}
	int f1(int a, int b, int c, int d) {
		 _a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "f1"; // no use, requre compiler must generate this fun
		return total;
	}
	int f2(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "f2"; // no use, requre compiler must generate this fun
		return total;
	}
	int f3(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "f3"; // no use, requre compiler must generate this fun
		return total;
	}
};

class MotherFucker {
public:

	MotherFucker() {
		printf("haha");
		f1(1, 2, 3, 4);
	}

	int f1(int a, int b, int c, int d) {

		std::cout << "MotherFucker" << std::endl; // no use, requre compiler must generate this fun
		return 0x111;
	}
};



class Base 
{
public:

	static 	 Base* container[100];
	static 	 int  c;
protected:
	int _a; int _b; int _c; int _d;

public:
	Base() :_a(0xbbbbbb), _b(0xbbbbbb), _c(0xbbbbbb), _d(0xbbbbbb) {

	}
	virtual int f1(int a, int b, int c, int d) {
		if(c<4)
			container[c++] = this;
		this->_a = a;
		if (a < 100) {
			this->_a = 0x1111;
		}

		std::cout << "Base-f1"<< std::endl; // no use, requre compiler must generate this fun
		return 0x111;
	}
	int f2(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "f2"; // no use, requre compiler must generate this fun
		return total;
	}
	virtual int f3(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "Base-f3"<<std::endl; // no use, requre compiler must generate this fun
		return total;
	}

    static int Recall() {
		for (int i = 0; i < 4; i++) {
			container[i]->f1(1, 2, 3, 4);
		}
		return 0;
	}

};
Base* Base::container[100];
int Base::c = 0;



class Derived1 : public Base {
public:
	Derived1() {
		_a = 0xd1d1d1d1;
		_b = 0xd1d1d1d1;
		_c = 0xd1d1d1d1;
		_d = 0xd1d1d1d1;
	}
	virtual int f1(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "Derived1 F1, total:"<<total<<std::endl; // no use, requre compiler must generate this fun
		return 0xDededede;
	}
};

class Derived2 : public Base {
public:
	Derived2() :Base() {
		_a = 0xd2d2d2d2;
		_b = 0xd2d2d2d2;
		_c = 0xd2d2d2d2;
		_d = 0xd2d2d2d2;
	}
	virtual int f3(int a, int b, int c, int d) {
		_a = a; _b = b; _c = c; _d = d;
		int total = a + b + c + d + _a + _b + _c + _d;
		std::cout << "Derived2 F3, Total:" << total<<std::endl; // no use, requre compiler must generate this fun
		return 0xDededede;
	}
};


class Weapon {
public:
	int a;
	Weapon() {
		a = 0xb0b0b0b0;
	}
	virtual void set() {
		a += 1;
	}
};

class Tank : virtual public Weapon{
public:
	int b;
	Tank() {
		b = 0xb1b1b1b1;
	}
	virtual void set() {
		Weapon::set();
		b += 1;
	}
};


class Warship : virtual public Weapon {
public:
	int b;
	Warship() {
		b = 0xb2b2b2b2;
	}
	//virtual void set() {
	//	Weapon::set();
	//	b += 1;
	//}

};

class AmphiCar :
		public Tank,
		public Warship
{
public:
	int c;
	int c2;
	int c3;
	AmphiCar() {
		c = 0xc1c1c1c1;
		c2 = 0xc2c2c2c2;
		c3 = 0xc3c3c3c3;
	}

	virtual void set() {
		Tank::set();
		c += 1;
	}

};

