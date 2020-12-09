#ifndef DEV_IOC_H
#define DEV_IOC_H

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

struct bttmon_ioc_stru
{
    int cmd;   
    
    union
    {
        struct hook_cmd hook;
        struct tuple tup;   //TCP_seq_analyze
    }u;
};

#define BTTMON_IOC_MAGIC 'P'

//type, nr, size
#define BTTMON_IOC_TM   _IOWR( BTTMON_IOC_MAGIC, 0x1, struct bttmon_ioc_stru)

enum{
    CMD_SET_TUPLE = 0,
    CMD_GET_TUPLE = 1,
    CMD_GET_RESULT = 2,    

    CMD_FEATURE_SET_RX = 3,
    CMD_FEATURE_SET_TX = 4,
    
};

#endif

