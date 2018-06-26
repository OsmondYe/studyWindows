#include "stdafx.h"

#include <boost/algorithm/algorithm.hpp>


#include <madCHook.h>
#include "hook.h"
#include <MsXml6.h>

#include "auto_disable.hpp"

typedef  DWORD (WINAPI *Func_CoCreateInstance)(
				IID &  rclsid,
				LPUNKNOWN pUnkOuter,  
				DWORD     dwClsContext,
				IID &    riid,
				LPVOID    *ppv);

Func_CoCreateInstance NextCoCreateInstance = NULL;

recursion_control rc;

inline std::ostream& operator <<(std::ostream& ss, GUID const& item) {
	OLECHAR* bstrGuid;
	::StringFromCLSID(item, &bstrGuid);
	ss << bstrGuid;
	::CoTaskMemFree(bstrGuid);
	return ss;
}

DWORD WINAPI  myCoCreateInstance(
	IID & rclsid,
	LPUNKNOWN pUnkOuter,
	DWORD dwClsContext,
	IID & riid,
	LPVOID * ppv) 
{
	if (rc.is_disabled()) {
		return NextCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	}

	recursion_control_auto auto_disable(rc);
	
	// begin
	OutputDebugStringA("on MyCoCreateInstance\n");



	HRESULT hr= NextCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);

	if (hr == S_OK) {
		// print all		
		std::ostringstream oss;
		oss << "In myCoCreateInstance rclsid: " << rclsid << std::endl;
		OutputDebugStringA(oss.str().c_str());

		IUnknown * p = *(IUnknown**)ppv;
		IXMLHttpRequest* pxhr = NULL;
		
		p->QueryInterface(IID_IXMLHTTPRequest, (void**)&pxhr);
		if (pxhr != NULL) {
			OutputDebugStringA("we have found the object\n");
		}
		p->QueryInterface(IID_IXMLHTTPRequest2, (void**)&pxhr);
		if (pxhr != NULL) {
			OutputDebugStringA("we have found the object\n");
		}
		p->QueryInterface(IID_IXMLHTTPRequest3, (void**)&pxhr);
		if (pxhr != NULL) {
			OutputDebugStringA("we have found the object\n");
		}
		

		//ED8C108D-4349-11D2-91A4-00C04F7969E8
		//if (IsEqualIID(riid, IID_IXMLHTTPRequest)) {
		//	OutputDebugStringA("this is IID_IXMLHTTPRequest");
		//}
		////E5D37DC0-552A-4D52-9CC0-A14D546FBD04
		//if (IsEqualIID(riid, IID_IXMLHTTPRequest2)) {
		//	OutputDebugStringA("this is IID_IXMLHTTPRequest2");
		//}
		////a1c9feee-0617-4f23-9d58-8961ea43567c
		//if (IsEqualIID(riid, IID_IXMLHTTPRequest3)) {
		//	OutputDebugStringA("this is IID_IXMLHTTPRequest2");
		//}



		
	}

	return hr;

}


typedef DWORD (WINAPI *Func_CXMLHttpRequest_send)(void* This, struct tagVARIANT);

Func_CXMLHttpRequest_send Next_CXMLHttpRequest_send = NULL;


DWORD _stdcall CXMLHttpRequest_send(void* This, struct tagVARIANT aaa) {
	if (rc.is_disabled()) {
		return Next_CXMLHttpRequest_send(This, aaa);
	}

	recursion_control_auto auto_disable(rc);

	// begin
	OutputDebugStringA("on CXMLHttpRequest_send\n");


	BSTR pHenry = ::SysAllocString(L"email=henry.hu%40nextlabs.com&password=c9808cfe7a98acf1088e791b41bdaeed&rememberMe=false");

	if (aaa.bstrVal != NULL) {
		OutputDebugStringW(L"find new post request\n");
		std::wstring str(aaa.bstrVal);
		str += L"\n";
		OutputDebugStringW(str.c_str());

		// repace it as scripte file defined;
		OutputDebugStringW(L"repace it as scripte file defined:\nIn D:\\test\\ScriptForLogin.txt\n");
		std::ifstream ifs("D:\\test\\ScriptForLogin.txt");
		std::istreambuf_iterator<char> beg(ifs), end;
		std::string content(beg, end);
		OutputDebugStringA(content.c_str());
		OutputDebugStringA("\n");

		// ainsi to unicode
		std::wstring wcontent(content.begin(), content.end());

		BSTR pReplcedOne = ::SysAllocString(wcontent.c_str());

		aaa.bstrVal = pReplcedOne;

	}

	return Next_CXMLHttpRequest_send(This, aaa);
}


DLL_FUN_EXPORT bool HookIXmlHttpRequestInterface()
{
	//IXMLHttpRequest
	OutputDebugStringA("============================================================\n");
	OutputDebugStringA("Test Inject dll into target process, Experimental used only\n");
	OutputDebugStringA("============================================================\n");

	OutputDebugStringA("Begin in HookIXmlHttpRequestInterface\n");

	InitializeMadCHook();
	
	//// call cocreateinstance	
	//if (!HookAPI("combase.dll", "CoCreateInstance", myCoCreateInstance, (PVOID*)&NextCoCreateInstance)) {
	//	OutputDebugStringA("failed hook combase!CoCreateInstance");
	//}
	//if (!HookAPI("ole32.dll", "CoCreateInstance", myCoCreateInstance, (PVOID*)&NextCoCreateInstance)) {
	//	OutputDebugStringA("failed hook ole32!CoCreateInstance");
	//}

	// get moudule base of mshtml.dll
	HMODULE hmodule = ::LoadLibraryW(L"MSHTML.dll");
	if (hmodule == NULL) {
		OutputDebugStringW(L"load MSHTML.dll is null");
	}
	DWORD offset = 0x4942d0;
	DWORD add = (DWORD)hmodule + offset;

	HookCode((PVOID)add, CXMLHttpRequest_send,(PVOID*)&Next_CXMLHttpRequest_send);

	// 
	return false;
}
