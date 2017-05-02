#include <p30f4011.h>
#include "UART.h"
#include "DISPLAY.h"
#include "tiempo.h"
#include "MUX.h"
#include "XMODEM.h"

#define	SEG_0	0b00111111
#define	SEG_1	0b00000110
#define	SEG_2	0b01011011
#define	SEG_3	0b01001111
#define	SEG_4	0b01100110
#define	SEG_5	0b01101101
#define	SEG_6	0b01111101
#define	SEG_7	0b00000111
#define	SEG_8	0b01111111
#define	SEG_9	0b01101111


void init_UART(void);
void Init_Timer(void);
#define UART_STATUS IFS1 
#define UART_BANDERA 0x100
#define REGISTRO_UART U2RXREG  
#define BAUD_RATE 0X33
#define MAX_PERIODICOS    4
#define MAX_TIMEOUT       3
#define TIMERSTAT IFS0
#define TIMER_TC 0X80
#define NUM_DISPLAYS 	3
#define FREC_DS 		2
#define PERIODICO_DS 	0
#define PERIODICO_MUX				1
#define FREC_MUX 					1
#define PERIODICO_INTERMITENTE_MUX	2
#define FREC_INTERMITENTE_MUX       240

#define TIPO			128
#define XM_TIMEOUT_1_MIN	0
#define XM_TIMEOUT_1_SEG	1

Tm_Control t_control;

char Atienda_timer (char Attention){
	if (TIMERSTAT & TIMER_TC){
		if (Attention)	TIMERSTAT &= ~TIMER_TC;
    return YES;
    } 
    else	return NO;
};

char Atender_uart (char Attention){
	if (Attention){
	    if(UART_STATUS & UART_BANDERA){
			IFS1bits.U2RXIF=0;
		    return 1;
		}
		else	return 0;
	} 
    else	return REGISTRO_UART;  
};
UART_Ctrl  control_uart; 
DS_Ctrl controlds;
unsigned char cp[NUM_DISPLAYS]={0,0,0};
MUX_Ctrl controlmux;
Tm_Periodico t_periodico[MAX_PERIODICOS];
Tm_Timeout t_timeout[MAX_TIMEOUT]; 
XM_Ctrl controlxm;
XM_Packet vector_paquetes[2];
unsigned char XM_Buffer1[TIPO], XM_Buffer2[TIPO];

int main (void){	  
ADPCFG=0X1FF;
TRISB=0X0;
PORTB=0x1FF;
TRISE=0X0;
PORTE=0X0;
init_UART();
UART_Init(&control_uart,BAUD_RATE,2, &Atender_uart );
Init_Timer();
Tm_Defina_control(&t_control, t_periodico, MAX_PERIODICOS, t_timeout,
                    MAX_TIMEOUT, &Atienda_timer);
XM_Init(&controlxm, vector_paquetes,&t_control,&control_uart,XM_TIMEOUT_1_MIN,
					   XM_TIMEOUT_1_SEG,XM_Buffer1, XM_Buffer2, TIPO);
DS_Init(&controlds, cp, &controlxm, PERIODICO_DS, 
	  					FREC_DS, NUM_DISPLAYS);
unsigned char P_MUX_Table[12]={~SEG_0, ~SEG_1, ~SEG_2, ~SEG_3, ~SEG_4, 
									 	 ~SEG_5, ~SEG_6, ~SEG_7, ~SEG_8, ~SEG_9, ~0b0100000, ~0b0100000}; 
MUX_Init(&controlmux, &t_control, PERIODICO_MUX,
	  					 PERIODICO_INTERMITENTE_MUX, FREC_MUX, 
	  					 FREC_INTERMITENTE_MUX,&controlds, P_MUX_Table);
Tm_Inicie_periodico(&t_control,3,60);	   
	while(YES){
		if ( Atienda_timer(NO) ){    	
	      	Tm_Procese_tiempo(&t_control, t_periodico, MAX_PERIODICOS);
	 	}     	
	    MUX_Flashing(&controlmux);
		XM_State_Machine(&controlxm);	
	    DS_Function(&controlds);	
   	};    
	 return 0;
  };


