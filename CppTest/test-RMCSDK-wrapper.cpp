#include "stdafx.h"
#include "utils.h"
#include "sdkwrapper.h"



wchar_t gRouter[] = L"https://rmtest.nextlabs.solutions";
//wchar_t gRouter[] = L"https://r.skydrm.com";
wchar_t gTenant[] = L"skydrm.com";
//wchar_t gURL[] = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms";

using namespace std;


class RMCSKDWrapperTest : public testing::Test {

protected:
	static std::wstring workingPath;
	static std::string userLoginStr;
	static HANDLE hSession;
	static HANDLE hTenant;
	static HANDLE hUser;
	static HANDLE hLocalFiles;

protected:
	static void SetUpTestCase() {
		// set current exe folder
		wchar_t path[255] = { 0 };
		::GetCurrentDirectoryW(255, path);
		workingPath.assign(path);
		workingPath += L"\\working";

		std::string t(workingPath.begin(), workingPath.end());
		t += "\\";
		::MakeSureDirectoryPathExists(t.c_str());
		std::cout << "working_path:" << t << endl;	

		auto error = CreateSDKSession((wchar_t*)workingPath.c_str(), &hSession);
		if (error || hSession == NULL) {
			helper_reportError("Error,CreateSDKSession", error);
		}
		else {
			cout << "hSession: 0x" << hex << hSession << endl;
		};

		// test Session
		error = SDWL_Session_Initialize(hSession, gRouter, gTenant);
		if (error) {
			helper_reportError("SDWL_Session_Initialize", error);
		}
		// Session Init2
		error = SDWL_Session_Initialize2(hSession, (wchar_t*)workingPath.c_str(), gRouter, gTenant);
		if (error) {
			helper_reportError("SDWL_Session_Initialize2", error);
		}

		// get login request cookies;
		NXL_LOGIN_COOKIES* pCookies;
		size_t len_Cookies = 0;
		error = SDWL_Session_GetLoginCookies(hSession, &pCookies, &len_Cookies);
		
		
		//
		helper_readLoginJson("userJson.txt", userLoginStr);

		std::cout << "userLoginStr:\n" << userLoginStr << std::endl;



		error = SDWL_Session_GetCurrentTenant(hSession, &hTenant);
		if (error || hTenant == NULL) {
			helper_reportError("SDWL_Session_GetCurrentTenant", error);
		}
		else {
			cout << "hTenant: 0x" << hex << hTenant << endl;
		}

		error = SDWL_Session_SetLoginRequest(hSession, (char*)userLoginStr.c_str(), &hUser);
		if (error) {
			helper_reportError("SDWL_Session_SetLoginRequest", error);
		}
		else {
			cout << "hUser: 0x" << hex << hUser << endl;
		}

		ASSERT_EQ(error, 0)<<"error occurs\n";

	}

	static void TearDownTestCase() {

		auto error = SDWL_Tenant_ReleaseTenant(hTenant);
		if (error) {
			helper_reportError("SDWL_Tenant_ReleaseTenant", error);
		}
		else {
			hTenant = NULL;
		}

		 error = SDWL_Session_SaveSession(hSession, (wchar_t*)workingPath.c_str());
		if (error) {
			helper_reportError("SDWL_Session_SaveSession", error);
		}

		error = DeleteSDKSession(hSession);
		if (error) {
			helper_reportError("Error,DeleteSDKSession", error);
		}
		hSession = NULL;

	}

	//// Sets up the test fixture.
	//virtual void SetUp() {
	//}

	//// Tears down the test fixture.
	//virtual void TearDown() {
	//}

private:
	static void helper_readLoginJson(std::string path, std::string& buf) {
		buf.clear();

		std::ifstream ifo = std::ifstream("userJson.txt");
		if (!ifo) {
			return;
		}

		buf.append(std::istreambuf_iterator<char>(ifo), std::istreambuf_iterator<char>());
	}
public:
	static std::string helper_reportError(const char* msg, int e) {
		std::stringstream ss;
		std::string rt;
		std::string en("others");
		switch (e) {
		case SDWL_SUCCESS:
			en = "SDWL_SUCCESS";
			break;
		case SDWL_PATH_NOTFOUND:
			en = "SDWL_PATH_NOTFOUND";
			break;
		case SDWL_INVALID_JSON_FORMAT:
			en = "SDWL_PATH_NOTFOUND";
			break;
		case SDWL_INVALID_DATA:
			en = "SDWL_INVALID_DATA";
			break;
		case SDWL_CERT_NOT_READY:
			en = "SDWL_CERT_NOT_READY";
			break;
		case SDWL_NOT_ENOUGH_MEMORY:
			en = "SDWL_NOT_ENOUGH_MEMORY";
			break;
		case SDWL_ERROR_NOTFOOUND:
			en = "SDWL_ERROR_NOTFOOUND";
			break;
		case SDWL_INTERNAL_ERROR:
			en = "SDWL_INTERNAL_ERROR";
			break;
		case SDWL_LOGIN_REQUIRED:
			en = "SDWL_LOGIN_REQUIRED";
			break;
		case SDWL_ACCESS_DENY:
			en = "SDWL_ACCESS_DENY";
			break;
		default:
			break;
		}
		// set error font as red
		HANDLE	hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole == NULL || hConsole == INVALID_HANDLE_VALUE) {	
			ss << "Error:" << msg << "\tCode:" << en << "," << e << endl;
			cerr << ss.str();
			return ss.str();
		}
		
