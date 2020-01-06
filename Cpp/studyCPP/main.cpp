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
	srand(time(NULL));	// give a randome seed


	testing::InitGoogleTest(&argc, argv);

	// filter *,?,-,:
	//	 aaa.*:bbb.*:ccc.*		->  only test aaa,bbb, ccc
	//	 aaa.*-aaa.version		->  test all all but except aaa.version
	//testing::FLAGS_gtest_filter = "Windows.RegOperation2";
	//testing::FLAGS_gtest_filter = "*Container.*:Algorithm.*:Syntax.*";
	//testing::FLAGS_gtest_filter = "Concurrency.MustJoinOrDetach";

	//testing::FLAGS_gtest_filter = "Idiom.*:Syntax.*:Container.*:Concurrency.*";
	testing::FLAGS_gtest_filter = "Syntax.OverrideNew";
	testing::FLAGS_gtest_filter = "Stream.*";
	testing::FLAGS_gtest_filter = "Regex.*";
	testing::FLAGS_gtest_filter = "Random.*";
	testing::FLAGS_gtest_filter = "Algorithm.*";
	testing::FLAGS_gtest_filter = "DSBase.Perm";
	testing::FLAGS_gtest_filter = "Iterator.Vector";
	testing::FLAGS_gtest_filter = "String.ReplaceAll";
	testing::FLAGS_gtest_filter = "Idiom.MacroParam";
	testing::FLAGS_gtest_filter = "Sort.Merge";
	testing::FLAGS_gtest_filter = "Concurrency.Mutex";
	testing::FLAGS_gtest_filter = "String.IEqual";
	testing::FLAGS_gtest_filter = "Chrono.*";
	testing::FLAGS_gtest_filter = "Set.*";
	testing::FLAGS_gtest_filter = "OyeAlgo.Fibonacci";
	testing::FLAGS_gtest_filter = "Sort.Quick";
	testing::FLAGS_gtest_filter = "DSLinear.MAX_SUB_ARRAY";
	testing::FLAGS_gtest_filter = "DSLinear.Vector_Remove_duplicate_Elem";
	testing::FLAGS_gtest_filter = "DSLinear.Steal_Cut_DP";
	testing::FLAGS_gtest_filter = "Memory.SmartPtr";
	testing::FLAGS_gtest_filter = "Map.Hash";
	testing::FLAGS_gtest_filter = "DSTree.*";
	testing::FLAGS_gtest_filter = "DSStr.*";
	testing::FLAGS_gtest_filter = "DSStr.*";
	testing::FLAGS_gtest_filter = "DSBase.ReverseInt";
	

	return RUN_ALL_TESTS();

}

