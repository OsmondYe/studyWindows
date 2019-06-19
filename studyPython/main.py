# basic

# files:  *.py

# print("good")

# x=input("enter ")
# print(x)

# for x in range(10):
#     print(x)

# using def to define a function
from random import randint

import amodule

import pybasic

import  sys


def aFunc(x="hello world"):
    return x+x

def fileRead():
    # files
    file = open('main.py', mode='r')
    for eachLine in file:
        print(eachLine)


def main():
    main.__doc__="this is main de doc"
    main.version=0.1
    help(main)
    # pybasic.pythonBasic()
    pybasic.pythonBuildin()
    pybasic.pytyhonSequence()
    pybasic.pythongObject()
    # fileRead()
    # test  write fun
    # amodule.writeTextFile()
    # test  read
    # amodule.readTextFile()

    print(len(sys.argv))
    print(sys.argv)

    print([randint(1,10) for x in range(3)])


    amodule.downloadWeb("http://www.baidu.com")


if __name__ == '__main__':
        main();
