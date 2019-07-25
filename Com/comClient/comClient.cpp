// comClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <objbase.h>
#include <atlbase.h>
#include "mso2016.tlh"
#include "msword2016.tlh"


using namespace Word2016;
using namespace std;
using namespace ATL;




void t_HResult() {
	// enum all
	for (size_t i = 0; i < 1000; i++) {
		LPSTR pMsgBuf=NULL;

		if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, i,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&pMsgBuf, 0, NULL)) {
			
			cout << "hr:" << i << ", msg:" << pMsgBuf << endl;

			LocalFree(pMsgBuf);
		}
	}
	// good macro
	HRESULT hr = S_OK;
	if (SUCCEEDED(hr) && FAILED(hr)) {

	}
}


/*
 Pure C format for define a guid
*/
// {5C677629-9017-4F35-8DE7-00D600663C52}
DEFINE_GUID(GUID_FOR_PURE_C_FORMAT,
	0x5c677629, 0x9017, 0x4f35, 0x8d, 0xe7, 0x0, 0xd6, 0x0, 0x66, 0x3c, 0x52);

void t_GUID() {
	// IID, GUID, CLSID, UUID   is  the same thing

	// {51C4BD49-BCE0-45E3-A2CD-15ED115E5017}
	GUID myGUID ={ 0x51c4bd49, 0xbce0, 0x45e3, { 0xa2, 0xcd, 0x15, 0xed, 0x11, 0x5e, 0x50, 0x17 } };
	
	auto uuid_MSWord = __uuidof(Application);

	// macros
	IsEqualGUID(myGUID, uuid_MSWord);
	InlineIsEqualGUID(myGUID, uuid_MSWord);

	/* ProgID <Program>.<Component>.<Version>
			Word.Applcation.16
	*/
	CLSID c1,c2,c3;
	::CLSIDFromProgID(L"Word.Application.16", &c1);
	::CLSIDFromProgID(L"Word.Application.15", &c2);
	::CLSIDFromProgID(L"Word.Application", &c3);
	
	if (IsEqualGUID(c1, c2)) {

	}

	//print in str
	wchar_t uuid[0x32];
	::StringFromGUID2(uuid_MSWord, uuid,0x32);
	cout << uuid;

	::CLSIDFromString(L"{5C677629-9017-4F35-8DE7-00D600663C52}", &c3);

	return;
}
int main()
{
	HRESULT hr= ::CoInitialize(NULL);
	cout << hr << endl;

	//t_HResult();
	t_GUID();

	// create office 

	auto uuid_MSWord = __uuidof(Application);
	CComPtr<IUnknown> spApp;

	// must use CLSCTX_LOCAL_SERVER,  or you can not create the object
	cout << spApp.CoCreateInstance(uuid_MSWord,NULL, CLSCTX_LOCAL_SERVER )<< endl;

	CComQIPtr<IDispatch> spApp2(spApp);

	CComQIPtr<_Application> spApp3(spApp);


	spApp3->put_Visible(VARIANT_TRUE);

    std::cout << "Hello World!\n"; 


	spApp3->Quit();

	//test2();
	//spApp->put_Visible(0);

}