		CONSOLE_SCREEN_BUFFER_INFO scbi = { 0 };
		::GetConsoleScreenBufferInfo(hConsole, &scbi);
		::SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_RED);
		ss << "Error:" << msg << "\tCode:" << en << "," << e << endl;
		cerr << ss.str();
		//restore
		::SetConsoleTextAttribute(hConsole, scbi.wAttributes);
		return ss.str();
	}
};

std::wstring RMCSKDWrapperTest::workingPath;
std::string RMCSKDWrapperTest::userLoginStr;
HANDLE RMCSKDWrapperTest::hSession;
HANDLE RMCSKDWrapperTest::hTenant;
HANDLE RMCSKDWrapperTest::hUser;
HANDLE RMCSKDWrapperTest::hLocalFiles;


TEST_F(RMCSKDWrapperTest, Version) {
	DWORD ver = -1;
	ver = GetSDKVersion();

	EXPECT_NE(-1, ver);
	// print ver
	char s[40] = { 0 };
	_itoa_s(ver, s, 2);
	printf("SDK_Version:\n\t0x%08X\n\t0b%s\n", ver, s);

}

TEST_F(RMCSKDWrapperTest, Tenant) {
		
	DWORD error = -1;
	// str tenant
	wchar_t* aTenant;
	error = SDWL_Tenant_GetTenant(hTenant, &aTenant);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_Tenant_GetTenant", error);
	EXPECT_NE((int)aTenant, NULL);
	wcout << L"Tenant:\t" << aTenant << endl;	
	
	// str router
	wchar_t* aRouter;
	error = SDWL_Tenant_GetRouterURL(hTenant, &aRouter);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_Tenant_GetRouterURL", error);
	EXPECT_NE((int)aRouter, NULL);
	wcout << L"Router:\t" << aRouter << endl;
	
	// str rms address
	wchar_t* aRMS;
	error = SDWL_Tenant_GetRMSURL(hTenant, &aRMS);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_Tenant_GetRMSURL", error);
	EXPECT_NE((int)aRMS, NULL);
	wcout << L"RMS:\t" << aTenant << endl;;
	
}

TEST_F(RMCSKDWrapperTest, UserBasic) {
	// Test for User Login	
	DWORD error = -1;	

	// str name
	wchar_t* name;
	error = SDWL_User_GetUserName(hUser, &name);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_User_GetUserName", error);
	EXPECT_NE((int)name, NULL);
	wcout << L"Name:\t" << name << endl;;
	// str email
	wchar_t* email;
	error = SDWL_User_GetUserEmail(hUser, &email);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_User_GetUserEmail", error);
	EXPECT_NE((int)email, NULL);
	wcout<< L"Email:\t" << email << endl;	
	// str type
	int type = -1;
	error = SDWL_User_GetUserType(hUser, &type);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_GetUserType", error);
	cout << "Type:\t" << type << endl;;
	// str passcode
	char* code;
	error = SDWL_User_GetPasscode(hUser, &code);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_User_GetPasscode", error);
	EXPECT_NE((int)code, NULL);
	cout << "Passcode:\t" << code << endl;
	
	
}

TEST_F(RMCSKDWrapperTest, UserSync) {
	// update user info
	DWORD error = -1;
	error = SDWL_User_UpdateUserInfo(hUser);
	EXPECT_EQ(error,0)<< helper_reportError("SDWL_User_UpdateUserInfo", error);
	
	// update mydrive info
	error = SDWL_User_UpdateMyDriveInfo(hUser);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_UpdateMyDriveInfo", error);
	
	// get mydrive info
	DWORD64 u, t;
	error = SDWL_User_GetMyDriveInfo(hUser, &u, &t);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_GetMyDriveInfo", error);
	cout << "My Drive Info:" << "Used: 0x" << hex << u << '\t' << "Total: 0x" << hex << t << endl;
}

