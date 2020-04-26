#include "pch.h"
#include "helper.hpp"




// override new and delete
struct OyeTrackMemory {

// for release
	void* operator new(size_t size);
	void operator delete(void* p);

	void* operator new(size_t size, void* p);
	void operator delete(void* p, void* place);

// for debug
	void*  operator new(size_t nSize, const wchar_t* lpszFileName, int nLine);
	void  operator delete(void* p, const wchar_t* lpszFileName, int nLine);

};