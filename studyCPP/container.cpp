#include "pch.h"

/*Container:  
	sequential
	relative
*/

#include <vector>
#include <list>
#include <forward_list>

using namespace std;

template<class Iterator>
void output(Iterator beg, Iterator end) {
	while (beg != end) {
		wcout << *beg++;
	}
	wcout << endl;
}

template<class Container>
void output(Container& c) {
	output(c.begin(), c.end());
}

TEST(CppContainer, Vector) {
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


TEST(CppContainer, List) {
	list<int> ll({ 1,1,2,2,3,3,3,3,4,5,5,5,5,5,6 });
	output(ll.begin(), ll.end());
	auto ll2 = ll;
	ll2.unique();
	output(ll);

	auto ll3 = ll2;
	
	ll3.splice(++ll3.begin(), ll2); // after call, ll2 is null


	output(ll3);

}