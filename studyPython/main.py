# basic

# files:  *.py

# print("good")

# x=input("enter ")
# print(x)

# for x in range(10):
#     print(x)

# using def to define a function
from amodule import fun1111


def aFunc(x="hello world"):
    return x+x


def main():
    print("you are good")
    #x=input("speak something:") # any type
    #print(x);
    print(abs(-4))
    # like printf "format"%(val1,val2)
    print("%s and then a num:%d"%("string",123))
    # ** is the power of, like  x^2, x^3
    print(3**3);
    # [] is list
    list=[1,2,4,5]
    print(list)
    print(list[:3])
    print(list[2:3])
    print(list[-1])  # -1 is the last
    print(list[0],list[1])
    # () is tuple
    someTuple=(1,2.3,"this is a good");
    print(someTuple)
    print(someTuple[2])
    print(someTuple[1]+someTuple[0])
    # {} is dictionary, map
    aMap={"haha":123 ,  5454:"nicacai" }
    print(aMap)
    print(aMap[5454])
    print(aMap["haha"])
    print(aMap.keys())
    print(aMap.values())
    for key in aMap:
        print(key,aMap[key])

    # for is like for_each,  jusing range() to gen []
    for i in range(10):
        print(i, end="," )

    sq= [x**2 for x in range(10)]
    for i in sq:
        print(i,end="," )

    # files
    file = open('main.py',mode='r')
    for eachLine in file:
        print(eachLine)

    # exception
    # try:
    #     for line in open(input("inter file name:"),'r'):
    #         print(line)
    # except Exception :
    #     print("error")

    # func
    print(aFunc(2))
    print(aFunc("python"))
    print(aFunc([1,2]))
    print(aFunc())

    fun1111()

    print(dir())


main()
