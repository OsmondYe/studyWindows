// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CPPDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CPPDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CPPDLL_EXPORTS
#define CPPDLL_API  __declspec(dllexport)
#else
#define CPPDLL_API __declspec(dllimport)
#endif

#include <string>


// This class is exported from the CppDll.dll
class  CCppDll {
public:
	CCppDll(void);
	// TODO: add your methods here.
	~CCppDll();
public:
	void testMemberFunc1(char a, int b, long c, float d, double e);
	void testMemberFunc2(std::string s1, std::string* ps2, std::string& rs3);
};




extern"C" CPPDLL_API void show();
extern"C" CPPDLL_API void showMsg(const wchar_t* str);
extern"C" CPPDLL_API void showMsgA(const char* str);

extern"C" CPPDLL_API void UntypedParamShow(void* data, int flag);

extern"C" CPPDLL_API void StrReverseA(char* in, char* out);
extern"C" CPPDLL_API void StrReverseW(wchar_t* in, wchar_t* out);

extern"C" CPPDLL_API wchar_t* GetStringFromHeap();
extern"C" CPPDLL_API void FreeStringFromHeap(wchar_t* p);

extern"C" CPPDLL_API int add(int a, int b);
extern"C" CPPDLL_API int sub(int a, int b);