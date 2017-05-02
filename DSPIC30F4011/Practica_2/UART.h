#if !defined (UART_H)
#define UART_H
typedef unsigned char UART_Data;
//Funcion Prototype
void UART_Init(void);
unsigned char UART_Rx(void);
void UART_Tx(UART_Data XX);

#endif



