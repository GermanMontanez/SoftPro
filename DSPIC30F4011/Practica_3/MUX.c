#include <p30f4011.h>
#include "MUX.h"
#include <math.h>

char German =0;
void MUX_Flashing(MUX_Ctrl *P_MUX_Ctrl){
	if(!(P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->Flags&XM_FLAG_READ) && (P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->Flags & XM_FLAG_ENABLE)){
		MUX_Enable(P_MUX_Ctrl,0);
		return;
	};
	if ((P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->Flags & XM_FLAG_READ)){
		MUX_Function(P_MUX_Ctrl);
		Tm_Inicie_timeout(P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,2,14400);
	}	
	else{
		if(P_MUX_Ctrl->P_DS_Ctrl->Flashing){
			MUX_Function(P_MUX_Ctrl);
			German =1;
		}	
		else	MUX_Enable(P_MUX_Ctrl,0);
	};
	if(Tm_Hubo_periodico(P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,P_MUX_Ctrl->Num_Flash)){
		P_MUX_Ctrl->P_DS_Ctrl->Flashing = ~(P_MUX_Ctrl->P_DS_Ctrl->Flashing); 
		Tm_Baje_periodico(P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,P_MUX_Ctrl->Num_Flash);
	};
		if(Tm_Hubo_timeout(P_MUX_Ctrl->P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,2) && German==1){
			P_MUX_Ctrl->Num_Flash=3;
		};
};	

void MUX_Function(MUX_Ctrl *P_MUX_Ctrl){
	if(Tm_Hubo_periodico(P_MUX_Ctrl->P_XM_TO,P_MUX_Ctrl->TM_Counts)){	
		static char DS_Selector=1;
		MUX_Enable(P_MUX_Ctrl,0);
		PORTB =P_MUX_Ctrl->P_MUX_Table[P_MUX_Ctrl->P_DS_Ctrl->dp[DS_Selector-1]];
		MUX_Enable(P_MUX_Ctrl,DS_Selector);
		if(DS_Selector>=(P_MUX_Ctrl->P_DS_Ctrl->Display_Number))	DS_Selector=1;	
		else	DS_Selector+=1;	
	Tm_Baje_periodico(P_MUX_Ctrl->P_XM_TO,P_MUX_Ctrl->TM_Counts);
	};		
};

void MUX_Init(MUX_Ctrl *P_MUX_Ctrl, Tm_Control *P_XM_TO, Tm_Num TM_Counts,
						Tm_Num Num_Flash,Tm_Contador TM_Period,
						Tm_Contador Flash_Period,DS_Ctrl *P_DS_Ctrl, 
						unsigned char *P_MUX_Table){
P_MUX_Ctrl->P_XM_TO=P_XM_TO;
P_MUX_Ctrl->TM_Counts=TM_Counts;
P_MUX_Ctrl->Num_Flash=Num_Flash;
P_MUX_Ctrl->TM_Period=TM_Period;
P_MUX_Ctrl->Flash_Period=Flash_Period;
P_MUX_Ctrl->P_DS_Ctrl=P_DS_Ctrl;
P_MUX_Ctrl->P_MUX_Table=P_MUX_Table;
Tm_Inicie_periodico(P_XM_TO,TM_Counts,TM_Period);
Tm_Inicie_periodico(P_XM_TO,Num_Flash,Flash_Period);
MUX_Enable(P_MUX_Ctrl,0);
};
void MUX_Enable(MUX_Ctrl *P_MUX_Ctrl,char Active_Display){
char nd=Active_Display-1;
	if (!Active_Display){
		PORTE=0b11111000;
	}
	else{
		PORTE=(pow(2,nd));
	};
};	
