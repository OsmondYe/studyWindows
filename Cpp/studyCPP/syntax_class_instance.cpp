#include "pch.h"

/*
  copy constructor
  assign construcotr
*/

namespace clsins {

    class IFun {
    public:
        virtual void SomeWork() noexcept = 0;  // c11 noexcept 
    };

    class Widget  :public IFun{
        using DataType = std::vector<int>;
    public:
        Widget() {}

        // Widget x(w);  
        // Widget z = x;
        // Copy-Constructor
        Widget(const Widget& rh) : data_(rh.data_) {        }
        // Asign operator
        Widget& operator = (const Widget& rh) {data_ = rh.data_;}

        virtual void SomeWork() noexcept override final {
            cout << "SomeWork" << endl;
        }


    private:

        DataType data_;
    };


}

using namespace clsins;

TEST(Syntax, ClsCopyConsturctor) {
    Widget w;
    Widget x(w);    // copy-constructor
    Widget z = x;   // copy-consturctor

    cout << "Widget z = x, \nalso copy-constructor\n";
    
}

namespace multiple_inheritance {
    class if1 {
    public:
        virtual void f1() { cout << "if1 f1()\n"; }
        virtual void f2() { cout << "if1 f2()\n"; }
        virtual void f3() { cout << "if1 f3()\n"; }
        virtual void f4() { cout << "if1 f4()\n"; }
        unsigned int x{0xf1f1000};
        unsigned int y{0xf1f1111};
        unsigned int z{0xf1f1222};
    };

    class if2 {
    public:
        virtual void f1() { cout << "if2 f1()\n"; }
        virtual void f2() { cout << "if2 f2()\n"; }
        virtual void f3() { cout << "if2 f3()\n"; }
        virtual void f44() { cout << "if2 f4()\n"; }
        unsigned int x{ 0xf2f2000 };
        unsigned int y{ 0xf2f2111 };
        unsigned int z{ 0xf2f2222 };
    };

    class base : public if1, public if2 {
    public:
        virtual void f1() { cout << "base f1()\n" << endl; }


    public:
        unsigned int x{ 0xbbbb0000 };
        unsigned int y{ 0xbbbb1111 };
        unsigned int z{ 0xbbbb2222 };
    };
}

TEST(Syntax, MultiInheritance) {
    using namespace multiple_inheritance;
    // if1_vt,x,y,z; if2_vt,x,y,z; x,y,z;    11 * 4 == 44;
    cout << "sizeof(base):" << sizeof(base) << endl;

    if1* p = new base;
    p->f1();
    p->f4();

    if2* p2 = dynamic_cast<if2*>(p);   // if1 if2之间没有必然关系，转型会失败
    if (p2 == NULL) {
        cout << "cast error from if1* to if2*\n";
    }
    else {
        p2->f44();
    }

    base* p3 = dynamic_cast<base*>(p);  // downcast 转型会成功
    if (p3 == nullptr) {
        cout << "case errorform if1* to base*\n";
    }
    else {
        //p3->f2(); // ambiguous access of 'f2'
        p3->if1::f2();
        p3->if2::f2();
    }
}