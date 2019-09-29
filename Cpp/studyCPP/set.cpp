#include "pch.h"

#include <set>
#include <unordered_set>

/*
set 和 mutilset 必然都是 以 tree为基类,tree能提供什么功能,他就是什么功能
*/



// 集合的存储建模, 要能实现快速查找,并且避免重复
TEST(Set, Basic) {
	set<int> s;
	// 类只提供了这2个,其他都是基类提供的
	//s.swap(); s.operator=()
	auto b = s.begin();

	multiset<int> ms;


	
}