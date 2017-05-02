# if !defined(CONTROL_PROCESS_H)
# define CONTROL_PROCESS_H


#include "states_process.h"

//Seleccionar que tipo de controlador se utilizara, solo se puede seleccionar uno.

//#define MPC_GN_CONTROL
//#define MPC_RN_CONTROL
//#define MPC_MP_CONTROL
#define MPC_SS_CONTROL



//Prototipos de las funciones pertenecientes a este modulo.
double exp(double x);
double tangh(double x);
int force2dutycicle(double fc);
void mpc_gn_control(state_space *ss_actual, state_space *ss_ant, double *ant_FC);
void mpc_mp_control(state_space *ss_actual, state_space *ss_ant, double *ant_FC);
void mpc_rn_control(state_space *ss_actual, state_space *ss_ant, double *ant_FC);
void mpc_ss_control(state_space *ss_actual, state_space *ss_ant, double *ant_FC);

#endif