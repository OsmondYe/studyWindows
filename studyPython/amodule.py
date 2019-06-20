import os

ls = os.linesep
import urllib.request

from socket import *


def writeTextFile():
    while True:
        filename = input("get file name:")
        if os.path.exists(filename):
            print("error: %s has already exsits" % filename)
        else:
            break

    # get all liens
    all = []

    print("enter lines , using . to quit")

    while True:
        entry = input(">")
        if entry == '.':
            break
        else:
            all.append(entry)

    # write lines to file
    fobj = open(filename, 'w')
    fobj.writelines(['%s%s' % (x, ls) for x in all])
    fobj.close()

    print("done")


def readTextFile():
    fname = input("get file name for read")
    fobj = open(fname, 'r')

    for line in fobj:
        print(line)

    fobj.close()


def ConnectTimeServer():
    HOST = 'localhost'
    PORT = 21567
    ADDR = (HOST, PORT)
    cs = socket(AF_INET, SOCK_STREAM)
    cs.connect(ADDR)
    cs.send('hello world'.encode())
    data = cs.recv(1024)
    print(data)
    cs.close()


def download_web(http_web):
    x = urllib.request.urlopen(http_web)
    print(x.url)
