#include "stdafx.h"
#include "utils.h"
#include "SDLAPI.h"

/*
Compile dependencies:
lib:
	Shlwapi.lib
	Dbghelp.lib
	Urlmon.lib
	Bcrypt.lib
	Winhttp.lib
	Wtsapi32.lib
	Mpr.lib
	Ws2_32.lib
	Secur32.lib
	Crypt32.lib
	libeay32.lib
	rmccore.lib
	sdkwrapper.lib
	gtestd.lib
	madCHook64mt.lib
	legacy_stdio_definitions.lib
	SDWRmcLib.lib
*/




namespace {
	wchar_t gRouter[] = L"https://rmtest.nextlabs.solutions";
	//wchar_t gRouter[] = L"https://r.skydrm.com";
	wchar_t gTenant[] = L"skydrm.com";
	//wchar_t gURL[] = L"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms";
}


using namespace std;

class RMCSKDTest : public testing::Test {
public:
	static std::wstring workingPath;
	static std::string userLoginStr;
	static std::wstring cookie_ClientID;
	static std::wstring cookie_PlatformID;
	static SDR_WATERMARK_INFO watermark;
	static SDR_Expiration expiration;
	//
	// SDK ptrs
	//
	static ISDRmcInstance* pIns;
	static ISDRmTenant* pTenant;
	static ISDRmUser *pUser;

protected:
	static void SetUpTestCase() {
		helper_setWorkingFolder(); //set current exe folder		
		helper_setWaterMark();  // set values for watermark and expiration
		helper_setExpiration();		
		// init sdk
		SDWLResult rt;
		rt=SDWLibCreateSDRmcInstance(workingPath.c_str(), workingPath.c_str(), &pIns);
		if (rt.GetCode() == 0) {			
			rt = pIns->Initialize(gRouter, gTenant);
			if (rt.GetCode() == 0) {
				pIns->GetCurrentTenant(&pTenant);
				wcout << L"Tenant ID:" << pTenant->GetTenant() << endl;
				wcout << L"Router URL:" << pTenant->GetRouterURL() << endl;
				wcout << L"RMS URL:" << pTenant->GetRMSURL() << endl;
				wcout << endl;

				rt = pIns->Save(workingPath.c_str());
				ISDRmHttpRequest *pRequest = NULL;
				rt = pIns->GetLoginRequest(&pRequest);
				wcout << L"Login URL:" << pRequest->GetDomainUrl() << endl;
				wcout << L"Login URI:" << pRequest->GetPath() << endl;
				wcout << L"HTTP Method:" << pRequest->GetMethod() << endl;
				wcout << endl;
				HttpCookies cookies = pRequest->GetCookies();

				std::for_each(cookies.begin(), cookies.end(), [&](HttpCookie c) {
					wcout << "Cookie name:" << c.first;
					wcout << "  Vaule:" << c.second << endl;
					if (boost::iequals(c.first, L"clientid")) {
						cookie_ClientID = c.second;
					}
					if (boost::iequals(c.first, L"platformid")) {
						cookie_PlatformID = c.second;
					}
				});	
			}
		}		
		// call python to simulate login
		userLoginStr = helper_simulatLoginByPython(cookie_ClientID);
		// try to login
		rt=pIns->SetLoginResult(userLoginStr, &pUser);
		if (rt.GetCode() == 0) {
			wcout << L"[====ok====]Login OK" << endl;
		}		
	}

	static void helper_setWorkingFolder() {
		wchar_t path[255] = { 0 };
		::GetCurrentDirectoryW(255, path);
		workingPath.assign(path);
		workingPath += L"\\working";

		std::string t(workingPath.begin(), workingPath.end());
		t += "\\";
		::MakeSureDirectoryPathExists(t.c_str());
		std::cout << "working_path:" << t << endl;
	}

	static void helper_setWaterMark() {
		watermark.text = new char[100]{ "watermark.text" };
		watermark.fontName = new char[100]{ "watermark.fontname" };
		watermark.fontColor = new char[100]{ "watermark.color" };
		watermark.repeat = true;
		watermark.rotation = NOROTATION;
		watermark.fontSize = 100;
		watermark.transparency = 10;
	}

	static void helper_setExpiration() {
		expiration.type = RANGEEXPIRE;
		expiration.start = 0x12345678;
		expiration.end = 0x87654321;
	}

	static const std::string helper_simulatLoginByPython(const wstring cliendID) {
		std::wstring wcmd = L"python rms-login.py -u osmond.ye@nextlabs.com -p 123blue! -o userLgonJson.txt -c ";
		wcmd += cliendID;
		system(std::string(wcmd.begin(), wcmd.end()).c_str());

		std::string rt;
		//
		helper_readLoginJson("userLgonJson.txt", rt);

		std::cout << "userLoginStr:\n" << rt << std::endl;

		return rt;

	}

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

};

std::wstring RMCSKDTest::workingPath;
std::string RMCSKDTest::userLoginStr;
 std::wstring RMCSKDTest::cookie_ClientID;
 std::wstring RMCSKDTest::cookie_PlatformID;
SDR_WATERMARK_INFO RMCSKDTest::watermark;
SDR_Expiration RMCSKDTest::expiration;

ISDRmcInstance* RMCSKDTest::pIns = NULL;
ISDRmTenant* RMCSKDTest::pTenant = NULL;
ISDRmUser* RMCSKDTest::pUser=NULL;



TEST_F(RMCSKDTest, Version) {
	DWORD ver = -1;
	ver = SDWLibGetVersion();

	EXPECT_NE(-1, ver);
	// print ver
	char s[40] = { 0 };
	_itoa_s(ver, s, 2);
	printf("SDK_Version:\n\t0x%08X\n\t0b%s\n", ver, s);
}

TEST_F(RMCSKDTest, Protect) {
	SDWLResult rt;
	std::vector<SDRmFileRight> rights{
		RIGHT_EDIT ,
		RIGHT_PRINT,
		RIGHT_CLIPBOARD,
		RIGHT_SAVEAS,
		RIGHT_DECRYPT ,
		RIGHT_SCREENCAPTURE ,
		RIGHT_SEND ,
		RIGHT_CLASSIFY ,
		RIGHT_SHARE ,
		RIGHT_DOWNLOAD,
		RIGHT_WATERMARK
	};

	const char* tags = "{\"a\":\"1.0\",\"b\":\"m39@skydrm.com\",\"c\":\"2018-01-10T09:36:21Z\",\"d\":[{\"a\":0,\"b\":\"Ad-hoc\",\"c\":1,\"d\":[\"DOWNLOAD\",\"VIEW\",\"PRINT\"],\"e\":{\"a\":{\"a\":1,\"b\":\"=\",\"c\":\"application.is_associated_app\",\"d\":true}},\"f\":[]}]}";

	std::wstring path(L"D:\\test\\dump.log");
	ASSERT_TRUE(win::file::is_file_exist(path.c_str())) << L"Path not exist" << path;

	rt=pUser->ProtectFile(path, rights, watermark, expiration, tags);
	EXPECT_EQ(rt.GetCode(), 0);

}