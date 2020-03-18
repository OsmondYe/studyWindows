// dllmain.h : Declaration of module class.

class CMFCApplicationHandlersModule : public ATL::CAtlDllModuleT<CMFCApplicationHandlersModule>
{
public :
	DECLARE_LIBID(LIBID_MFCApplicationHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MFCAPPLICATIONHANDLERS, "{e0f0666a-17bb-44a7-bebf-4dc66ef188c0}")
};

extern class CMFCApplicationHandlersModule _AtlModule;
