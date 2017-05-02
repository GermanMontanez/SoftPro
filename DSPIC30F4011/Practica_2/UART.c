//UART Configuration Module for dsPIC30F4011
#include <p30f4011.h>
#include "UART.h"

void UART_Init(void){
	U2BRG = 47;
	U2MODEbits.PDSEL = 0b00;
	U2MODEbits.STSEL = 0;
	U2STAbits.URXISEL = 1;
	U2MODEbits.UARTEN = 1;
	U2STAbits.UTXEN = 1;
};
	
unsigned char UART_Rx(void){
	if (IFS1bits.U2RXIF){
		IFS1bits.U2RXIF = 0;
		return U2RXREG;
	};
return(0);
};

void UART_Tx(UART_Data XX){
	if(IFS1bits.U2TXIF){
	U2TXREG = XX;
	IFS1bits.U2TXIF = 0;
};
};

