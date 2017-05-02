#if (!defined(DISPLAY_H))
#define DIAPLAY_H 
#include "Buffer.h"
#include "tiempo.h"
 
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
#define	SEG_A	0b01111101
#define	SEG_B	0b01100111
#define	SEG_C	0b00110011
#define SEG_D	0b01001111
#define	SEG_E	0b01110011
#define	SEG_F	0b01110001
#define SEG_DSH 0b01000000

#define	SIGN_AMPERSAND	0x26
#define	SIGN_DOLLAR	0x24
#define	SIGN_SHARP	0x23
#define	SIGN_PERCENT	0x25

#define	COMPLEMENTE	0b00000001
#define	TM_2S		0b00000010
#define	TM_5S_TP_2HZ	0b00000100
#define ON_OFF		0b00001000

#define TO_PER  1
#define TO_SHA  2
#define TP_SHA  1


typedef unsigned char Dis_Data;
typedef unsigned char Dis_Index;

typedef struct Display Display;
struct Display{
Buff_Data DS_In;
Dis_Index DS_Flag;
};

void Ds_Init(Display *P_Display);
void Ds_Procese(Display *P_Display, Buffer *P_Buffer,Tm_Control *ctp);


#endif

