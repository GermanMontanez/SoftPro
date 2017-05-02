/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "math.h"

void initMPC_conf(void);

float aux2=0;

//Algoritmo de exonenciacion
//tiempo de ejecucion promedio 3.3184ms apx 106190 CCLK
double exp(double x){
	double s;
	double term;
	double r;
	int n;
	
	if (x<0){
		x=-x;
		s = 1;
		term = 1;
		n = 0;
		r = 0;
		while (r != s){
			r = s;
			n = n + 1;
			term = (x/n)*term;
			s = s + term;
			
		}
		s=1/s;
		}else{
		s = 1;
		term = 1;
		n = 0;
		r = 0;
		while (r != s){
			r = s;
			n = n + 1;
			term = (x/n)*term;
			s = s + term;
			
		}
	}
	
	return s;
}

ISR(PORTA_INT0_vect){

	for (int i=0;i<1;i++)
	{
		if (i==2)
		{
			aux2++;
		}else{
			
			aux2=aux2+2;
		}
		
	
	
	}
	
	//LED_Toggle(LED2_GPIO);
	
}
static void my_callback(void)
{	LED_Toggle(LED3_GPIO);
	
	
		//LED_Toggle(LED3_GPIO);
		aux2=(2/(exp(-2*aux2)+1))-1;
		//LED_Toggle(LED3_GPIO);
		if (aux2>10)
		{
			aux2=1/aux2;
		}
	
	LED_Toggle(LED3_GPIO);
}



void initMPC_conf(void){
	
		board_init();	//Inicializacion de los pines.
		sysclk_init();	//Inicializacion de el sistema de reloj 32MHz
		pmic_init();
				
		//Codigo de manejo de interrupciones por evento.
		PORTA.DIR=0;	
		PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
		PORTA.INT0MASK = PIN0_bm;
		PORTA.INTCTRL = PORT_INT0LVL_HI_gc;
		PMIC.CTRL |= PMIC_HILVLEN_bm;
		
		
	
		tc_enable(&TCC0);
		tc_set_overflow_interrupt_callback(&TCC0, my_callback);
		tc_set_wgm(&TCC0, TC_WG_NORMAL);
		tc_write_period(&TCC0, 8000);
		tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
		cpu_irq_enable();
		tc_write_clock_source(&TCC0, TC_CLKSEL_DIV4_gc);
		
		sei();
	
}

int main (void)
{

	 initMPC_conf();
	
	float x=0.6793;
	float y=0.14159;
	float z=0;
	int aux=0;

	
	while (1){
	//y=0.14159;
	//if (aux==2)
	//{
		//aux=0;
		//
		//
		//LED_Toggle(LED1_GPIO);
	//}
	//for (int i=0;i<6;i++)
	//{
			//if (y<1)
			//{
				//aux++;
				//LED_Toggle(LED0_GPIO);
						//
				//y=y*1.5;
				//LED_Toggle(LED0_GPIO);
			//}
			//
	//}
//
	//}
	//// Insert application code here, after the board has been initialized.
}
}
