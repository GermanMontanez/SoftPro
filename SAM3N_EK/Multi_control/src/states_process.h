# if !defined(STATES_PROCESS_H)
# define STATES_PROCESS_H

#include "enc_handle.h"

#define STATE_X2_X1		1
#define STATE_dX2 		2
#define STATE_X1_Zr		3
#define STATE_dX1		4
#define OTHMES_X1		5
#define OTHMES_Zr		6
#define OTHMES_dZr		7




// definición del tipo de dato

typedef double state;
typedef double othmes;

//Estructura que contiene la informacion de los encoders
typedef struct {
	//'x2-x1','dx2','x1-Zr','dx1'
	state x2_x1;
	state dx2;
	state x1_Zr;
	state dx1;
	othmes x2;
	othmes x1;
	othmes Zr;
	othmes dZr;
	
}state_space;

void init_states(state_space *cup);
int get_state(state_space *cup, int st);
void set_state(state_space *cup, int st,int val);
void process_states(state_space *cup,state_space *ant_cup, enc_count *enc);

#endif