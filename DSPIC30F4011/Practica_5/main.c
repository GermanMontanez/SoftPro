#include <p30f4011.h>
#include "UART.h"
#include "Display.h"
#include "tiempo.h"
#include "MUX.h"
#include "XModem.h"
#include "BUFFER.h"

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
void Init_Interrupt();
#define UART_STATUS IFS1 
#define UART_BANDERA 0x100
#define REGISTRO_UART U2RXREG  
#define BAUD_RATE 0x33
#define MAX_PERIODICOS    4
#define MAX_TIMEOUT       3
#define TIMERSTAT IFS0
#define TIMER_TC 0x80
#define NUM_DISPLAYS 	3
#define FREC_DS 		2
#define PERIODICO_DS 	0
#define PERIODICO_MUX				1
#define FREC_MUX 					1
#define PERIODICO_INTERMITENTE_MUX	2
#define FREC_INTERMITENTE_MUX       240

#define MAX_BYTE			128
#define XM_TIMEOUT_1_MIN	0
#define XM_TIMEOUT_1_SEG	1

//Declaracion timeout de rebotes
#define TO_REBOTE 2
#define TIEMPO_REBOTE 20


// Coroutine
//DECLARATION OF COROUTINE VARIABLES
#define	LONG_STACK_XM	70
#define	LONG_STACK_PRES	70

//BUFFER
static Buffer buffer;
static unsigned char buf[5];


// ************ Coroutine Definition
typedef void Coroutine(void *Parameter);
typedef struct{
  void *sp;
  void *fp;
}Tcb;

Tcb *tcb_actual;
Tcb tcb_ppal, tcb_xm, tcb_pres;
int stack_xm[LONG_STACK_XM], stack_pres[LONG_STACK_PRES];

void CR_Init(Tcb *tcb, void *sp, Coroutine *P_CR);


// ************************
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
DS_Ctrl *P_DS_Ctrl = &controlds;
unsigned char cp[NUM_DISPLAYS]={0,0,0};
MUX_Ctrl controlmux;
Tm_Periodico t_periodico[MAX_PERIODICOS];
Tm_Timeout t_timeout[MAX_TIMEOUT]; 
XM_Ctrl controlxm;
XM_Ctrl *P_XM_Ctrl= &controlxm;
XM_Packet XM_Buffers[2];
unsigned char XM_Buffer1[MAX_BYTE], XM_Buffer2[MAX_BYTE];
void CR_Execute(Tcb *tcb);

int main (void){	  
ADPCFG=0X1FF;
TRISB=0X0;
PORTB=0x1FF;
TRISE=0X0;
PORTE=0X0;

// INT
TRISD=0x3;
// Init Buffer
Buffer_Init(&buffer, buf, 5);

init_UART();
UART_Init(&control_uart,BAUD_RATE,2, &Atender_uart );
Init_Timer();
Tm_Defina_control(&t_control, t_periodico, MAX_PERIODICOS, t_timeout,
                    MAX_TIMEOUT, &Atienda_timer);
XM_Init(&controlxm, XM_Buffers ,&t_control,&control_uart,XM_TIMEOUT_1_MIN,
					   XM_TIMEOUT_1_SEG,XM_Buffer1, XM_Buffer2, MAX_BYTE, &buffer);
DS_Init(&controlds, cp, &controlxm, PERIODICO_DS, 
	  					FREC_DS, NUM_DISPLAYS);
unsigned char P_MUX_Table[12]={~SEG_0, ~SEG_1, ~SEG_2, ~SEG_3, ~SEG_4, 
									 	 ~SEG_5, ~SEG_6, ~SEG_7, ~SEG_8, ~SEG_9, ~0b0100000, ~0b0100000}; 
MUX_Init(&controlmux, &t_control, PERIODICO_MUX,
	  					 PERIODICO_INTERMITENTE_MUX, FREC_MUX, 
	  					 FREC_INTERMITENTE_MUX,&controlds, P_MUX_Table);
// Start of Interrupt
Init_Interrupt();
// ******** START OF CORROUTINE ********** //
	//Inicializacion Corrutinas
	tcb_actual = &tcb_ppal;
	CR_Init(&tcb_xm, stack_xm, XM_State_Machine);
	CR_Init(&tcb_pres, stack_pres, DS_Function);
// ******************************************* //

Tm_Inicie_periodico(&t_control,3,60);
	   
	while(YES){
		MUX_Flashing(&controlmux);
		IEC1bits.U2RXIE=0;
		IEC0&=~INTERRUPT_ON;
		CR_Execute(&tcb_xm);
		IEC0|=INTERRUPT_ON;
		IEC0&=~INTERRUPT_ON;
		CR_Execute(&tcb_pres);
		IEC0|=INTERRUPT_ON;
		IEC1bits.U2RXIE=1;	
   	};    
	 return 0;
  };

