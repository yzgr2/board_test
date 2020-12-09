#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kallsyms.h>

typedef enum rt_error_code_e
{
    RT_ERR_FAILED = -1,                             /* General Error                                                                    */

    /* 0x0000xxxx for common error code */
    RT_ERR_OK = 0,                                  /* 0x00000000, OK                                                                   */
    RT_ERR_INPUT,                                   /* 0x00000001, invalid input parameter                                              */
    RT_ERR_UNIT_ID,                                 /* 0x00000002, invalid unit id                                                      */
    RT_ERR_PORT_ID,                                 /* 0x00000003, invalid port id                                                      */
    RT_ERR_PORT_MASK,                               /* 0x00000004, invalid port mask                                                    */
    RT_ERR_PORT_LINKDOWN,                           /* 0x00000005, link down port status                                                */
    RT_ERR_ENTRY_INDEX,                             /* 0x00000006, invalid entry index                                                  */
    RT_ERR_NULL_POINTER,                            /* 0x00000007, input parameter is null pointer                                      */
    RT_ERR_QUEUE_ID,                                /* 0x00000008, invalid queue id                                                     */
    RT_ERR_QUEUE_NUM,                               /* 0x00000009, invalid queue number                                                 */
    RT_ERR_BUSYWAIT_TIMEOUT,                        /* 0x0000000a, busy watting time out                                                */
    RT_ERR_MAC,                                     /* 0x0000000b, invalid mac address                                                  */
    RT_ERR_OUT_OF_RANGE,                            /* 0x0000000c, input parameter out of range                                         */
    RT_ERR_CHIP_NOT_SUPPORTED,                      /* 0x0000000d, functions not supported by this chip model                           */
    RT_ERR_SMI,                                     /* 0x0000000e, SMI error                                                            */
    RT_ERR_NOT_INIT,                                /* 0x0000000f, The module is not initial                                            */
    RT_ERR_CHIP_NOT_FOUND,                          /* 0x00000010, The chip can not found                                               */
    RT_ERR_NOT_ALLOWED,                             /* 0x00000011, actions not allowed by the function                                  */
    RT_ERR_DRIVER_NOT_FOUND,                        /* 0x00000012, The driver can not found                                             */
    RT_ERR_SEM_LOCK_FAILED,                         /* 0x00000013, Failed to lock semaphore                                             */
    RT_ERR_SEM_UNLOCK_FAILED,                       /* 0x00000014, Failed to unlock semaphore                                           */
    RT_ERR_ENABLE,                                  /* 0x00000015, invalid enable parameter                                             */
    RT_ERR_TBL_FULL,                                /* 0x00000016, input table full                                                     */
}rtk_error_code;

typedef int (*f_rtk_rate_egrBandwidthCtrlRate_get)(unsigned int port, unsigned int *pRate, unsigned int *pIfg_include);
typedef int (*f_rtk_rate_igrBandwidthCtrlRate_get)(unsigned int port, unsigned int *pRate, unsigned int *pIfg_include, unsigned int *pFc_enable);
typedef int (*f_rtk_rate_egrQueueBwCtrlRate_get)(unsigned int port, unsigned int queue, unsigned int *pIndex);

typedef int (*f_rtk_rate_shareMeter_get)(unsigned int index, unsigned int *pRate , unsigned int *pIfg_include);
typedef int (*f_rtk_rate_shareMeter_set)(unsigned int index, unsigned int rate, unsigned int ifg_include);

typedef int (* f_rtk_storm_controlRate_get)(unsigned int port, unsigned int storm_type, unsigned int *pRate, unsigned int *pIfg_include, unsigned int mode);
typedef int (* f_rtk_storm_controlRate_set)(unsigned int port, unsigned int storm_type, unsigned int rate, unsigned int ifg_include, unsigned int mode);


f_rtk_rate_egrBandwidthCtrlRate_get c_rtk_rate_egrBandwidthCtrlRate_get ;
f_rtk_rate_igrBandwidthCtrlRate_get c_rtk_rate_igrBandwidthCtrlRate_get ;
f_rtk_rate_egrQueueBwCtrlRate_get   c_rtk_rate_egrQueueBwCtrlRate_get;

f_rtk_rate_shareMeter_get c_rtk_rate_shareMeter_get;
f_rtk_rate_shareMeter_set c_rtk_rate_shareMeter_set;
f_rtk_storm_controlRate_get c_rtk_storm_controlRate_get;
f_rtk_storm_controlRate_set c_rtk_storm_controlRate_set;


