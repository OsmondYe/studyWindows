#include "pch.h"

/*Container:  
	sequential
	relative
*/

#include<vector>

using namespace std;

template<typename T, typename Allocator = std::allocator<T>>
class MyVector {

};

TEST(CppContainer, Vector) {
	//using std::allocator<T>  to new or delete node	
	vector<int> vi({ 1,2,3,4,5,6 });

}