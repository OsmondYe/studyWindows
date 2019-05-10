// CNLOffice.h : Declaration of the CCNLOffice

#pragma once
#include "resource.h"       // main symbols

#include "OfficePlugin_i.h"
#include "msaddndr.tlh"

using namespace ATL;


// CCNLOffice

class ATL_NO_VTABLE CCNLOffice :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCNLOffice, &CLSID_CNLOffice>,
	public IDispatchImpl<ICNLOffice, &IID_ICNLOffice, &LIBID_OfficePluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<_IDTExtensibility2, &__uuidof(_IDTExtensibility2), &LIBID_AddInDesignerObjects, /* wMajor = */ 1>
{
public:
	CCNLOffice()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CCNLOffice)
		COM_INTERFACE_ENTRY(ICNLOffice)
		COM_INTERFACE_ENTRY2(IDispatch, _IDTExtensibility2)
		COM_INTERFACE_ENTRY(_IDTExtensibility2)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:




	// _IDTExtensibility2 Methods
public:
	STDMETHOD(OnConnection)(LPDISPATCH Application, ext_ConnectMode ConnectMode, LPDISPATCH AddInInst, SAFEARRAY * * custom);
	STDMETHOD(OnDisconnection)(ext_DisconnectMode RemoveMode, SAFEARRAY * * custom)
	{
		::OutputDebugString(__FUNCTIONW__ L"\n");
		return E_NOTIMPL;
	}
	STDMETHOD(OnAddInsUpdate)(SAFEARRAY * * custom)
	{
		::OutputDebugString(__FUNCTIONW__ L"\n");
		return E_NOTIMPL;
	}
	STDMETHOD(OnStartupComplete)(SAFEARRAY * * custom)
	{
		::OutputDebugString(__FUNCTIONW__ L"\n");
		return E_NOTIMPL;
	}
	STDMETHOD(OnBeginShutdown)(SAFEARRAY * * custom)
	{
		::OutputDebugString(__FUNCTIONW__ L"\n");
		return E_NOTIMPL;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(CNLOffice), CCNLOffice)
