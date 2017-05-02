#if !defined(BUFFER_H)

#define BUFFER_H


#define BF_SIZE_QUEUE        5
#define BF_SIZE             128



typedef struct Buffer Buffer; 

struct Buffer
{
	unsigned char stored;
	unsigned char *buf;
        unsigned char capacidad;
        unsigned char index_in;
        unsigned char   index_out;
};


void BF_NewBuff(Buffer *buffer,unsigned char* buf, unsigned char cap);
unsigned char BF_ReadBF(Buffer *buffer);
void BF_WriteBF(Buffer *buffer,unsigned char dato);
float BF_InUse(Buffer *buffer);

#endif
