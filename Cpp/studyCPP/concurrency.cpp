#include "pch.h"

#include <mutex>
using namespace std;


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