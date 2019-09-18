#include "pch.h"
#include "helper.hpp"

using namespace aux;

TEST(Aux, Pair) {
	// wrapper two obj into a one obj
	pair<vector<int>, string> a(getSorted(),"hello world");
	a.first = getSorted();
	a.second = "hello world";
	a = make_pair(getSorted(), "hello world");
	pair<vector<int>, string> b = make_pair(a.first, a.second);
	pair<vector<int>, string> c(a.first, a.second);
}

TEST(Aux, Tuple) {
	// wrapper multi objs into one obj
}


TEST(Aux, Ref) {
	//std::ref()
}

TEST(Aux, Bind) {
	//std::ref()
}
