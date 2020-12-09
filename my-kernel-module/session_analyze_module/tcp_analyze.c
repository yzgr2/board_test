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

#ifdef HOOK_ON_NETFILTER
static struct nf_hook_ops nfho;
#endif

unsigned int sip;
unsigned int dip;
unsigned short sp, dp;
unsigned int seq = 0;

module_param(sip, uint,  S_IRUGO);
module_param(dip, uint,  S_IRUGO);
module_param(sp, ushort,  S_IRUGO);
module_param(dp, ushort,  S_IRUGO);

int cnt = 0;

struct monitor_session
{
    unsigned int sip;  //network endian.
    unsigned int dip;  //network endian.   
};
struct monitor_session g_monitor_sess;

#define INFO_LEN   32
struct monitor_info
{
    struct list_head list;
    unsigned short sp;
    unsigned short dp;

    unsigned int start_seq;
    unsigned int last_seq;
    
    unsigned int seq;

    unsigned int miss_cnt;         //found frame miss.
    unsigned int miss_frame_cnt;   //how many frame missed.
    unsigned int retrans_cnt;       
    unsigned int retrans_frame_cnt; 

    unsigned int seq_found_miss[INFO_LEN];
    int seq_miss_index;
    
    unsigned int seq_found_retrans[INFO_LEN];
    int seq_retrans_index;
    
};

#define SUPPORT_SESSION_CNT 16
#define QUEUE_LEN   16
struct list_head g_lists[QUEUE_LEN];
int g_session_cnt = 0;

#define hash_index(a,b)   ((a^b)%QUEUE_LEN)

void init_session_info(void)
{
    int i = 0;
    
    memset(&g_monitor_sess, 0, sizeof(struct monitor_session));

    for(i=0; i<QUEUE_LEN; i++)
    {
        INIT_LIST_HEAD(&g_lists[i]);
    }    
}

struct monitor_info * alloc_monitor_item_add_to_queue(unsigned char hash)
{
    struct monitor_info *p_minfo;
    unsigned char index;

    printk(">>alloc_monitor_item_add_to_queue.hash=0x%x\n", hash);

    p_minfo = kmalloc(sizeof(struct monitor_info), GFP_KERNEL);
    if( !p_minfo)
    {
        return NULL;
    }

    memset(p_minfo, 0, sizeof(struct monitor_info) );
    INIT_LIST_HEAD( &p_minfo->list);

    index = hash % QUEUE_LEN;
    list_add_tail( &p_minfo->list, &g_lists[index]);

    g_session_cnt++;

    return p_minfo;    
}

// 1: found item
// 0: not found item.
struct monitor_info * found_monitor_item(unsigned short sp, unsigned short dp)
{
    unsigned char index = hash_index(sp, dp);
    struct monitor_info *pos;
    struct list_head *head;

    head = &g_lists[index];

    if( list_empty( &g_lists[index] ))
    {
        return NULL;
    }

    list_for_each_entry(pos,head, list)
    {
        if( (pos->sp == sp) && (pos->dp == dp))
        {
            return pos;
        }
    }

    return NULL;
}

char msg_buf[2048];
int show_item_info(struct monitor_info *pos)
{
    int len;
    int offset = 0;
    int ind;
    len = sprintf(msg_buf, "sp:%u dp:%u start_seq:%x ", pos->sp, pos->dp, pos->start_seq );
    offset += len;
    
    len = sprintf(msg_buf + offset , "seq:%x ", pos->seq);
    offset += len;
    
    len = sprintf(msg_buf + offset, "miss_cnt:%d  recorded:%d \n",pos->miss_cnt, pos->seq_miss_index);
    offset += len;
    for(ind = 0; ind < pos->seq_miss_index; ind++)
    {
         len = sprintf(msg_buf + offset, "MISS: %x \n",pos->seq_found_miss[ind]);
         offset += len;
         if( offset > 1500)
         {
            printk("> truncated...\n");
            return 0;
         }
            
    }
    
    len = sprintf(msg_buf + offset, "retrans_cnt:%d record:%d \n",pos->retrans_cnt, pos->seq_retrans_index);
    offset += len;
    for(ind = 0; ind < pos->seq_retrans_index; ind++)
    {
         len = sprintf(msg_buf + offset, "Retrans: %x\n",pos->seq_found_retrans[ind]);
         offset += len;
         if( offset > 1500)
         {
            printk("> truncated...\n");
            return 0;
         }
    }
    
    printk("%s----------\n\n", msg_buf);    
    return 0;
}

