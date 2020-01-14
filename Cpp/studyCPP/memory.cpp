#include "pch.h"

// smart pointer

TEST(Memory, SmartPtr_Basic) {
	int* p = NULL;
	delete p;	// delete NULL has no side-effect

	cout << "delete NULL is safe\n";

	cout << "smart ptr is a class, will call delete in it's destructor\n";
}


TEST(Memory, SmartPtr_Auto) {

	cout << "been removed in 2017\n";

	//auto_ptr has been removed in 2017
	{
		// auto_ptr  , wrap an object pointer to ensure destruction
		std::auto_ptr<int> sp1(new int(123));


		// can't be using in STL container
		// ownership will be transferred to sp2 from sp1, sp1 will become a NULL-wrapee
		auto sp2 = sp1;	// sp2 will get sp1's wrappee, auto_ptr(auto_ptr& _Right) noexcept : _Myptr(_Right.release()) {}


		// can't make sp1 sp2 refer to the same obj,
		// dangerous code
		//int* ppp = new int(12);
		//std::auto_ptr<int> spDan1(ppp);
		//std::auto_ptr<int> spDan2(ppp);	// ppp will be delete twice

	}

}

TEST(Memory, SmartPtr_Unique) {
	cout << "unique means: unique object ownershipi semantics\ncan be used in stl";
}

TEST(Memory, SmartPtr_Weak) {
}


TEST(Memory, SmartPtr_Shared) {
	// shared_ptr
	std::shared_ptr<int> spInt(new int(12));
}



TEST(Memory, SmartPtr_Array) {
	// unique_prt has buildin

	std::unique_ptr<int[]> spArray(new int[10]); // must init the value

	// give init value
	std::fill(spArray.get(), spArray.get() + 10, 0);



	for (int i = 0; i < 10; ++i) {
		spArray[i] = i;
	}
	

}

