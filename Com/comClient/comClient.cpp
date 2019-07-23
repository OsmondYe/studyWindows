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



//void test2()
//{
//	CLSID clsid;
//	CLSIDFromProgID(L"Word.Application", &clsid);
//	IDispatch* m_pWApp=NULL;
//
//	CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&m_pWApp);
//
//	if (m_pWApp) {
//	}
//}


int main()
{
	// create office 
	HRESULT hr= ::CoInitialize(NULL);
	cout << hr << endl;

	auto uuid_MSWord = __uuidof(Application);
	CComPtr<IUnknown> spApp;

	cout << spApp.CoCreateInstance(uuid_MSWord,NULL, CLSCTX_LOCAL_SERVER )<< endl;

	CComQIPtr<IDispatch> spApp2(spApp);

	CComQIPtr<_Application> spApp3(spApp);


	spApp3->put_Visible(VARIANT_TRUE);

    std::cout << "Hello World!\n"; 


	spApp3->Quit();

	//test2();
	//spApp->put_Visible(0);

}




