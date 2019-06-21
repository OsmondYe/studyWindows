import os
import requests
# 3rd
from bs4 import BeautifulSoup
import pprint



def writetofile(filename,text):
    f=open(filename,'w',encoding="utf-8")
    f.write(text)
    f.close()

def inbookinfo():
    link="https://rms-centos7513.qapf1.qalab01.nextlabs.com:8444/rms/"
    # ignore ssl
    r= requests.get(link,verify=False)
    text=str(r.text)

    soup=BeautifulSoup(text)
    g=soup.prettify()
    # print(g)

    title= soup.find("title")
    print(title.text)


    # finally write it t
    writetofile('rms.html',g)

def baidu():
    key={"wd":"nextlabs"}
    url="http://www.baidu.com/s?"
    r= requests.get(url,key)
    print(r.url)
    print(r.status_code)
    print(r.encoding)
    pp = pprint.PrettyPrinter(indent=0,width=10,depth=2)
    pp.pprint(r.headers)

    bs = BeautifulSoup(r.text)
    # print(bs.prettify())

    writetofile('result.html',bs.prettify())

if __name__ == '__main__':
    # inbookinfo()
    baidu()