#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>

// fs related.
#include <linux/fs.h>



static struct nf_hook_ops nfho;

unsigned int sip;
unsigned int dip;
unsigned short sp, dp;
unsigned int seq = 0;

module_param(sip, uint,  S_IRUGO);
module_param(dip, uint,  S_IRUGO);
module_param(sp, ushort,  S_IRUGO);
module_param(dp, ushort,  S_IRUGO);

int first_print = 0;
int cnt = 0;

struct monitor_session
{
    unsigned int sip;
    unsigned int dip;    
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

void init_session_info()
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

    p_minfo = kmalloc(sizeof(struct monitor_info));
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

static int ip_match(unsigned int sip, unsigned char dip)
{
    if( (sip == g_monitor_sess.sip) && (dip == g_monitor_sess.dip) )
    {
        return 1;
    }
    else
        return 0;
}
//=============================================================================

//analyze the packet.
void baicells_analyze(struct sk_buff *skb)
{
    unsigned char *pmac;
    struct iphdr *iph;
    struct tcphdr *tcph;
    unsigned short payloadLen = 0;
    struct monitor_info *pminfo;
    
    first_print++;
    
    if( !skb)
        return ;

   if( first_print < 10)
   {
        printk("dbg, function in.\n");
   }

    //if(skb->protocol != )
    //skb_mac_header
    pmac = skb_mac_header(skb);
    if( (pmac[12] == 0x08) &&  (pmac[13] == 0) )
    {
       if( first_print < 10)
       {
            printk("dbg, get ip pkt.\n");
       }
    }
    else
    {
       if( first_print < 10)
       {
            printk("dbg, Not ip pkt.\n");
       }
       
        return ; //not ip
    }
    
    iph = ip_hdr(skb);
    if( iph->protocol != 6) //Not TCP
    {
       if( first_print < 10)
       {
            printk("dbg, get tcp pkt.\n");
       }
        return ;  
    }

    
    //if( (ntohl(iph->saddr) == sip) && (ntohl(iph->daddr) == dip) )
    if(ip_match(ntohl(iph->saddr), ntohl(iph->daddr)))
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
            printk("dbg, ip not match..saddr=%x  daddr=%x \n", ntohl(iph->saddr), ntohl(iph->daddr));
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
    }       
    //============================
    seq = pminfo->seq;
    
    //if( (ntohs(tcph->source) == sp) && (ntohs(tcph->dest)== dp) )
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
            
            printk(">> monitor start: seq=%x\n", seq);
        }
        else
        {
            if( ntohl(tcph->seq) < seq)
            {
                //retransmit pkt.
                printk("> Retransmit: cur_seq:%x  pkt_seq:%x\n", seq, ntohl(tcph->seq));

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
            }
            else        //ntohl(tcph->seq) > seq
            {
                printk("!!! Miss pkt before this, cur_seq:%x pkt_seq:%x\n", seq, ntohl(tcph->seq) );

                pminfo->seq_found_miss++;
                if( pminfo->seq_miss_index < INFO_LEN)
                {                    
                    pminfo->seq_found_retrans[pminfo->seq_miss_index] = seq;
                    pminfo->seq_miss_index++;
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

unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
    baicells_analyze(skb);
    
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

struct tuple
{
    unsigned int sip;
    unsigned int dip;
};

struct bttmon_ioc_stru
{
    int cmd;   
    
    union
    {
        struct tuple tup;
    }u;
};

#define BTTMON_IOC_MAGIC 'P'

//type, nr, size
#define BTTMON_IOC_TM   _IOWR(BTTMON_IOC_MAGIC, 1, sizeof(struct bttmon_ioc_stru))

enum{
    CMD_SET_TUPLE = 0,
    CMD_GET_TUPLE = 1,
    
};


int bottmon_handle_ioc(struct bttmon_ioc_stru *pstru);

static long bttmon_ioctl(struct file *file, unsigned int req, unsigned long arg)
{
    int ret = 0;
    int r;
    struct bttmon_ioc_stru stru;
    
    switch(req)
    {
        case BTTMON_IOC_TM:
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
        }
        default:
        {
            return -1;
        }
    }    

    return ret;
}

int bottmon_handle_ioc(struct bttmon_ioc_stru *pstru)
{
    int ret = 0;
    
    switch(pstru->cmd)
    {
        case CMD_SET_TUPLE:
        {
            g_monitor_sess.sip = pstru->u.tup.sip;
            g_monitor_sess.dip = pstru->u.tup.dip;
            return 0;
        }
        case CMD_GET_TUPLE:
        {
            pstru->u.tup.sip = g_monitor_sess.sip ;
            pstru->u.tup.dip = g_monitor_sess.dip ;
            return 0;
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


//=========================================================

void unregister_our_hook(void)
{
    printk(">> unregister our hook.\n");
    nf_unregister_hook(&nfho);
}


int __init start_dbg(void)
{   
    int ret;
    ret = register_our_hook();
    return ret;
}


module_init(start_dbg);
module_exit(unregister_our_hook);

MODULE_DESCRIPTION("debug session");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhigang");

