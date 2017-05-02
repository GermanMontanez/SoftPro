#include "Xmodem.h"
extern XM_Ctrl	*P_XM_Ctrl;
typedef struct{
void *sp;
void *fp;
}Tcb;
extern Tcb tcb_ppal;

void XM_Init(XM_Ctrl *P_XM_Ctrl, XM_Packet *P_XM_Packet,
					   Tm_Control *P_XM_TO,UART_Ctrl *P_UART_Ctrl,Tm_Num XM_TO_10Sec,
					   Tm_Num XM_TO_1Sec, unsigned char *XM_Buffer1, unsigned char *XM_Buffer2,
					   char Type, Buffer *P_UART_Buffer){
P_XM_Ctrl->P_XM_Packet = P_XM_Packet;
P_XM_Ctrl->Packet_Counts = 1;
P_XM_Ctrl->Packet_Writer = 0;
P_XM_Ctrl->Packet_Reader = 0;
P_XM_Ctrl->State = XM_STATE_WAIT4_START;
P_XM_Ctrl->Error_Counts = 0;
P_XM_Ctrl->Flags = 0x0;
P_XM_Ctrl->P_XM_TO = P_XM_TO;
P_XM_Ctrl->P_UART_Ctrl=P_UART_Ctrl;
P_XM_Ctrl->XM_TO_10Sec = XM_TO_10Sec;
P_XM_Ctrl->XM_TO_1Sec = XM_TO_1Sec;
P_XM_Ctrl->P_XM_Packet[0].dp=XM_Buffer1;
P_XM_Ctrl->P_XM_Packet[1].dp=XM_Buffer2;
P_XM_Ctrl->P_XM_Packet[0].Flag=0;
P_XM_Ctrl->P_XM_Packet[1].Flag=0;
P_XM_Ctrl->P_XM_Packet[0].Type=Type;
P_XM_Ctrl->P_XM_Packet[1].Type=Type;
P_XM_Ctrl->P_UART_Buffer=P_UART_Buffer;
};	


void XM_State_Machine(XM_Ctrl *P_XM_Ctrl){
	unsigned char Data;
unsigned char Char_Blk_Num;
XM_Packet *P_XM_Packet_4write;
while(YES){
	switch (P_XM_Ctrl->State){
		case XM_STATE_WAIT4_START:
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK );
			P_XM_Ctrl->Packet_Counts=1;
			P_XM_Ctrl->State=XM_STATE_WAIT4_PACKET;
			P_XM_Ctrl->Flags |= XM_FLAG_SOT;
			P_XM_Ctrl->Flags |= XM_FLAG_ENABLE;
			break;
	
		case XM_STATE_WAIT4_PACKET:

	if (!P_XM_Ctrl->P_UART_Buffer->Counts){
		if(Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_10Sec)){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK );
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			if (!(P_XM_Ctrl->Flags & XM_FLAG_SOT)){
				(P_XM_Ctrl->Error_Counts)++; 
				if (P_XM_Ctrl->Error_Counts == XM_MAXIMUM_ERROR){
					P_XM_Ctrl->State = XM_STATE_DISCARD_TX;
				};	
			};	
		};	
		return;
	};
Data =Buffer_Read(P_XM_Ctrl->P_UART_Buffer);
P_XM_Ctrl->Flags &= ~XM_FLAG_SOT;
	if (Data==XM_P_SOH){
		Tm_Inicie_timeout(P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_1Sec , XM_TO_1SEC_BASE);
		P_XM_Ctrl->State=XM_STATE_WAIT4_SECUENCES;
		P_XM_Ctrl->Error_Counts=0;
	}
	else if (Data==XM_P_EOT){
		UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_ACK );
		P_XM_Ctrl->State=XM_STATE_WAIT4_START;
		P_XM_Ctrl->Flags|=XM_FLAG_EOT;
	}
	else{
	return;
	};
			break;
	
		case XM_STATE_WAIT4_SECUENCES:
				
	P_XM_Packet_4write = P_XM_Ctrl->P_XM_Packet + P_XM_Ctrl->Packet_Writer;
	if (!P_XM_Ctrl->P_UART_Buffer->Counts){
		if (Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_1Sec)){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
		};
		return;
	};
Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_1Sec, XM_TO_1SEC_BASE);
P_XM_Packet_4write->Blk_Num =Buffer_Read(P_XM_Ctrl->P_UART_Buffer);
P_XM_Ctrl->State=XM_STATE_WAIT4_SECUENCES_COMP;
			break;
	// Complement of Sequences
		case XM_STATE_WAIT4_SECUENCES_COMP:
P_XM_Packet_4write = P_XM_Ctrl->P_XM_Packet + P_XM_Ctrl->Packet_Writer;
	if (!P_XM_Ctrl->P_UART_Buffer->Counts){
		if (Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_1Sec)){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
		};
		return;
	};
Char_Blk_Num =Buffer_Read(P_XM_Ctrl->P_UART_Buffer);		
	if((P_XM_Packet_4write->Blk_Num)==(255-Char_Blk_Num)){
		if((P_XM_Packet_4write->Blk_Num)==(P_XM_Ctrl->Packet_Counts)){
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_1Sec , XM_TO_1SEC_BASE);
			P_XM_Ctrl->State=XM_STATE_DATA_RX;
			P_XM_Packet_4write->Data_Counts=0;
			P_XM_Packet_4write->CheckSum=0;
		}
		else if((P_XM_Packet_4write->Blk_Num)<(P_XM_Ctrl->Packet_Counts)){
			P_XM_Ctrl->State=XM_STATE_IGNORE_DATA; 
			P_XM_Packet_4write->Flag |= XM_FLAG_ERROR_ACK;
		}
		else	P_XM_Ctrl->State=XM_STATE_DISCARD_TX;
	}
	else{
		P_XM_Ctrl->State=XM_STATE_IGNORE_DATA;
		P_XM_Packet_4write->Flag=XM_FLAG_ERROR_NAK;
	};
			break;
	
		case XM_STATE_IGNORE_DATA:
	if (!P_XM_Ctrl->P_UART_Buffer->Counts){
		if (Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_1Sec)){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
		};
		return;
	};
// ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
//char Data;
//XM_Packet *P_XM_Packet_4write;
P_XM_Packet_4write = P_XM_Ctrl->P_XM_Packet + P_XM_Ctrl->Packet_Writer;
Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_1Sec , XM_TO_1SEC_BASE);
Data=Buffer_Read(P_XM_Ctrl->P_UART_Buffer);
(P_XM_Packet_4write->Data_Counts)++;
	if(P_XM_Packet_4write->Data_Counts > P_XM_Packet_4write->Type){	 
		if(P_XM_Packet_4write->Flag & XM_FLAG_ERROR_ACK){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl, XM_P_ACK);
		}
		else	UART_Tx(P_XM_Ctrl->P_UART_Ctrl, XM_P_NAK);
		P_XM_Packet_4write->Data_Counts = 0;
		P_XM_Ctrl->State=XM_STATE_WAIT4_PACKET;
	};
			break;
	// ****************************************************************************************
		case XM_STATE_DATA_RX:
	if (!P_XM_Ctrl->P_UART_Buffer->Counts){
		if (Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_1Sec)){
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
			Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec, XM_TO_10SEC_BASE);
			P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
		};
		return;
	};
Tm_Inicie_timeout(P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_1Sec, XM_TO_1SEC_BASE);
P_XM_Packet_4write = P_XM_Ctrl->P_XM_Packet + P_XM_Ctrl->Packet_Writer;
// Read Data into the Buffer
Data=Buffer_Read(P_XM_Ctrl->P_UART_Buffer);
P_XM_Packet_4write->CheckSum = P_XM_Packet_4write->CheckSum + Data;
*(P_XM_Packet_4write->dp + P_XM_Packet_4write->Data_Counts) = Data; 
(P_XM_Packet_4write->Data_Counts)++;
	if(P_XM_Packet_4write->Data_Counts >= P_XM_Packet_4write->Type){
		P_XM_Packet_4write->Data_Counts = 0;
		P_XM_Ctrl->State=XM_STATE_WAIT4_CHKSUM;
	};
			break;
	
		case XM_STATE_DISCARD_TX:
	P_XM_Ctrl->Flags|=XM_FLAG_DESCARD;
	P_XM_Ctrl->Flags|=XM_FLAG_READ;
			break;
	
		case XM_STATE_WAIT4_CHKSUM:
	if (!P_XM_Ctrl->P_UART_Buffer->Counts && !(P_XM_Ctrl->Flags & XM_FLAG_CHECKSUM)){
		if (Tm_Hubo_timeout(P_XM_Ctrl->P_XM_TO,P_XM_Ctrl->XM_TO_1Sec)){
			Tm_Inicie_timeout(P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
			UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
			P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
		};
		return;
	}
	else if (!P_XM_Ctrl->P_UART_Buffer->Counts && (P_XM_Ctrl->Flags & XM_FLAG_CHECKSUM)){
		int i;
		XM_Packet *P_XM_Packet;
		for (i=0;i < 2;i++){
			P_XM_Packet = P_XM_Ctrl->P_XM_Packet + i;
			if (!(P_XM_Packet->Flag & XM_FLAG_OK)){
				Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
				UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_ACK);
				P_XM_Ctrl->Flags &= ~XM_FLAG_CHECKSUM;
				P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
			};
		};
		return;
	};
XM_Packet *P_XM_Packet_write;
P_XM_Packet_write = P_XM_Ctrl->P_XM_Packet + P_XM_Ctrl->Packet_Writer;
unsigned char XM_Data_Buffer = Buffer_Read(P_XM_Ctrl->P_UART_Buffer); 	
if (XM_Data_Buffer == P_XM_Packet_write->CheckSum){
		P_XM_Packet_write->Flag |= XM_FLAG_OK;
		P_XM_Ctrl->Flags |= XM_FLAG_READ;
		P_XM_Ctrl->Flags |= XM_FLAG_CHECKSUM;
		P_XM_Ctrl->Flags &=~XM_FLAG_ENABLE; 
		P_XM_Ctrl->Packet_Counts++;	

		(P_XM_Ctrl->Packet_Writer)++;
		if (P_XM_Ctrl->Packet_Writer >= 2){
			P_XM_Ctrl->Packet_Writer = 0;
		};
	}
	else{
		Tm_Inicie_timeout (P_XM_Ctrl->P_XM_TO, P_XM_Ctrl->XM_TO_10Sec , XM_TO_10SEC_BASE);
		UART_Tx(P_XM_Ctrl->P_UART_Ctrl,XM_P_NAK);
		P_XM_Ctrl->Flags &= ~XM_FLAG_CHECKSUM;
		P_XM_Ctrl->State = XM_STATE_WAIT4_PACKET;
	};
			break;
	};
		IEC1bits.U2RXIE=0;
		IEC0&=~INTERRUPT_ON;
		CR_Execute(&tcb_ppal);
		IEC1bits.U2RXIE=1;
		IEC0|=INTERRUPT_ON;
	};


};