int show_all_monitor_item(void)
{
    unsigned char index ;
    struct monitor_info *pos;
    struct list_head *head;

    for( index=0; index<QUEUE_LEN; index++)
    {
        head = &g_lists[index];

        if( list_empty( head ))
        {
            continue;
        }

        list_for_each_entry(pos,head, list)
        {
            show_item_info(pos);
        }
    }

    return 0;
}

#if 0
static int ip_match(unsigned int tsip, unsigned char tdip)
{
    if( (tsip == g_monitor_sess.sip) && (tdip == g_monitor_sess.dip) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
//=============================================================================

//assume: packet was original TCP format. 
//analyze the packet.
void tcp_analyze_in(struct sk_buff *skb)
{
    unsigned char *pmac;
    struct iphdr *iph;
    struct tcphdr *tcph;
    unsigned short payloadLen = 0;
    struct monitor_info *pminfo;
    
    static int first_print = 0;
    static int tcp_in_cnt = 0;
 
    if( !skb)
        return ;
        
   first_print++;
   if( first_print < 10)
   {
        printk("dbg, function tcp_analyze_in in.\n");
   }

    //if(skb->protocol != )
    //skb_mac_header
    pmac = skb_mac_header(skb);
    if( (pmac[12] == 0x08) &&  (pmac[13] == 0) )
    {
       if( first_print < 10)
       {
            printk("dbg, tcp_analyze_in get ip pkt.\n");
       }
    }
    else
    {       
        return ; //not ip
    }

#ifdef HOOK_ON_NETFILTER    
    iph = ip_hdr(skb);  //since we are 
#else
    iph = (struct iphdr *)(pmac + 14);
#endif
    if( iph->protocol != 6) //Not TCP
    {
       if( first_print < 10)
       {
            printk("dbg, Not tcp pkt, iph->protocol=%d.\n", iph->protocol);
       }
       return ;  
    }

    tcp_in_cnt++;
    if( tcp_in_cnt < 10)
    {
        printk("dbg, TCP, iph->saddr=%x iph->daddr=%x\n", iph->saddr, iph->daddr);
    }
    
    //if( (ntohl(iph->saddr) == sip) && (ntohl(iph->daddr) == dip) )
    //if(ip_match(iph->saddr, iph->daddr))
    if( (iph->saddr == g_monitor_sess.sip) && (iph->daddr == g_monitor_sess.dip) )
    {
        //ip match.
       if( first_print < 10)
       {
            printk("dbg, ip match..\n");
       }
    }
    else
    {
       if( first_print < 10)
       {
            printk("dbg, ip not match..saddr=%x  daddr=%x   Monitor sip=%x dip=%x \n", (iph->saddr), (iph->daddr), g_monitor_sess.sip, g_monitor_sess.dip);
       }
        return ;
    }
    
    tcph = (struct tcphdr *)((char *)iph + (iph->ihl<<2)) ;
    if( (unsigned long)((char *)tcph + 20) > (unsigned long)(skb->data + skb->len) )
    {
        printk("dbg, bad tcp frame..\n");
        return ; //bad frame.
    }
    
    //============================
    sp = ntohs(tcph->source);
    dp = ntohs(tcph->dest);
    
    //filter out some known port.
    // SSH : 22
    if( (sp==22) || (dp==22) )
    {
        return;
    }

    pminfo = found_monitor_item(sp,dp);
    if( !pminfo)
    {   
        if( g_session_cnt > SUPPORT_SESSION_CNT)
        {
            return ;
        }
        
        pminfo = alloc_monitor_item_add_to_queue( hash_index(sp,dp) );
        if( pminfo == NULL)
        {
            return ;      //NO mem.
        }
        pminfo->sp = sp;
        pminfo->dp = dp;
    }       
    //============================
    seq = pminfo->seq;
    
    //if( (ntohs(tcph->source) == sp) && (ntohs(tcph->dest)== dp) )
    if(pminfo)
    {
        payloadLen = ntohs(iph->tot_len) - (iph->ihl<<2) - (tcph->doff<<2);

        cnt++;
        if( (cnt%1000000) == 0)  // 1G  10w /sec
        {
            printk(">> cur_seq=%u \n", seq);
        }
        
        //process this packet.
        if( seq == 0)
        {
            seq = ntohl(tcph->seq); 
            pminfo->start_seq = seq;
            
            seq += payloadLen;        
            pminfo->seq = seq;
            if( tcph->syn )
            {
                pminfo->seq++;
            }
            
            printk(">> monitor start: seq=%x  [sp=%u dp=%u]\n", seq, sp, dp);
        }
        else
        {
            if( ntohl(tcph->seq) < seq)
            {
                //retransmit pkt.
                printk("> Retransmit: cur_seq:%x  pkt_seq:%x  (sp=%d dp=%d)\n", seq, ntohl(tcph->seq), sp, dp);

                pminfo->retrans_frame_cnt++;
                pminfo->retrans_cnt++;

                if( pminfo->seq_retrans_index < INFO_LEN)
                {                    
                    pminfo->seq_found_retrans[pminfo->seq_retrans_index] = ntohl(tcph->seq);
                    pminfo->seq_retrans_index++;
                }
                
                //enhancement. not likely.
                if( unlikely(ntohl(tcph->seq) + payloadLen > seq) )
                {
                    seq = ntohl(tcph->seq) + payloadLen;
                    printk("> update seq by re-transmit pkt. cur_seq=%x\n", seq);
                }
                
            }
            else if( ntohl(tcph->seq) == seq )
            {
                //match
                seq += payloadLen;

                if( tcph->fin )
                    seq++;
            }
            else        //ntohl(tcph->seq) > seq
            {
                printk("!!! Miss pkt before this, cur_seq:%x pkt_seq:%x (sp=%d dp=%d)\n", seq, ntohl(tcph->seq), sp, dp );
                
                if( pminfo->seq_miss_index < INFO_LEN)
                {   
                    pminfo->seq_found_miss[pminfo->seq_miss_index] = seq;
                    pminfo->seq_miss_index++;
                    pminfo->miss_cnt++;
                }
                
                //miss packet before this frame.
                seq = ntohl(tcph->seq) + payloadLen;
            }

            pminfo->seq = seq;
        }
        
    }
    else
    {
       if( first_print < 10)
       {
            printk("dbg, port not match.. sport=%u  dport=%u \n", ntohs(tcph->source), ntohs(tcph->dest));
       }
        return; //not care about this session.
    }    
    
}


int rx_tcp_analyze_in(struct sk_buff *skb)
{
    tcp_analyze_in(skb);
    return 0;
}

#ifdef HOOK_ON_NETFILTER    //not use this one, hook on eth driver.

unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
    tcp_analyze_in(skb);
    
    return NF_ACCEPT;           
}

int register_our_hook(void)
{
    int ret;
    
    /* 填充我们的hook数据结构 */
    nfho.hook = hook_func;         /* 处理函数 */
    nfho.hooknum  = NF_INET_PRE_ROUTING; /* 使用IPv4的第一个hook */
    nfho.pf       = PF_INET;
    nfho.priority = NF_IP_PRI_MANGLE + 10;   /* 让我们的函数首先执行 */

    ret = nf_register_hook(&nfho);
    if( ret == 0)
    {
        printk("---register nfhook success--\n");        
    }
    else
    {
        printk("---register nfhook Failed--\n"); 
    }

    printk("sip=%x sp=%u dip=%x dp=%u\n", sip, sp, dip, dp);
    
    return ret;
}
#endif

int tcp_analyze_ioc(struct bttmon_ioc_stru *pstru)
{
    int ret = 0;
    
    switch(pstru->cmd)
    {
        case CMD_SET_TUPLE:
        {
            g_monitor_sess.sip = pstru->u.tup.sip;
            g_monitor_sess.dip = pstru->u.tup.dip;
            printk(">> set sip=%x  dip=%x \n", g_monitor_sess.sip, g_monitor_sess.dip);            
            
            return 0;
        }
        case CMD_GET_TUPLE:
        {
            pstru->u.tup.sip = g_monitor_sess.sip ;
            pstru->u.tup.dip = g_monitor_sess.dip ;           
            
            return 0;
        }
        case CMD_GET_RESULT:
        {
            printk(">> show result now <<\n");
            show_all_monitor_item();
            return 0;
        }
        default:
        {
            return -1;
        }
   }

   return ret;
}



