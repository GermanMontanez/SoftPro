# if !defined(ENC_HANDLE_H)
# define ENC_HANDLE_H

#define ENC_POSPLATE				1
#define ENC_FLOOR_DISTURBANCE 		2
#define ENC_MOTOR_DIFERENCIAL		3


// definición del tipo de dato

typedef signed long count;

//Estructura que contiene la informacion de los encoders
typedef struct 
{
	
	count posplate;
	count floor_disturbance;
	count motor_diferencial;

}enc_count;

void init_count(enc_count *cup);
count get_count(enc_count *cup, int enc);
void set_count(enc_count *cup, int enc,count val);
void inc_count(enc_count *cup, int enc);
void dec_count(enc_count *cup, int enc);

#endif