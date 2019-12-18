#include "pch.h"
#include "helper.hpp"
using namespace aux;
using namespace std;

int counts = 1000;


void insertion_sort(vector<int>& s) {
	if (s.empty()) {	
		return;
	}

	for (auto out = s.begin() + 1; out != s.end(); ++out) {	
		auto key = *out;
		auto cur = out;		
		while (cur > s.begin() && *(cur-1) > key) // 
		{
			*(cur) = *(cur-1);
			--cur;
		}
		*(cur) = key;
	}
}

TEST(Sort, Insertion) {
	auto sort = [](vector<int>& v) {
		cout << "before sort:\n"; output(v);
		insertion_sort(v);
		cout << "after sort:\n"; output(v);
	};

	std::vector<int> v;	
	sort(v);
	v = { 12 };
	sort(v);
	v = { 12,5 };
	sort(v);
	// like play card
	v = getRandom(counts);
	sort(v);
	v = getSorted(counts);
	sort(v);
	std::reverse(v.begin(), v.end());
	sort(v);


}

void merge_helper(vector<int>& v, vector<int>::iterator p, vector<int>::iterator q, vector<int>::iterator r) {
	vector<int> L(p, q);
	vector<int> R(q, r);

	auto L_i = L.begin();
	auto R_i = R.begin();
	for (auto i = p; i != r; ++i) {
		if (L_i == L.end()) {
			*i = *R_i++;
			continue;
		}
		if (R_i == R.end()) {
			*i = *L_i++;
			continue;
		}
		if (*L_i <= *R_i) {
			*i = *L_i++;
		}
		else {
			*i = *R_i++;
		}
	}

}

void merge_sort(vector<int>& v, vector<int>::iterator begin, vector<int>::iterator end) {

	if (begin < end) {
		vector<int>::iterator mid = begin + (end-begin) / 2;
		if (begin == mid) {		

			return;
		}
		merge_sort(v, begin, mid);
		merge_sort(v, mid, end);
		merge_helper(v, begin, mid, end);
	}
}
TEST(Sort, Merge) {
	std::vector<int> v;
	// test merge_helper first
	auto sort_helper = [](vector<int>& v) {
		cout << "before sort_helper:\n"; output(v);
		merge_helper(v, v.begin(), v.begin() + (v.end() - v.begin()) / 2, v.end());
		cout << "after sort_helper:\n"; output(v);
	};
	v.clear(); sort_helper(v);
	v = { 9 }; sort_helper(v);
	v = { 2,1 }; sort_helper(v);
	v = { 2,1,9 }; sort_helper(v);
	v = { 1,5,4,7 }; sort_helper(v);
	v = { 1,5,4,7,9 }; sort_helper(v);
	v = { 1,2,3,6,7,0,4,5,8,9 }; sort_helper(v);
	


	auto sort = [](vector<int>& v) {
		if (v.empty()) {
			return;
		}
		cout << "before sort:\n"; output(v);
		merge_sort(v,v.begin(),v.end());
		cout << "after sort:\n"; output(v);
	};
	v.clear();
	sort(v);
	/*v = { 12 };
	sort(v);*/
	//v = { 12,5 };
	//sort(v);
	v = getRandom(2);
	sort(v);
	v = getSorted(counts);
	sort(v);
	std::reverse(v.begin(), v.end());
	sort(v);

}

TEST(Sort, DISABLED_Selection) {
	auto v = getRandom(counts);
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




TEST(Sort, DISABLED_ExchangeBubble) {
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

TEST(Sort, DISABLED_QuickSort_PartitionExchange) {
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

