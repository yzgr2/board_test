#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/delay.h>
#include<linux/fs.h>
#include<linux/kthread.h>
#include<linux/jiffies.h>

int gcnt=0;
volatile int gn=0;
int gkill=0;
static int mycnt=0;


unsigned long get_tick(void)
{
	return jiffies;
}

int test_func(int agc)
{	
	if( get_tick()&1 )
	{
		gn = 0;
	}
	else
	{
		gn = 1;
	}
	if((mycnt&0x1FF) == 0)
	{
		printk("mycnt=%x\n", mycnt);
	}
	mycnt++;
	return gn;
}

int example_kernel_thread(void *arg)
{
	int ret;
	printk("-example_kernel_thread  running--\n");
	printk("> func addr:example_kernel_thread=%p test_func=%p get_tick=%p\n", example_kernel_thread,test_func, get_tick);

	while(gkill==0)
	{
		gcnt++;		
		ret = test_func(gcnt);
		if( ret )
		{
			gcnt += 2;
		}
		else
		{
			gcnt += 1;
		}
		
		if( (gcnt%100000000) == 0)
		{
			printk("gcnt=%u\n", gcnt);
			msleep(10);
		}
		msleep(10);
	}
	
	printk(">> demo thread exit.\n");
	
	return 0;
}

struct task_struct *pt;

static __init int init_hello_world(void)
{
	//not support.
	//status=kernel_thread(example_kernel_thread,NULL,CLONE_FS);
	pt = kthread_create(example_kernel_thread, NULL, "yzgdemo");
	if( pt == NULL)
		printk("> Failed to create thread.");
	
	wake_up_process(pt);
	
	return 0;
}

static __exit void exit_hello_world(void)
{
	printk("kill thread\n");
	gkill = 1;
	msleep(2000);
	printk("exit module\n");
	
	return;
}

module_init(init_hello_world);
module_exit(exit_hello_world);
MODULE_LICENSE("GPL");
