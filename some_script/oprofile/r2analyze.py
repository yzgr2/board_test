#

class sample(object):	
	def __init__(self, v1, v2, v3, v4):
		self.sample_cnt = int(v1)
		self.percent = float(v2)
		self.app_name = v3
		self.symbol_name = v4

cpu0_slist= []
cpu1_slist= []
		
def extract_data_from_file(path):
	f = open(path)
	start = 0
	for line in f.readlines():		
		if (start==0) and (line.find('app name') >0):
			start = 1	
			continue
		
		if start:
			data = line.split()
			s0 = sample(data[0], data[2], data[8], data[9])
			s1 = sample(data[4], data[6], data[8], data[9])
			cpu0_slist.append(s0)
			cpu1_slist.append(s1)
	f.close()

def summary_list(lst):
	current=1
	last=0
	result=[]
	result.append(lst[0])
	result_index=0
	while current < len(lst):
		if( lst[current].app_name == lst[last].app_name):
			result[result_index].sample_cnt += lst[current].sample_cnt
			result[result_index].percent += lst[current].percent
		else:
			result.append(lst[current])
			last=current
			result_index+=1			
		current+=1
	
	result.sort(key=lambda x:x.sample_cnt)
	#for s in result:
	#	print( s.sample_cnt, '  ', s.percent, '  ', s.app_name, '  ', s.symbol_name)
	return result	

def generate_result(plist, filepath):
	f = open(filepath,'w')
	plist.sort(key=lambda x:x.sample_cnt)

	for s in plist:
		f.write('{0}  {1}  {2}  {3}\r\n'.format(s.sample_cnt, s.percent, s.app_name, s.symbol_name))

	f.write('\r\n~~~~~~~~~~~~summary by app name ~~~~~~~~~~~~~~~~~~\r\n')

	plist.sort(key=lambda x:x.app_name)
	sum0 = summary_list(plist)

	for s in sum0:
		f.write('{0}  {1}  {2}  {3}\r\n'.format(s.sample_cnt, s.percent, s.app_name, s.symbol_name))
	
	f.close()	

def main():
	extract_data_from_file('./result_02.txt')
	
	generate_result(cpu0_slist, 'cpu0_detail.txt')
	generate_result(cpu1_slist, 'cpu1_detail.txt')
	
	#for s in cpu1_slist:
	#	print( s.sample_cnt, '  ', s.percent, '  ', s.app_name, '  ', s.symbol_name)

main()

