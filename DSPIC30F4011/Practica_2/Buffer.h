#if (!defined(BUFFER_H))
#define BUFFER_H
typedef unsigned char Buff_Data;
typedef signed char Buff_Index; 
typedef struct Buffer Buffer;
struct	Buffer{
	unsigned int size;
	Buff_Index	Buff_In;
	Buff_Index	Buff_Out;
	Buff_Index	Buff_Max;	
	Buff_Index	Buff_Min;	
	Buff_Index	Buff_Stored; 
	Buff_Data	*Buff_Array;
};
void Buff_XON_OFF(Buffer *P_Buffer);
unsigned char Buff_Load(Buffer *P_Buffer, Buff_Data NewData);
Buff_Data Buff_Unload(Buffer *P_Buffer);
void Buff_Init(Buffer *P_Buffer, Buff_Data *NewArray);
void incremente_ISalida(Buffer *P_Buffer);

#endif



