#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>

#include "dev_ioc.h"

//===========================================
extern int tx_iperf3_rec_timestamp(struct sk_buff *skb);

extern int tx_bs_iperf3_rec_timestamp(struct sk_buff *skb);
//====================================================
typedef int (*hook_func_tx)( struct sk_buff * pSkb) ;

//hook_func_tx pHooks_tx[FEA_TX_MAX] = {
//    tx_iperf3_rec_timestamp,
//    tx_bs_iperf3_rec_timestamp,
//};


hook_func_tx pHooks_tx[FEA_TX_MAX] = {
    NULL,
    NULL,
};


void hook_init_tx(void)
{

}

int hook_tx_in( struct sk_buff * pSkb)
{
	int i=0;
	hook_func_tx fp;	

	if(!pSkb)
	{
		return 0;
	}
	
	for(i=0; i<FEA_TX_MAX; i++)
	{
		if( pHooks_tx[i] )
		{
			fp = pHooks_tx[i];
			fp(pSkb);
		}
	}
	
	return 0;
}

extern int (*gemac_tx_hook_cb)( struct sk_buff * pSkb);

void set_hook_tx(void)
{
    printk("Enable Tx Debug Module.\n");
	gemac_tx_hook_cb = hook_tx_in;
	
}

void remove_hook_tx(void)
{
    printk("Disable Tx Debug Module.\n");
	gemac_tx_hook_cb = NULL;
}

void check_feature_tx(void)
{
    int i = 0;
    int feature_on = 0;
    
    for( i=0; i<FEA_TX_MAX; i++)
    {
        if(pHooks_tx[i] )
        {
            feature_on = 1;     
            printk(">> dbg: TX feature [%d] is ON.\n", i);
            break;
        }
    }

    if( feature_on)
        set_hook_tx();
    else
        remove_hook_tx();
                        
}

void hook_clean_tx(void)
{
    remove_hook_tx();
}

//=====================================================================



//========================================================================

int bottmon_hook_ioc_tx(struct bttmon_ioc_stru *pstru)
{   
    switch( pstru->cmd )
    {
        case CMD_FEATURE_SET_TX:
        {
            switch(pstru->u.hook.feature)
            {
                case FEA_TX_GTP_TCP_TIMESTAMP:
                    printk("TX [%d] change to [%s]\n", FEA_TX_GTP_TCP_TIMESTAMP, pstru->u.hook.enable?"ON":"OFF");
                    if(pstru->u.hook.enable)
                    {
                        pHooks_tx[FEA_TX_GTP_TCP_TIMESTAMP] = tx_iperf3_rec_timestamp;
                    }
                    else
                    {
                        pHooks_tx[FEA_TX_GTP_TCP_TIMESTAMP]= NULL;
                    }
                    check_feature_tx();
                    break;      
                case FEA_TX_BS_TCP_TIMESTAMP:
                    printk("TX [%d] change to [%s]\n", FEA_TX_BS_TCP_TIMESTAMP, pstru->u.hook.enable?"ON":"OFF");
                    if(pstru->u.hook.enable)
                    {
                        pHooks_tx[FEA_TX_BS_TCP_TIMESTAMP] = tx_bs_iperf3_rec_timestamp;
                    }
                    else
                    {
                        pHooks_tx[FEA_TX_BS_TCP_TIMESTAMP]= NULL;
                    }
                    check_feature_tx();
                    break;      
                default:
                    printk(">bottmon_hook_ioc_tx: Not support cmd=%d\n", pstru->cmd);
                    return -1;                    
            }
            break;
        }
        
    }

    return 0;
}


