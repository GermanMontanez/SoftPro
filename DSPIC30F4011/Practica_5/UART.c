#include <p30f4011.h>
#include "UART.h"
void UART_Tx(UART_Ctrl *P_UART_Ctrl,unsigned char Data ){
U2TXREG=Data;
return;
};	

UART_Data UART_Rx(UART_Ctrl *P_UART_Ctrl){
return P_UART_Ctrl->P_UART_Attent(NO);
};

UART_Index UART_Data_Status(UART_Ctrl *P_UART_Ctrl){
return P_UART_Ctrl->P_UART_Attent(YES);  
};	

void UART_Init(UART_Ctrl *P_UART_Ctrl, UART_BaudRate MyBaudRate,
						unsigned char UART_Select, UART_PayAttention  *UART_Attention){
P_UART_Ctrl->MyBaudRate=MyBaudRate;
P_UART_Ctrl->UART_Select=UART_Select;
P_UART_Ctrl->P_UART_Attent=UART_Attention;
return;
};

void init_UART(void){
U2MODEbits.UARTEN=0;
U2STAbits.UTXEN=0;
U2BRG = 0X33;
U2MODEbits.PDSEL=0;
U2MODEbits.STSEL=0;
IFS1bits.U2RXIF=0;
IEC1bits.U2RXIE=0;
U2STAbits.URXISEL=1;
U2MODEbits.UARTEN=1;
U2STAbits.UTXEN=1;
};
