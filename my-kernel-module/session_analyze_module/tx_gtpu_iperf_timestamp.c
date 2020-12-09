#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#include <linux/dma-attrs.h>
#include <asm/page.h>


unsigned char pkt_buf[2048*2];

#define FRAG_START 2048

int get_t2200_tx_skb_data(struct sk_buff *skb, unsigned char **ptr)
{
    unsigned int i;
    struct skb_shared_info * pinfo;
    unsigned int size;
    unsigned int offset = FRAG_START;
    unsigned int len = 0;
    unsigned int first_frag_len = 0;
    unsigned int first_frag_offset = 0;
    
    pinfo = skb_shinfo(skb);
    if(pinfo->map_fr_num == 0)
    {
        *ptr = skb->data;
        return skb->len;
    }
    
	for (i = 1; i < pinfo->map_fr_num; i++)
	{
	     size = pinfo->frags[i].size;

	     if(offset + size >= 2048*2)
	     {
	        printk(">>Not support length.\n");
	        return -1;
	     }
	     //memcpy( pkt_buf + offset, (void *)page_to_virt(pinfo->frags[i].page), size);
	     memcpy( pkt_buf + offset, (void *)(pinfo->frags[i].page), size);
	     offset += size;
	     len += size;
	}

	first_frag_len = (skb->len - len);
	if( first_frag_len > 2048)
	{
	    printk(">>First frag length error.\n");
	    *ptr = NULL;
	    return -1;
	}
	
	first_frag_offset = 2048 - first_frag_len;

	memcpy( pkt_buf + first_frag_offset, skb->data, first_frag_len);
    
	*ptr = pkt_buf + first_frag_offset;

	//debug.
	//printk("> skb->len:%u  frags:%u\n", skb->len,  pinfo->map_fr_num);	

    return skb->len;	
}

void record_packet_timestamp(unsigned char *data, unsigned int len, unsigned long now);

int tx_iperf3_rec_timestamp(struct sk_buff *skb)
{
    unsigned int len;   
    struct iphdr *iph;
    unsigned char *data;
    int totLen;
    unsigned long now;

    now = jiffies;
    
    len = skb->len;
    if( len < 14 + 20 + 8 + 8 + 20 + 20)    //MAC + ip + udp + gprs + ip + tcp
    {
        return -1;
    }

    data = skb->data;
    if( (data[12] == 0x08) && (data[13] == 0x00) )
    {
        iph = (struct iphdr *)(skb->data + 14);
        if( iph->protocol != 17)
        {
            return -1;
        }

        totLen = get_t2200_tx_skb_data(skb, &data);
        if( (totLen < 0) || (data == NULL))
        {
            return -1;
        } 

        record_packet_timestamp(data, totLen, now);
        return 0;
    }
    else
    {
        return -1;
    }
    
    return 0;    
}

