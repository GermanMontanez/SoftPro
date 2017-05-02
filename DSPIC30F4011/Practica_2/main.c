#include <p30f4011.h>
#include "Buffer.h"
#include "UART.h"
#include "display.h"
#include "tiempo.h"
#include <stdio.h>


#define BufferSize	64

#define MAX_PERIODICO  3
#define MAX_TIMEOUT 3
#define TP_OP 0
#define TO_10S 0



Buffer MyBuffer;
Buff_Data MyBuffVector[BufferSize]; 

Tm_Control t_control;
static Tm_Periodico t_periodico[MAX_PERIODICO];
static Tm_Timeout t_timeout[MAX_TIMEOUT];

Display MyDisplay;

unsigned char Atienda_timer (char atienda);

unsigned char Table_7SEG[16]={SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7, SEG_8, SEG_9,
						      SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F};

							  
							  
int main(){
	ADPCFG = 0b111111111;
	TRISB = 0b000000000;
	PORTB = 0b000000000;
	TMR3 = 0;
	PR3 = 12500;
	T3CONbits.TCKPS = 0b00;
	T3CONbits.TCS = 0;
	T3CONbits.TON = 1;
	IFS0bits.T3IF = 0;
	IFS1bits.U2RXIF=0;
	UART_Init();
	Buff_Init(&MyBuffer, MyBuffVector);
	Ds_Init(&MyDisplay);
	Tm_Defina_control(&t_control, t_periodico, MAX_PERIODICO, t_timeout, MAX_TIMEOUT, &Atienda_timer);
	
	Tm_Inicie_periodico (&t_control, TP_OP, 1);
	Tm_Inicie_timeout (&t_control, TO_10S, 3200);

	
	
	while(1){
	
		if(Atienda_timer (NO)){
								Tm_Procese_tiempo (&t_control,t_periodico,MAX_PERIODICO);
								};
		if(MyBuffer.Buff_Stored<MyBuffer.size){
                   
                    Buff_XON_OFF(&MyBuffer);
                    UART_Data MyUARTData = (UART_Rx());
						
                    if(MyUARTData){
                        Tm_Inicie_timeout (&t_control, TO_10S, 3200);
                        Buff_Load(&MyBuffer, MyUARTData);
                    };
                    
						
                   				};

//(Tm_Hubo_periodico(&t_control,TP_OP))&&(MyBuffer.Buff_Stored>0)
                 if((Tm_Hubo_periodico(&t_control,TP_OP))&&(MyBuffer.Buff_Stored>0)){

                     Tm_Baje_periodico(&t_control,TP_OP);

                     if(!(MyDisplay.DS_Flag&TM_2S)){
                        Ds_Procese(&MyDisplay,&MyBuffer,&t_control);
                     };



                     if(MyDisplay.DS_Flag&TM_2S){
                         PORTB=0;
                     }else{
                     

                        if((MyDisplay.DS_Flag&TM_5S_TP_2HZ)&&(!(MyDisplay.DS_Flag&ON_OFF))&&(Tm_Hubo_periodico(&t_control,TP_SHA))){
                            //Ds_Procese(&MyDisplay,&MyBuffer,&t_control);
                            Tm_Baje_periodico(&t_control,TP_SHA);
                            MyDisplay.DS_In&=0xf;
                            PORTB=Table_7SEG[MyDisplay.DS_In];
                            MyDisplay.DS_Flag|= ON_OFF;

                        }else if((MyDisplay.DS_Flag&TM_5S_TP_2HZ)&&((MyDisplay.DS_Flag&ON_OFF))&&(Tm_Hubo_periodico(&t_control,TP_SHA))){
                            //Ds_Procese(&MyDisplay,&MyBuffer,&t_control);
                            Tm_Baje_periodico(&t_control,TP_SHA);
                            PORTB=0;
                            MyDisplay.DS_Flag&= (~ON_OFF);
                           

                        }else if(!(MyDisplay.DS_Flag&TM_5S_TP_2HZ)){
                            //Ds_Procese(&MyDisplay,&MyBuffer,&t_control);

                            MyDisplay.DS_In&=0xf;
                            PORTB=Table_7SEG[MyDisplay.DS_In];
                        };

                     };

                     if(Tm_Hubo_timeout(&t_control,TO_SHA)){

                         MyDisplay.DS_Flag&= ~TM_5S_TP_2HZ;

                     }

                     if(Tm_Hubo_timeout(&t_control,TO_PER)){

                         MyDisplay.DS_Flag&= ~TM_2S;

                     };


                 };


		if(Tm_Hubo_timeout (&t_control, 0)){
						
						
                    
                                                PORTB=SEG_DSH;

					//	break;
			};

	

	}; 
};



unsigned char Atienda_timer (char atienda) {
  if (IFS0bits.T3IF){
    if (atienda)
        IFS0bits.T3IF = 0; 
		return SI;
    } 
		else
		return NO;
return 0;
};
