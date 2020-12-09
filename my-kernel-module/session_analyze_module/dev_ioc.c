#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>

// fs related.
#include <linux/fs.h>
#include <linux/ioctl.h>

#include "dev_ioc.h"


extern void init_session_info(void);

//==================================================
static int bttmon_open(struct inode * inode, struct file * file)
{
    try_module_get(THIS_MODULE);
    return 0;
}

static int bttmon_release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

extern int bottmon_handle_ioc(struct bttmon_ioc_stru *pstru);
extern int bottmon_hook_ioc_tx(struct bttmon_ioc_stru *pstru);


extern int bottmon_hook_ioc(struct bttmon_ioc_stru *pstru);
extern int tcp_analyze_ioc(struct bttmon_ioc_stru *pstru);

int bottmon_handle_ioc(struct bttmon_ioc_stru *pstru)
{
    int ret = 0;
    
    switch(pstru->cmd)
    {
        case CMD_SET_TUPLE:        
        case CMD_GET_TUPLE:       
        case CMD_GET_RESULT:
        {
            ret = tcp_analyze_ioc(pstru);
            break;
        }
        case CMD_FEATURE_SET_RX:
        {
            ret = bottmon_hook_ioc(pstru);
            break;
        }   
        case CMD_FEATURE_SET_TX:
        {
            ret = bottmon_hook_ioc_tx(pstru);
            break;
        }
        default:
        {
            return -1;
        }
    }

    return ret;    
}


static long bttmon_ioctl(struct file *file, unsigned int req, unsigned long arg)
{
    int ret = 0;
    int r;
    struct bttmon_ioc_stru stru;
    
    switch( req )
    {
        case (BTTMON_IOC_TM) :       
        {
            r = copy_from_user( &stru,  (unsigned char *)arg, sizeof(struct bttmon_ioc_stru) );
            if( r )
            {
                printk("> Failed to copy_from_user. r=%d\n", r);
                return -ENOMEM;
            }
            
            //dispatch
            ret = bottmon_handle_ioc(&stru);
            if( ret < 0)
            {
                return -1;  //error occurs.
            }
            
            r = copy_to_user( (unsigned char *)arg, &stru,  sizeof(struct bttmon_ioc_stru));
            if( r)
            {
                printk("> Failed to copy_from_user. r=%d\n", r);
                return -ENOMEM;
            }            
            break;
        }
        default:
        {
            return -1;
        }
    }    

    return ret;
}


//baicells tcp traffic monitor.
static const struct file_operations bttmon_fops =
{
	.owner		    = THIS_MODULE,
	.unlocked_ioctl	= bttmon_ioctl,
	.open		    = bttmon_open,
	.release	    = bttmon_release,
};

#define DEVNAME "tcpsmon"

int major;
static struct class *tcpsmon_class;

int create_our_ioc_device(void)
{   
    major = register_chrdev(0, DEVNAME,&bttmon_fops);
	if (major < 0) {
		printk(": unable to register character device\n");
		return major;
	}

	tcpsmon_class = class_create(THIS_MODULE,DEVNAME);
	if (IS_ERR(tcpsmon_class)) {
		unregister_chrdev(major, DEVNAME);
		return -1;
	}
	device_create(tcpsmon_class,NULL,MKDEV(major,0),NULL, DEVNAME);

    printk("-->dbg created device: %s\n", DEVNAME);	
    return 0;
}

void clean_our_ioc_device(void)
{
    device_destroy(tcpsmon_class, MKDEV(major, 0));
	class_destroy(tcpsmon_class);
	unregister_chrdev(major, DEVNAME);
}

//=========================================================
extern void hook_clean(void);
extern void hook_clean_tx(void);

void clean_dbg_module(void)
{
    printk(">> unregister our hook.\n");

    hook_clean();

    hook_clean_tx();
    
#ifdef HOOK_ON_NETFILTER    
    nf_unregister_hook(&nfho);
#endif

    clean_our_ioc_device();
}


extern void hook_init(void);

int __init start_dbg_module(void)
{   
    int ret;

    printk(">> Load dbg module. built_time:%s %s\n", __DATE__, __TIME__);
    
    ret = create_our_ioc_device();
    if( ret < 0)
    {
        return ret;
    }
    
    hook_init();

    init_session_info();

#ifdef HOOK_ON_NETFILTER    //not use this one, hook on eth driver.
    ret = register_our_hook();    
#endif

    return ret;
}


module_init(start_dbg_module);
module_exit(clean_dbg_module);

MODULE_DESCRIPTION("debug session");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhigang");

