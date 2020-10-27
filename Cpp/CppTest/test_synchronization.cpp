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
	{
		::EnterCriticalSection(&cs);::EnterCriticalSection(&cs);
		::EnterCriticalSection(&cs);::EnterCriticalSection(&cs);
		//
		::LeaveCriticalSection(&cs);::LeaveCriticalSection(&cs);
		::LeaveCriticalSection(&cs);::LeaveCriticalSection(&cs);
	}
	::DeleteCriticalSection(&cs);
	std::cout << "before leaving, delete the cs resouce\n";
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



namespace demo_conditionvariable {
	// 类似于<操作系统哲学原理>中提到的 producer 和 consumer 各自等不同的锁,唤醒对方的锁
	// Condition:  not_full   not_empty
	// producer (not-full)   -> if is full, sleep(relase lock), others will signal not_full(re-acqure lock)
	// consumer (not-empty)  -> if is empty, sleep(relase lock), others will signal not_empty(re-acquire lock) 

	const int PRODUCER_SLEEP_TIME_MS = 500;
	const int CONSUMER_SLEEP_TIME_MS = 300;
	const int BUFFER_SIZE = 10;

	class Buffer{
	public:
		int buffer[BUFFER_SIZE];
		long last_item_produced;
		int queue_size;
		int queue_start_offset;

		int total_item_produced;
		int total_item_consumed;

		::CONDITION_VARIABLE cv_buffer_not_empty;	 // consumers
		::CONDITION_VARIABLE cv_buffer_not_full;     // producers
		::CRITICAL_SECTION  cs_buffer_lock;

		Buffer() {
			::InitializeConditionVariable(&cv_buffer_not_empty);
			::InitializeConditionVariable(&cv_buffer_not_full);
			::InitializeCriticalSection(&cs_buffer_lock);
		}
		bool stop_request;

		bool is_stop() {
			return stop_request;
		}

		void set_stop() {
			::EnterCriticalSection(&cs_buffer_lock);
			stop_request = true;
			::LeaveCriticalSection(&cs_buffer_lock);
		}

		void produce_item(int item) {
			buffer[(queue_start_offset + queue_size) % BUFFER_SIZE] = item;
			queue_size++;
			total_item_produced++;
		}

		int consume_item() {
			int item = buffer[queue_start_offset];
			queue_size--;
			queue_start_offset++;
			if (queue_start_offset == BUFFER_SIZE) {
				queue_start_offset = 0;
			}

			total_item_consumed++;
			return item;
		}

	};

	Buffer buf;

	DWORD WINAPI Producer_Proc(PVOID p) {

		int producerID = (int)p;
		while (true) {
			// produce an item
			::Sleep(rand() % PRODUCER_SLEEP_TIME_MS);
			int item = ::InterlockedIncrement(&buf.last_item_produced);


			// let buf as a critical_section
			::EnterCriticalSection(&buf.cs_buffer_lock);
			{
				while (buf.queue_size == BUFFER_SIZE && !buf.is_stop()) {
					// buffer is full,  - sleep so consumer can get items
					::SleepConditionVariableCS(&buf.cv_buffer_not_full, &buf.cs_buffer_lock, INFINITE);
				}

				if (buf.stop_request == true) {
					::LeaveCriticalSection(&buf.cs_buffer_lock);
					break;
				}

				// producing
				buf.produce_item(item);
				printf("Producer %d, item %d, queue size %d \n", producerID, item, buf.queue_size);
			}
			::LeaveCriticalSection(&buf.cs_buffer_lock);

			// if a consumer is waiting, wake it.
			::WakeConditionVariable(&buf.cv_buffer_not_empty);

		}
		printf("Producer %d existing\n", producerID);
		return 0;
	}

