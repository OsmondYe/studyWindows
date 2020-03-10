#include "pch.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <nspapi.h>
#include <Svcguid.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")



inline void print_ws_error(int x = ::WSAGetLastError()) {

#define FEM(x) {x,#x}

	std::map<int, string> em{
		FEM(WSA_INVALID_HANDLE),
		FEM(WSA_NOT_ENOUGH_MEMORY),
		FEM(WSA_INVALID_PARAMETER),
		FEM(WSA_OPERATION_ABORTED),
		FEM(WSA_IO_INCOMPLETE),
		FEM(WSA_IO_PENDING),
		FEM(WSAEINTR),
		FEM(WSAEBADF),
		FEM(WSAEACCES),
		FEM(WSAEFAULT),
		FEM(WSAEINVAL),
		FEM(WSAEMFILE),
		FEM(WSAEWOULDBLOCK),
		FEM(WSAEINPROGRESS),
		FEM(WSAEALREADY),
		FEM(WSAENOTSOCK),
		FEM(WSAEDESTADDRREQ),
		FEM(WSAEMSGSIZE),
		FEM(WSAEPROTOTYPE),
		FEM(WSAENOPROTOOPT),
		FEM(WSAEPROTONOSUPPORT),
		FEM(WSAESOCKTNOSUPPORT),
		FEM(WSAEOPNOTSUPP),
		FEM(WSAEPFNOSUPPORT),
		FEM(WSAEAFNOSUPPORT),
		FEM(WSAEADDRINUSE),
		FEM(WSAEADDRNOTAVAIL),
		FEM(WSAENETDOWN),
		FEM(WSAENETUNREACH),
		FEM(WSAENETRESET),
		FEM(WSAECONNABORTED),
		FEM(WSAECONNRESET),
		FEM(WSAENOBUFS),
		FEM(WSAEISCONN),
		FEM(WSAENOTCONN),
		FEM(WSAESHUTDOWN),
		FEM(WSAETOOMANYREFS),
		FEM(WSAETIMEDOUT),
		FEM(WSAECONNREFUSED),
		FEM(WSAELOOP),
		FEM(WSAENAMETOOLONG),
		FEM(WSAEHOSTDOWN),
		FEM(WSAEHOSTUNREACH),
		FEM(WSAENOTEMPTY),
		FEM(WSAEPROCLIM),
		FEM(WSAEUSERS),
		FEM(WSAEDQUOT),
		FEM(WSAESTALE),
		FEM(WSAEREMOTE),
		FEM(WSASYSNOTREADY),
		FEM(WSAVERNOTSUPPORTED),
		FEM(WSANOTINITIALISED),
		FEM(WSAEDISCON),
		FEM(WSAENOMORE),
		FEM(WSAECANCELLED),
		FEM(WSAEINVALIDPROCTABLE),
		FEM(WSAEINVALIDPROVIDER),
		FEM(WSAEPROVIDERFAILEDINIT),
		FEM(WSASYSCALLFAILURE),
		FEM(WSASERVICE_NOT_FOUND),
		FEM(WSATYPE_NOT_FOUND),
		FEM(WSA_E_NO_MORE),
		FEM(WSA_E_CANCELLED),
		FEM(WSAEREFUSED),
		FEM(WSAHOST_NOT_FOUND),
		FEM(WSATRY_AGAIN),
		FEM(WSANO_RECOVERY),
		FEM(WSANO_DATA),
		FEM(WSA_QOS_RECEIVERS),
		FEM(WSA_QOS_SENDERS),
		FEM(WSA_QOS_NO_SENDERS),
		FEM(WSA_QOS_NO_RECEIVERS),
		FEM(WSA_QOS_REQUEST_CONFIRMED),
		FEM(WSA_QOS_ADMISSION_FAILURE),
		FEM(WSA_QOS_POLICY_FAILURE),
		FEM(WSA_QOS_BAD_STYLE),
		FEM(WSA_QOS_BAD_OBJECT),
		FEM(WSA_QOS_TRAFFIC_CTRL_ERROR),
		FEM(WSA_QOS_GENERIC_ERROR),
		FEM(WSA_QOS_ESERVICETYPE),
		FEM(WSA_QOS_EFLOWSPEC),
		FEM(WSA_QOS_EPROVSPECBUF),
		FEM(WSA_QOS_EFILTERSTYLE),
		FEM(WSA_QOS_EFILTERTYPE),
		FEM(WSA_QOS_EFILTERCOUNT),
		FEM(WSA_QOS_EOBJLENGTH),
		FEM(WSA_QOS_EFLOWCOUNT),
		FEM(WSA_QOS_EUNKOWNPSOBJ),
		FEM(WSA_QOS_EPOLICYOBJ),
		FEM(WSA_QOS_EFLOWDESC),
		FEM(WSA_QOS_EPSFLOWSPEC),
		FEM(WSA_QOS_EPSFILTERSPEC),
		FEM(WSA_QOS_ESDMODEOBJ),
		FEM(WSA_QOS_ESHAPERATEOBJ)
	};

	if (em.count(x) == 0) {
		printf("error, %d, unknown", x);
	}
	else {
		printf("error,%d,%s", x, em[x].c_str());
	}

}

