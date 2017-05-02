#include <p30f4011.h>
#include "Display.h"



void DS_show(Ds_Control *c_ds)
{
	Xm_pack *paq_lectura;
	unsigned char dato_buffer;
	paq_lectura = c_ds -> Ctrl_XM -> bf_index + (c_ds->Ctrl_XM->bf_read);
	dato_buffer = paq_lectura->buff_ds[paq_lectura->data_cnt];
	unsigned int pow=1;
        char i=c_ds->n_ds;
        unsigned char  j=0;
	while(i)
	{
		unsigned char op = dato_buffer/pow;
		char rta=0;

		if (op >=10)
		rta = (op)%10;
		else
		rta = (op);
		c_ds->ds[j]=rta;
		j++;
		i--;
		pow*=10;
	}
};

void DS_handle(Ds_Control *c_ds)
{
	if(c_ds->Ctrl_XM->flag&XM_F_CANCEL)
	{
		c_ds->ds[0]=11;
		c_ds->ds[1]=11;
		c_ds->ds[2]=10;
		return;
	}	
	if (!(Tm_Hubo_periodico(c_ds->Ctrl_XM->ctp,c_ds->num)))
	{
		return;
	}	
	Tm_Baje_periodico(c_ds->Ctrl_XM->ctp,c_ds->num);
	
	Xm_pack *paq_lectura;
	paq_lectura = c_ds->Ctrl_XM->bf_index + c_ds->Ctrl_XM->bf_read;
	
	if (c_ds->Ctrl_XM->flag & XM_F_READ)
	{
		
		DS_show(c_ds);
		(paq_lectura->data_cnt)++;
		
		if(paq_lectura->data_cnt >= paq_lectura->size)
		{
			(c_ds->sequense)++;
			paq_lectura->data_cnt = 0;
			paq_lectura->flags &= ~XM_READY;
			c_ds-> Ctrl_XM ->bf_read++;
			if(c_ds-> Ctrl_XM ->bf_read >= 2)
			{
				c_ds-> Ctrl_XM ->bf_read = 0;
			}
			paq_lectura = c_ds->Ctrl_XM->bf_index + c_ds->Ctrl_XM->bf_read;
			if (paq_lectura->flags & XM_READY)
			{
				DS_show(c_ds);
				(paq_lectura->data_cnt)++;
			}	
		}		
	}	
				
	
	if (((paq_lectura->flags) & XM_READY)&&
		 (paq_lectura->n_blk== c_ds->sequense))
	{
		c_ds->Ctrl_XM->flag|=XM_F_READ;
	}
	else
	{
		if(c_ds->Ctrl_XM->flag&XM_F_EOT)
		{
			c_ds->Ctrl_XM->flag&=~XM_F_EOT;
			c_ds->sequense=1;
		}	
		c_ds->Ctrl_XM->flag &= ~XM_F_READ;
	}	
}

void DS_Control_Init(Ds_Control *c_ds, Ds_Data *dp, Xm_Control *cxp, Tm_Num num, Tm_Contador periodo, char num_displays)
{
	c_ds->num=num;
	c_ds->periodo=periodo;
	c_ds->ds=dp;
	c_ds->n_ds=num_displays;
	c_ds->Ctrl_XM=cxp;
	c_ds->sequense=1;
	c_ds->bilnk = 0;
	c_ds->v_muestra=2;



	Tm_Inicie_periodico(c_ds->Ctrl_XM->ctp,num,periodo);

};