void Init_Interrupt(){
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
};	

void CR_Execute(Tcb *tcb){
  static Tcb *tcb_t;
  tcb_t=tcb;
  asm("PUSH	SR");
  asm("PUSH	W0");
  asm("PUSH	W1");
  asm("PUSH	W2");
  asm("PUSH	W3");
  asm("PUSH	W4");
  asm("PUSH	W5");
  asm("PUSH	W6");
  asm("PUSH	W7");
  asm("PUSH	W8");
  asm("PUSH	W9");
  asm("PUSH	W10");
  asm("PUSH	W11");
  asm("PUSH	W12");
  asm("PUSH	W13");
  asm("PUSH	TBLPAG");
  asm("PUSH	RCOUNT");
  asm("PUSH	DCOUNT");
  asm("PUSH	DOSTARTL");
  asm("PUSH	DOSTARTH");
  asm("PUSH	DOENDL");
  asm("PUSH	DOENDH");
asm("PUSH CORCON");
tcb_actual->fp = WREG14; // Actualiza tcb_actual
tcb_actual->sp = WREG15;
tcb_actual=tcb_t;
WREG14 = tcb_actual->fp;
WREG15 = tcb_actual->sp;
asm("POP CORCON");
asm("POP DOENDH");
asm("POP DOENDL");
asm("POP DOSTARTH");
asm("POP DOSTARTL");
asm("POP DCOUNT");
asm("POP RCOUNT");
asm("POP TBLPAG");
asm("POP W13");
asm("POP W12");
asm("POP W11");
asm("POP W10");
asm("POP W9");
asm("POP W8");
asm("POP W7");
asm("POP W6");
asm("POP W5");
asm("POP W4");
asm("POP W3");
asm("POP W2");
asm("POP W1");
asm("POP W0");
asm("POP SR");
};

void CR_Init(Tcb *tcb, void *sp, Coroutine *P_CR){
*((Coroutine**)sp)=P_CR;
(sp)+=sizeof(Coroutine*);
(sp)+=sizeof(int*);
(sp)+=sizeof(int*);
tcb->fp=sp;
*(int*)sp = 0;
sp+=sizeof(int);
sp+=22*sizeof(int);
tcb->sp=sp;
return;
};

void __attribute__((__interrupt__)) _U2RXInterrupt(void){
	IFS1bits.U2RXIF=0;
	unsigned char Data;
	Data=UART_Rx(&control_uart);
	Buffer_Write(&buffer,Data);
};

void __attribute__((__interrupt__)) _T3Interrupt(void){
	Tm_Procese_tiempo(&t_control, t_periodico, MAX_PERIODICOS);	
};

void __attribute__((__interrupt__)) _IC1Interrupt(void){
	IFS0bits.IC1IF=0;
	if(!Tm_Hubo_timeout(&t_control, TO_REBOTE))
	{
		return;
	}	
	Tm_Inicie_timeout (&t_control,TO_REBOTE, TIEMPO_REBOTE);
	if(controlds.Speed<5)
	{
		(controlds.Speed)++;
	}
	Tm_Inicie_periodico(&t_control, controlds.TM_Counts,(controlds.TM_Period)*(controlds.Speed));
	return;
};	

void __attribute__((__interrupt__)) _IC2Interrupt(void){
	IFS0bits.IC2IF=0;
	if(!Tm_Hubo_timeout(&t_control, TO_REBOTE))
	{
		return;
	}
	Tm_Inicie_timeout (&t_control,TO_REBOTE, TIEMPO_REBOTE);
	if(controlds.Speed>1)
	{
		(controlds.Speed)--;
	}	
	Tm_Inicie_periodico(&t_control,controlds.TM_Counts,(controlds.TM_Period)*(controlds.Speed));
	return;
};
