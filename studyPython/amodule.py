import os
ls = os.linesep
import  urllib.request

def writeTextFile():

    while True:
        filename = input("get file name:")
        if os.path.exists(filename):
            print("error: %s has already exsits"%filename)
        else:
            break

    #get all liens
    all =[]

    print("enter lines , using . to quit")

    while True:
        entry= input(">")
        if entry=='.':
            break
        else:
            all.append(entry)

    # write lines to file
    fobj = open(filename,'w')
    fobj.writelines(['%s%s' %(x,ls) for x in all ])
    fobj.close()

    print("done")

def readTextFile():
    fname=input("get file name for read")
    fobj=open(fname,'r')

    for line in fobj:
        print(line)

    fobj.close()


def downloadWeb(httpweb):
    x=urllib.request.urlopen(httpweb)
    print(x.url)



