#if !defined(DISPLAY_H)

#define DISPLAY_H


#include "tiempo.h"
#include "xmodem.h"


#define DS_N            0X3u
#define DS_TP 		2				
#define DS_N_TP 	0



typedef unsigned char Ds_Data;
typedef struct Ds_Control Ds_Control; 

struct Ds_Control 
{

        Tm_Contador periodo;
	Tm_Num num;
        Ds_Data *ds;
	char n_ds;
        unsigned char sequense;
        unsigned char v_muestra;
	char bilnk;
	Xm_Control *Ctrl_XM;

};



void DS_Control_Init(Ds_Control *c_ds,Ds_Data *dp, Xm_Control *cxp, Tm_Num num, Tm_Contador periodo, char num_displays);


void DS_show(Ds_Control *c_ds);


void DS_handle(Ds_Control *c_ds);


#endif
