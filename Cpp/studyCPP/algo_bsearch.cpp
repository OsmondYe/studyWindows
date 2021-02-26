#include "pch.h"
#include "helper.hpp"

using namespace std;
using namespace aux;


/*
*	Divide and Conquer for quick search
*	binary search must apply on sorted range
*		upper_bound
*		lower_bound
*		binary_search
*		equal_range
*/




TEST(Algorithm, UpperBound) {
	
	auto v = vector<int>{ 1,3,6,8,10,14,20};

	int d = std::distance(v.begin(), v.end());

	// it to 1st_elem in range, >=4
	auto i = *lower_bound(v.begin(), v.end(), 4);

	// it to 1st_elem in range, >5
	auto j = *upper_bound(v.begin(), v.end(), 11);

	auto b = binary_search(v.begin(), v.end(), 20);

	// equal_range
	
	cout << i <<" " <<j<<" "<<b<< endl;

}