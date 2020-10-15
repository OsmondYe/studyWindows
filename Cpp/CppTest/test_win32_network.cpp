#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")



//typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);

unsigned _stdcall server(void*) {

    printf("Waiting for a client to connect...\n");
    // 初始化
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    // 建立socket
    SOCKET server;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // 绑定socket
    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(27015);

    if (::bind(server, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        printf("bind() failed.\n");
        closesocket(server);
        return 0;
    }

    // 监听 socket
    if (listen(server, 1) == SOCKET_ERROR)
        printf("Error listening on socket.\n");

    // 接受连接
    SOCKET AcceptSocket;

    printf("Waiting for a client to connect...\n");
    while (1) {
        AcceptSocket = SOCKET_ERROR;
        while (AcceptSocket == SOCKET_ERROR) {
            AcceptSocket = accept(server, NULL, NULL);
        }
        printf("Client Connected.\n");
        server = AcceptSocket;
        break;
    }

    // 发送接受数据
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    char sendbuf[32] = "Server: Sending Data.";
    char recvbuf[32] = "";

    bytesRecv = recv(server, recvbuf, 32, 0);
    printf("Bytes Recv: %ld\n", bytesRecv);

    bytesSent = send(server, sendbuf, strlen(sendbuf), 0);
    printf("Bytes Sent: %ld\n", bytesSent);

    return 0;
}

unsigned _stdcall client(void*) {

    // 初始化 Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    // 建立socket socket.
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // 连接到服务器.
    sockaddr_in clientService;

    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(27015);

    if (connect(client, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Failed to connect.\n");
        WSACleanup();
        return 0;
    }

    // 发送并接收数据.
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    char sendbuf[32] = "Client: Sending data.";
    char recvbuf[32] = "";

    bytesSent = send(client, sendbuf, strlen(sendbuf), 0);
    printf("Bytes Sent: %ld\n", bytesSent);

    while (bytesRecv == SOCKET_ERROR) {
        bytesRecv = recv(client, recvbuf, 32, 0);
        if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
            printf("Connection Closed.\n");
            break;
        }
        if (bytesRecv < 0)
            return 0;
        printf("Bytes Recv: %ld\n", bytesRecv);
    }


	return 0;
}

TEST(Winsock2, Basic) {

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

    //
    ::Sleep(5 * 1000);

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
		10,			// total wait 10 counts
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