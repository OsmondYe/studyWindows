#include "pch.h"
#include "helper.hpp"
using namespace aux;
using namespace std;

TEST(Sort, Selection) {
	auto v = getRandom(100000);
	//cout << "before sort:\n"; output(v);
	// divide seq into left and right, left sorted, right unsorted;
	// each time select max_val form right and put into left,
	// until left is full
	for (auto cur = v.begin(); cur != v.end(); ++cur) {
		auto ush = cur; 
		for (++ush; ush != v.end(); ++ush) {
			if (*cur < *ush) {
				std::swap(*cur, *ush);
			}
		}
	}
	//cout << "after sort:\n"; output(v);
	// each time, select max_val, right_range will shrink by 1
}

TEST(Sort, Insertion) {
	// like play card
	auto v = getRandom(100000);
	//cout << "before sort:\n"; output(v);
	for (auto cur = v.begin() + 1; cur != v.end(); ++cur) {
		for (auto q = cur; (q > v.begin()) && (*(q - 1) < *q); --q) {
			std::swap(*(q - 1), *q);
		}		
	}
	//cout << "after sort:\n"; output(v);
}


TEST(Sort, ExchangeBubble) {
	auto v = getRandom();

	auto last = v.end() - 1;

	cout << "before sort:\n"; output(v);
	for (auto cur = v.begin(); cur != v.end()-1; ++cur) {
		for (auto inner = cur; inner != v.end()-1; inner++) {
			if (*inner < *(inner + 1)) {
				std::swap(*inner, *(inner + 1));
			}
		}
	}

	cout << "after sort:\n"; output(v);
}

TEST(Sort, QuickSort_PartitionExchange) {
	auto v = getRandom();
	cout << "before sort:\n"; output(v);
	// todo: add your code here
	cout << "after sort:\n"; output(v);
}


TEST(Sort, DISABLED_Heap) {
	auto v = getRandom();
	cout << "before sort:\n"; output(v);
	// todo: add your code here
	cout << "after sort:\n"; output(v);
}

TEST(Sort, DISABLED_Merge) {
	auto v = getRandom();
	cout << "before sort:\n"; output(v);
	// todo: add your code here
	cout << "after sort:\n"; output(v);
}