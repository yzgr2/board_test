ccflags-y += -g
ifeq ($(KERNELRELEASE),)
	#指向你的内核代码目录
#	KERNELDIR ?=/home/yuzhigang/FAP_TDD_GA4.0/branches/last/build_dir/linux-transcede-2200_t2200/linux-3.0.51   
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

clean:
	rm -f *.o

install:
	cp demo.ko /lib/modules/3.13.0-65-generic/
else
obj-m    := myprobe.o 
myprobe-objs = main.o     
endif

