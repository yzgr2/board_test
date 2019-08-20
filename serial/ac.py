#!/usr/bin/env python
# -*- coding: utf-8 -*

import serial
import serial.tools.list_ports
import time
import thread
import struct

plist = list(serial.tools.list_ports.comports())

if len(plist) <= 0:
    print "The Serial port can't find!"
else:
    plist_0 =list(plist[0])
    serialName = plist_0[0]
    print "Name:",serialName
	
#ser = serial.Serial('COM9', 9600, timeout=5)

def print_6d01_response(str):
	pos =  2 + 1 + 6 + 1 + 2
	
	if( len(str) < pos + 10 ):
		return
		
	print "analyze the response :"
	print "SETTMP:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "SETUPDN:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "SETMDWD:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "NOP:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "WORDA:%x" % struct.unpack('H',str[pos:pos+2] )
	pos += 2
	
	print "SETHUMIDITY:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "SETPERADLR:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "WORDB:%x" % struct.unpack('H',str[pos:pos+2] )
	pos += 2
	
	print "TEMP:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "HUMIDITY:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "OUTTEMP:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "AIRPM:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "ERROR:%x" % struct.unpack('B',str[pos] )
	pos += 1
	
	print "NOP:%x" % struct.unpack('B',str[pos] )
	pos += 1
		

def serial_rx_thread(no, ser):
	while (1):
		str = ser.read(256)
		
		for i in range(0, len(str)-1):
			print "%.2x " % struct.unpack('B',str[i] )
		
		print_6d01_response(str)


frame_query="\xFF\xFF\x0A\x00\x00\x00\x00\x00\x00\x01\x4d\x01\x59"
frame_on="\xFF\xFF\x0A\x00\x00\x00\x00\x00\x00\x01\x4d\x02\x5A"
frame_ESC="\xFF\xFF\x0B\x00\x00\x00\x00\x00\x00\x01\xAA\x55\xFF\x55\x5F"

def serial_cmd(no, ser):
	while (1):	
		prompt = "Choose which frame to send, 1: query frame  \n2.Trun on  \n3. data frame contain FF \n>"
		
		s = raw_input(prompt)
		
		if( s == "1"):
			print ">>TX:"
			ser.write(frame_query)
		
		if( s == "2"):
			print ">>TX:"
			ser.write(frame_on)
		
		if(s == "3"):
			print ">>TX:"
			ser.write(frame_ESC)
		


def test():			
	ser = serial.Serial('COM10', 9600, timeout=5)
			
	thread.start_new_thread(serial_rx_thread, (1,ser))
	thread.start_new_thread(serial_cmd, (1,ser) )
	
	time.sleep(1000*1000)

if __name__== '__main__':  
	test() 




	
