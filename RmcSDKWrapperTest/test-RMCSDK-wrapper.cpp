#include "stdafx.h"
#include "utils.h"
#include "sdkwrapper.h"


namespace {
	wchar_t gRouter[] = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8443";
	wchar_t gTenant[] = L"caee73af-c572-4ddc-b784-7e1f46190fde";
	//wchar_t gRouter[] = L"https://rmtest.nextlabs.solutions";
	//wchar_t gRouter[] = L"https://r.skydrm.com";
	//wchar_t gTenant[] = L"skydrm.com";
	//wchar_t gURL[] = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms";

	int full_rights[] = { 1,2,4,8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x40000000 };
	int full_rights_len = 12;

	int adhoc_full_rights[] = { 1 ,4,0x200,0x400,0x40000000};
	int adhoc_full_rights_len = 5;

	const char* recipients[] = {
		"henry.hu@nextlabs.com",
		"jach.zhou@nextlabs.com",
		"allen.ning@nextlabs.com"
	};
	const wchar_t* comments = L"this is a comments";

	const char* tags = ""; // tags

	const char* separate_line = "=============================\n";
}
using namespace std;


class RMCSKDWrapperTest : public testing::Test {

public:
	static std::wstring workingPath;
	static std::string userLoginStr;
	static HANDLE hSession;
	static HANDLE hTenant;
	static HANDLE hUser;
	static HANDLE hLocalFiles;

	static WaterMark watermark;
	static Expiration expiration;

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
		std::cout << "SetUpTestCase: working_path:" << t << endl;	

		// Seesion Create
		auto error = CreateSDKSession((wchar_t*)workingPath.c_str(), &hSession);
		if (error || hSession == NULL) {
			helper_reportError("Error,CreateSDKSession", error);
		}
		else {
			cout << "hSession: 0x" << hex << hSession << endl;
		};

		// Session Init2
		error = SDWL_Session_Initialize2(hSession, (wchar_t*)workingPath.c_str(), gRouter, gTenant);
		if (error) {
			helper_reportError("SDWL_Session_Initialize2", error);
		}

		//if (userLoginStr.size() < 10) {

		// recover user first
		string email, passcode;
		if (load_login_user(email, passcode)) {
			error=SDWL_Session_GetLoginUser(hSession, email.c_str(), passcode.c_str(), &hUser);
			if (error != 0) {
				cout << "Failed to recover user,call login()" << endl;
				login();
			}
			else {
				cout << "Recover user session successfully" << endl;
			}
		}
		else {
			login();
		}
			

		// set values for watermark and expiration		
		watermark.text= new char[100] {"watermark.text"};
		watermark.fontName = new char[100] {"watermark.fontname"};
		watermark.fontColor = new char[100] {"watermark.color"};
		watermark.repeat = true;
		watermark.rotation = 0x1;
		watermark.fontSize = 100;
		watermark.transparency = 10;

