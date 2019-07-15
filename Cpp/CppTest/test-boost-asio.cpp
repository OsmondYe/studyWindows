#include "stdafx.h"

#include <boost/bind.hpp>
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
	size_t size;
	shared_buffer(size_t size) : buff(new char[size]), size(size) {

	}
	mutable_buffer asio_buff() const {
		return buffer(buff.get(), size);
	}
};

void print_ec(const boost::system::error_code& ec) {
	if (ec) {

		std::cerr << "Error:" << endl
			<< "msg: " + ec.message() << endl
			<< "code: " << ec.value() << endl
			<< "ec:" << ec << endl;
		

		if (ec == asio::error::eof) {
			cout << "Connection closed" << endl;
		}
	}

}

TEST(BoostAsio, HttpsRmTest) {

	ip::tcp::resolver resolver(myService);  // class-obj must give a service
	ip::tcp::resolver::iterator iter = resolver.resolve({ "rmtest.nextlabs.solutions", "443" });
	ip::tcp::endpoint ep_rmtest = *iter;
	
	
	ip::tcp::socket sock(myService);
	
	sock.connect(ep_rmtest);

	char buf[0x100] = { 0 };

	buf[0] = 0x16; // handshake
	buf[1] = 3; buf[2] = 3; // TLS 1.2


	boost::system::error_code ec;

	sock.write_some(buffer(buf, 100),ec); print_ec(ec);
	sock.write_some(buffer(buf, 100),ec); print_ec(ec);
	sock.write_some(buffer(buf, 100),ec); print_ec(ec);

	sock.read_some(buffer(buf, 100),ec); print_ec(ec);
	

}


TEST(BoostAsio, basic) {
	
}

TEST(BoostAsio, ip) {
	//using boost::asio::ip::address;
	////
	//// address
	////
	//address add = address::from_string("127.0.0.1");
	//// to_string
	//cout << add.to_string() << endl;

	//// Error!!!
	////cout << address::from_string("www.skycode.online") << endl;

	//// ip::host_name 
	//cout << ip::host_name() << endl;

	////
	//// ip-address resolve
	////
	//ip::tcp::resolver resolver(myService);  // class-obj must give a service
	//ip::tcp::resolver::query query("skycode.online","443");
	//ip::tcp::resolver::iterator iter = resolver.resolve(query);
	//ip::tcp::endpoint ep_skycode = *iter;
	//cout << ep_skycode.address().to_string() << endl;

	////
	//// endpoint
	////
	//ip::tcp::endpoint ep(address::from_string("127.0.0.1"),80);

	


}


//using boost::asio::ip::tcp
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;


