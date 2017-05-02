#include <p30f4011.h>
#include "Buffer.h"

void BF_NewBuff(Buffer *buffer,unsigned char* buf,unsigned char cap)
{
	buffer->stored=0;
	buffer->index_in=0;
	buffer->index_out=0;
	buffer->capacidad=cap;
	buffer->buf=buf;
}

void BF_WriteBF(Buffer *buffer,unsigned char dato)
{

	buffer->buf[buffer->index_in]=dato;
	++buffer->index_in;
	if (buffer->index_in == BF_SIZE_QUEUE)
	buffer->index_in=0;
	BF_InUse(buffer);
}

unsigned char BF_ReadBF(Buffer *buffer)
{
	unsigned char dato; 
	dato=buffer->buf[buffer->index_out];
	buffer->buf[buffer->index_out]=0;
        ++buffer->index_out;
	if (buffer->index_out == BF_SIZE_QUEUE)
	buffer->index_out=0;
	BF_InUse(buffer);
	return dato;
}


float BF_InUse(Buffer *buffer)
{
	IEC1bits.U2RXIE=0;
	if (buffer->index_in >= buffer->index_out)
	buffer->stored = buffer->index_in - buffer->index_out;
	else
	buffer->stored = buffer->index_in - buffer->index_out + buffer->capacidad;
	float usedit=0.0;
	usedit=100*(float)(buffer->stored)/(float)(buffer->capacidad);
	IEC1bits.U2RXIE=1;
	return usedit;
}

