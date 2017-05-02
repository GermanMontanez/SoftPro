#include "enc_handle.h"

//Inicializacion de la estructura que contiene los conteos de los encoder.
void init_count(enc_count *cup){
	
	(cup->floor_disturbance)=0;
	(cup->motor_diferencial)=0;
	(cup->posplate)=0;
	
}

count get_count(enc_count *cup, int enc){
	
	
	switch ( enc )
	{
		case ENC_FLOOR_DISTURBANCE:
			return (cup->floor_disturbance);
		case ENC_POSPLATE:
			return (cup->posplate);
		case ENC_MOTOR_DIFERENCIAL:
			return (cup->motor_diferencial);
			default:
			return 0;
	}
	
}

//Funcion para incrementar el conteo del el encoder que activo la interrupcion.
void inc_count(enc_count *cup, int enc){
	
		switch ( enc )
		{
			case ENC_FLOOR_DISTURBANCE:
				(cup->floor_disturbance)++;
				break;
			case ENC_POSPLATE:
				(cup->posplate)++;
				break;
			case ENC_MOTOR_DIFERENCIAL:
				(cup->motor_diferencial)++;
				break;
			default:
			break;
		}
}

//Funcion para decrementar el conteo del encoder que activo la interrupcion.
void dec_count(enc_count *cup, int enc){
	
	switch ( enc )
	{
		case ENC_FLOOR_DISTURBANCE:
		(cup->floor_disturbance)--;
		break;
		case ENC_POSPLATE:
		(cup->posplate)--;
		break;
		case ENC_MOTOR_DIFERENCIAL:
		(cup->motor_diferencial)--;
		break;
		default:
		break;
	}
}
void set_count(enc_count *cup, int enc,long val){
	
	switch ( enc )
	{
		case ENC_FLOOR_DISTURBANCE:
		(cup->floor_disturbance)=val;
		break;
		case ENC_POSPLATE:
		(cup->posplate)=val;
		break;
		case ENC_MOTOR_DIFERENCIAL:
		(cup->motor_diferencial)=val;
		break;
		default:
		break;
	}
}