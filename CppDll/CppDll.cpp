// CppDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CppDll.h"
#include <Windows.h>

#include <algorithm>




extern"C"
CPPDLL_API void show()
{
	::MessageBox(NULL, L"test", L"test", 0);
}
CPPDLL_API void showMsg(const wchar_t* str)
{
	::MessageBoxW(NULL, str, str, 0);
}
CPPDLL_API void showMsgA(const char * str)
{
	::MessageBoxA(NULL, str, str, 0);
}
CPPDLL_API void UntypedParamShow(void * data, int flag)
{
	if (1 == flag) {
		int a = *(int*)data;
		char buf[10] = { 0 };
		_itoa_s(a, buf, 10);
		::MessageBoxA(NULL, buf, buf, 0);
	}
	else if (2 == flag) {
		char* buf = (char*)data;
		::MessageBoxA(NULL, buf, buf, 0);
	}
}

extern"C"
CPPDLL_API void StrReverseW(wchar_t * in, wchar_t * out)
{
	std::wstring p = std::wstring(in);
	
	std::reverse(p.begin(), p.end());

	wmemcpy(out, p.c_str(), p.length());
	out[p.length()] = L'\0';
}

CPPDLL_API wchar_t * GetStringFromHeap()
{
	std::wstring str = std::wstring(L"The quick brown fox jumps over the lazy dog.");

	wchar_t* ret = new wchar_t[str.length() + 1]{ 0 };

	size_t pos = 0;
	std::for_each(str.begin(), str.end(), [=,&pos](wchar_t c) {ret[pos++] = c; });

	return ret;



}

CPPDLL_API void FreeStringFromHeap(wchar_t * p)
{
	if (p != NULL) {
		delete[] p;
	}
}

extern"C"
CPPDLL_API void StrReverseA(char * in, char * out)
{
	std::string p = std::string(in);
	std::reverse(p.begin(), p.end());
	memcpy(out, p.c_str(), p.length());
	out[p.length()] = '\0';
}
CPPDLL_API int add(int a, int b)
{
	return a+b;
}

extern"C" CPPDLL_API int sub(int a, int b)
{
	return a-b;
}

// This is the constructor of a class that has been exported.
// see CppDll.h for the class definition
CCppDll::CCppDll()
{
    return;
}

CCppDll::~CCppDll()
{
}

void CCppDll::testMemberFunc1(char a, int b, long c, float d, double e)
{
}

void CCppDll::testMemberFunc2(std::string s1, std::string * ps2, std::string & rs3)
{
}
