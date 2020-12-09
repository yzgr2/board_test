ifeq ($(KERNELRELEASE),)
#指向你的内核代码目录
KERNELDIR ?=/home/yuzhigang/FAP_TDD_GA4.0/branches/last/build_dir/linux-transcede-2200_t2200/linux-3.0.51   
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

else

obj-m	:= dbgsession.o 
dbgsession-objs = tcp_analyze.o 	
endif

#export CROSS_COMPILE=arm-none-linux-gnueabi-
#make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-
