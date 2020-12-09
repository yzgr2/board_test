#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>


//=========================================================
//assume: packet format was : MAC IP + UDP + GTPU + IP + UDP + iperf payload
// function was called before netif_rx_skb, after eth_type_trans.
//@udp : the first udp header.
//GTP.
void rec_iperf_udp_seq(struct udphdr * udp)
{
    struct iphdr *iph2;
    struct udphdr *uh2;
    unsigned char *iperf_payload;
    unsigned int seq = 0;
    static unsigned int pre_seq = 0;   
    static unsigned int dbg_cnt = 0;

    //UDP + GTP + IP + UDP + iperf_data
    if( udp->len < 8 + 8 + 20 + 8 + 8)
    {
        return;
    }
    
	if( (udp->source == htons(2152)) && (udp->dest== htons(2152)) )
	{
		//It is GTP-U
	}
	else
	{
		return;
	}

	iph2 = (struct iphdr *)(((char *)udp) + 16);    //skip UDP hdr + GTP header. 8 + 8
	if( iph2->protocol != 17 ) //UDP
	{
	    return;
	}
   
    uh2 = (struct udphdr *)(((char *)iph2) + (iph2->ihl<<2)) ;
    iperf_payload = (char *)uh2 + 8;
    
    if( (uh2->source == htons(5001)) || (uh2->dest == htons(5001)) )  // iperf
    {
        seq = ntohl(*(unsigned int *)iperf_payload);   
    }
    else if( (uh2->source== htons(5201)) || (uh2->dest== htons(5201)) ) //iperf3 -R/
    {
        seq = ntohl(*(unsigned int *)(iperf_payload+8));           
    }
    else
    {        
        return; //not iperf traffic.
    }

    dbg_cnt++;
    if( (dbg_cnt&0x1FF) ==0)
    {
        printk(">> track pkt counter =%d pre_seq=%u seq=%u \n", dbg_cnt, pre_seq, seq);
    }    

    if( seq <= 1 )
    {
        pre_seq = seq;
        dbg_cnt = 0;
        printk(">> iperf udp start. [source=%u dest=%u]\n", ntohs(uh2->source), ntohs(uh2->dest) );
    }
    else
    {
        if( pre_seq + 1 < seq )
        {
            printk("> Pkt missed before. pre_seq=%u (0x%x)  cur_pkt_seq:%u (0x%x)\n", pre_seq, pre_seq, seq, seq);
        }
        else if( pre_seq + 1 > seq )
        {
            printk("> Out of order. pre_seq=%u (0x%x)  cur_pkt_seq:%u (0x%x)\n", pre_seq, pre_seq, seq, seq);
        }
        
        pre_seq = seq;
    }    
	
}

//=============================================================

int rx_gtpu_iperf_udp_analyze_in(struct sk_buff *skb)
{
    unsigned char *pmac;
    struct iphdr *iph;
    struct udphdr *udph;
    static int first_print = 0;

    first_print++;
    
    pmac = skb_mac_header(skb);
    if( (pmac[12] == 0x08) &&  (pmac[13] == 0) )    //ip pkt.
    {
       if( first_print < 10)
       {
            first_print++;
            printk("dbg, get ip pkt.\n");
       }
    }
    else
    {       
        return -1; //not ip
    }
    
#ifdef HOOK_ON_NETFILTER    
    iph = ip_hdr(skb);  //since we are 
#else
    iph = (struct iphdr *)(pmac + 14);
#endif
    if( first_print < 10)
    {
        first_print++;
        printk(" protocol:%u.\n", iph->protocol );
    }
       
    if( iph->protocol != 17) //Not UDP
    { 
       return -1;  
    }

    udph = (struct udphdr *)((char *)iph + (iph->ihl<<2));
    rec_iperf_udp_seq( udph );
    
    return 0;
}



