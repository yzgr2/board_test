#!/usr/bin/env python

import sys
import string
import os

def get_k_addr():
	cmd = 'grep --binary-files=text "NMI Watchdog interrupt" -A 30 '
	cmd += sys.argv[1]
	cmd += ' > msg.txt'
	print(cmd)
	os.system(cmd)
				
def format_addr():
	start_rec=0	
	fp=open("./msg.txt","r")
	wfp=open("./addr.txt","w")
	for line in fp.readlines():
		#skip 2 lines,
		if( start_rec > 0  and string.find(line, "SP")<>0 ):
			item = string.split(line, ",")
			for addr in item:
				if( string.find(addr,"R2_BACKUP_STACK_END") <>0):
					#print(addr)
					wfp.write(addr+"\r\n")

		if string.find(line,"R2_BACKUP_STACK_START") >=0:
			start_rec=1
		if string.find(line,"R2_BACKUP_STACK_END") >=0 :
			start_rec=0
			wfp.write("\r\n-----------another core---------")

		

def main():
	if len(sys.argv)<3:
		print("usage: r2stack_ana.py kernel_dump_file kallsym_file")
		sys.exit(1)
	
	print(">> search NMI watchdog from core file")
	get_k_addr()
	
	print(">>translate stack addr to new file");
	format_addr()
	
	print(">>addr to sym: "+"./addr2sym.py  < addr.txt -m "+sys.argv[2]);

	os.system("./addr2sym.py < addr.txt -m "+sys.argv[2])

main()

#kcrash_dump_file = open(sys.argv[1])

