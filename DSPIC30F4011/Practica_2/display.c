#include "display.h"
#include "Buffer.h"
#include "tiempo.h"
void Ds_Init(Display *P_Display){
P_Display->DS_Flag = 0;
};

void Ds_Procese(Display *P_Display, Buffer *P_Buffer, Tm_Control *ctp){
char x=0;
while(!x){
P_Display->DS_In = Buff_Unload(P_Buffer);
Dis_Data Display_Input = P_Display->DS_In;

	switch(Display_Input) {
		case SIGN_DOLLAR:
			P_Display->DS_Flag &= ~COMPLEMENTE;
			break;
		
		case SIGN_AMPERSAND:
			P_Display->DS_Flag |= COMPLEMENTE;
			break;
		
		case SIGN_PERCENT:
			P_Display->DS_Flag |= TM_2S;
			Tm_Inicie_timeout (ctp, TO_PER, 320);
			break;
		
		case SIGN_SHARP:
			P_Display->DS_Flag |= TM_5S_TP_2HZ;
			P_Display->DS_Flag |= ON_OFF;
			Tm_Inicie_timeout (ctp, TO_SHA, 1600);
			Tm_Inicie_periodico (ctp, TP_SHA, 80);
			break;
		default:
		x=1;
		};
	};
	
	if((P_Display->DS_Flag)&COMPLEMENTE){
	
	P_Display->DS_In = (~P_Display->DS_In);
	
	} else {};
	
	
	
};


