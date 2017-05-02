#if !defined(XMODEM_H)
#define XMODEM_H

#include "tiempo.h"
#include "uart.h"
#include "buffer.h"

#define XM_SOH 1
#define XM_NAK 21
#define XM_EOT 4
#define XM_CAN 24
#define XM_ACK 6

#define XM_MAX_TRY       10
#define XM_1S		 240
#define XM_1M 		 2407
#define XM_DAT_INIT	 0	
//flags
#define XM_READY          	0x4
#define XM_F_E_ACK      0X1
#define XM_F_E_NAK	0x2
#define XM_F_SOT        0x10 
#define XM_F_CKSUM      0x20 
#define XM_F_READ	0X40
#define XM_F_EOT	0x80
#define XM_F_CANCEL	0x8
#define XM_F_ENABLE	0x4


# define XM_E_INIT                      0
# define XM_E_WAIT_FOR_BLK 	    	1
# define XM_E_WAIT_FOR_SEC              2
# define XM_E_WAIT_FOR_NSEC 		3
# define XM_E_DISCART                   4
# define XM_E_RDATA			5
# define XM_E_ABORT			6
# define XM_E_WAIT_FOR_CKSUM		7



typedef struct Xm_pack Xm_pack;
typedef struct Xm_Control Xm_Control;



struct Xm_pack
{
	unsigned char size;
	unsigned char *buff_ds;
	unsigned char flags;
	unsigned char n_blk;  
	unsigned char cksum;
	unsigned char data_cnt;
};

struct Xm_Control
{
	unsigned char seq_pack; 
	Xm_pack *bf_index;
	unsigned char bf_write;
	unsigned char bf_read; 
	
	unsigned char flag;
	Tm_Control *ctp;
	Tm_Num to_1m;
	Tm_Num to_1s;

        Uart_Control *Ctrl_uart;
	Buffer *bf_uart;
        unsigned char state;
	unsigned char   try_cnt;
};


void XM_Control_Init(Xm_Control *cxp, Xm_pack *apuntador_paquete,
        Tm_Control *ctp,Uart_Control *cup,Tm_Num timeout_1_min,
					   Tm_Num timeout_1_seg,unsigned char *buffer1, unsigned char *buffer2,
					   char tipo,Buffer *bf_uart);

void XM_Process(Xm_Control *cxp);


void XM_Read_Data(Xm_Control *cxp);
void XM_Discard(Xm_Control *cxp);
void XM_Protocol_init(Xm_Control *cxp);
void XM_waitfor_pack(Xm_Control *cxp);
void XM_waitfor_seq(Xm_Control *cxp);
void XM_waitfornseq(Xm_Control *cxp);
void XM_Abort(Xm_Control *cxp);
void XM_waitfor_cksum(Xm_Control *cxp);

#endif

