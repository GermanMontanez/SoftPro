#if !defined(XMODEM_H)
#define XMODEM_H

#include "tiempo.h"
#include "UART.h"

// XMODEM TIMEOUT BASE 1 SEC & 10 SEC
#define XM_TO_1SEC_BASE		 				0xF0
#define XM_TO_10SEC_BASE 					0x967

#define XM_MAXIMUM_ERROR 					0xA

# define XM_P_SOH							0x01
# define XM_P_EOT							0x04
# define XM_P_ACK							0x06
# define XM_P_NAK							0x15
# define XM_P_CAN							0x18

# define XM_STATE_WAIT4_START				0x0
# define XM_STATE_WAIT4_PACKET				0x1
# define XM_STATE_WAIT4_SECUENCES		    0x2
# define XM_STATE_WAIT4_SECUENCES_COMP 		0x3
# define XM_STATE_WAIT4_CHKSUM				0x4
# define XM_STATE_IGNORE_DATA				0x5
# define XM_STATE_DATA_RX					0x6
# define XM_STATE_DISCARD_TX				0x7

#define XM_FLAG_ERROR_ACK       			0X1
#define XM_FLAG_ERROR_NAK					0x2
#define XM_FLAG_OK          				0x4

#define XM_FLAG_SOT      					0x10
#define XM_FLAG_CHECKSUM 					0x20
#define XM_FLAG_READ						0X40
#define XM_FLAG_EOT							0x80
#define XM_FLAG_DESCARD						0x8
#define XM_FLAG_ENABLE						0x4

typedef struct XM_Packet XM_Packet; 
typedef struct XM_Ctrl XM_Ctrl;
// ************* XMODEM PACKET STRUCTURE ***************
struct XM_Packet{
unsigned char Type;
unsigned char *dp;
unsigned char Flag;
unsigned char Blk_Num;
unsigned char CheckSum;
unsigned char Data_Counts;
};
 
// **************** XMODEM CONTROL STRUCTURE *****************
struct XM_Ctrl{
unsigned char Packet_Counts; 
XM_Packet *P_XM_Packet;
unsigned char Packet_Writer;
unsigned char Packet_Reader;
unsigned char State,
			Error_Counts;
unsigned char Flags;
Tm_Control *P_XM_TO;
Tm_Num XM_TO_10Sec;
Tm_Num XM_TO_1Sec;
UART_Ctrl *P_UART_Ctrl;
};
// *************** XMODEM FUNCTIONS PROTOTYPES *****************
void XM_Init(XM_Ctrl *P_XM_Ctrl, XM_Packet *P_XM_Packet,
					   Tm_Control *P_XM_TO,UART_Ctrl *P_UART_Ctrl,Tm_Num XM_TO_10Sec,
					   Tm_Num XM_TO_1Sec,unsigned char *XM_Buffer1, unsigned char *XM_Buffer2,
					   char Type);
void XM_State_Machine(XM_Ctrl *P_XM_Ctrl);
#endif
