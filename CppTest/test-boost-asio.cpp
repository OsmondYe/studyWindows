#include "stdafx.h"

//basic ns
using namespace std;
using namespace boost;

// boost using asio as a namespace 
using namespace boost::asio;

#pragma region InAsio

// asio::io_service 
boost::asio::io_service myService;
// read read_at read_until

#pragma region IP

// boost::asio::ip  networking part reside.

// address
// endpoing
// three types of socket:  tcp, udp, icmp
// tcp::resolver := from "www.baidu.com" to ip_address

#pragma endregion

#pragma region Error

#pragma endregion

#pragma region SSL

#pragma endregion

#pragma region windows

#pragma endregion


#pragma endregion

#include <boost/shared_array.hpp>

struct shared_buffer {
	boost::shared_array<char> buff;
	int size;
	shared_buffer(size_t size) : buff(new char[size]), size(size) {

	}
	mutable_buffer asio_buff() const {
		return buffer(buff.get(), size);
	}
};


TEST(BoostAsio, basic) {

	string str = "1A";

	char a = str.at(0) - '0';
	char b = str.at(1) - '0';

	char out = ((str.at(0) - '0') << 4) + (str.at(1) - '0');

	cout << "str:" << str << "\t" << "short:" << (int)out << endl;


	
	

	
}

TEST(BoostAsio, ip) {
	using boost::asio::ip::address;
	//
	// address
	//
	address add = address::from_string("127.0.0.1");
	// to_string
	cout << add.to_string() << endl;

	// Error!!!
	//cout << address::from_string("www.skycode.online") << endl;

	// ip::host_name 
	cout << ip::host_name() << endl;

	//
	// ip-address resolve
	//
	ip::tcp::resolver resolver(myService);  // class-obj must give a service
	ip::tcp::resolver::query query("skycode.online","443");
	ip::tcp::resolver::iterator iter = resolver.resolve(query);
	ip::tcp::endpoint ep_skycode = *iter;
	cout << ep_skycode.address().to_string() << endl;

	//
	// endpoint
	//
	ip::tcp::endpoint ep(address::from_string("127.0.0.1"),80);

	


}


//using boost::asio::ip::tcp
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;


