// CppTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sdkwrapper.h"

#include <gtest/gtest.h>


using namespace std;

class GlobalEnvironment : public testing::Environment {
	virtual void SetUp() {
		wcout << L"OsmondTest:Global Setup" << endl;
	}

	// Override this to define how to tear down the environment.
	virtual void TearDown() {
		wcout << L"OsmondTest:Global Teardown" << endl;
	}
};

//void binary_to_ascii(unsigned int value) {
//	unsigned int quotient;
//	quotient = value / 10;
//	if (quotient != 0)
//	{
//		binary_to_ascii(quotient);
//	}
//	putchar(value % 10 + '0');
//}

int main(int argc, wchar_t** argv) {
	testing::InitGoogleTest(&argc, argv);
	
	// filter *,?,-,:
	//	 aaa.*:bbb.*:ccc.*		->  only test aaa,bbb, ccc
	//	 aaa.*-aaa.version		->  test all all but except aaa.version
	//testing::FLAGS_gtest_filter = "Windows.*";
	//testing::FLAGS_gtest_filter = "MadchookTest.*";
	testing::FLAGS_gtest_filter = "RMCSKDWrapperTest.*";
	//testing::FLAGS_gtest_filter = "RMCSKDWrapperTest.UserProtectFile";
	//testing::FLAGS_gtest_filter = "RMCSKDTest.*";
	testing::FLAGS_gtest_filter = "Utils.*";

	return RUN_ALL_TESTS();
}





