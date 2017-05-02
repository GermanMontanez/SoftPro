/* Definiciones del modulo de tiempo. */
#if !defined(TIEMPO_H)

#define TIEMPO_H
#define YES 1
#define NO 0
//#include<p30f4011.h>
#include <stddef.h>

/* ------------------- Tipos de datos -------------------- */
// Contador
typedef unsigned int Tm_Contador;

// Numero de periodos o timeouts
typedef unsigned char Tm_Num;

// Estructura para generacion de referencias de tiempo periodicas

#define TM_PER_B_ACTIVO		0x01U
#define TM_PER_B_TC			0x02U
typedef struct
	{
	Tm_Contador	contador,
				TM_Period;
	unsigned char Flags;
	}Tm_Periodico;

// Tipo para generacion de timeouts
typedef Tm_Contador Tm_Timeout;

/* Rutina que verifica y actualiza el timer en hardware: el parametro indica si 
     el timer debe atenderse (cuando lo requiera). Si el timer completo un conteo 
     retorna un valor diferente de 0 y si no fue asi retorna 0. */
typedef char Tm_Atender_timer (char Attention);

/* Estructura de control del timer */
typedef struct 
	{
	Tm_Periodico *pp;
	Tm_Num max_p;
	Tm_Timeout *tp;
	Tm_Num max_t;
	Tm_Atender_timer *r_atencion;
	}Tm_Control;

/* ----------------------- Rutinas ------------------------ */
/* Configura y da condicion inicial a la estructura de control del timer */
void Tm_Defina_control (Tm_Control *P_XM_TO, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico, Tm_Timeout *t_timeout,
                        Tm_Num l_timeout, Tm_Atender_timer *atencion);

/* Rutina que revisa el timer en hardware e implementa la funcionalidad de los
   contadores. */
void Tm_Procese_tiempo (Tm_Control *P_XM_TO, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico);
						
/* Rutina para dar condicion inicial y empezar a utilizar un contador periodico */   
void Tm_Inicie_periodico (Tm_Control *P_XM_TO, Tm_Num TM_Counts, Tm_Contador TM_Period);

/* Rutina para chequear si se completo un ciclo de conteo de un contador periodico */
char Tm_Hubo_periodico (const Tm_Control *P_XM_TO, Tm_Num TM_Counts);

/* Rutina para bajar la Flag de fin de conteo. */
void Tm_Baje_periodico (Tm_Control *P_XM_TO, Tm_Num TM_Counts);

/* Rutina para dejar de utilizar (inactivar) el contador. */
void Tm_Termine_periodico (Tm_Control *P_XM_TO, Tm_Num TM_Counts);

/* Rutina para dar valor inicial a un contador de timeout. Para inactivar el 
   contador simplemente inicielo en 0. */
void Tm_Inicie_timeout (Tm_Control *P_XM_TO, Tm_Num TM_Counts, Tm_Contador tiempo);

/* Rutina para ver si hubo timeout (esta en 0). */
char Tm_Hubo_timeout (const Tm_Control *P_XM_TO, Tm_Num TM_Counts);

#endif
