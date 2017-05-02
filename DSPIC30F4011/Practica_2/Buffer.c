// 64 Char Buffer 0.25 to 0.75 Implemented on dsPIC30F4011
#include  "BUFFER.h"
//Add UART Tx Rx Algorithm
#include "UART.h"

#define	XON	 17
#define	XOFF 19

void Buff_Init(Buffer *P_Buffer, Buff_Data *NewArray){
P_Buffer->Buff_Stored = 0;
P_Buffer->Buff_Max = 48;
P_Buffer->Buff_Min = 16;
P_Buffer->size = 64;
P_Buffer->Buff_In = 0;
P_Buffer->Buff_Out = 0;
P_Buffer->Buff_Array = NewArray;
};

void Buff_XON_OFF(Buffer *P_Buffer){
	if((P_Buffer->Buff_Stored) <= (P_Buffer->Buff_Min)){ 
		UART_Tx(XON);
	}
		else if((P_Buffer->Buff_Stored) >= (P_Buffer->Buff_Max)){ 
			UART_Tx(XOFF);
		}
};

unsigned char Buff_Load(Buffer *P_Buffer, Buff_Data NewData){
	if (!(P_Buffer->Buff_Stored >= P_Buffer->size)){
		P_Buffer->Buff_Array[P_Buffer->Buff_In] = NewData;
		Buff_Index Index_In = P_Buffer->Buff_In;
		Index_In++;
			if(Index_In >= (P_Buffer->size)) Index_In = 0;
		P_Buffer->Buff_In = Index_In;
		++(P_Buffer->Buff_Stored);
		return (1);
	};
return(0);
};

void incremente_ISalida(Buffer *P_Buffer){
Buff_Index Index_Out = P_Buffer->Buff_Out;
		Index_Out++;
			if(Index_Out >= P_Buffer->size) Index_Out=0;
		P_Buffer->Buff_Out = Index_Out;
};

Buff_Data Buff_Unload(Buffer *P_Buffer){
	if (!(P_Buffer->Buff_Stored <= 0)){
		Buff_Data Buffer_Out = P_Buffer->Buff_Array[P_Buffer->Buff_Out];
		Buff_Index Index_Out = P_Buffer->Buff_Out;
		Index_Out++;
			if(Index_Out >= P_Buffer->size) Index_Out=0;
		P_Buffer->Buff_Out = Index_Out;
	--(P_Buffer->Buff_Stored);
	return Buffer_Out;
	};
return(0);
};

