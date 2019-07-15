// dllmain.h : Declaration of module class.

class COfficePluginModule : public ATL::CAtlDllModuleT< COfficePluginModule >
{
public :
	DECLARE_LIBID(LIBID_OfficePluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OFFICEPLUGIN, "{c4d5c990-0bf3-4f85-8e78-6303801664a4}")
};

extern class COfficePluginModule _AtlModule;
