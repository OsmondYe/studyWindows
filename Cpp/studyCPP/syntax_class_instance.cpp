#include "pch.h"

/*
  copy constructor
  assign construcotr
*/

namespace clsins {

    class Widget {
        using DataType = std::vector<int>;
    public:
        Widget() {}

        // Widget x(w);  
        // Widget z = x;
        // Copy-Constructor
        Widget(const Widget& rh) : data_(rh.data_) {        }
        // Asign operator
        Widget& operator = (const Widget& rh) {data_ = rh.data_;}

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