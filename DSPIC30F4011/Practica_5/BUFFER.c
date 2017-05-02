#include <p30f4011.h>
#include "Buffer.h"

void Buffer_Init(Buffer *P_Buffer,unsigned char* P_Buffer_Data,unsigned char Buffer_Cap){
P_Buffer->Counts=0;
P_Buffer->Buffer_Write_Data=0;
P_Buffer->Buffer_Read_Data=0;
P_Buffer->Buffer_Capacity=Buffer_Cap;
P_Buffer->P_Buffer_Data=P_Buffer_Data;
};

float Buffer_Percent_Usage(Buffer *P_Buffer){
IEC1bits.U2RXIE=0;
	if (P_Buffer->Buffer_Write_Data >= P_Buffer->Buffer_Read_Data){
		P_Buffer->Counts = P_Buffer->Buffer_Write_Data - P_Buffer->Buffer_Read_Data; 
	}
	else{
		P_Buffer->Counts = P_Buffer->Buffer_Write_Data - P_Buffer->Buffer_Read_Data + P_Buffer->Buffer_Capacity;
	};
float Used_Percentage=0.0; 
Used_Percentage=100*(float)(P_Buffer->Counts)/(float)(P_Buffer->Buffer_Capacity);
IEC1bits.U2RXIE=1;	
return Used_Percentage;
};

unsigned char Buffer_Read(Buffer *P_Buffer){
unsigned char Data;
Data=P_Buffer->P_Buffer_Data[P_Buffer->Buffer_Read_Data];
P_Buffer->P_Buffer_Data[P_Buffer->Buffer_Read_Data]=0;
++P_Buffer->Buffer_Read_Data;
	if (P_Buffer->Buffer_Read_Data == 5){
		P_Buffer->Buffer_Read_Data=0;
	};
Buffer_Percent_Usage(P_Buffer);
return Data;
};

void Buffer_Write(Buffer *P_Buffer,unsigned char Data){
P_Buffer->P_Buffer_Data[P_Buffer->Buffer_Write_Data]=Data; 
++P_Buffer->Buffer_Write_Data;	
	if (P_Buffer->Buffer_Write_Data == 5){
		P_Buffer->Buffer_Write_Data=0;
	};
Buffer_Percent_Usage(P_Buffer);
};



