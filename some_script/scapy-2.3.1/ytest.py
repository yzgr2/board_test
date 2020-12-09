#!/usr/bin/python
import time

from scapy.all import *
dip="222.88.1.99"
payload="A"*1000+"B"*1000

packet=IP(dst=dip,id=12345)/UDP(sport=1500,dport=5007)/payload
 
frags=fragment(packet,fragsize=1000)
 
counter=1
for fragment in frags:
  print "Packet no#"+str(counter)
  print "==================================================="
  fragment.show() #displays each fragment
  counter+=1
  send(fragment)
  