	DWORD WINAPI Consumer_Proc(PVOID p) {
		int consumerID = (int)p;
		//  1 cs[get_consume_item], 2 wake_producer, 3 consuem
		while (true)
		{
			::EnterCriticalSection(&buf.cs_buffer_lock);
			{
				// buf可工作,同时size为0,说明需要激活producer了
				while (buf.queue_size == 0 && !buf.is_stop()) {
					::SleepConditionVariableCS(&buf.cv_buffer_not_empty, &buf.cs_buffer_lock, INFINITE);
				}

				if (buf.stop_request == true && buf.queue_size == 0) {
					::LeaveCriticalSection(&buf.cs_buffer_lock);
					break;
				}

				int item = buf.consume_item();

				printf("Consumer %d, item %d, queue size%d\n", consumerID, item, buf.queue_size);
			}
			::LeaveCriticalSection(&buf.cs_buffer_lock);

			// if a producer is waiting, wake it ;
			::WakeConditionVariable(&buf.cv_buffer_not_full);


			// simulate processing of the itme.
			::Sleep(rand() % CONSUMER_SLEEP_TIME_MS);
		}
		return 0;
	}


	void bounded_circular_buffer() {

		DWORD id;
		vector<HANDLE> hs;
		hs.push_back(::CreateThread(NULL, 0, Producer_Proc, (PVOID)1, 0, &id));
		hs.push_back(::CreateThread(NULL, 0, Consumer_Proc, (PVOID)1, 0, &id));
		hs.push_back(::CreateThread(NULL, 0, Consumer_Proc, (PVOID)2, 0, &id));
		
		{
			puts("Press Enter to stop...");
			getchar();
			buf.set_stop();
		}
		{
			::WakeAllConditionVariable(&buf.cv_buffer_not_full);
			::WakeAllConditionVariable(&buf.cv_buffer_not_empty);
		}

		// 
		::WaitForMultipleObjects(hs.size(), hs.data(), TRUE, INFINITE);

		printf("Total item produced %d, TotalItemsConsumed %d\n", buf.total_item_produced, buf.total_item_consumed);
	}

}
// 生产者,消费者
// ConditionVariable, user_mode,  , 
// 
// 线程调用  SleepConditionXXX 会释放已有的资源,然后sleep, 等其他线程 WakeConditonXXX 将其激活, atomically release lock and enter the sleeping state
// 激活后会自动获得已有资源,  after threa is woken, it re-acquires the lock that it release when enter sleeping state.
// ConditionVariable user land 
// 允许线程sleep等待,直到指定资源可用, 等待时会自动释放自己的占有的锁, 被唤醒后会再次占有自己先前释放的锁,全程原子操作
// 搭配 CS 和 SRW 使用
// 类似于monitor,管程,  得不到锁先释放自己的资源,然后sleep, 等别人唤醒
TEST(Synchronization, ConditionVariable) {
	//::InitializeConditionVariable()
	//::SleepConditionVariableCS();
	//::SleepConditionVariableSRW();
	//::WakeConditionVariable();
	//::WakeAllConditionVariable();
	using namespace demo_conditionvariable;

	// copy demo code 
	// https://docs.microsoft.com/en-us/windows/win32/sync/using-condition-variables

	demo_conditionvariable::bounded_circular_buffer();

}

namespace kernelobject {

	//typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);
	unsigned __stdcall thread_function(void* p) {
		auto tid = ::GetCurrentThreadId();
		cout << "current running thread id is: " << tid << endl;

		while (1)
		{
			printf("press any key to kill thread\n");
			getchar();
			::TerminateThread(::GetCurrentThread(), 0);
		}

		return 0;
	}

	HANDLE create_demo_thread() {
		return (HANDLE)::_beginthreadex(0, 0, thread_function, 0, 0, 0);
	}

	unsigned __stdcall thread_file_function(void* p) {
				
		HANDLE h = (HANDLE)p;

		std::wstring buf = L"hello world\n";

		for (int i = 0; i < 100; i++) {
			if (!::WriteFile(h, buf.c_str(), buf.size() * 2, NULL, NULL)) {
				printf("Error to write file\n");
			}
			
		}


		CloseHandle(h);
		printf("in  thread of thread_file_function, waiting for exit\n");
		getchar();

		return 0;
	}

