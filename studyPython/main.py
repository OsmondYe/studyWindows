# sys
import sys
import os
# 3rd

# self
from oye_basic import syntax
import oyeWebUrl
import oyeSqlite3
from oye_scrawler import scrawler


def aFunc(x="hello world"):
    return x + x


def fileRead():
    # files
    file = open('main.py', mode='r')
    for eachLine in file:
        print(eachLine)


def main():
    main.__doc__ = "this is main de doc"
    main.version = 0.1
    help(main)

    print(len(sys.argv))
    print(sys.argv)

    l=["123","4234","hellworld"]
    text ="+=*/".join(l)   # text =  123+=*/4234+=*/hellworld
    print(text)


if __name__ == '__main__':
    main();
