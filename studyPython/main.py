# basic

# files:  *.py

# print("good")

# x=input("enter ")
# print(x)

# for x in range(10):
#     print(x)

# using def to define a function


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

    print(len(sys.argv))
    print(sys.argv)

    # pybasic.pythonBasic()
    pybasic.pythonBuildin()
    pybasic.pytyhonSequence()
    pybasic.pythonObject()





if __name__ == '__main__':
        main();