		expiration.type = range;
		expiration.start = 0x12345678;
		expiration.end = 0x87654321;
		

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

private:
	static void login() {
		// get login request cookies;
		NXL_LOGIN_COOKIES* pCookies;
		size_t len_Cookies = 0;
		wchar_t* pURL = NULL;
		auto error = SDWL_Session_GetLoginParams(hSession, &pURL, &pCookies, &len_Cookies);
		if (error) {
			helper_reportError("SDWL_Session_GetLoginCookies", error);
		}
		wchar_t* clientID = NULL;
		for (int i = 0; i < len_Cookies; i++) {
			wchar_t* key = (pCookies + i)->key;
			wchar_t* value = (pCookies + i)->values;
			std::wcout << L"key:" << key << "\t\t" << L"value:" << value << endl;
			if (!_wcsicmp(key, L"clientid")) {
				clientID = value;
			}
		}

		// call python to simulate login
		std::wstring wcmd = L"python rms-login.py -u osmond.ye@nextlabs.com -p 123blue! -o userLgonJson.txt -c ";
		wcmd += clientID;
		system(std::string(wcmd.begin(), wcmd.end()).c_str());

		// python will store user login info in a txt file, load its contents into userLoginStr
		helper_readLoginJson("userLgonJson.txt", userLoginStr);


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

		ASSERT_EQ(error, 0) << "error occurs\n";
	}

private:
	static void helper_readLoginJson(std::string path, std::string& buf) {
		buf.clear();

		std::string fpath;
		char p[255] = { 0 };
		::GetCurrentDirectoryA(255, p);
		fpath.assign(p);
		fpath += "\\" + path;

		std::ifstream ifo = std::ifstream(fpath);
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

	static void save_login_user(std::string email, std::string passcode) {
		cout << "save user session into a file called running_session.txt" << endl;

		{
			ofstream("running_session.txt") << email << "\n" << passcode << endl;
		}
		
	}

	static bool load_login_user(string& email, string& passcode) {
		ifstream ifs("running_session.txt", ios_base::in | ios_base::_Nocreate);
		if (!ifs.good()) {
			return false;
		}

		ifs >> email >> passcode;

		cout << "load_login_user:"
			<< email << passcode
			<< endl;

		return true;
	}
};

std::wstring RMCSKDWrapperTest::workingPath;
std::string RMCSKDWrapperTest::userLoginStr;
HANDLE RMCSKDWrapperTest::hSession;
HANDLE RMCSKDWrapperTest::hTenant;
HANDLE RMCSKDWrapperTest::hUser;
HANDLE RMCSKDWrapperTest::hLocalFiles;
WaterMark RMCSKDWrapperTest::watermark;
Expiration RMCSKDWrapperTest::expiration;


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
	EXPECT_NE(reinterpret_cast<int>(aTenant), NULL);
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

	// once login ok, save User Session infos into a file called runnning_session;
	// do it when test user basic info 
	// save user session into a file called running_session.txt
	
	wstring wstrEmail(email);

