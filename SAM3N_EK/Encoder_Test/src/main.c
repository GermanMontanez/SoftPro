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

#define F_CPU 32000000UL 



#include <asf.h>

//#include "math.h"





 int x=0;

 static void my_callback(void)
 {
	 // User code to execute when the overflow occurs here
	//PORTB.OUT=((0b00010000)^(PORTB.OUT));
	LED_Toggle(LED0_GPIO);
	
		
			
			for(long i=0;i<=4000;i++){}
	LED_Toggle(LED2_GPIO);
	
	x=PORTB.OUT;
	
 }

int main (void)
{
	

	//float my_matrix[3][3];

	//my_matrix[0][0]=-0.00953854578924394;	
	//my_matrix[0][1]=0.0115660013447277;
	//my_matrix[0][2]=-0.000560295653775730;
	//my_matrix[0][3]=0.000179581623547102;
	//my_matrix[1][0]=5.95771819849181e-06;
	//my_matrix[1][1]=0.00393259466454164;
	//my_matrix[1][2]=0.00788998059826681;
	//my_matrix[1][3]=3.71197983056811e-05;
	//my_matrix[2][0]=0.00791458881832966;
	//my_matrix[2][1]=4.88873455886385e-07;
	//my_matrix[2][2]=0.00237983090204913;
	//my_matrix[2][3]=5.95771819849181e-06;
	//my_matrix[3][0]=-0.00667782893402720;
	//my_matrix[3][1]=0.00778896799879194;
	//my_matrix[3][2]=-0.000407640919758241;
	//my_matrix[3][3]=2.79812966734471e-06;


	
	board_init();
	
	 pmic_init();
	 sysclk_init();
	 
	 tc_enable(&TCC0);
	 tc_set_overflow_interrupt_callback(&TCC0, my_callback);
	 tc_set_wgm(&TCC0, TC_WG_NORMAL);
	 tc_write_period(&TCC0, 500);
	 tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_HI);
	 cpu_irq_enable();
	 tc_write_clock_source(&TCC0, TC_CLKSEL_DIV64_gc);
		PORTB.DIR=0xFF;
		PORTB.OUT=0;
	
		while(1)
		{
			
			for(long i=0;i<=500;i++){
			

			
			}
			
			LED_Toggle(LED1_GPIO);
			
		}
	
	// Insert application code here, after the board has been initialized.
}