int get_function_addr_from_kernel(void)
{
	unsigned long addr;
	
	addr = kallsyms_lookup_name("rtk_rate_egrBandwidthCtrlRate_get");
	if( 0 == addr )
	{
		printk("Not fund rtk_rate_egrBandwidthCtrlRate_get.\n");
		return -1;
	}
	c_rtk_rate_egrBandwidthCtrlRate_get = (f_rtk_rate_egrBandwidthCtrlRate_get)addr;
	
	addr = kallsyms_lookup_name("rtk_rate_igrBandwidthCtrlRate_get");
	if( 0 == addr )
	{
		printk("Not fund rtk_rate_igrBandwidthCtrlRate_get.\n");
		return -1;
	}
	c_rtk_rate_igrBandwidthCtrlRate_get = (f_rtk_rate_igrBandwidthCtrlRate_get)addr;

	addr = kallsyms_lookup_name("rtk_rate_egrQueueBwCtrlRate_get");
	if( 0 == addr )
	{
		printk("Not fund rtk_rate_egrQueueBwCtrlRate_get.\n");
		return -1;
	}
	c_rtk_rate_egrQueueBwCtrlRate_get = (f_rtk_rate_egrQueueBwCtrlRate_get)addr;

	addr = kallsyms_lookup_name("rtk_rate_shareMeter_get");
	if( 0 == addr )
	{
		printk("Not fund rtk_rate_shareMeter_get.\n");
		return -1;
	}
	c_rtk_rate_shareMeter_get = (f_rtk_rate_shareMeter_get)addr;

	addr = kallsyms_lookup_name("rtk_rate_shareMeter_set");
	if( 0 == addr )
	{
		printk("Not fund rtk_rate_shareMeter_set.\n");
		return -1;
	}
	c_rtk_rate_shareMeter_set = (f_rtk_rate_shareMeter_set)addr;

	addr = kallsyms_lookup_name("rtk_storm_controlRate_get");
	if( 0 == addr )
	{
		printk("Not fund rtk_storm_controlRate_get.\n");
		return -1;
	}
	c_rtk_storm_controlRate_get = (f_rtk_storm_controlRate_get)addr;

	addr = kallsyms_lookup_name("rtk_storm_controlRate_set");
	if( 0 == addr )
	{
		printk("Not fund rtk_storm_controlRate_set.\n");
		return -1;
	}
	c_rtk_storm_controlRate_set = (f_rtk_storm_controlRate_set)addr;
	
	return 0;
}

int test_func(void)
{
	int port;
	unsigned int rate;
	unsigned int ifg_inc;
	unsigned int fc_inc;
	unsigned int index;
	int strom_group;
	
	int ret;
	int queue;

	for( index=0; index<32; index++)
	{
	    ret = c_rtk_rate_shareMeter_get(index, &rate, &ifg_inc);
	    if( ret == RT_ERR_OK)
		{
			printk("Meter index=%u rate=%u ifg_inc=%u \n", index, rate, ifg_inc);
		}	

		//change share-meter to max.
		ret = c_rtk_rate_shareMeter_set(index, 1048568, 0);
	    if( ret == RT_ERR_OK)
		{
			printk("Set Meter index=%u rate=%u ifg_inc=%u \n", index, 1048568, 0);
		}			
	}
	printk("\n");
	
	for(port=0; port<32; port++)
	{
		ret = c_rtk_rate_egrBandwidthCtrlRate_get(port, &rate, &ifg_inc );
		if( ret == RT_ERR_OK)
		{
			printk("EgrBw : port=%d rate=%u ifg_inc=%u \n", port, rate, ifg_inc);
		}
		else
		{
			printk("EgrBw : port=%d Failed to get\n", port);
		}
		
		ret = c_rtk_rate_igrBandwidthCtrlRate_get(port, &rate, &ifg_inc, &fc_inc );
		if( ret == RT_ERR_OK)
		{
			printk("IgrBw : port=%d rate=%u ifg_inc=%u fc_inc=%u\n", port, rate, ifg_inc, fc_inc);
		}
		else
		{
			printk("IgrBw : port=%d Failed to get\n", port);
		}

		for( queue=0; queue<8; queue++)
		{
		    ret = c_rtk_rate_egrQueueBwCtrlRate_get(port, queue, &index);
		    if( ret == RT_ERR_OK)
		    {
		        printk("    Egr_queue=%u index=%u\n", queue, index);
		    }
		}	

		printk("\n");
		printk("storm ctl info.\n"); 
		for( strom_group=0; strom_group<=3; strom_group++)
		{
		    ret = c_rtk_storm_controlRate_get(port, strom_group, &rate, &ifg_inc, 0 );
		    if( ret == RT_ERR_OK)
		    {
		        printk("    Mode 0: storm ctr: strom_group=%d rate:%u\n", strom_group, rate);
		    }
		    
		    ret = c_rtk_storm_controlRate_get(port, strom_group, &rate, &ifg_inc,1 );
		    if( ret == RT_ERR_OK)
		    {
		        printk("    Mode 1: storm ctr: strom_group=%d rate:%u\n", strom_group, rate);
		    }
		}

		printk("\n");
		
	}

	//
	

	return 0;
	
}

int __init swtichm_enter(void)
{
	int ret;
	ret = get_function_addr_from_kernel();
	if( ret < 0)
	{
		return -1;
	}
	
	test_func();
	
	//not really load this module to kernel.
	return -1;
}

void swtichm_exit(void)
{
	printk(">> unload module.\n");
}

module_init(swtichm_enter);
module_exit(swtichm_exit);

MODULE_DESCRIPTION("debug session");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhigang");


