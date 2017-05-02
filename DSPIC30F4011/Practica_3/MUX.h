#if !defined(MULTIPLEXACION_H) 
#define MULTIPLEXACION_H
#include "Display.h"
#include "tiempo.h" 
// **************** MULTIPLEXER CONTROL STRUCTURE *******************
typedef struct MUX_Ctrl MUX_Ctrl;
struct MUX_Ctrl{
unsigned char *P_MUX_Table;	
DS_Ctrl *P_DS_Ctrl;
Tm_Control *P_XM_TO;
Tm_Num TM_Counts;
Tm_Num Num_Flash;
Tm_Contador TM_Period;
Tm_Contador Flash_Period;
};
// *************** MULTIPLEXER FUNCTION PROTOTYPES ******************
void MUX_Init(MUX_Ctrl *P_MUX_Ctrl, Tm_Control *P_XM_TO, Tm_Num TM_Counts,
						Tm_Num Num_Flash,Tm_Contador TM_Period,
						Tm_Contador Flash_Period,DS_Ctrl *P_DS_Ctrl,
						unsigned char *P_MUX_Table);
void MUX_Function(MUX_Ctrl *P_MUX_Ctrl); 
void MUX_Enable(MUX_Ctrl *P_MUX_Ctrl,char Active_Display); 
void MUX_Flashing(MUX_Ctrl *P_MUX_Ctrl);
#endif
