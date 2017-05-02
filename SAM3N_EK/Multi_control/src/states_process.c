#include "states_process.h"

//const double fd_enc_dist= -0.000002197265625000000;
//const double pp_enc_dist= 0.000004870389001536911;
//const double md_enc_dist= 0.000009424777960769379;

const double fd_enc_dist=  -0.000002197265;
const double pp_enc_dist=  0.000004870389;
const double md_enc_dist=  0.000009424778;




void init_states(state_space *cup){
	
	cup->dx1=0;
	cup->dx2=0;
	cup->dZr=0;
	cup->x2=0;
	cup->x1=0;
	cup->x1_Zr=0;
	cup->x2_x1=0;
	cup->Zr=0;
		
}

int get_state(state_space *cup, int st){
	
		switch ( st )
		{
			case STATE_X2_X1:
			return cup->x2_x1;
			case STATE_dX2:
			return cup->dx2;
			case STATE_X1_Zr:
			return cup->x1_Zr;
			case STATE_dX1:
			return cup->dx1;
			case OTHMES_X1:
			return cup->x1;
			case OTHMES_Zr:
			return cup->Zr;
			case OTHMES_dZr:
			return cup->dZr;
			default:
			return 0;
		}
	
	
}

void set_state(state_space *cup, int st,int val){
	
	
}

void process_states(state_space *cup,state_space *ant_cup, enc_count *enc){
	
	
	cup->x2=((enc->posplate)*pp_enc_dist);	
	cup->x2_x1=((enc->motor_diferencial)*md_enc_dist);
	cup->Zr=((enc->floor_disturbance)*fd_enc_dist);
	cup->x1=(cup->x2-(cup->x2_x1));
	cup->x1_Zr=(cup->x1)-(cup->Zr);
	cup->dx2=((cup->x2)-(ant_cup->x2))*1000;
	cup->dx1=((cup->x1)-(ant_cup->x1))*1000;
	cup->dZr=((cup->Zr)-(ant_cup->Zr))*1000;
	
}

