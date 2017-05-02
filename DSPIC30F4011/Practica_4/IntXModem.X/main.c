#include <p30f4011.h>
#include "uart.h"
#include "display.h"
#include "buffer.h"
#include "tiempo.h"
#include "multiplexor.h"
#include "xmodem.h"

#define MAX_PERIODICOS    4
#define MAX_TIMEOUT       4
#define TIMERSTAT IFS0
#define TIMER_TC 0X80

#define	SEG_0	0b00111111
#define	SEG_1	0b00001100
#define	SEG_2	0b01011011
#define	SEG_3	0b01011110
#define	SEG_4	0b01101100
#define	SEG_5	0b01110110
#define	SEG_6	0b01110111
#define	SEG_7	0b00011100
#define	SEG_8	0b01111111
#define	SEG_9	0b01111110


#define TO_REBOTE 2
#define TIEMPO_REBOTE 5



#define XM_TIMEOUT_1_MIN	0
#define XM_TIMEOUT_1_SEG	1



void Init_Timer(void)
{
	T3CONbits.TON = 0; 		
	IEC0bits.T3IE = 1;		
	IFS0bits.T3IF = 0;		
	T3CONbits.TCS = 0; 		
	PR3 = 8333;			
	T3CONbits.TON = 1;
        TMR3 = 0;
	T3CONbits.TON = 0;
	T3CONbits.TSIDL = 0;
	T3CONbits.TGATE = 0;
	T3CONbits.TCKPS = 0x0;
}

Tm_Control t_control;

void init_interrupcion()
{

	IC1CONbits.ICSIDL=0;	
	IC1CONbits.ICTMR=1;		
	IC1CONbits.ICI=0;		
	IC1CONbits.ICM=3;		

	IEC0bits.IC1IE = 1;
	IFS0bits.IC1IF=0;		

	IC2CONbits.ICSIDL=0;	
	IC2CONbits.ICTMR=1;		
	IC2CONbits.ICI=0;		
	IC2CONbits.ICM=3;		

	IEC0bits.IC2IE = 1;
	IFS0bits.IC2IF=0;		
}


char Atienda_timer (char atienda)
{
	if (TIMERSTAT & TIMER_TC)
    {
	    if (atienda)
     	TIMERSTAT &= ~TIMER_TC; 
   		return SI;
   	}
   	else
   		return NO;
}


Mux_Control controlmux;
Tm_Periodico t_periodico[MAX_PERIODICOS];
Tm_Timeout t_timeout[MAX_TIMEOUT];
Xm_Control controlxm;
Xm_pack vector_paquetes[2];
unsigned char buffer1[BF_SIZE], buffer2[BF_SIZE];

char Atender_uart (char atienda)
{
	if (atienda)
  	{
   		if(UART_STATUS & UART_FLAG)
    	{
     		IFS1bits.U2RXIF=0;
     		return 1;
 		}
 		else
 		{
	 		return 0;
 		}
	}
  	else
  	{
   		return REGISTRO_UART;
	}
}



static Buffer buffer;
static unsigned char buf[BF_SIZE_QUEUE];
Uart_Control  control_uart;
Ds_Control controlds;
unsigned char cp[DS_N]={0,0,0};

int main (void)
{
  	ADPCFG=0X1FF;
	TRISB=0X0;   
	PORTB=0x1FF; 
	TRISE=0X0;    
	PORTE=0X0;   
	TRISD=0X3;   
	BF_NewBuff(&buffer,buf,BF_SIZE_QUEUE);
	UART_init();
	Uart_Defina_control(&control_uart,BAUD_RATE,2, &Atender_uart );
	Init_Timer();
	Tm_Defina_control(&t_control, t_periodico, MAX_PERIODICOS, t_timeout,
        MAX_TIMEOUT, &Atienda_timer);
	XM_Control_Init(&controlxm, vector_paquetes,&t_control,&control_uart,XM_TIMEOUT_1_MIN, XM_TIMEOUT_1_SEG,buffer1, buffer2, BF_SIZE, &buffer);
	DS_Control_Init(&controlds, cp, &controlxm, DS_N_TP,DS_TP, DS_N);
	unsigned char tabla[12]={SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,SEG_5, SEG_6, SEG_7, SEG_8, SEG_9, ~SEG_0, ~SEG_0};
	MUX_Control_Init(&controlmux, &t_control, MUX_TP, MUX_TP_BLINK, MUX_N_TP,MUX_N_TP_BLINK,&controlds, tabla);
	init_interrupcion();
        Tm_Inicie_periodico(&t_control,3,60);
	while(SI)
	{
	Mux_Intermitente(&controlmux);
	XM_Process(&controlxm);
        DS_handle(&controlds);
	}
return 0;
}


void __attribute__((__interrupt__)) _U2RXInterrupt(void)
{
	IFS1bits.U2RXIF=0;
	unsigned char data;
	data=Uart_Lectura_dato(&control_uart);
	BF_WriteBF(&buffer,data);
}

void __attribute__((__interrupt__)) _T3Interrupt(void)
{
	Tm_Procese_tiempo(&t_control, t_periodico, MAX_PERIODICOS);
}

void __attribute__((__interrupt__)) _IC1Interrupt(void)
{
	IFS0bits.IC1IF=0;
	if(!Tm_Hubo_timeout(&t_control, TO_REBOTE))
	{
		return;
	}
	Tm_Inicie_timeout (&t_control,TO_REBOTE, TIEMPO_REBOTE);
	if(controlds.v_muestra<5)
	{
		(controlds.v_muestra)++;
	}
	Tm_Inicie_periodico(&t_control,controlds.num,(controlds.periodo)*(controlds.v_muestra));


	return;
}

void __attribute__((__interrupt__)) _IC2Interrupt(void)
{
	IFS0bits.IC2IF=0;
	if(!Tm_Hubo_timeout(&t_control, TO_REBOTE))
	{
		return;
	}
	Tm_Inicie_timeout (&t_control,TO_REBOTE, TIEMPO_REBOTE);
	if(controlds.v_muestra>1)
	{
		(controlds.v_muestra)--;
	}
	Tm_Inicie_periodico(&t_control,controlds.num,(controlds.periodo)*(controlds.v_muestra));

	return;
}





