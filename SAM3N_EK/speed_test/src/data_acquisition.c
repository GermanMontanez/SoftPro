#include "data_acquisition.h"
#include "conf_uart_serial.h"
#include <uart.h>

		
	
	 	




int digit_count(long dg){
	int i=0;

	while(dg!=0){
		dg/=10;
		i++;
	}
		return i;
}

//int decode_data(long data){
	//
	//int ndigit=0;
	//int tx_data=0;
	//int pow=0;
	//
	//if (data==0)
	//{
		//
		//}else{
		//
		//ndigit=3;
		//pow=ten_pow[(ndigit-1)];
		//
		//while(ndigit!=0)
		//{
			//tx_data=(int)((data/pow)%(10));
			//
			//pow/=10;
			//ndigit--;
						//
		//
		//}
		//
	//}
	//
	//
	//}
	//
	//
	//