#include "stdafx.h"
#include <Windows.h>
#include <shellapi.h>
#include <dbghelp.h>
#include "sdkwrapper.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;


wchar_t gRouter[] = L"https://rmtest.nextlabs.solutions";
wchar_t gTenant[] = L"skydrm.com";
wchar_t gURL[] = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms";

bool bThrowIfError = false;



class RMCSDK_Test {
public:
	DWORD error;
	DWORD _version;
	std::wstring _workingPath;
	std::string _userLoginStr;
	HANDLE _hSession;
	HANDLE _hTenant;
	HANDLE _hUser;
	HANDLE _hLocalFiles;
public:

	RMCSDK_Test()
	{
		error = -1;
		_version = 0;
		_hSession = NULL;
		_hTenant = NULL;
		_hUser = NULL;
		_hLocalFiles = NULL;
		// set current exe folder
		wchar_t path[255] = { 0 };
		::GetCurrentDirectoryW(255, path);
		_workingPath.assign(path);
		_workingPath += L"\\working";

		std::string t(_workingPath.begin(), _workingPath.end());
		t += "\\";
		::MakeSureDirectoryPathExists(t.c_str());




	}
	void PrepareEnv() {
		helper_readLoginJson("userJson.txt", this->_userLoginStr);
	}

	void T_Version() {
		// test GetSDKVersion
		_version = GetSDKVersion();
		helper_PrintVersion();
	}

	void T_CreateSDK() {
		// test CreateSDKSession
		// Create SDK			
		error = CreateSDKSession((wchar_t*)_workingPath.c_str(), &_hSession);
		if (error || _hSession == NULL) {
			helper_reportErrorAndThrow("Error,CreateSDKSession", error);
		}
		else {
			cout << "hSession: 0x" << hex << _hSession << endl;
		};
	}

