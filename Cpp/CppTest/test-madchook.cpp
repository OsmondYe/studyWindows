#include "stdafx.h"

#include "utils.h"
#include <madCHook.h>

class MadchookTest : public testing::Test {
protected:
	static void SetUpTestCase() {		
		InitializeMadCHook();
	}

	static void TearDownTestCase() {
		FinalizeMadCHook();
	}
};

TEST_F(MadchookTest, Basic) {
	// basic

	// get target process handle

	HANDLE hTargetProcess = win::get_process(L"wtlStudy.exe");
	EXPECT_NE(hTargetProcess, INVALID_HANDLE_VALUE)<<"Can not find the process of wtlStudy.exe";
	// inject cppdll.dll into target process
	//EXPECT_TRUE(InjectLibraryW(L"cppdll.dll", hTargetProcess)) << "Hook failed";
	
	
}

TEST_F(MadchookTest, InjectScreenCapture) {

	HANDLE hTargetProcess = win::get_process(L"SnippingTool.exe");
	EXPECT_NE(hTargetProcess, INVALID_HANDLE_VALUE) << "Can not find the process of SnippingTool";
	// inject cppdll.dll into target process
	EXPECT_TRUE(InjectLibraryW(L"cppdll.dll", hTargetProcess)) << "Hook failed";
}


TEST_F(MadchookTest, ScreenCapture) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	EXPECT_TRUE(CreateProcessExW(L"C:\\Windows\\System32\\SnippingTool.exe", 
		NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi,
		L"cppdll.dll"));
}


TEST_F(MadchookTest, OfficeWord) {
	// basic

	// get target process handle

	HANDLE hTargetProcess = win::get_process(L"WINWORD.EXE");
	EXPECT_NE(hTargetProcess, INVALID_HANDLE_VALUE) << "Can not find the process of WINWORD.EXE";
	// inject cppdll.dll into target process
	EXPECT_TRUE(InjectLibraryW(L"cppdll.dll", hTargetProcess)) << "Hook failed";
}

TEST_F(MadchookTest, SkyDRMViewer) {
	// basic

	// get target process handle

	HANDLE hTargetProcess = win::get_process(L"Viewer.exe");
	EXPECT_NE(hTargetProcess, INVALID_HANDLE_VALUE) << "Can not find the process of Viewer.exe";
	// inject cppdll.dll into target process
	EXPECT_TRUE(InjectLibraryW(L"cppdll.dll", hTargetProcess)) << "Hook failed";
}