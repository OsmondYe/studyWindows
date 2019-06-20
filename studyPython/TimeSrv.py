from socket import *
from time import ctime

HOST = ''
PROT = 21567
BUFSIZE = 1024
ADDR = (HOST,PROT)

tcpSrvSock=socket(AF_INET,SOCK_STREAM)
tcpSrvSock.bind(ADDR)
tcpSrvSock.listen(5)

while True:
    print("waiting for connection")
    tcpClient,addr = tcpSrvSock.accept()
    print("connect from:")
    print(addr)
    while True:
        data = tcpClient.recv(BUFSIZE)
        if not data:
            break
        tcpClient.send(("[%s]%s"%(ctime(),data)).encode())
        tcpClient.close()

    # tcpSrvSock.close()