	void T_InitSession() {
		// test Session
		error = SDWL_Session_Initialize(_hSession, gRouter, gTenant);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Session_Initialize", error);
		}
		// Session Init2
		error = SDWL_Session_Initialize2(_hSession, (wchar_t*)_workingPath.c_str(), gRouter, gTenant);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Session_Initialize2", error);
		}
	}

	void T_Tenant() {
		// test Tenant
		error = SDWL_Session_GetCurrentTenant(_hSession, &_hTenant);
		if (error || _hTenant == NULL) {
			helper_reportErrorAndThrow("SDWL_Session_GetCurrentTenant", error);
		}
		else {
			cout << "hTenant: 0x" << hex << _hTenant << endl;
		}
		// fetch tenant related res
		error = SDWL_Session_GetCurrentTenant(_hSession, &_hTenant);
		if (error || _hTenant == NULL) {
			helper_reportErrorAndThrow("SDWL_Session_GetCurrentTenant", error);
		}
		wchar_t aTenant[255] = { 0 };
		error = SDWL_Tenant_GetTenant(_hTenant, aTenant);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Tenant_GetTenant", error);
		}
		else {
			wcout << L"Tenant:" << aTenant << endl;
		}

		wchar_t aRouter[255] = { 0 };
		error = SDWL_Tenant_GetRouterURL(_hTenant, aRouter);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Tenant_GetRouterURL", error);
		}
		else {
			wcout << L"Router:" << aRouter << endl;
		}
		wchar_t aRMS[255] = { 0 };
		error = SDWL_Tenant_GetRMSURL(_hTenant, aRMS);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Tenant_GetRMSURL", error);
		}
		else {
			wcout << L"RMS:" << aRMS << endl;
		}
		error = SDWL_Tenant_ReleaseTenant(_hTenant);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Tenant_ReleaseTenant", error);
		}
		else {
			_hTenant = NULL;
		}
	}

	void T_DeleteSDK() {
		error = DeleteSDKSession(this->_hSession);
		if (error) {
			helper_reportErrorAndThrow("Error,DeleteSDKSession", error);
		}
		_hSession = NULL;
	}

	void T_SaveSession() {
		error = SDWL_Session_SaveSession(_hSession, (wchar_t*)_workingPath.c_str());
		if (error) {
			helper_reportErrorAndThrow("SDWL_Session_SaveSession", error);
		}
	}

	void T_User() {
		// Test for User Login	
		error = SDWL_Session_SetLoginRequest(_hSession, (char*)_userLoginStr.c_str(), &_hUser);
		if (error) {
			helper_reportErrorAndThrow("SDWL_Session_SetLoginRequest", error);
		}
		else {
			cout << "hUser:0x" << hex << _hUser << endl;
		}

		// for user
		auto name = std::wstring(255, 0);
		error = SDWL_User_GetUserName(_hUser, (wchar_t*)name.data());
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetUserName", error);
		}
		else {

			name = name.substr(0, name.find_first_of(L'\0'));
		}
		auto email = std::wstring(255, 0);
		error = SDWL_User_GetUserEmail(_hUser, (wchar_t*)email.data());
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetUserEmail", error);

		}
		else {
			email = email.substr(0, email.find_first_of(L'\0'));

		}
		int type = -1;
		error = SDWL_User_GetUserType(_hUser, &type);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetUserType", error);

		}
		else {
			wcout << L"name:" << name << L"email:" << email << "type" << type << endl;

		}

		// passcode
		auto passcode = std::string(255, 0);
		error = SDWL_User_GetPasscode(_hUser, (char*)passcode.data());
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetPasscode", error);
		}
		else {
			passcode = passcode.substr(0, passcode.find_first_of('\0'));
			cout << "passcode:" << passcode << endl;
		}


		// update user info
		error = SDWL_User_UpdateUserInfo(_hUser);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_UpdateUserInfo", error);

		}
		// update mydrive info
		error = SDWL_User_UpdateMyDriveInfo(_hUser);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_UpdateMyDriveInfo", error);

		}
		// get mydrive info
		DWORD64 u, t;
		error = SDWL_User_GetMyDriveInfo(_hUser, &u, &t);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetMyDriveInfo", error);
		}
		else {
			cout << "My Drive Info:" << "Used: 0x" << hex << u << '\t' << "Total: 0x" << hex << t << endl;
		}

		// open
		//T_OpenFile();

		// protect file
		T_ProtectShareFile();




		// get local files
		error = SDWL_User_GetLocalFile(_hUser, &_hLocalFiles);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_GetLocalFile", error);
		}
		else {
			T_TestLocalFile();
		}

	}

	void T_TestLocalFile() {
		int size = -1;
		error = SDWL_File_GetListNumber(_hLocalFiles, &size);
		if (error) {
			helper_reportErrorAndThrow("SDWL_File_GetListNumber", error);
		}
		else {
			wcout << L"SDWL_File_GetListNumber:" << size << endl;
		}

		wchar_t ** pStr = NULL;
		int pSize = -1;
		error = SDWL_File_GetList(_hLocalFiles, pStr, &pSize);
		if (error) {
			helper_reportErrorAndThrow("SDWL_File_GetList", error);
		}
		else {
			wcout << "Files:\t pSize=" << pSize << endl;
		}

		wchar_t** p = new wchar_t*[pSize];

		error = SDWL_File_GetList(_hLocalFiles, p, &pSize);




	}

	void T_OpenFile() {
		// open a local nxl file
		std::wstring path(L"D:\\test\\SkyDRM-2018-05-21-12-32-53.vsd.nxl");

		HANDLE hNxlFile = NULL;

		error = SDWL_User_OpenFile(_hUser, (wchar_t*)path.c_str(), &hNxlFile);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_OpenFile", error);

		}

	}


	void T_ProtectShareFile() {
		std::wstring testProFile(L"D:\\test\\SkyDRM.vsd");
		int rights[] = { 0,1,2,3,4,5,6,7,8,9,10 };
		error = SDWL_User_ProtectFile(_hUser, (wchar_t*)testProFile.c_str(), rights, 11);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_ProtectFile", error);
		}
		else {
			wcout << L"OK to Protect the File" << testProFile.c_str() << endl;
		}

		// share file
		const char* recipients[] = {
			"henry.hu@nextlabs.com",
			"jach.zhou@nextlabs.com",
			"allen.ning@nextlabs.com"
		};

		const wchar_t* comments = L"this is a comments";

		error = SDWL_User_ShareFile(_hUser, (wchar_t*)testProFile.c_str(), rights, 11, (char**)recipients, 3, (wchar_t*)comments);

		if (error) {
			helper_reportErrorAndThrow("SDWL_User_ShareFile", error);

		}
		else {
			wcout << L"OK to share the File" << testProFile.c_str() << endl;

		}

		//

		error = SDWL_User_UploadActivityLogs(_hUser);
		if (error) {
			helper_reportErrorAndThrow("SDWL_User_UploadActivityLogs", error);
		}
		



	}

	void T_All() {
		try {
			T_Version();

			T_CreateSDK();

			T_InitSession();

			T_Tenant();

			T_User();

			// be test in T_User();
			//T_ProtectShareFile();

			// be test in T_User();
			//T_TestLocalFile();

			T_SaveSession();

			T_DeleteSDK();

		}
		catch (...) {
			// ignore all
			cerr << "Exception Occured" << endl;
		}
	}

private:
	void helper_PrintVersion() {
		char s[40] = { 0 };
		_itoa_s(_version, s, 2);
		printf("SDK_Version:\n\t0x%08X\n\t0b%s\n", _version, s);
	}

	void helper_reportErrorAndThrow(const char* msg, int e) {

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
			cerr << "Error:" << msg << "\tCode:" << en << "," << e << endl;

		}
		else {
			CONSOLE_SCREEN_BUFFER_INFO scbi = { 0 };
			::GetConsoleScreenBufferInfo(hConsole, &scbi);
			::SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_RED);
			cerr << "Error:" << msg << "\tCode:" << en << "," << e << endl;
			//restore
			::SetConsoleTextAttribute(hConsole, scbi.wAttributes);

		}


		if (bThrowIfError)
			throw new std::exception();
	}

	void helper_readLoginJson(std::string path, std::string& buf) {
		buf.clear();

		std::ifstream ifo = std::ifstream("userJson.txt");
		if (!ifo) {
			return;
		}

		buf.append(std::istreambuf_iterator<char>(ifo), std::istreambuf_iterator<char>());
	}

};


void testRMCSDK_wrapper() {

	RMCSDK_Test t;
	t.PrepareEnv();
	t.T_All();

}