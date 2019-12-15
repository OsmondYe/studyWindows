/*
实现所有一直的排序问题
	- 插入排序, 类似于打扑克,每摸到一张牌时,我们肯定是再已经排序的序列中,找到新牌的位置,然后插入
		- 插入点后面的所有元素是不是需要依次后移,
		- 腾出可插入的位置
		- 书上说对于少量元素,这样来很快
	- 归并排序(Merge, 分治算法的应用), 
		- 序列分为左,右,然后merge左右 
*/
#include "pch.h"
#include "helper.hpp"
using namespace aux;
using namespace std;

int counts = 1000;

/*
插入排序
经验:
	--out;  stl不允许--操作小于begin(), 这是绝对正确的
	从右往左,给腾出位置,	(cur > s.begin() && *(cur-1) > key) 
		只有cur>s.begin(), cur-1 做比较才是有意义的,否则会出现begin()左边的问题
*/
void insertion_sort(vector<int>& s) {
	if (s.empty()) {	// 这里可以去看一下,s.end()+1 为什么会失败?
		return;
	}
	// 第一个元素不考虑, 
	//从begin+1开始,往左边找合适的插入点
	for (auto out = s.begin() + 1; out != s.end(); ++out) {
		// 依次往左边找
		//for (auto cur = out; cur > s.begin() && (*(cur - 1) > *cur); --cur) {
		//	std::swap(*(cur - 1), *cur);
		//}
		
		// 选取关键字
		auto key = *out;
		auto cur = out;
		// 和已经排序的数据从右往左依次比较
		// 给Key腾出位置就好
		while (cur > s.begin() && *(cur-1) > key) // 
		{
			*(cur) = *(cur-1);
			--cur;
		}
		*(cur) = key;// 如果cur根本就没有挪动,那也可以自己给自己赋值

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

/*
归并排序 Merge:
	-把原问题分解为规模小的和原问题类似的子问题,递归的解决子问题,再合并子问题
	-两个已经排序的序列合并为问题的解
	-cpp下标从0开始,这个很讨厌啊
*/
// merging  array[p...q] array[q+1 ... r] as a sorted arrary
//void merging(int* array, int p, int q, int r) {
//	int n1 = q - p + 1;
//	int n2 = r - q;
//	int* L = new int[n1];
//	for (int i = 0; i < n1; ++i) {
//		L[i] = array[p+1-1];
//	}
//	int* R = new int[n2];
//	for (int i = 0; i < n2; ++i) {
//		R[i] = array[q + i];
//	}
//	// merge [p -> r]
//	int i = 0; 
//	int j = 0;
//	for (int k = p; k != r; ++k) {
//		if (i == n1) {
//			array[k] = R[j++];
//			continue;
//		}
//		if (j == n2) {
//			array[k] = L[i++];
//			continue;
//		}
//		if (L[i] < R[j]) {
//			array[k] = L[i++];
//		}
//		else {
//			array[k] = R[j++];
//		}
//	}
//}


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
	//if (begin == end) {  // 加在这里,会形成死循环
	//	return;
	//}
	if (begin < end) {
		vector<int>::iterator mid = begin + (end-begin) / 2;
		if (begin == mid) {		
			// 这行是我半天才调试出来的,如果diff(end,begin)=1,那么begin和mid必然是同一个点
			// 同一个点是不用再排序的,直接返回
			// 不能加头部,必然死循环
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

