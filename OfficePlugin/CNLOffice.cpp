// CNLOffice.cpp : Implementation of CCNLOffice

#include "stdafx.h"
#include "CNLOffice.h"
#include "importedTLH/mso2016.tlh"
#include "importedTLH/msword2016.tlh"
#include "importedTLH/excel2016.tlh"
#include "importedTLH/msppt2016.tlh"

// CCNLOffice

STDMETHODIMP_(HRESULT __stdcall) CCNLOffice::OnConnection(LPDISPATCH Application, ext_ConnectMode ConnectMode, LPDISPATCH AddInInst, SAFEARRAY ** custom)
{
	::OutputDebugString(__FUNCTIONW__ L"\n");

	ATL::CComQIPtr<IDispatch> sp(Application);	

	ATL::CComQIPtr<Word2016::_Application> spWord;


	HRESULT hr = sp.QueryInterface(&spWord);
	if (SUCCEEDED(hr)) {
		::OutputDebugStringW(L"for word plugin");
		return hr;
	}

	ATL::CComQIPtr<Excel2016::_Application> spExcel;
	hr = sp.QueryInterface(&spExcel);
	if (SUCCEEDED(hr)) {
		::OutputDebugStringW(L"for excel plugin");
		return hr;
	}

	ATL::CComQIPtr<PowerPoint2016::_Application> spPpt;
	hr = sp.QueryInterface(&spPpt);
	if (SUCCEEDED(hr)) {
		::OutputDebugStringW(L"for ppt plugin");
		return hr;
	}

	   
	return E_NOTIMPL;
}
