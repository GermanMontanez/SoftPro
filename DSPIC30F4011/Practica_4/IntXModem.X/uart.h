# if !defined(UART_H)
# define UART_H
# define SI 1
# define NO 0

#define BAUD_RATE 0x47
#define UART_STATUS IFS1    
#define REGISTRO_UART U2RXREG
#define UART_FLAG 0x100

typedef unsigned int Uart_Baudrate;
typedef unsigned char Uart_Data;
typedef unsigned char Uart_Flags;



typedef char Uart_Atender_uart(char atienda); 

typedef struct Uart_Receptor Uart_Receptor;
struct Uart_Receptor
{
	Uart_Data data;
	Uart_Flags flags;
};


typedef struct Uart_Control Uart_Control;
struct Uart_Control
{
	
	Uart_Baudrate baudrate;
	unsigned char selector_uart;
        Uart_Atender_uart  *atienda_uart;

};



void UART_init(void);
void UART_Control_Init(Uart_Control *cup, Uart_Baudrate baudrate,unsigned char selector_uart, Uart_Atender_uart  *atienda_uart );
Uart_Data UART_Read(Uart_Control *cup );
Uart_Flags UART_check(Uart_Control *cup);
void UART_write(unsigned char dato );


# endif