TEST_F(RMCSKDWrapperTest, UserOpenFile) {
	DWORD error = -1;
	// open a local nxl file
	std::wstring path(L"D:\\test\\SkyDRM-2018-05-21-12-32-53.vsd.nxl");
	ASSERT_TRUE(win::is_file_exist(path.c_str())) << L"Path not exist" << path;

	HANDLE hNxlFile = NULL;

	error = SDWL_User_OpenFile(hUser, (wchar_t*)path.c_str(), &hNxlFile);
	
	EXPECT_EQ(error,0)<< helper_reportError("SDWL_User_OpenFile", error);
	EXPECT_NE((int)hNxlFile, NULL);
}

TEST_F(RMCSKDWrapperTest, UserDecryptFile) {
	DWORD error = -1;
	HANDLE hNxlFile = NULL;
	// open a local nxl file first
	std::wstring path(L"D:\\test\\SkyDRM-2018-05-21-12-32-53.vsd.nxl");
	ASSERT_TRUE(win::is_file_exist(path.c_str())) << L"Path not exist" << path;


	error = SDWL_User_OpenFile(hUser, (wchar_t*)path.c_str(), &hNxlFile);

	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_OpenFile", error);
	EXPECT_NE((int)hNxlFile, NULL);

	std::wstring outpath(L"D:\\test\\decrpted-SkyDRM-2018-05-21-12-32-53.vsd");

	error = SDWL_User_DecryptNXLFile(hUser, hNxlFile, (wchar_t*)outpath.c_str());
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_DecryptNXLFile", error);
	ASSERT_TRUE(win::is_file_exist(outpath.c_str())) << L"Path not exist" << path;
}

TEST_F(RMCSKDWrapperTest, UserProtectFile) {
	DWORD error = -1;
	int rights[] = { 0,1,2,3,4,5,6,7,8,9,10 };

	std::wstring path(L"D:\\test\\SkyDRM.vsd");
	ASSERT_TRUE(win::is_file_exist(path.c_str())) << L"Path not exist" << path;


	error = SDWL_User_ProtectFile(hUser, (wchar_t*)path.c_str(), rights, 11);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_ProtectFile", error);
	
	wcout << L"OK to Protect the File" << path.c_str() << endl;


}

TEST_F(RMCSKDWrapperTest, UserShareFile) {
	DWORD error = -1;
	int rights[] = { 0,1,2,3,4,5,6,7,8,9,10 };
	const char* recipients[] = {
		"henry.hu@nextlabs.com",
		"jach.zhou@nextlabs.com",
		"allen.ning@nextlabs.com"
	};
	const wchar_t* comments = L"this is a comments";
	std::wstring path(L"D:\\test\\SkyDRM-2018-05-21-12-32-53.vsd.nxl");
	ASSERT_TRUE(win::is_file_exist(path.c_str())) << L"Path not exist" << path;
	

	error = SDWL_User_ShareFile(hUser, (wchar_t*)path.c_str(), rights, 11, (char**)recipients, 3, (wchar_t*)comments);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_User_ShareFile", error);
	

	wcout << L"OK to share the File" << path.c_str() << endl;
}

TEST_F(RMCSKDWrapperTest, UserUploadActivityLogs) {
	DWORD error = -1;
	error = SDWL_User_UploadActivityLogs(hUser);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_UploadActivityLogs", error);
}

TEST_F(RMCSKDWrapperTest, UserCheckLocalFiles) {
	DWORD error = -1;
	// get local files
	error = SDWL_User_GetLocalFile(hUser, &hLocalFiles);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_User_GetLocalFile", error);
	ASSERT_NE((int)hLocalFiles, NULL) << "hLocalFiles == NULL";
}

TEST_F(RMCSKDWrapperTest, UserLocalFiles) {
	DWORD error = -1;
	int size = -1;
	wcout << L"By osmond,SDWL_File_GetListNumber not impled" << endl;
	error = SDWL_File_GetListNumber(hLocalFiles, &size);
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_File_GetListNumber", error);
	wcout << L"SDWL_File_GetListNumber:" << size << endl;
	

	wchar_t * pStr = NULL;
	int pSize = -1;
	error = SDWL_File_GetList(hLocalFiles, &pStr, &pSize);
	EXPECT_EQ(error, 0)<< helper_reportError("SDWL_File_GetList", error);
	EXPECT_NE(pSize, -1) << "a wrong pSize:" << pSize;
	
	wcout << "Files:\t pSize=" << pSize << endl;
	// print each Files
	for (int i = 0; i < pSize; i++) {
		wchar_t* p = *((wchar_t**)pStr + 1);
		wcout << p << endl;
	}

	
	//
	//  TBD
	//
	wcout << L"other apis ,TBD" << endl;
	//wchar_t** p = new wchar_t*[pSize];
	//error = SDWL_File_GetList(_hLocalFiles, p, &pSize);
}

