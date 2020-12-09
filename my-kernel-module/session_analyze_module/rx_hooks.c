#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>

#include "dev_ioc.h"

//==========feature define ===========================
extern int rx_tcp_analyze_in(struct sk_buff *skb);

extern int rx_gtpu_iperf_udp_analyze_in(struct sk_buff *skb);

//====================================================
typedef int (*hook_func)( struct sk_buff * pSkb) ;

//hook_func pHooks[FEA_RX_MAX] = {
//    rx_tcp_analyze_in,
//    rx_gtpu_iperf_udp_analyze_in,      
//};

hook_func pHooks[FEA_RX_MAX] = {
    NULL,
    NULL,      
};

void hook_init(void)
{

}

int hook_in( struct sk_buff * pSkb)
{
	int i=0;
	hook_func fp;	

	if(!pSkb)
	{
		return 0;
	}
	
	for(i=0; i<FEA_RX_MAX; i++)
	{
		if( pHooks[i] )
		{
			fp = pHooks[i];
			fp(pSkb);
		}
	}
	
	return 0;
}

extern int (*gemac_rx_hook_cb)( struct sk_buff * pSkb);

void set_hook(void)
{
    printk("Enable Rx Debug Module.\n");
	gemac_rx_hook_cb = hook_in;
	
}

void remove_hook(void)
{
    printk("Disable Rx Debug Module.\n");
	gemac_rx_hook_cb = NULL;
}

void check_feature(void)
{
    int i = 0;
    int feature_on = 0;
    
    for( i=0; i<FEA_RX_MAX; i++)
    {
        if(pHooks[i] )
        {
            feature_on = 1;     
            printk(">> dbg: feature [%d] is ON.\n", i);
            break;
        }
    }

    if( feature_on)
        set_hook();
    else
        remove_hook();
                        
}

void hook_clean(void)
{
    remove_hook();
}
///////////////////////////////////////////////////////////////////////////

int bottmon_hook_ioc(struct bttmon_ioc_stru *pstru)
{   
    switch( pstru->cmd )
    {
        case CMD_FEATURE_SET_RX:
        {
            switch(pstru->u.hook.feature)
            {
                case FEA_RX_TCP_SEQ:
                    if(pstru->u.hook.enable)
                    {
                        pHooks[FEA_RX_TCP_SEQ] = rx_tcp_analyze_in;
                    }
                    else
                    {
                        pHooks[FEA_RX_TCP_SEQ]= NULL;
                    }
                    check_feature();
                    break;
                case FEA_RX_IPERF_UDP_SEQ:
                    if(pstru->u.hook.enable)
                    {
                        pHooks[FEA_RX_IPERF_UDP_SEQ] = rx_gtpu_iperf_udp_analyze_in;
                    }
                    else
                    {
                        pHooks[FEA_RX_IPERF_UDP_SEQ]= NULL;
                    }     
                    check_feature();    
                    break;
                default:
                    printk(">bottmon_hook_ioc: Not support cmd=%d, feature=%d\n", pstru->cmd, pstru->u.hook.feature);
                    return -1;                    
            }
            break;
        }
        default:
        {
            printk(">bottmon_hook_ioc, not our cmd. pstru->cmd=%d\n", pstru->cmd);
        }
        
    }

    return 0;
}

