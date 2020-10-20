#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")

/*
* references:
*   https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
*   
*/

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
            else {
                printf("WSA No error\n");
            }
		}

	};

#define WinSockInit()			MakeSureInit msi;
#define WinSockCheckError()		msi.PrintLastError();

}


TEST(Socket, ByteOrder) {
    printf("sizeof(u_short):%d \t sizeof(u_long):%d\n", sizeof(u_short), sizeof(u_long));

    u_short n = 0xabcd;
    u_short n2 = htons(n);  // return  (n<<8) | (n>>8);
    u_short n3 = (n << 8) | (n >> 8);
    printf("n:%x,\tn2:%x,\tn3:%x\n", n, n2,n3);

    u_long x = 0x89abcdef;
    u_long x2 = htonl(x);  
    u_long x3 = (x << 24) | ((x << 8)&0x00FF0000) | ( (x>>8)&0x0000FF00  ) | (x>>24)   ;
    printf("x:%x,\tx2:%x,\tx3:%x\n", x, x2,x3);


    long ip = inet_addr("100.200.50.255");
    u_char* pc = (u_char*)&ip;
    printf("%u.%u.%u.%u", pc[0], pc[1], pc[2], pc[3]);

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
		WinSockCheckError();
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

TEST(Socket, Connect) {
    int rt=0;
    cout << "connect to baidu.com:80" << endl;
    WinSockInit();

    SOCKET cBaidu = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (cBaidu == INVALID_SOCKET) {
       WinSockCheckError();
       return;
    }

    // pre pare baidu's address info
    sockaddr_in baidu_address = { 0 };
    baidu_address.sin_family = AF_INET;
    baidu_address.sin_port = htons(80);
    baidu_address.sin_addr.S_un.S_addr = inet_addr("180.101.49.11");

    if (SOCKET_ERROR == ::connect(cBaidu, (sockaddr*)&baidu_address, sizeof(baidu_address))) {
        WinSockCheckError();
        return;
    }


    string str = "GET / HTTP/1.0\r\n\r\n";

    if (SOCKET_ERROR == (rt = ::send(cBaidu, str.c_str(), str.length(), 0))) {
        WinSockCheckError();
        return;
    }
    else {
        printf("Send %d bytes\n", rt);
    }

    if (SOCKET_ERROR == ::shutdown(cBaidu, SD_SEND)) {  // 不再发送
        WinSockCheckError();
        return;
    }

    string receive;
    int total = 0;
    while (1) {
        char buf[255] = { 0 };
        if (SOCKET_ERROR == (rt= ::recv(cBaidu, buf, 255, 0))) {
            WinSockCheckError();
            break;
        }
        else if (rt == 0) {
            // server has shutdown
            printf("rt == 0 in ::recv --> the connect has been gracefully closed, data has finished the transportation\n");
            break;
        }
        else {
            total += rt;
            receive.append(buf, rt);
        }
    }

    if (SOCKET_ERROR == ::shutdown(cBaidu, SD_RECEIVE)) {  // 不再接受
        WinSockCheckError();
        return;
    }

    printf("total received %d from server\n\n", total);
    printf(receive.c_str());

    if (SOCKET_ERROR == ::closesocket(cBaidu)) {
        WinSockCheckError();
        return;
    }
}


namespace {
	// 确保 服务器起来后,在运行客户线程

	struct {
		// init 0 , max 1,  release +1, wait -1
		HANDLE hServer_Semaphore;       // init no, if server up, signal it
	}Global;


    //typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);
    typedef void(_stdcall* service_fun)(SOCKET s);

    class Server {
    private:
        string ip_;
        int port_;
        SOCKET server_;
    public:
        Server(const char* ip, int port) :ip_(ip), port_(port), server_(NULL) {}

        bool init() {
            // create tcp server
            server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (server_ == INVALID_SOCKET) {
                printf("Error at socket(): %ld\n", WSAGetLastError());
                return 0;
            }
            //bind
            sockaddr_in service;
            service.sin_family = AF_INET;
            service.sin_addr.s_addr = inet_addr(ip_.c_str());
            service.sin_port = htons(port_);

            if (::bind(server_, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
                printf("bind() failed.\n");
                closesocket(server_);
                return 0;
            }

            // 监听 socket
            if (listen(server_, 1) == SOCKET_ERROR) {
                printf("Error listening on socket.\n");
                return 0;
            }
        }

        void serving(service_fun callback) {
            printf("Server: Waiting for a client to connect...\n");


            SOCKET AcceptSocket;
            while (1) {
                AcceptSocket = SOCKET_ERROR;
                while (AcceptSocket == SOCKET_ERROR) {
                    AcceptSocket = accept(server_, NULL, NULL);
                }
                printf("Client Connected.\n");
                //server_ = AcceptSocket;
                callback(AcceptSocket);
                break;
            }

        }
    };



    void __stdcall  simple_echo_service(SOCKET s) {
        // 发送接受数据
        int bytesSent;
        int bytesRecv = SOCKET_ERROR;
        char recvbuf[32] = "";

        bytesRecv = recv(s, recvbuf, 32, 0);
        printf("Server Recv: %s\n", recvbuf);

        bytesSent = send(s, recvbuf, strlen(recvbuf), 0);
        printf("Server Sent: %s\n", recvbuf);
    }


    unsigned _stdcall server(void*) {

        Server s("127.0.0.1", 27015);

        s.init();
        // tell client thread, you can go now
        ::ReleaseSemaphore(Global.hServer_Semaphore, 1, NULL);

        s.serving(simple_echo_service);

        return 0;
    }



    class Client {
    private:
        string ip_;
        int port_;
        SOCKET client_;

    public:
        Client(const char* ip, int port) : ip_(ip), port_(port), client_(NULL) {}

        bool init() {
            // create socket
            client_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (client_ == INVALID_SOCKET) {
                printf("Error at socket(): %ld\n", WSAGetLastError());
                return 0;
            }

            // 连接到服务器.
            sockaddr_in clientService;

            clientService.sin_family = AF_INET;
            clientService.sin_addr.s_addr = inet_addr(ip_.c_str());
            clientService.sin_port = htons(port_);

            if (connect(client_, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
                printf("Failed to connect.\n");
                return 0;
            }
        }

        bool doing(string& buf) {
            // 发送并接收数据.
            int bytesSent;
            int bytesRecv = SOCKET_ERROR;
            char recvbuf[32] = "";

            bytesSent = send(client_, buf.c_str(), buf.length(), 0);
            printf("Client Sent: %s\n", buf.c_str());

            while (bytesRecv == SOCKET_ERROR) {
                bytesRecv = recv(client_, recvbuf, 32, 0);
                if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
                    printf("Connection Closed.\n");
                    break;
                }
                if (bytesRecv < 0)
                    return 0;
                printf("Client Recv: %s\n", recvbuf);
            }
        }

    };


    unsigned _stdcall client(void*) {
        // wait for semaphore can be used
        ::WaitForSingleObject(Global.hServer_Semaphore, INFINITE);

        Client c("127.0.0.1", 27015);
        c.init();

        // get form console
        while (1) {
            cin.clear();
            string buf;
            std::getline(cin, buf);
            if (buf == "quit") {
                printf("client: quit\n");
                break;
            }
            else {
                c.doing(buf);
            }
        }

        return 0;
    }


}

TEST(Socket, SimpleCS) {

    WinSockInit();


    Global.hServer_Semaphore = ::CreateSemaphoreW(
        NULL,           // defualt SA
        0,              // init 0
        1,              // max 1
        L"ServerSignal" // name
    );
    if (Global.hServer_Semaphore == NULL) {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return ;
    }

    HANDLE h[2];

    // server
    {
        unsigned int tid = NULL;
        HANDLE th = (HANDLE)::_beginthreadex(
            NULL,		// no security
            NULL,		// default stack size
            server,       // standard thread function
            NULL,		// void function's inpass param
            NULL,		// func's inital control,  0 -> run immediately ,  or ,CREATE_SUSPENDED 
            &tid		// a dword to accept tid;
        );
        h[0] = th;
    }


    // client
    {
        unsigned int tid = NULL;
        HANDLE th = (HANDLE)::_beginthreadex(
            NULL,		// no security
            NULL,		// default stack size
            client,       // standard thread function
            NULL,		// void function's inpass param
            NULL,		// func's inital control,  0 -> run immediately ,  or ,CREATE_SUSPENDED 
            &tid		// a dword to accept tid;
        );
        h[1] = th;
    }


    DWORD api_rt = ::WaitForMultipleObjects(
        2,			// total wait 10 counts
        h,			// the address of buf[10]
        true,		// wait all to be signaled
        INFINITE	// INFINITE, 
    );

    if (api_rt == WAIT_FAILED) {
        // CALL API ERROR
        cerr << "faile of call WaitForMultipleObjects" << endl;
    }

    if (api_rt == WAIT_TIMEOUT) {
        // time out
        cerr << "timeout of call WaitForMultipleObjects" << endl;
    }

}