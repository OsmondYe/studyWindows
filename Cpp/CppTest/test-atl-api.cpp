#include "stdafx.h"

#include <atlbase.h>
#include <atlpath.h>

//use DISABLED_ prefix to filter out 

TEST(ATL, Basic) {

	cout << "atl basic" << endl;

	// get uuid;
	GUID iun = __uuidof(IUnknown);

	EXPECT_TRUE(ATL::InlineIsEqualUnknown(iun));

}


TEST(ATL, ComAllocator) {
	cout << "com require using ::CoTaskMemRealloc(), ::CoTaskMemAlloc(), ::CoTaskMemFree()\n";
}


TEST(ATL, CRBTree) {

	cout << "atl CRBTree" << endl;
	//ATL::cRBtree
}

TEST(ATL, CPath) {
	ATL::CPath path;

}





class CCSLock
{
public:
	CCSLock(CRITICAL_SECTION& cs,bool bInitialLock = true) : m_cs(cs), m_bLocked(false) {
		if (bInitialLock) Lock();
	}

	~CCSLock() throw()
	{
		if (m_bLocked) Unlock();
	}

	void Lock(){
		::EnterCriticalSection(&m_cs);
		m_bLocked = true;
	}

	void Unlock() throw(){
		::LeaveCriticalSection(&m_cs);
		m_bLocked = false;
	}
private:
	CRITICAL_SECTION& m_cs;
	bool m_bLocked;
	// Private to avoid accidental use
	CCSLock(_In_ const CCSLock&) throw();
	CCSLock& operator=(_In_ const CCSLock&) throw();
};

TEST(ATL, CSwrapper) {
	cout << "CCSLock like \n";
}