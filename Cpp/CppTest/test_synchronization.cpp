#include "stdafx.h"

//
//	Synchronization
//



TEST(Synchronization, InterLock) {

	DWORD ginterlockedInt = 3;

	// 返回 旧值
	// 为什么叫 Exchange  ->  新旧会交换,   新值= 旧+5,  
	auto old_value = ::InterlockedExchangeAdd(&ginterlockedInt, 5);

	/*
	原子指令 lock,   xadd (先交换,再相加)
		lock xadd   dword ptr [rcx],eax    ;

		before:
							rcx  :  address of ginterlockedInt
							eax  :  5;
		after:
							eax = (old) ginterlockedInt
							rcx = eax + 5;
	此函数会返回ginterlockedInt 原先的老值
	*/
	EXPECT_EQ(old_value, 3);

	old_value = ::InterlockedExchangeAdd(&ginterlockedInt, 10);

	EXPECT_EQ(old_value, 8);
	EXPECT_EQ(ginterlockedInt, 18);


	InterlockedIncrement(&ginterlockedInt);  // [safe] ++
	InterlockedDecrement(&ginterlockedInt);  // [safe] --

}


TEST(Synchronization, CriticalSection) {
	/*	用户级别线程同步工具
		经我测试，同一个线程时可以冲入的
		Spin_CS, 我们经常认为关键段里面的代码很短，很快就完成
			因此 如果尝试进入不成功，不要放弃线程，而是一个循环内多次等待，很快就好
	*/
	::CRITICAL_SECTION cs;
	::InitializeCriticalSectionEx(&cs, 0x400, 0);


	::EnterCriticalSection(&cs);
	::EnterCriticalSection(&cs);
	::EnterCriticalSection(&cs);
	::EnterCriticalSection(&cs);

	::LeaveCriticalSection(&cs);
	::LeaveCriticalSection(&cs);
	::LeaveCriticalSection(&cs);
	::LeaveCriticalSection(&cs);


	cout << "before leaving, delete the cs resouce\n";
	::DeleteCriticalSection(&cs);
}



TEST(Synchronization, SRWLock) {
	/*  本进程级别的读写锁，slim应该是轻量级的意思， 读者-写着问题的用户层面实现
		同一线程可以多次获取shared，
		禁止（同线程）：
			- 多次获取写锁
			- 做锁升级-》 先读锁，再加写锁
			- 做锁降级->  先写锁，再加读锁

	*/
	::SRWLOCK lock;
	::InitializeSRWLock(&lock);
	cout << "SRW do not supprot Lock upgrade \n";
	cout << "acqure shared(not release), then to acquire exclusive, dead lock by current thread ifself\n";
	{
		::AcquireSRWLockShared(&lock);
		::AcquireSRWLockShared(&lock);

		::ReleaseSRWLockShared(&lock);
		::ReleaseSRWLockShared(&lock);
		cout << "Can multiple acquire shared\n";
	}
	{
		::AcquireSRWLockExclusive(&lock);
		cout << "Can Not multiple acquire excluesive by same thread\n";
		//::AcquireSRWLockExclusive(&lock);   // dead lock by self-thread
		::ReleaseSRWLockExclusive(&lock);
	}
	{
		cout << "Can not lock upgrade\n";
	}
	{
		//::AcquireSRWLockExclusive(&lock);
		cout << "test lock downgrade\n";
		//::AcquireSRWLockShared(&lock);
		//::ReleaseSRWLockShared(&lock);
	}

	cout << "No lock delete api\n";
}