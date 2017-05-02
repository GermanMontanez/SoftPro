
#if !defined(TIEMPO_H)

#define TIEMPO_H
#define SI 1
#define NO 0




typedef unsigned int Tm_Contador;


typedef unsigned char Tm_Num;



#define TM_PER_B_ACTIVO		0x01U
#define TM_PER_B_TC			0x02U
typedef struct
	{
	Tm_Contador	contador,
				periodo;
	unsigned char banderas;
	}Tm_Periodico;


typedef Tm_Contador Tm_Timeout;


typedef char Tm_Atender_timer (char atienda);


typedef struct
	{
	Tm_Periodico *pp;
	Tm_Num max_p;
	Tm_Timeout *tp;
	Tm_Num max_t;
	Tm_Atender_timer *r_atencion;
	}Tm_Control;


void Tm_Defina_control (Tm_Control *ctp, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico, Tm_Timeout *t_timeout,
                        Tm_Num l_timeout, Tm_Atender_timer *atencion);

void Tm_Procese_tiempo (Tm_Control *ctp, Tm_Periodico *t_periodico,
                        Tm_Num l_periodico);

void Tm_Inicie_periodico (Tm_Control *ctp, Tm_Num num, Tm_Contador periodo);

char Tm_Hubo_periodico (const Tm_Control *ctp, Tm_Num num);

void Tm_Baje_periodico (Tm_Control *ctp, Tm_Num num);

void Tm_Termine_periodico (Tm_Control *ctp, Tm_Num num);

void Tm_Inicie_timeout (Tm_Control *ctp, Tm_Num num, Tm_Contador tiempo);

char Tm_Hubo_timeout (const Tm_Control *ctp, Tm_Num num);

#endif

