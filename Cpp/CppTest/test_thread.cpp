#include "stdafx.h"

HANDLE ht[10];

//typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);
unsigned __stdcall TFun(void* p) {
	auto tid = ::GetCurrentThreadId();
	cout << "current running thread id is: " << tid << endl;
	return 0;
}

// create multi functions
TEST(Win32, Thread_Multi) {
	for (int i = 0; i < 10; i++) {
		unsigned int tid = NULL;
		HANDLE th = (HANDLE)::_beginthreadex(
			NULL,		// no security
			NULL,		// default stack size
			TFun,       // standard thread function
			NULL,		// void function's inpass param
			NULL,		// func's inital control,  0 -> run immediately ,  or ,CREATE_SUSPENDED 
			&tid		// a dword to accept tid;
		);
		cout << "new create thread id is: " << tid << endl;
		ht[i] = th;
	}

	DWORD api_rt;

	api_rt= ::WaitForMultipleObjects(
		10,			// total wait 10 counts
		ht,			// the address of buf[10]
		true,		// wait all to be signaled
		INFINITE	// INFINITE, 
	);	

	if (api_rt == WAIT_FAILED) {
		// CALL API ERROR
		cerr << "faile of call WaitForMultipleObjects" << endl;
	}

	if (api_rt == WAIT_TIMEOUT) {
		// time out
		cerr << "timeout of call WaitForMultipleObjects" << endl;
	}

}