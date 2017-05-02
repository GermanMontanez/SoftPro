#if !defined(DISPLAY_H) 
#define DISPLAY_H
#include "tiempo.h"  
#include "Xmodem.h"
typedef unsigned char DS_Data; 
typedef struct DS_Ctrl DS_Ctrl;
struct DS_Ctrl{
DS_Data *dp; 
char Display_Number;
XM_Ctrl *P_XM_Ctrl;
Tm_Num TM_Counts;
Tm_Contador TM_Period;
char Flashing;
unsigned char Secuences;
};
void DS_Init(DS_Ctrl *P_DS_Ctrl,DS_Data *dp, XM_Ctrl *P_XM_Ctrl,
					   	Tm_Num TM_Counts,
						Tm_Contador TM_Period,char Display_Number);
void DS_Conversion(DS_Ctrl *P_DS_Ctrl);
void DS_Function(DS_Ctrl *P_DS_Ctrl);
#endif
