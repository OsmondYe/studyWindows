#include "stdafx.h"

//
//	Synchronization
//

// https://docs.microsoft.com/en-us/windows/win32/sync/about-synchronization

// user land
TEST(Synchronization, InterLock) {

	DWORD sInt = 3;

	// 返回 旧值
	// 为什么叫 Exchange  ->  新旧会交换,   新值= 旧+5,  
	auto old_value = ::InterlockedExchangeAdd(&sInt, 5);

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

	old_value = ::InterlockedExchangeAdd(&sInt, 10);

	EXPECT_EQ(old_value, 8);
	EXPECT_EQ(sInt, 18);


	InterlockedIncrement(&sInt);  // [safe] ++
	InterlockedDecrement(&sInt);  // [safe] --

}

// user land
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


// user land
TEST(Synchronization, SRWLock) {
	//::InitializeSRWLock(&lock);
	//::Aquire/ReleaseSRWLockExclusive/Shared();
	//::TryAcquireSRWLockExclusive/Shared();   // 尝试获取,成功拿到,失败为0,但不阻塞


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

// user land 
// 允许线程sleep等待,直到指定资源可用, 等待时会自动释放自己的占有的锁, 被唤醒后会再次占有自己先前释放的锁,全程原子操作
// 搭配 CS 和 SRW 使用
// 类似于monitor,管程,  得不到锁先释放自己的资源,然后sleep, 等别人唤醒

namespace demo_conditionvariable {
	// 类似于<操作系统哲学原理>中提到的 producer 和 consumer 各自等不同的锁,唤醒对方的锁
}

TEST(Synchronization, ConditionVariable) {
	//::InitializeConditionVariable()
	//::SleepConditionVariableCS();
	//::SleepConditionVariableSRW();
	//::WakeConditionVariable();
	//::WakeAllConditionVariable();
	using namespace demo_conditionvariable;


}