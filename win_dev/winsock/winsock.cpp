// winsock.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <string>
#include <iostream>

using namespace std;
#pragma comment(lib, "ws2_32.lib")


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
    Server(const char* ip, int port) :ip_(ip), port_(port),server_(NULL){}

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
    char sendbuf[32] = "Server: Sending Data.";
    char recvbuf[32] = "";

    bytesRecv = recv(s, recvbuf, 32, 0);
    printf("Server Recv: %s\n", recvbuf);

    bytesSent = send(s, sendbuf, strlen(sendbuf), 0);
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
        char sendbuf[32] = "Client: Sending data.";
        char recvbuf[32] = "";

        bytesSent = send(client_, buf.c_str(), buf.length(), 0);
        printf("Client Sent: %s\n", sendbuf);

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
        string buf;       
        while (cin >> buf) {

        }
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


int main()
{
    // 初始化  wsa
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 1;
    }


    Global.hServer_Semaphore = ::CreateSemaphoreW(
        NULL,           // defualt SA
        0,              // init 0
        1,              // max 1
        L"ServerSignal" // name
    );
    if (Global.hServer_Semaphore == NULL) {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return 1;
    }


//    ::WaitForSingleObject(Global.hServer_Semaphore, INFINITE);



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


    WSACleanup();

}


