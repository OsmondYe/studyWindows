#include "pch.h"
#include "helper.hpp"


//
// standard 
//
#include <chrono>


/*
   chrone is a sub namesapce  std::chrone
	处理日期和时间, 
	hour, minute, second, millisecond, miscrosecond, nanosecond

	精度中立的时间类

	duration
	timepoint
	tick 
	epoch
	clock 
	timer
	data 
	time

*/



TEST(Chrono, Duration) {
	/*		
		chrono::duration, base unit is second, 
		某个时间单位上一个明确的时间数,基本单位是秒,
			- std::ratio<1,3>  基本单位为1/3 秒
	*/
	chrono::duration<int, std::ratio<1, 3>> d1(1);	// 1 tick of 1/3 second
	cout << d1.count() << endl;		// base unit is 1/3 second
	//using hours        = duration<int, ratio<3600>>;
	std::chrono::hours    hour(20);		// 20h
	//using minutes      = duration<int, ratio<60>>;
	std::chrono::minutes  min(20);		// 20min
	//using seconds      = duration<long long>;
	std::chrono::seconds  sec(20);		// 20s
	//using milliseconds = duration<long long, milli>;  // using milli = ratio<1, 1000>;
	std::chrono::milliseconds  millisec(20);		//20milli,一个milli的单位是1/1000秒

	chrono::duration<double, std::ratio<60>> halfAMin(0.5); // 基本单位为60s, tick是0.5 也就是半分钟,30秒

	cout << min.count() << endl;			// 20
	cout << (min - sec).count() << endl;	// 1180  (s)
	cout << halfAMin.count() << endl;

	// duration_cast
	chrono::minutes myMin = chrono::duration_cast<chrono::hours>(hour);
	cout << myMin.count() << endl;	// 1200
}



std::string asStringGMT(const std::chrono::system_clock::time_point& tp) {
	// convert to system time
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts(std::asctime(std::gmtime(&t)));
	return ts;
}

std::string asStringLocal(const std::chrono::system_clock::time_point& tp) {
	// convert to system time
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts(std::ctime(&t));
	return ts;
}

TEST(Chrono, Clock_TimePoint) {
	/*
		Clock     定义一个epoch(起始点) 和tick周期,
		TimePoint 表现出一个特定的时间点

		c++的三个 clock
			- sytem_clock 
			- steady_clock				保证不会被调整
			- high_resolution_clock 
	*/

	// 判断程序起止
	chrono::system_clock::time_point system_start = chrono::system_clock::now();
	// ---------doing something ---------------
	auto diff = chrono::system_clock::now() - system_start;
	chrono::milliseconds toMilli = chrono::duration_cast<chrono::milliseconds>(diff);
	cout << toMilli.count() << endl;

	cout <<"system_start GMT:" <<asStringGMT(system_start) << endl;
	cout <<"system_start Local:" << asStringLocal(system_start) << endl;

	std::chrono::system_clock::time_point tp;
	cout << "epoch_GMT:" << asStringGMT(tp) << endl;
	cout << "epoch_Local:" << asStringLocal(tp) << endl;

	cout << "let this thread sleep 5s" << endl;
	std::this_thread::sleep_for(chrono::seconds(5));

	cout << "let this thread sleep until 5s later" << endl;
	std::this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(5));
}