#if !defined(BUFFER_H) 
#define BUFFER_H
#define CAPACITY_MAX	5
typedef struct Buffer Buffer;
struct Buffer{
unsigned char Counts, Buffer_Capacity, Buffer_Write_Data, Buffer_Read_Data;
unsigned char *P_Buffer_Data;
};
void Buffer_Init(Buffer *P_Buffer, unsigned char* P_Buffer_Data, unsigned char Buffer_Cap);
unsigned char Buffer_Read(Buffer *P_Buffer);
void Buffer_Write(Buffer *P_Buffer, unsigned char Data);
float Buffer_Percent_Usage(Buffer *P_Buffer);  
#endif
