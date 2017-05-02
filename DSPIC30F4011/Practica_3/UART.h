# if !defined(UART_H)
# define UART_H
# define YES 1
# define NO 0
typedef unsigned char UART_Data;
typedef unsigned char UART_Index;
typedef unsigned int UART_BaudRate;
typedef char UART_PayAttention(char Attention);
typedef struct{
UART_PayAttention  *P_UART_Attent; 
UART_BaudRate MyBaudRate;
unsigned char UART_Select; 
}UART_Ctrl;
void UART_Init(UART_Ctrl *P_UART_Ctrl, UART_BaudRate MyBaudRate,unsigned char UART_Select, UART_PayAttention  *UART_Attention);
UART_Data UART_Rx(UART_Ctrl *P_UART_Ctrl);
UART_Index UART_Data_Status(UART_Ctrl *P_UART_Ctrl);
void UART_Tx(UART_Ctrl *P_UART_Ctrl, unsigned char Data);
# endif
