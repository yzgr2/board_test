#include<stdio.h>
#include<linux/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//============= HOOK Function ===============
enum HOOK_FEATURE
{
    FEA_RX_TCP_SEQ = 0,
    FEA_RX_IPERF_UDP_SEQ = 1,

    FEA_RX_MAX,
    
};

enum HOOK_FEATURE_TX
{
    FEA_TX_GTP_TCP_TIMESTAMP = 0,    
    FEA_TX_BS_TCP_TIMESTAMP = 1,
    FEA_TX_MAX,    
};


struct hook_cmd
{
    int enable;
    int feature;  //HOOK_FEATURE
};
//==========================================

struct tuple
{
    unsigned int sip;
    unsigned int dip;
};

//================================
enum{
    CMD_SET_TUPLE = 0,
    CMD_GET_TUPLE = 1,
    CMD_GET_RESULT = 2,    

    CMD_FEATURE_SET_RX = 3,
    CMD_FEATURE_SET_TX = 4,
    
};

struct bttmon_ioc_stru
{
    int cmd;   
    
    union
    {
        struct hook_cmd hook;
        struct tuple tup;   //TCP_seq_analyze
    }u;
};


#define DEVNAME "/dev/tcpsmon"

#define BTTMON_IOC_MAGIC 'P'

//type, nr, size
#define BTTMON_IOC_TM   _IOWR( BTTMON_IOC_MAGIC, 0x1, struct bttmon_ioc_stru)

//=================================================================================

int fp;

void set_ip(unsigned int sip, unsigned int dip)
{
	int ret;
	struct bttmon_ioc_stru stru;
	memset(&stru, 0, sizeof(struct bttmon_ioc_stru));
	
	stru.cmd = CMD_SET_TUPLE;
	stru.u.tup.sip = sip;
	stru.u.tup.dip = dip;

	ret = ioctl(fp, BTTMON_IOC_TM, &stru ); 
	if( ret == 0)
	{
		printf("success.");
	}
	else
	{
		printf("fail.");
	}	
}

void get_result()
{
	int ret;
	struct bttmon_ioc_stru stru;
	memset(&stru, 0, sizeof(struct bttmon_ioc_stru));
	
	stru.cmd = CMD_GET_RESULT;	

	ret = ioctl(fp, BTTMON_IOC_TM, &stru ); 
	if( ret == 0)
	{
		printf("success.");
	}
	else
	{
		printf("fail.");
	}	
}

///////////////////////////////////////////////////////////

void set_feature(int feature, int ena)
{
    int ret;
	struct bttmon_ioc_stru stru;
	memset(&stru, 0, sizeof(struct bttmon_ioc_stru));
	
	stru.cmd = CMD_FEATURE_SET_RX;	
	stru.u.hook.feature = feature;
	stru.u.hook.enable = ena;

	ret = ioctl(fp, BTTMON_IOC_TM, &stru ); 
	if( ret == 0)
	{
		printf("Set Feature success. [feautre=%d enable=%d]\n",feature, ena);
	}
	else
	{
		printf("Set Feature  fail.\n");
	}	
}

void set_feature_tx(int feature, int ena)
{
    int ret;
	struct bttmon_ioc_stru stru;
	memset(&stru, 0, sizeof(struct bttmon_ioc_stru));
	
	stru.cmd = CMD_FEATURE_SET_TX;	
	stru.u.hook.feature = feature;
	stru.u.hook.enable = ena;

    printf("dbg cmd=%d \n", stru.cmd);
    
	ret = ioctl(fp, BTTMON_IOC_TM, &stru ); 
	if( ret == 0)
	{
		printf("Set Feature Tx success. [feautre=%d enable=%d]\n",feature, ena);
	}
	else
	{
		printf("Set Feature Tx fail.\n");
	}	
}


///////////////////////////////////////////////////////////


void usage(char *cmd)
{
	printf("%s -a source_ip dest_ip  : set the server ip to be monitored.\n", cmd);
	printf(" -s  : show session info and result.\n");
	printf(" -e feature  : Enable featuer. [0:RX_TCP_SEQ analyze, 服务器到基站发iperf TCP \n 1:GTPU iperf udp seq analyze (服务器到终端发iperf UDP) 2:Tx_iperf_timestamp]\n");
	printf("\t Feature 3 :\n");
	printf("\t BS as iperf3 client, send ACK to the server. the ACK packet was analyzed.\n");
	printf(" -d feature  : Disable featuer.\n");

	
}

int main(int argc, char *argv[])
{
	int show = 0, add = 0;
	char *ipaddr;	
	int feature;
	struct in_addr saddr;
	struct in_addr daddr;
	int ret;
	
	if(argc < 2)
	{
		usage(argv[0]);
		return 0;
	}
	
	if( 0 == strncmp(argv[1], "-s", 2) )
	{
		show = 1;
	}
	else if(0 == strncmp(argv[1], "-a", 2))
	{
		add = 1;
		if( argc < 4 )
		{
			usage(argv[0]);	
			return 0;
		}
		
		ret = inet_aton(argv[2], &saddr);
		ret = inet_aton(argv[3], &daddr);
		printf("sip:%s ,%x  dip:%s , %x\n", argv[2], saddr.s_addr, argv[3], daddr.s_addr );
	}
	else if(0 == strncmp(argv[1], "-e", 2))
	{
        fp = open(DEVNAME, O_RDWR);
        if( fp < 0 )
        {
            printf(">> Failed to open device.\n");
            return -1;
        }
	
	    feature = strtoul(argv[2], NULL, 0 );
	    if(feature < 2)
	    {
	        set_feature(feature, 1);
	    }
	    else
	    {
	        feature -= 2;
	        set_feature_tx(feature, 1);
	    }
	    return 0;
	}
	else if(0 == strncmp(argv[1], "-d", 2))
	{
	    fp = open(DEVNAME, O_RDWR);
        if( fp < 0 )
        {
            printf(">> Failed to open device.\n");
            return -1;
        }
        
	    feature = strtoul(argv[2], NULL, 0 );
	    if(feature < 2)
	    {
	        set_feature(feature, 0);
	    }
	    else
	    {
	        feature -= 2;
	        set_feature_tx(feature, 0);
	    }  
	    return 0;
	}
	else
	{
	    usage(argv[0]);
		return 0;
	}
	
	
	fp = open(DEVNAME, O_RDWR);
	if( fp < 0 )
	{
		printf(">> Failed to open device.\n");
		return -1;
	}
	
	if( show)
	{
		get_result();
		printf("---show result .\n");
	}
	else if(add)
	{
		printf("---set ip.\n");
		set_ip( saddr.s_addr, daddr.s_addr);
	}
	
	return 0;	
}

//arm-none-linux-gnueabi-gcc -o tcp_analyze_app tcp_analyze_app.c
