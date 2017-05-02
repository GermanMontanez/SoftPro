/* Implementacion del modulo de tiempo. */

#include "tiempo.h"
//#include <Varios.h>

/* Configura y da condicion inicial a la estructura de control del timer */
void Tm_Defina_control (Tm_Control *P_XM_TO, 
								Tm_Periodico *t_periodico,
                        Tm_Num l_periodico,
								Tm_Timeout *t_timeout,
                        Tm_Num l_timeout, 
								Tm_Atender_timer *atencion)
	{
	Tm_Periodico *pp;
	Tm_Timeout *tp;
	Tm_Num TM_Counts;
	
	// Condicion inicial de la P_MUX_Table de contadores periodicos: todos inactivos
	for (pp = t_periodico, TM_Counts = l_periodico;
		  TM_Counts;
		  ++pp, --TM_Counts)
		pp->Flags = 0;
	P_XM_TO->pp = t_periodico;
	P_XM_TO->max_p = l_periodico;

   // Condicion inicial de la P_MUX_Table de contadores de timeout: todos inactivos
	for (tp = t_timeout, TM_Counts = l_timeout;
		  TM_Counts;
		  ++tp, --TM_Counts)
		*tp = 0;
	P_XM_TO->tp = t_timeout;
	P_XM_TO->max_t = l_timeout;

	P_XM_TO->r_atencion = atencion;
	}
 
 
 /* Rutina que revisa el timer en hardware e implementa la funcionalidad de los
   contadores. */
void Tm_Procese_tiempo (Tm_Control *P_XM_TO, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico)
	{
	Tm_Periodico *pp;
	Tm_Timeout *tp;
	Tm_Num TM_Counts;
	
	if ( !P_XM_TO->r_atencion(YES) )
		return;
	
	for (pp = t_periodico, TM_Counts = l_periodico;
		  TM_Counts;
		  ++pp, --TM_Counts)
		if (pp->Flags & TM_PER_B_ACTIVO)
			{
			--(pp->contador);
			if ( !(pp->contador) )
				{
				pp->contador = pp->TM_Period;
				pp->Flags |= TM_PER_B_TC;
				};
			};
	

	for (tp = P_XM_TO->tp, TM_Counts = P_XM_TO->max_t; 
        TM_Counts > 0; --TM_Counts, ++tp)
      if (*tp)
		   --(*tp);
	};

 
 /* Rutinas para empezar a utilizar un contador periodico, chequear si se
   completo un ciclo de conteo, bajar la Flag de fin de conteo y dejar de
   utilizar (inactivar) el contador. */
void Tm_Inicie_periodico (Tm_Control *P_XM_TO,
                          Tm_Num TM_Counts,
                          Tm_Contador TM_Period)
  {
  Tm_Periodico *pp = P_XM_TO->pp + TM_Counts;

	pp->Flags |= TM_PER_B_ACTIVO;
	pp->Flags &= ~TM_PER_B_TC;
	pp->contador = pp->TM_Period = TM_Period;
	}

char Tm_Hubo_periodico (const Tm_Control *P_XM_TO,
                        Tm_Num TM_Counts)
   {
	return ((P_XM_TO->pp + TM_Counts)->Flags & TM_PER_B_TC);
	}

void Tm_Baje_periodico (Tm_Control *P_XM_TO,
                        Tm_Num TM_Counts)
   {
	(P_XM_TO->pp + TM_Counts)->Flags &= ~TM_PER_B_TC;
	}

void Tm_Termine_periodico (Tm_Control *P_XM_TO,
                           Tm_Num TM_Counts)
   {
	(P_XM_TO->pp + TM_Counts)->Flags &= ~TM_PER_B_ACTIVO;
	}

   
/* Rutina para dar valor inicial a un contador de timeout. Para inactivar el 
   contador simplemente inicielo en 0. */
void Tm_Inicie_timeout (Tm_Control *P_XM_TO, 
								Tm_Num TM_Counts, 
								Tm_Contador tiempo)
	{
	P_XM_TO->tp[TM_Counts] = tiempo;
	}

/* Rutina para ver si hubo timeout (esta en 0). */
char Tm_Hubo_timeout (const Tm_Control *P_XM_TO, 
							 Tm_Num TM_Counts)
	{
	return !(P_XM_TO->tp[TM_Counts]);
	}