void record_packet_timestamp(unsigned char *data, unsigned int len, unsigned long  now)
{    
    static unsigned long pre_jiffes=0;
    static int pktcnt=0;

    struct iphdr *iph;
    struct iphdr *iph2;
    struct udphdr *uh1;
    struct tcphdr *th;    
    //unsigned long now;
    unsigned int first_ack_no = 0;
    
    //len = skb->len;
    if( len < 14 + 20 + 8 + 8 + 20 + 20)    //MAC + ip + udp + gprs + ip + tcp
    {
        return;
    }
   
    //now = jiffies;

    //data = skb->data;
    if( (data[12] == 0x08) && (data[13] == 0x00) )
    {
        iph = (struct iphdr *)(data + 14);
        if( iph->protocol != 17)
        {
            return;
        }
        
        //first UDP header.
        uh1 = (struct udphdr *)( ((char *)iph) + (iph->ihl*4));
        
        if( (uh1->source == htons(2152)) && (uh1->dest== htons(2152)) )
        {    
            //GTP GPRS.
            iph2 = (struct iphdr *)(((char *)uh1) + 16);
            
            if( iph2->protocol == 6 ) //TCP
            {
                th = (struct tcphdr *)(((char *)iph2) + (iph2->ihl<<2)) ;
                
                if( (th->dest == htons(5201)) ||  (th->source == htons(5201))) //iperf3
                {
                    if( th->syn && th->ack)
                    {
                        first_ack_no = ntohl(th->ack_seq);
                        pre_jiffes = now;
                        
                        printk(">>Start, ack_seq=%x start_jiffies=%lx\n", first_ack_no, now );
                        return;
                    }
                    
                    if( now - pre_jiffes > 20)  //could be first packet.
                    {
                        printk(">> Suspection: DeltaTime:%lu , pre_jiffes=%lx  now=%lx ack_seq:%x\n", 
                            now - pre_jiffes, 
                            pre_jiffes,
                            now,
                            ntohl(th->ack_seq) );
                    }
                    
                    pre_jiffes = now;                    

                    //debug                    
                    //if( (pktcnt%3000) == 0)
                    //{
                        //printk(">> DBG iperf3: prev_jiffies=%lu  now=%lu.\n", pre_jiffes, now);
                    //}
                    pktcnt++;
                }     
                else
                {                   
                     //debug.                     
                     //printk("th->dest=%d\n", ntohs(th->dest));                      
                }
            } 
            else
            {         
                #if 0
                printk("\n\nskb->data: %p uh1:%p iph2:%p \n", (char *)data,  (char *)uh1, (char *)iph2 );
                printk("\n\nFirst IpHdr:%u  cal:%u\n", (iph->ihl*4), (unsigned int)uh1 - (unsigned int)iph );

                #if 0
                {
                    int t=0;
                    for( t=14; t<len; t++)
                    {
                        printk(">>%.2x ", skb->data[t]&0xFF);    
                    }
                }
                #endif
                
                printk("iph2->protocol=%d sip=%x dip=%x\n", iph2->protocol, iph2->saddr, iph2->daddr);
                #endif
                 
            }
        }
        else
        {
            //printk(">>Not GTPU..");
            return;
        }
    }
    else
    {
        return;
    }    
}

////////////////////////////////////////////////////////////////

//assume: packet was TCP ACK,  from BS station to server.
//        not GTPU frame, the original mac + ip + tcp.
int tx_bs_iperf3_rec_timestamp(struct sk_buff *skb)
{
    unsigned int len;   
    struct iphdr *iph;
    unsigned char *data;
    struct tcphdr *th; 
    
    static unsigned long pre_jiffes=0;
    static int pktcnt=0;
    unsigned long now;
    unsigned int first_ack_no = 0;

    now = jiffies;
    
    len = skb->len;
    if( len < 14 + 20 + 20 )    //MAC + ip + tcp
    {
        return -1;
    }

    data = skb->data;
    if( (data[12] == 0x08) && (data[13] == 0x00) )
    {
        iph = (struct iphdr *)(skb->data + 14);
        if( iph->protocol != 6)
        {
            return -1;
        }

        th = (struct tcphdr *)(((char *)iph) + (iph->ihl<<2)) ;
        
        if( (th->source == htons(5201)) || (th->dest == htons(5201)) ) //iperf3
        {
            if( th->syn && th->ack)
            {
                first_ack_no = ntohl(th->ack_seq);
                pre_jiffes = now;
                
                printk(">>Iperf3 Start, ack_seq=%x start_jiffies=%lx\n", first_ack_no, now );
                return 0;
            }
            
            if( now - pre_jiffes > 20)  //could be first packet.
            {
                printk(">> Suspection: DeltaTime:%lu , pre_jiffes=%lx  now=%lx ack_seq:%x\n", 
                    now - pre_jiffes, 
                    pre_jiffes,
                    now,
                    ntohl(th->ack_seq) );
            }
            
            pre_jiffes = now;                    

            //debug                    
            //if( (pktcnt%5000) == 0)
            //{
                //printk(">> DBG iperf3: prev_jiffies=%lu  now=%lu.\n", pre_jiffes, now);
            //}
            pktcnt++;
        }     
        else
        {                   
             //debug.                     
             //printk("th->dest=%d Not iperf3\n", ntohs(th->dest));                      
        }
    } 

    return 0;    
}
    


