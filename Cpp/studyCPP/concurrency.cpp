#include "pch.h"
// class thread,   namesapce this_thread
#include <thread>	
/*
* class:
	mutex, timed_mutex, recursive_mutex, recursive_timed_mutex
	lock_guard, unique_lock, 
	once_flag
  function:
    try_lock
	lock
	call_once
*/
#include <mutex>					
#include <future>
#include <condition_variable>

/*
直接wrapper win32 API, 施加了访问限制,这样更加安全,可以强制我们写出安全和框架思路良好的代码,而屏蔽底层差异
c11_thread 建模非常漂亮
如何学习到里面的惊奇思路
*/

using namespace std;

TEST(Concurrency, Basic) {
	cout << "thread::hardware_concurrency():" << thread::hardware_concurrency() << endl;
	
	::SYSTEM_INFO si;
	::GetSystemInfo(&si);
	cout << "si.dwNumberOfProcessors:" << si.dwNumberOfProcessors << endl;

	{// tid
		cout << "this_thread::get_id():" << this_thread::get_id() << endl;
		cout << "::GetCurrentThreadId()::" << ::GetCurrentThreadId() << endl;
	}
	cout << "call this_thread::yield(), yield to let other thread to runing"<< endl;
	this_thread::yield();
	cout << "== call ::SwitchToThread()" << endl;
	::SwitchToThread();

	{// sleep [sec]s
		int sec = 2;
		chrono::system_clock::time_point system_start = chrono::system_clock::now();

		printf("sleep %ds\n",sec);
		this_thread::sleep_for(chrono::seconds(sec));
		//this_thread::sleep_for(3s); // same with above

		// post:
		auto diff = chrono::system_clock::now() - system_start;		
		chrono::milliseconds toMilli = chrono::duration_cast<chrono::milliseconds>(diff);
		cout << "Duration is: "<<toMilli.count() << endl;
	}
}

TEST(Concurrency, Threads) {
	// 可以让容器管理线程
	
}


std::mutex gMutex;
std::recursive_mutex gRMutex;


class thread_guard {
	thread& _t;
	thread_guard(const thread_guard& r) = delete;
	thread_guard& operator=(const thread_guard& r) = delete;
public:
	explicit thread_guard(thread& t_) : _t(t_) {}
	~thread_guard() {
		if (_t.joinable()) _t.join();
	}

};


TEST(Concurrency, MustJoinOrDetach) {
	thread t1([]() {
		cout << "hello world" << endl;
	});
	t1.detach();   // i know there is a thread but I dont need th managed it


	thread t2([]() {
		cout << "hello world t2" << endl;
	});
	thread_guard g(t2);
}


void f0() {
	cout << "begin come with: " << hex << this_thread::get_id() << endl;

	std::lock_guard<std::mutex> g(gMutex);
	cout << "end come with: " << hex << this_thread::get_id() << endl;

}

void f1() {

	std::mutex aM;

	std::lock_guard<std::mutex> g(gMutex);
	std::lock_guard<std::mutex> g2(gMutex);
	std::lock_guard<std::mutex> g3(gMutex);
}

void f2() {

	cout << "begin come with: " <<hex<< this_thread::get_id() << endl;

	std::lock_guard<std::recursive_mutex> g(gRMutex);
	std::lock_guard<std::recursive_mutex> g2(gRMutex);
	std::lock_guard<std::recursive_mutex> g3(gRMutex);


	cout << "end come with: " <<hex<< this_thread::get_id() << endl;

}

TEST(Concurrency, Mutex) {
	
	EXPECT_THROW(f1(), std::exception);

	EXPECT_NO_THROW(f2());


	std::list<thread> ll;

	for (size_t i = 0; i < 10; i++)
	{
		ll.push_back(std::thread(f2));
	}

	for (auto& t : ll) {
		t.join();
	}
}