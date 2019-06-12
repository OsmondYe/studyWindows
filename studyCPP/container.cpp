#include "pch.h"
#include "helper.hpp"
/*Container:  
	sequential
	relative
*/



using namespace std;
using namespace aux;


TEST(Container, Vector) {
	//using std::allocator<T>  to new or delete node	
	vector<int> vi({ 1,2,3,4,5,6 });
	
	output(vi.begin(), vi.end());
	output(vi);
	vi.push_back(12);
	vi.push_back(12);
	vi.push_back(12);
	vi.push_back(12);
	vi.push_back(12);
	// sizeof(vi)==4;
}


TEST(Container, List) {
	list<int> ll({ 1,1,2,2,3,3,3,3,4,5,5,5,5,5,6 });
	output(ll.begin(), ll.end());
	auto ll2 = ll;
	ll2.unique();
	output(ll);

	auto ll3 = ll2;
	
	ll3.splice(++ll3.begin(), ll2); // after call, ll2 is null


	output(ll3);
}

// depend on algorithem  make_heap, push_heap, pop_heap
TEST(Container, PriorityQueue) {
	auto l = { 12,34,33,1,3,5,9,56,457,35,87,568 };
	priority_queue<int> pq(l.begin(),l.end());

	while (!pq.empty()) {
		cout << pq.top()<<" " ;
		pq.pop();

	}

	
}