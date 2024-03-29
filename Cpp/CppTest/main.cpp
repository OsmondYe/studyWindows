// CppTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main(int argc, wchar_t** argv) {
	testing::InitGoogleTest(&argc, argv);
	

	// filter *,?,-,:
	//	 aaa.*:bbb.*:ccc.*		->  only test aaa,bbb, ccc
	//	 aaa.*-aaa.version		->  test all all but except aaa.version
	//testing::FLAGS_gtest_filter = "Windows.*";
	testing::FLAGS_gtest_filter = "Win32.*";
	testing::FLAGS_gtest_filter = "User32.*";
	testing::FLAGS_gtest_filter = "User32.PrinterRelated";
	testing::FLAGS_gtest_filter = "Shell32.*";
	testing::FLAGS_gtest_filter = "Windows.FClearFolder";
	testing::FLAGS_gtest_filter = "Utils.Str";
	testing::FLAGS_gtest_filter = "Process.Enum";
	testing::FLAGS_gtest_filter = "Process.AdjustDebugRights";
	testing::FLAGS_gtest_filter = "gdi.ScreenCapture";
	testing::FLAGS_gtest_filter = "System.LockScreen";
	testing::FLAGS_gtest_filter = "INet.DownloadFile";
	testing::FLAGS_gtest_filter = "System.LogOff";
	testing::FLAGS_gtest_filter = "Process.Exit";
	testing::FLAGS_gtest_filter = "Process.Terminate";
	testing::FLAGS_gtest_filter = "System.Information";
	testing::FLAGS_gtest_filter = "Socket.QueryIP";
	testing::FLAGS_gtest_filter = "Socket.CheckIP";
	testing::FLAGS_gtest_filter = "Socket.SimpleCS";
	testing::FLAGS_gtest_filter = "Socket.ByteOrder";
	testing::FLAGS_gtest_filter = "Socket.Connect";
	testing::FLAGS_gtest_filter = "Synchronization.ConditionVariable";
	testing::FLAGS_gtest_filter = "Synchronization.KernelObject";
	testing::FLAGS_gtest_filter = "Synchronization.Event";
	testing::FLAGS_gtest_filter = "Synchronization.WaitableTimer";
	testing::FLAGS_gtest_filter = "Synchronization.Semaphore";
	

	return RUN_ALL_TESTS();
}





