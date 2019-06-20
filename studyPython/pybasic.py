import  sys
from random import randint
# using pass as NULL statement
def pythonPass():
    pass

# this is for python basic study
def pythonBasic():
    # permit multiple set value
    a=b=c=d=1.3
    print("%f%f%f%f"%(a,b,c,d))
    # permit change var types at run time
    a="hello world"
    print(a)

    b=c=1
    c=2
    if(b is a):  # id(b)==id(a)
        print("b is a")
    elif(b is c):
            print("b is c")


    # [list] (tuple) {map}
    print("you are good")
    # x=input("speak something:") # any type
    # print(x);
    print(abs(-4))
    # like printf "format"%(val1,val2)
    print("%s and then a num:%d" % ("string", 123))
    # ** is the power of, like  x^2, x^3
    print(3 ** 3);
    # [] is list
    list = [1, 2, 4, 5]
    print(list)
    print(list[:3])
    print(list[2:3])
    print(list[-1])  # -1 is the last
    print(list[0], list[1])
    # () is tuple
    someTuple = (1, 2.3, "this is a good");
    print(someTuple)
    print(someTuple[2])
    print(someTuple[1] + someTuple[0])
    # {} is dictionary, map
    aMap = {"haha": 123, 5454: "nicacai"}
    print(aMap)
    print(aMap[5454])
    print(aMap["haha"])
    print(aMap.keys())
    print(aMap.values())
    for key in aMap:
        print(key, aMap[key])

    # for is like for_each,  jusing range() to gen []
    for i in range(10):
        print(i, end=",")

    sq = [x ** 2 for x in range(10)]
    for i in sq:
        print(i, end=",")


def pythonBuildin():
    a= "hello world"
    b= "good"
    # id
    print(id(a))
    # type
    print(type(a))
    # repr
    print(repr(a))
    #str  get var's string representation
    print(str(a))


def pytyhonSequence():
    # string, [] ()
    # in not in
    a = "hello world"
    print("%s"%("lo" in a))
    print("%s"%("chine" in a))
    print("%s"%("chine" not in a))

    # [index] [b:e]
    # seq list c++ stl [b:e)
    print("%s"%(a[2]))
    print("%s"%(a[0:3]))

    # seq*num
    print(a*3)
    print(a+a)


    # string
    a= a[:6]+ "python"
    print(a)
    len(a)
    # idnex:  forward  0,1,2,3,4   backward(rever) -1(the last ),-2,-3,-4

    a="China"
    print(a[0])
    print(a[1])
    print(a[-1])
    print(a[-2])
    print(a[1:4])
    print(a[-5:])
    print(a[-5:-1]) # no the last 'a'

    for c in a[0:]:
        print(c)

    print([randint(1,10) for x in range(3)])




class MyFirstClass(object):
    'this is my first class'
#class suite
    # static data member
    MFCVersion=1.0
    MFCfirstStaticMember=100
    #funcitons
    def FistClassFuncton(self):
        print("first class function calling")
        print(self.MFCfirstStaticMember)
        print(self.sales)
    def __init__(self,sales=0.085,rm=0.1,aInt=10,aFloat=1.1):
        print("this is constructor")
        self.sales=0.085
        self.rm=0.1
        self.aInt=10
        self.aFloat=1.1
    def __del__(self):
        print("this is destructor")



# using this to study python object
# object model?  object ( identidy, type value)
# build-in:   id()  type()
def pythonObject():
    MyFirstClass.firstStaticMember=10
    print(MyFirstClass)
    xx=MyFirstClass()   #set a class
    xx.FistClassFuncton()   # calling a class function
    print(MyFirstClass.__dict__)
    print("")
    # range ret it,    stard,end,step
    for x in list(range(0,10,3)):
        print(x)

    t=(123,423432,4543,34)
    it=iter(t)
    # for is using iterator
    for i in it:
        print(i)

    # for line in file  :: iter file's each line

    # lamba   [expr for iter_val in iterable]
    x= [ (i**2) for i in range(4)]
    print(x)




    # using matrix

    xy =[ (x,y) for x in range(10) for y in range(10) ]
    print(xy)




