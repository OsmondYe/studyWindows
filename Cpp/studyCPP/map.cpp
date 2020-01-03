#include "pch.h"

#include <map>
#include <unordered_map>


TEST(Map, Hash) {

	std::unordered_map<int, int> m;
	m.reserve(100);

	for (int i = 10; i > 0; i--) {
		m[i] = 1;
		m.insert({ i,1 });
		m.insert(std::make_pair(i, 1));
		m.emplace(i, 1);// using pair's template constructor
	}

	for (auto i : m) {
		cout << i.first << " " << i.second << endl;
	}

}

