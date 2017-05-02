#include <p30F4011.h>
#include "uart.h"

void UART_init(void)
{
	U2MODEbits.UARTEN=0;
	U2STAbits.UTXEN=0;
	U2BRG = 47;
	U2STAbits.URXISEL=1;
	U2MODEbits.UARTEN=1;
	U2STAbits.UTXEN=1;
        U2MODEbits.PDSEL=0;
	U2MODEbits.STSEL=0;
	IFS1bits.U2RXIF=0;
	IEC1bits.U2RXIE=1;
}
void Uart_Defina_control(Uart_Control *cup, Uart_Baudrate baudrate,
						unsigned char selector_uart, Uart_Atender_uart  *atienda_uart)
{
	cup->baudrate=baudrate;
	cup->selector_uart=selector_uart;
	cup->atienda_uart=atienda_uart;

	return;
}
void UART_write(unsigned char dato )
{
	U2TXREG=dato;
	return;
};
Uart_Data Uart_Lectura_dato(Uart_Control *cup)
{

	return cup->atienda_uart(NO);
}
Uart_Flags Uart_Estado_dato(Uart_Control *cup)
{
	return cup->atienda_uart(SI);
}



