#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")

namespace {
	class MakeSureInit {
	public:
		MakeSureInit() {
			WSADATA wsaData;
			::WSAStartup(MAKEWORD(2, 2), &wsaData);
		}
		~MakeSureInit() {::WSACleanup();}

		bool PrintLastError() {
			DWORD dwError = ::WSAGetLastError();
			if (dwError != 0) {
				if (dwError == WSAHOST_NOT_FOUND) {
					printf("Host not found\n");
					return 1;
				}
				else if (dwError == WSANO_DATA) {
					printf("No data record found\n");
					return 1;
				}
				else {
					printf("Function failed with error: %ld\n", dwError);
					return 1;
				}
			}
		}

	};

#define WinSockInit()			MakeSureInit msi;
#define WinSockLastError()		msi.PrintLastError();

}

TEST(Socket, CheckIP) {

	WinSockInit();

	char name[255] = { 0 };

	::gethostname(name, 255);

	hostent* hostinfo = ::gethostbyname(name);

	assert(hostinfo->h_addrtype == AF_INET);

	for (int i = 0; i < hostinfo->h_length; i++) {

		struct in_addr* paddr = (struct in_addr*)hostinfo->h_addr_list[i];
		
		if (paddr) {
			printf("ip:%s\n", inet_ntoa(*paddr));
		}
	}
}


TEST(Socket, QueryIP) {

	WinSockInit();

	printf("Enther the host name you want:\n");
	char host[255] = { 0 };
	scanf("%s", host);
	
	printf("host: %s, you input\n", host);


	struct hostent* remoteHost = ::gethostbyname(host);
	if (remoteHost == NULL) {
		WinSockLastError();
		return;
	}
	printf("\tOfficial name: %s\n", remoteHost->h_name);
	int i = 0;
	for (char** pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
		printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
	}

	printf("\tAddress type: ");
	switch (remoteHost->h_addrtype) {
	case AF_INET:
		printf("AF_INET\n");
		break;
	case AF_NETBIOS:
		printf("AF_NETBIOS\n");
		break;
	default:
		printf(" %d\n", remoteHost->h_addrtype);
		break;
	}

	printf("\tAddress length: %d\n", remoteHost->h_length);
	i = 0;
	struct in_addr addr;
	if (remoteHost->h_addrtype == AF_INET)
	{
		while (remoteHost->h_addr_list[i] != 0) {
			addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
			printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
		}
	}

}