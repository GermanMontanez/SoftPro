#include <p30f4011.h>
#include "multiplexor.h"
#include <math.h>
char German =0;
void MUX_Control_Init(Mux_Control *c_mux, Tm_Control *ctp, Tm_Num num, Tm_Num n_blink, Tm_Contador periodo, Tm_Contador periodo_intermitente,Ds_Control *c_ds, unsigned char *tabla)
{
	c_mux->Ctrl_tp=ctp;
	c_mux->num=num;
	c_mux->n_blink=n_blink;
	c_mux->tm_show=periodo;
	c_mux->tm_blink=periodo_intermitente;
	c_mux->Ctrl_ds=c_ds;
	c_mux->table=tabla;
	Tm_Inicie_periodico(ctp,num,periodo);
	Tm_Inicie_periodico(ctp,n_blink,periodo_intermitente);
	Mux_Enable(0);
	
}




void Mux_Funcion(Mux_Control *c_mux)
{
	  	if(Tm_Hubo_periodico(c_mux->Ctrl_tp,c_mux->num))
	  	{

			static char display_f=1;

			Mux_Enable(0);
			PORTB =c_mux->table[c_mux->Ctrl_ds->ds[display_f-1]];
			Mux_Enable(display_f);
			
			if(display_f>=(c_mux->Ctrl_ds->n_ds))
			display_f=1;
			else
			display_f+=1;
			

			Tm_Baje_periodico(c_mux->Ctrl_tp,c_mux->num);
		}

	}

void Mux_Intermitente(Mux_Control *c_mux)
{
	if(!(c_mux->Ctrl_ds->Ctrl_XM->flag&XM_F_READ) && (c_mux->Ctrl_ds->Ctrl_XM->flag & XM_F_ENABLE))
	{
		Mux_Enable(0);
		return;
	}
	if ((c_mux->Ctrl_ds->Ctrl_XM->flag & XM_F_READ))
	{
		Mux_Funcion(c_mux);
                Tm_Inicie_timeout(c_mux->Ctrl_tp,3,14000);
                
	}
	else
	{   
		if(c_mux->Ctrl_ds->bilnk)
		{
			Mux_Funcion(c_mux);
                        German =1;
		}
		else
		{
			Mux_Enable(0);
		}
	}

	if (Tm_Hubo_periodico(c_mux->Ctrl_ds->Ctrl_XM->ctp,c_mux->n_blink))
	{
		c_mux->Ctrl_ds->bilnk = ~(c_mux->Ctrl_ds->bilnk);
		Tm_Baje_periodico(c_mux->Ctrl_ds->Ctrl_XM->ctp,c_mux->n_blink);
	}
        if(Tm_Hubo_timeout(c_mux->Ctrl_tp,3)&&German==1){

            c_mux->n_blink=3;

        }
}
void Mux_Enable(char active_ds)
{

	char nd=active_ds-1;
	if (!active_ds)
	{
		PORTE=0X7;
	}
	else
	{
		PORTE=7-(pow(2,nd));
	}
}
