# -*- coding: utf-8 -*-
"""
Created on Sat Feb 14 08:46:15 2015

@author: dev

You need to install dnspython library for this script
"""
import dns.resolver

iplist=list()

for i in range(1,1000):
    req="upload"+str(i)+".files.mail.ru"
    #print(req)
    try:
        res=dns.resolver.query(req,"A")
        for address in res:
            #print address
            iplist.append(address)
    except :
        pass
    
print "Ok"
iplist.sort()
file_out=open("files.mail.ru","w")

for el in iplist:
    print el
    file_out.write(str(el)+"\n")

file_out.flush()
file_out.close()
    
    