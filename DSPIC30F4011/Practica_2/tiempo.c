/* Implementacion del modulo de tiempo. */

#include "tiempo.h"
//#include <Varios.h>

/* Configura y da condicion inicial a la estructura de control del timer */
void Tm_Defina_control (Tm_Control *ctp, 
								Tm_Periodico *t_periodico,
                        Tm_Num l_periodico,
								Tm_Timeout *t_timeout,
                        Tm_Num l_timeout, 
								Tm_Atender_timer *atencion)
	{
	Tm_Periodico *pp;
	Tm_Timeout *tp;
	Tm_Num num;
	
	// Condicion inicial de la tabla de contadores periodicos: todos inactivos
	for (pp = t_periodico, num = l_periodico;
		  num;
		  ++pp, --num)
		pp->banderas = 0;
	ctp->pp = t_periodico;
	ctp->max_p = l_periodico;

   // Condicion inicial de la tabla de contadores de timeout: todos inactivos
	for (tp = t_timeout, num = l_timeout;
		  num;
		  ++tp, --num)
		*tp = 0;
	ctp->tp = t_timeout;
	ctp->max_t = l_timeout;

	ctp->r_atencion = atencion;
	}
 
 
 /* Rutina que revisa el timer en hardware e implementa la funcionalidad de los
   contadores. */
void Tm_Procese_tiempo (Tm_Control *ctp, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico)
	{
	Tm_Periodico *pp;
	Tm_Timeout *tp;
	Tm_Num num;
	
	if ( !ctp->r_atencion(SI) )
		return;
	
	for (pp = t_periodico, num = l_periodico;
		  num;
		  ++pp, --num)
		if (pp->banderas & TM_PER_B_ACTIVO)
			{
			--(pp->contador);
			if ( !(pp->contador) )
				{
				pp->contador = pp->periodo;
				pp->banderas |= TM_PER_B_TC;
				};
			};
	

	for (tp = ctp->tp, num = ctp->max_t; 
        num > 0; --num, ++tp)
      if (*tp)
		   --(*tp);
	};

 
 /* Rutinas para empezar a utilizar un contador periodico, chequear si se
   completo un ciclo de conteo, bajar la bandera de fin de conteo y dejar de
   utilizar (inactivar) el contador. */
void Tm_Inicie_periodico (Tm_Control *ctp,
                          Tm_Num num,
                          Tm_Contador periodo)
  {
  Tm_Periodico *pp = ctp->pp + num;

	pp->banderas |= TM_PER_B_ACTIVO;
	pp->banderas &= ~TM_PER_B_TC;
	pp->contador = pp->periodo = periodo;
	}

char Tm_Hubo_periodico (const Tm_Control *ctp,
                        Tm_Num num)
   {
	return ((ctp->pp + num)->banderas & TM_PER_B_TC);
	}

void Tm_Baje_periodico (Tm_Control *ctp,
                        Tm_Num num)
   {
	(ctp->pp + num)->banderas &= ~TM_PER_B_TC;
	}

void Tm_Termine_periodico (Tm_Control *ctp,
                           Tm_Num num)
   {
	(ctp->pp + num)->banderas &= ~TM_PER_B_ACTIVO;
	}

   
/* Rutina para dar valor inicial a un contador de timeout. Para inactivar el 
   contador simplemente inicielo en 0. */
void Tm_Inicie_timeout (Tm_Control *ctp, 
								Tm_Num num, 
								Tm_Contador tiempo)
	{
	ctp->tp[num] = tiempo;
	}

/* Rutina para ver si hubo timeout (esta en 0). */
char Tm_Hubo_timeout (const Tm_Control *ctp, 
							 Tm_Num num)
	{
	return !(ctp->tp[num]);
	}
