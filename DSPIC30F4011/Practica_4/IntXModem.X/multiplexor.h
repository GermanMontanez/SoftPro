#if !defined(MULTIPLEXOR_H)

#define MULTIPLEXOR_H

#include "display.h" 
#include "tiempo.h"  


#define MUX_TP                  1	
#define MUX_N_TP                1
#define MUX_TP_BLINK            2	
#define MUX_N_TP_BLINK          120 	



typedef struct Mux_Control Mux_Control; 

struct Mux_Control 
{
	
	Ds_Control *Ctrl_ds;
	Tm_Control *Ctrl_tp;
	Tm_Num num;
	Tm_Num n_blink;
	Tm_Contador tm_blink;
        Tm_Contador tm_show;
	unsigned char *table;
};


void MUX_Control_Init(Mux_Control *c_mux, Tm_Control *ctp, Tm_Num num, Tm_Num n_blink, Tm_Contador periodo, Tm_Contador periodo_intermitente,Ds_Control *c_ds, unsigned char *tabla);
void MUX_shift(Mux_Control *c_mux);
void MUX_Enable(char active_ds);
void MUX_Blink(Mux_Control *c_mux);


#endif