	void create_operation_file_thread_demo(HANDLE hf) {
		::_beginthreadex(NULL, NULL, thread_file_function, hf, 0, 0);
	}
}

TEST(Synchronization, KernelObject) {
	using namespace kernelobject;
	printf("kernel object: 2 state,  signaled and nonsignaled\n");
	printf("nonsignled -> create KernelObject\n");
	printf("signaled ->  closehandle, killtheObject\n");

	// thread:
	{
		printf("for Thread, Terminate or exit will become signaled\n");
		::WaitForSingleObject(create_demo_thread(), INFINITE);
	}

	// file
	{
		HANDLE hf = ::CreateFileW(L"xxx.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_TEMPORARY, NULL);
		create_operation_file_thread_demo(hf);

		if (::WaitForSingleObject(hf, INFINITE) == WAIT_OBJECT_0) {
			printf("Ok to wait for file object, some one call CloseHandle \n");
		}
		else {
			printf("Failed to wait for file object \n");
		}
	}

}


namespace event_demo {

	char shared_buf[255] = { 0 };
	//typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);
	HANDLE hWriter;

	unsigned int reader_proc(void* p) {

		DWORD tid = ::GetCurrentThreadId();
		bool noError = true;

		while (noError) {
			printf("thread:%d is waiting to read\n",tid);
			DWORD dwWaitResult = ::WaitForSingleObject(hWriter, INFINITE);
			switch (dwWaitResult)
			{
			case WAIT_OBJECT_0:
				printf("thread(%d) wait ok, to content\n", tid);
				printf("\t%s\n",shared_buf);
				break;
			case WAIT_FAILED:
				printf("thread(%d) is WAIT_FAILED",tid);
				noError = false;
				break;
			case WAIT_ABANDONED:
				printf("thread(%d) is WAIT_ABANDONED", tid);
				noError = false;
				break;
			case WAIT_TIMEOUT:
				printf("thread(%d) is WAIT_TIMEOUT", tid);
				noError = false;
				break;
			default:
				break;
			}
		}

		return 0;
	}

	void demo() {
		// prepare reader thread;
		HANDLE hReader[3];
		for (int i = 0; i < 3; i++) {
			hReader[i] = (HANDLE)::_beginthreadex(0, 0, reader_proc, 0, 0, 0);
		}

		// prepare writer;
		hWriter = ::CreateEvent(
			NULL,  // null security attribute,
			true,  // manual reset
			false, // no signal at init,
			L"RandomName"
		);

		
		while (1)
		{
			scanf("%s\r\n",shared_buf);
			if (strstr(shared_buf, "quit")) {
				break;
			}
			else {
				::SetEvent(hWriter);
				::ResetEvent(hWriter);
			}
		}
	}
}

TEST(Synchronization, Event) {

	//::CreateEvent();
	//::PulseEvent();
	//::ResetEvent();  // for manual-reset type
	//::SetEvent();
	//::OpenEvent();
	//::CloseHandle();
	
	printf("Notifying a waiting thread of the occurrence of an event.");
	printf("Manual reset, all threads that is waiting for this signal will be signaled\n");
	printf("Auto reset, one of waiting threads will be signaled\n");
	printf("Auto reset type, if one waitfunction return, the type will be set to nonsignaled\n");

	// 例子： 针对Manual reset而言
	// 读取一个文件前，准备好Thread{wordcounts, spellcounts, gramarcheck}
	// 这些线程初始都是死等Event， 文件打开ok后， 直接给发消息，同时激活3个线程各跑各的任务

	printf("Reader-Writer problem using Event\n");
	event_demo::demo();

}

TEST(Synchronization, WaitableTimer) {

}