#include "xmodem.h"


void XM_Control_Init(Xm_Control *Ctrl_XM, Xm_pack *p_pack,Tm_Control *ctp,Uart_Control *c_uart,Tm_Num timeout_1_min, Tm_Num timeout_1_seg, unsigned char *bff1, unsigned char *bff2,  char bf_size,Buffer *bf_uart)
{
	Ctrl_XM->bf_index = p_pack;
	Ctrl_XM->seq_pack = 1;
	Ctrl_XM->bf_write = 0;
	Ctrl_XM->bf_read = 0;
	Ctrl_XM->state = XM_E_INIT;
	Ctrl_XM->try_cnt = 0;
	Ctrl_XM->flag = 0x0;
	Ctrl_XM->ctp = ctp;
	Ctrl_XM->Ctrl_uart=c_uart;
	Ctrl_XM->to_1m = timeout_1_min;
	Ctrl_XM->to_1s = timeout_1_seg;
	Ctrl_XM->bf_uart = bf_uart;
        Ctrl_XM->bf_index[0].buff_ds=bff1;
	Ctrl_XM->bf_index[1].buff_ds=bff2;
	Ctrl_XM->bf_index[0].flags=0;
	Ctrl_XM->bf_index[1].flags=0;
	Ctrl_XM->bf_index[0].size=bf_size;
	Ctrl_XM->bf_index[1].size=bf_size;
}


void XM_Process (Xm_Control *Ctrl_XM)
{
	switch (Ctrl_XM->state)
	{
		case XM_E_INIT:
			XM_Protocol_init(Ctrl_XM);
			break;

		case XM_E_WAIT_FOR_BLK:
			XM_waitfor_pack(Ctrl_XM);
			break;

		case XM_E_WAIT_FOR_SEC:
			XM_waitfor_seq(Ctrl_XM);
			break;

		case XM_E_WAIT_FOR_NSEC:
			XM_waitfornseq(Ctrl_XM);
			break;

		case XM_E_WAIT_FOR_CKSUM:
			XM_waitfor_cksum(Ctrl_XM);
			break;

                case XM_E_DISCART:
			XM_Discard(Ctrl_XM);
			break;

		case XM_E_RDATA:
			XM_Read_Data(Ctrl_XM);
			break;

		case XM_E_ABORT:
			XM_Abort(Ctrl_XM);
			break;
        }
}


void XM_Protocol_init(Xm_Control *Ctrl_XM)
{
	Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);

    UART_write(XM_NAK );

	Ctrl_XM->seq_pack=1;

	Ctrl_XM->state=XM_E_WAIT_FOR_BLK;

	Ctrl_XM->flag |= XM_F_SOT;

	Ctrl_XM->flag |= XM_F_ENABLE;
	return;
}


void XM_waitfor_pack(Xm_Control *Ctrl_XM)
{
	unsigned char dato;

	if (!Ctrl_XM->bf_uart->stored)
	{
		if(Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1m))
		{
			UART_write(XM_NAK );
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			if (!(Ctrl_XM->flag & XM_F_SOT))
			{
				(Ctrl_XM->try_cnt)++;
				if (Ctrl_XM->try_cnt == XM_MAX_TRY)
				{
					Ctrl_XM->state = XM_E_ABORT;
				}
			}
		}
		return;
	}


	dato =BF_ReadBF(Ctrl_XM->bf_uart);
	Ctrl_XM->flag &= ~XM_F_SOT;

	if (dato==XM_SOH)
	{
		Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1s , XM_1S);

		Ctrl_XM->state=XM_E_WAIT_FOR_SEC;
		Ctrl_XM->try_cnt=0;
	}

	else if (dato==XM_EOT)
	{
		UART_write(XM_ACK );

		Ctrl_XM->state=XM_E_INIT;
		Ctrl_XM->flag|=XM_F_EOT;
	}

	else
	{
		return;
	}
};


void XM_waitfor_seq(Xm_Control *Ctrl_XM)
{
	Xm_pack *paq_escritura;
	paq_escritura = Ctrl_XM->bf_index + Ctrl_XM->bf_write;

	if (!Ctrl_XM->bf_uart->stored)
	{
		if (Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1s))
		{
			UART_write(XM_NAK);
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
		}
		return;
	};

	paq_escritura->n_blk =BF_ReadBF(Ctrl_XM->bf_uart);

	Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1s, XM_1S);

	Ctrl_XM->state=XM_E_WAIT_FOR_NSEC;
	return;
};

