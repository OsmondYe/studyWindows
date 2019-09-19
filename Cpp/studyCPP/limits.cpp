#include "pch.h"

#include<limits>

// numeric_limits 

// 可以用来确定不同内置类型的数值上下界问题

TEST(Limits, Basic) {

	cout << numeric_limits<int>::max();
	cout << numeric_limits<int>::min();

}