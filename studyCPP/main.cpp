// studyCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"


/*
container_hearders:
	vector 
	deque
	forward_list		:  single linked list
	list				:  double linked list
	-----
	map set
	unordered_map/set
	-----
	queue/ stack		:	adapter
	-----
	array				:  fixed dimenstion array
	bitset				:  bool array
*/

int main(int argc, wchar_t** argv)
{
	testing::InitGoogleTest(&argc, argv);

	// filter *,?,-,:
	//	 aaa.*:bbb.*:ccc.*		->  only test aaa,bbb, ccc
	//	 aaa.*-aaa.version		->  test all all but except aaa.version
	//testing::FLAGS_gtest_filter = "Windows.RegOperation2";
	testing::FLAGS_gtest_filter = "Algorithm.*:Syntax.*";
	return RUN_ALL_TESTS();

}