void XM_waitfornseq(Xm_Control *Ctrl_XM)
{
	unsigned char c_blk_num;
	Xm_pack *paq_escritura;
	paq_escritura = Ctrl_XM->bf_index + Ctrl_XM->bf_write;

	if (!Ctrl_XM->bf_uart->stored)
	{
		if (Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1s))
		{
			UART_write(XM_NAK);
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
		}
		return;
	}


	c_blk_num =BF_ReadBF(Ctrl_XM->bf_uart);

	if ((paq_escritura->n_blk)==(255-c_blk_num))
	{
		if ((paq_escritura->n_blk)==(Ctrl_XM->seq_pack))
		{
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1s , XM_1S);

			Ctrl_XM->state=XM_E_RDATA;


			paq_escritura->data_cnt=0;
			paq_escritura->cksum=0;
		}

		else if ( (paq_escritura->n_blk)<(Ctrl_XM->seq_pack))
		{
			Ctrl_XM->state=XM_E_DISCART;
			paq_escritura->flags |= XM_F_E_ACK;
		}
		else
	    {
			Ctrl_XM->state=XM_E_ABORT;
	 	}
	}
	else
	{
		Ctrl_XM->state=XM_E_DISCART;
		paq_escritura->flags=XM_F_E_NAK;
	}

	return;
};


void XM_Read_Data(Xm_Control *Ctrl_XM)
{

	Xm_pack *paq_escritura;
	char dato;
	if (!Ctrl_XM->bf_uart->stored)
	{
		if (Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1s))
		{
			UART_write(XM_NAK);
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
		}
		return;
	}
	Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1s , XM_1S);
	paq_escritura = Ctrl_XM->bf_index + Ctrl_XM->bf_write;

	
	dato =BF_ReadBF(Ctrl_XM->bf_uart); 

	paq_escritura->cksum = paq_escritura->cksum + dato;
	*(paq_escritura->buff_ds + paq_escritura->data_cnt) = dato;
	(paq_escritura->data_cnt)++;
	if(paq_escritura->data_cnt >= paq_escritura->size)
	{
		paq_escritura->data_cnt = 0;
		Ctrl_XM->state=XM_E_WAIT_FOR_CKSUM;
	}
	return;

};


void XM_Abort(Xm_Control *Ctrl_XM)
{
	Ctrl_XM->flag|=XM_F_CANCEL;
	Ctrl_XM->flag|=XM_F_READ;
};


void XM_waitfor_cksum(Xm_Control *Ctrl_XM)
{
	if (!Ctrl_XM->bf_uart->stored && !(Ctrl_XM->flag & XM_F_CKSUM))
	{
		if (Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1s))
		{
			UART_write(XM_NAK);
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
		}
		return;
	}
	else if (!Ctrl_XM->bf_uart->stored && (Ctrl_XM->flag & XM_F_CKSUM))
	{
		int i;
		Xm_pack *paq;

		for (i=0;i < 2;i++)
		{
			paq = Ctrl_XM->bf_index + i;
			if (!(paq->flags & XM_READY))
			{
				Ctrl_XM->flag &= ~XM_F_CKSUM;
				UART_write(XM_ACK);
				Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
				Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
			}
		}
		return;
	}


	Xm_pack *paq_escritura;
	paq_escritura = Ctrl_XM->bf_index + Ctrl_XM->bf_write;


	unsigned char dato =BF_ReadBF(Ctrl_XM->bf_uart); 

	if (dato == paq_escritura->cksum)
	{
		paq_escritura->flags |= XM_READY;
		Ctrl_XM->flag |= XM_F_READ;
		Ctrl_XM->flag |= XM_F_CKSUM;
		Ctrl_XM->flag &=~XM_F_ENABLE;
		Ctrl_XM->seq_pack++;

		(Ctrl_XM->bf_write)++;
		if (Ctrl_XM->bf_write >= 2)
		{
			Ctrl_XM->bf_write = 0;
		}
	}
	else
	{
		UART_write(XM_NAK);
		Ctrl_XM->flag &= ~XM_F_CKSUM;
		Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
		Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
	}
}


void XM_Discard(Xm_Control *Ctrl_XM)
{
	if (!Ctrl_XM->bf_uart->stored)
	{
		if (Tm_Hubo_timeout(Ctrl_XM->ctp,Ctrl_XM->to_1s))
		{
			UART_write(XM_NAK);
			Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1m , XM_1M);
			Ctrl_XM->state = XM_E_WAIT_FOR_BLK;
		}
		return;
	}
	char dato;
	Xm_pack *paq_escritura;
	paq_escritura = Ctrl_XM->bf_index + Ctrl_XM->bf_write;
	Tm_Inicie_timeout (Ctrl_XM->ctp, Ctrl_XM->to_1s , XM_1S);


	dato =BF_ReadBF(Ctrl_XM->bf_uart);

	(paq_escritura->data_cnt)++;
	if(paq_escritura->data_cnt > paq_escritura->size)
	{
		if(paq_escritura->flags & XM_F_E_ACK)
		{
			UART_write(XM_ACK);
		}else
		{
			UART_write(XM_NAK);
		}
		paq_escritura->data_cnt = 0;
		Ctrl_XM->state=XM_E_WAIT_FOR_BLK;
	}
	return;
}
