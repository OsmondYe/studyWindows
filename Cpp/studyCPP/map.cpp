#include "pch.h"

#include <map>
#include <unordered_map>

using namespace std;

TEST(Map, Basic) {
	map<int, int> m;

	m[1] = 10;
	m[2] = 20;

	for (auto& i : m) {
		cout << i.first << "->" << i.second << endl;
	}
	cout << m[1] << " " << m[2] << endl;
	cout << m[3] << endl;

	// pair<first,second> will be returned;
	auto it = m.find(1);

	cout << it->first << "  " << it->second << endl;


	it = m.find(4);		
	// get mud, must check its validation
	//cout << it->first << "  " << it->second << endl;
	if (it == m.end()) {
		cout << "can not find key=4" << endl;
	}

}

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