void query_address_by_name(const string& name);


void init_win_sock() {
	WSADATA data = { 0 };
	auto rt = WSAStartup(MAKEWORD(2, 2), &data);
	if (rt != 0) {
		cout << "WSAStartup failed:" << rt << endl;
		exit(1);
	}
}

void query_address_by_name(const string& name) {
	init_win_sock();

	struct addrinfo hints { 0 };
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct sockaddr_in* sockaddr_ipv4;
	LPSOCKADDR sockaddr_ip;
	char ipstringbuffer[46];
	DWORD ipbufferlength = 46;


	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	auto rt = getaddrinfo(name.c_str(), NULL, &hints, &result);

	if (rt != 0) {
		printf("getaddrinfo failed with error: %d\n", rt);
		exit(1);
	}

	printf("getaddrinfo returned success\n");

	int i = 1;
	// Retrieve each address and print out the hex bytes
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		printf("getaddrinfo response %d\n", i++);
		printf("\tFlags: 0x%x\n", ptr->ai_flags);
		printf("\tFamily: ");
		switch (ptr->ai_family) {
		case AF_UNSPEC:
			printf("Unspecified\n");
			break;
		case AF_INET:
			printf("AF_INET (IPv4)\n");
			sockaddr_ipv4 = (struct sockaddr_in*) ptr->ai_addr;
			printf("\tIPv4 address %s\n",
				inet_ntoa(sockaddr_ipv4->sin_addr));
			break;
		case AF_INET6:
			printf("AF_INET6 (IPv6)\n");
			// the InetNtop function is available on Windows Vista and later
			// sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
			// printf("\tIPv6 address %s\n",
			//    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );

			// We use WSAAddressToString since it is supported on Windows XP and later
			sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
			// The buffer length is changed by each call to WSAAddresstoString
			// So we need to set it for each iteration through the loop for safety
			ipbufferlength = 46;
			if (WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
				(wchar_t*)ipstringbuffer, &ipbufferlength))
				printf("WSAAddressToString failed with %u\n", WSAGetLastError());
			else
				printf("\tIPv6 address %s\n", ipstringbuffer);
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS (NetBIOS)\n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_family);
			break;
		}
		printf("\tSocket type: ");
		switch (ptr->ai_socktype) {
		case 0:
			printf("Unspecified\n");
			break;
		case SOCK_STREAM:
			printf("SOCK_STREAM (stream)\n");
			break;
		case SOCK_DGRAM:
			printf("SOCK_DGRAM (datagram) \n");
			break;
		case SOCK_RAW:
			printf("SOCK_RAW (raw) \n");
			break;
		case SOCK_RDM:
			printf("SOCK_RDM (reliable message datagram)\n");
			break;
		case SOCK_SEQPACKET:
			printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_socktype);
			break;
		}
		printf("\tProtocol: ");
		switch (ptr->ai_protocol) {
		case 0:
			printf("Unspecified\n");
			break;
		case IPPROTO_TCP:
			printf("IPPROTO_TCP (TCP)\n");
			break;
		case IPPROTO_UDP:
			printf("IPPROTO_UDP (UDP) \n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_protocol);
			break;
		}
		printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
		printf("\tCanonical name: %s\n", ptr->ai_canonname);
	}

	freeaddrinfo(result);

}

void query_host_name() {

	char name[255] = { 0 };
	auto rt = ::gethostname(name, 255);
	if (rt == 0) {
		printf("hostname is %s\n", name);
	}
	else {
		print_ws_error();
		cout << "failed: call gethostname" << endl;
	}
}