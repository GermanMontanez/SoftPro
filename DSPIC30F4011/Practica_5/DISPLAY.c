#include <p30f4011.h>
#include "Display.h"
extern DS_Ctrl	*P_DS_Ctrl;

typedef struct Tcb Tcb;
struct Tcb{
	void *sp;
	void *fp;
};
extern Tcb tcb_ppal;


void DS_Function(DS_Ctrl *P_DS_Ctrl){
	while(1){
	if(P_DS_Ctrl->P_XM_Ctrl->Flags&XM_FLAG_DESCARD){
		P_DS_Ctrl->dp[0]=11;
		P_DS_Ctrl->dp[1]=11;
		P_DS_Ctrl->dp[2]=10;
		return;
	};	
	if (Tm_Hubo_periodico(P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,P_DS_Ctrl->TM_Counts)){
		Tm_Baje_periodico(P_DS_Ctrl->P_XM_Ctrl->P_XM_TO,P_DS_Ctrl->TM_Counts);
		XM_Packet *P_XM_Packet_Read;
		P_XM_Packet_Read = P_DS_Ctrl->P_XM_Ctrl->P_XM_Packet + P_DS_Ctrl->P_XM_Ctrl->Packet_Reader;	
		if (P_DS_Ctrl->P_XM_Ctrl->Flags & XM_FLAG_READ){
			DS_Conversion(P_DS_Ctrl);
			(P_XM_Packet_Read->Data_Counts)++;
			if(P_XM_Packet_Read->Data_Counts >= P_XM_Packet_Read->Type){
				(P_DS_Ctrl->Secuences)++;
				P_XM_Packet_Read->Data_Counts = 0;
				P_XM_Packet_Read->Flag &= ~XM_FLAG_OK;
				P_DS_Ctrl-> P_XM_Ctrl ->Packet_Reader++;
					if(P_DS_Ctrl-> P_XM_Ctrl ->Packet_Reader >= 2){
						P_DS_Ctrl-> P_XM_Ctrl ->Packet_Reader = 0;
					};
				P_XM_Packet_Read = P_DS_Ctrl->P_XM_Ctrl->P_XM_Packet + P_DS_Ctrl->P_XM_Ctrl->Packet_Reader;
			if (P_XM_Packet_Read->Flag & XM_FLAG_OK){
				DS_Conversion(P_DS_Ctrl);
				(P_XM_Packet_Read->Data_Counts)++;	
			};	
			};		
		};	
	if (((P_XM_Packet_Read->Flag) & XM_FLAG_OK)&&
		 (P_XM_Packet_Read->Blk_Num== P_DS_Ctrl->Secuences)){
		P_DS_Ctrl->P_XM_Ctrl->Flags|=XM_FLAG_READ;
	}
	else{
		if(P_DS_Ctrl->P_XM_Ctrl->Flags&XM_FLAG_EOT){
			P_DS_Ctrl->P_XM_Ctrl->Flags&=~XM_FLAG_EOT;
			P_DS_Ctrl->Secuences=1;
		};	
		P_DS_Ctrl->P_XM_Ctrl->Flags &= ~XM_FLAG_READ;
	};
	};
		IEC1bits.U2RXIE=0;
		IEC0&=~INTERRUPT_ON;
		CR_Execute(&tcb_ppal);
		IEC1bits.U2RXIE=1;
		IEC0|=INTERRUPT_ON;
	};
};

void DS_Conversion(DS_Ctrl *P_DS_Ctrl){
XM_Packet *XM_Packet_Read;
unsigned char Data_Buffer;
XM_Packet_Read = P_DS_Ctrl->P_XM_Ctrl->P_XM_Packet + (P_DS_Ctrl->P_XM_Ctrl->Packet_Reader);
Data_Buffer = XM_Packet_Read->dp[XM_Packet_Read->Data_Counts];
char j=P_DS_Ctrl->Display_Number;
unsigned char  i=0;
unsigned int pow=1;	
	while(j){	
		unsigned char op = Data_Buffer/pow;
		char rta=0;
		if (op >=10)	rta = (op)%10;
		else	rta = (op);
		P_DS_Ctrl->dp[i]=rta;
		i++;
		j--;
		pow*=10;
	};
};

void DS_Init(DS_Ctrl *P_DS_Ctrl,DS_Data *dp, XM_Ctrl *P_XM_Ctrl,
					   	Tm_Num TM_Counts,
						Tm_Contador TM_Period,char Display_Number){
P_DS_Ctrl->TM_Counts=TM_Counts;
P_DS_Ctrl->TM_Period=TM_Period;
P_DS_Ctrl->dp=dp;
P_DS_Ctrl->Display_Number=Display_Number;
P_DS_Ctrl->P_XM_Ctrl=P_XM_Ctrl;
P_DS_Ctrl->Secuences=1;
P_DS_Ctrl->Flashing = 0; 
Tm_Inicie_periodico(P_DS_Ctrl->P_XM_Ctrl->P_XM_TO, TM_Counts, TM_Period);
};	
