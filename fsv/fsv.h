#ifdef SV_EXPORTS
#define SV_API __declspec(dllexport)
#else
#define SV_API __declspec(dllimport)
#endif



extern "C" SV_API bool _stdcall init_file_virtualization();