	save_login_user(string(wstrEmail.begin(), wstrEmail.end()), code);
	
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

TEST_F(RMCSKDWrapperTest, UserProtectFile) {
	DWORD error = -1;

	std::wstring path(L"D:\\test\\dump.log");
	ASSERT_TRUE(win::file::is_file_exist(path.c_str())) << L"Path not exist" << path;

	const char* t = "{\"a\":\"1.0\",\"b\":\"m39@skydrm.com\",\"c\":\"2018-01-10T09:36:21Z\",\"d\":[{\"a\":0,\"b\":\"Ad-hoc\",\"c\":1,\"d\":[\"DOWNLOAD\",\"VIEW\",\"PRINT\"],\"e\":{\"a\":{\"a\":1,\"b\":\"=\",\"c\":\"application.is_associated_app\",\"d\":true}},\"f\":[]}]}";

	error = SDWL_User_ProtectFile(hUser, (wchar_t*)path.c_str(), 
		adhoc_full_rights, adhoc_full_rights_len, 
		watermark, expiration, (char*)t);

	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_ProtectFile", error);

	wcout << L"OK to Protect the File" << path.c_str() << endl;
}

TEST_F(RMCSKDWrapperTest, UserShareFile) {
	DWORD error = -1;
	
	std::wstring path(L"D:\\test\\dump.log");
	ASSERT_TRUE(win::file::is_file_exist(path.c_str())) << L"Path not exist" << path;

	error = SDWL_User_ShareFile(hUser, (wchar_t*)path.c_str(), 
		adhoc_full_rights, adhoc_full_rights_len,
		(const char**)recipients, 3,
		(wchar_t*)comments, 
		watermark, expiration, 
		(char*)tags);
	
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_ShareFile", error);

	wcout << L"OK to share the File" << path.c_str() << endl;
}

TEST_F(RMCSKDWrapperTest, UserOpenFile) {
	DWORD error = -1;
	// open a local nxl file
	std::wstring path(L"D:\\test\\SkyDRM-2018-05-21-12-32-53.vsd.nxl");
	ASSERT_TRUE(win::file::is_file_exist(path.c_str())) << L"Path not exist" << path;

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
	ASSERT_TRUE(win::file::is_file_exist(path.c_str())) << L"Path not exist" << path;


	error = SDWL_User_OpenFile(hUser, (wchar_t*)path.c_str(), &hNxlFile);

	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_OpenFile", error);
	EXPECT_NE((int)hNxlFile, NULL);

	std::wstring outpath(L"D:\\test\\decrpted-SkyDRM-2018-05-21-12-32-53.vsd");

	error = SDWL_User_DecryptNXLFile(hUser, hNxlFile, (wchar_t*)outpath.c_str());
	EXPECT_EQ(error, 0) << helper_reportError("SDWL_User_DecryptNXLFile", error);
	ASSERT_TRUE(win::file::is_file_exist(outpath.c_str())) << L"Path not exist" << path;
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

  


void dump_projects(HANDLE hUser, ProjtectInfo*& pInfo, int& size) {
	SDWL_User_GetListProjtects(hUser, 1, 1000, (char*)"name", ProjtectFilter::All);

	DWORD rt = SDWL_User_GetProjectsInfo(hUser, &pInfo, &size);

	EXPECT_EQ(rt, 0);

	//dump pInfo
	cout << separate_line << "Dump Projects:\n" << separate_line << endl;
	for (int i = 0; i < size; i++) {
		ProjtectInfo *p = pInfo + i;

		cout << separate_line
			<< "project:\n"
			<< "id:\t" << p->id << "\n"
			<< "name:\t" << p->name << "\n"
			<< "display_name:\t" << p->displayname << "\n"
			<< "description:\t" << p->description << "\n"
			<< "owner:\t" << std::boolalpha << p->owner << "\n"
			<< "total_files:" << p->totalfiles << "\n"
			<< separate_line
			<< endl;
	}
}

void dump_project_files_internal(HANDLE hUser,uint32_t project_id,const char* path_id) {
	ProjectFileInfo * pFiles = NULL;
	uint32_t file_size = NULL;
	auto rt = SDWL_User_ProjectListFiles(hUser, project_id, 1, 1000, (char*)"name", (char*)path_id, (char*)"", &pFiles, &file_size);
	EXPECT_EQ(rt, 0);

	for (size_t i = 0; i < file_size; i++)
	{
		ProjectFileInfo* pp = pFiles + i;

		cout << "id:\t" << pp->id << "\n"
			<< "duid:\t" << pp->duid << "\n"
			<< "display_path:\t" << pp->displayPath << "\n"
			<< "nxl_name:\t" << pp->nxlName << "\n"
			<< "path_id:\t" << pp->pathId << "\n"
			<< endl;

		// once fount path_id end with /, 
		// regar path_id as a foder and call SDWL_User_ProjectListFiles again to list path_id forder contents
		auto predict_root = [](string path) {return path.size() == 1 && path.front() == '/';};
		auto predict_folder_non_root = [predict_root](string path) {return !predict_root(path) && path.back() == '/';};

		if (predict_root(path_id)){
			if (predict_folder_non_root(pp->pathId)) {
				dump_project_files_internal(hUser, project_id, pp->pathId);
			}
		}
		else {
			if (predict_folder_non_root(pp->pathId) && 
				!boost::equals(pp->pathId, path_id) &&
				boost::starts_with(pp->pathId, path_id)) {
				dump_project_files_internal(hUser, project_id, pp->pathId);
			}
		}


		// download the file
		if (!predict_folder_non_root(pp->pathId)) {
			string f(pp->pathId);
			wstring fileName(f.begin(),f.end());
			
			wstring path = RMCSKDWrapperTest::workingPath;
			auto rt=SDWL_User_ProjectDownloadFile(hUser, project_id, fileName.c_str(), path.c_str(), true);
			EXPECT_EQ(rt, 0);
			
		}

	}

}

void dump_project_files(HANDLE hUser, const ProjtectInfo* pInfo) {	
		cout << "======" << pInfo->name << " " << pInfo->totalfiles << "======" << endl;
		dump_project_files_internal(hUser, pInfo->id, "/");
}

TEST_F(RMCSKDWrapperTest, Projects) {

	ProjtectInfo * pInfo = NULL;
	int size = 0;

	dump_projects(hUser, pInfo, size);

	cout << separate_line << "Dump all files in each project:\n" << separate_line << endl;
	for (int i = 0; i < size; i++) {
		dump_project_files(hUser, pInfo + i);


	}
}