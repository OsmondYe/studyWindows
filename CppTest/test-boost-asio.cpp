#include "stdafx.h"

#include <boost\asio\ssl.hpp>
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



enum { max_length = 1024 };

class client
{
public:
	client(boost::asio::io_context& io_context,
		boost::asio::ssl::context& context,
		boost::asio::ip::tcp::resolver::results_type endpoints)
		: socket_(io_context, context)
	{
		socket_.set_verify_mode(boost::asio::ssl::verify_peer);
		socket_.set_verify_callback(
			bind(&client::verify_certificate, this, _1, _2));

		boost::asio::async_connect(socket_.lowest_layer(), endpoints,
			bind(&client::handle_connect, this,
				boost::asio::placeholders::error));
	}

	bool verify_certificate(bool preverified,
		boost::asio::ssl::verify_context& ctx)
	{
		// The verify callback can be used to check whether the certificate that is
		// being presented is valid for the peer. For example, RFC 2818 describes
		// the steps involved in doing this for HTTPS. Consult the OpenSSL
		// documentation for more details. Note that the callback is called once
		// for each certificate in the certificate chain, starting from the root
		// certificate authority.

		// In this example we will simply print the certificate's subject name.
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		std::cout << "Verifying " << subject_name << "\n";

		return preverified;
	}

	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket_.async_handshake(boost::asio::ssl::stream_base::client,
				boost::bind(&client::handle_handshake, this,
					boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Connect failed: " << error.message() << "\n";
		}
	}

	void handle_handshake(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "Enter message: ";
			std::cin.getline(request_, max_length);
			size_t request_length = strlen(request_);

			boost::asio::async_write(socket_,
				boost::asio::buffer(request_, request_length),
				boost::bind(&client::handle_write, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			std::cout << "Handshake failed: " << error.message() << "\n";
		}
	}

	void handle_write(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		if (!error)
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(reply_, bytes_transferred),
				boost::bind(&client::handle_read, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			std::cout << "Write failed: " << error.message() << "\n";
		}
	}

	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		if (!error)
		{
			std::cout << "Reply: ";
			std::cout.write(reply_, bytes_transferred);
			std::cout << "\n";
		}
		else
		{
			std::cout << "Read failed: " << error.message() << "\n";
		}
	}

private:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	char request_[max_length];
	char reply_[max_length];
};


TEST(BoostAsio, HttpsRmTest) {

	io_service& svc = myService;

	ip::tcp::resolver resolver(svc);
	auto it = resolver.resolve({ "rmtest.nextlabs.solutions","443" });
	//auto it = resolver.resolve({ "www.baidu.com","443" });
	cout <<"host:"+ it->host_name()+ " ip:"+ it->endpoint().address().to_string() <<endl ;

	ssl::context ctx(ssl::context::method::sslv23_client);

	client c(svc, ctx, it);

	myService.run();

	//boost::system::error_code ec;
	//ctx.set_verify_mode(ssl::verify_peer);

	//ctx.set_verify_callback(
	//	[](bool preverified,boost::asio::ssl::verify_context& ctx)->bool 
	//{
	//	cout<<"calling here";
	//	return preverified;
	//});

	////ctx.load_verify_file("ca.pem");

	////
	////  connect
	////


	//ssl::stream<ip::tcp::socket> ssock(svc, ctx);
	//ssock.lowest_layer().connect(*it, ec);
	//if (ec) {
	//	cout << ec.message();
	//}

	////
	////  handshake
	////
	//ssock.handshake(ssl::stream_base::handshake_type::client,ec);
	//if (ec) {
	//	cout << ec.message();
	//}

	//// send request
	//char request[] = "GET /rms/intro HTTP/1.1\r\n\r\n";
	//boost::asio::write(ssock, buffer(request),ec);
	//if (ec) {
	//	cout << ec.message();
	//}

	//// read response
	//std::string response;

	//do {
	//	char buf[1024];
	//	size_t bytes_transferred = ssock.read_some(buffer(buf), ec);
	//	if (!ec) response.append(buf, buf + bytes_transferred);
	//} while (!ec);

	//// print and exit
	//std::cout << "Response received: '" << response << "'\n";

}


TEST(BoostAsio, basic) {
	
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


