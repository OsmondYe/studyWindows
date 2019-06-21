from urllib.request import *



def getfunc():
    f=urlopen(r"http://www.baidu.com")
    print(dir(f))
    print(type(f))
    print(f.info())
    print(f.fileno())
    print(f.geturl())

    # for line in f:
    #     print(str(line))

    x=f.readlines()

    f.close()
