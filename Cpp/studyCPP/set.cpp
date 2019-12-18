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



	s.insert({ 1,2,3,4,5,6,7,8,9,10 });
	aux::output(s);

	set<int> toBeDel{ 2,8,9 };
	// 搞笑了， s集合只能删除他自己的
	//s.erase(toBeDel.begin(), toBeDel.end());
	//aux::output(s);

	set<int> anew;
	// 用差集来实现
	std::set_difference(s.begin(), s.end(), toBeDel.begin(), toBeDel.end(), inserter(anew,anew.end()));
	aux::output(anew);
}

TEST(Set, Multiset) {

}

// -未排序，也就是hash(key) o(1) 最佳情况， o(n)最坏情况
TEST(Set, Unordered_Set) {
	std::unordered_set<int> us{ 1,1000,2,200,3,300,4,400 };

	// 存储的数据没有排序
	for (auto s : us) {
		cout << s << endl;
	}

}

TEST(Set, Unordered_Multiset) {

}