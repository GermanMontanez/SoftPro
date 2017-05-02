# if !defined(ENC_HANDLE_H)
# define ENC_HANDLE_H

#define ENC_POSPLATE				1
#define ENC_FLOOR_DISTURBANCE 		2
#define ENC_MOTOR_DIFERENCIAL		3


// definición del tipo de dato

typedef signed int count;


typedef char Uart_Atender_uart(char atienda); // declaración de la funcion atender Uart

struct encoder_count{
	
	count posplate;
	count floor_disturbance;
	count motor_diferencial;

}encoder_count;

void init_count(encoder_count *cup, int enc);
void get_count(encoder_count *cup, int enc);
void inc_count(encoder_count *cup, int enc);

#endif