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
#include "enc_handle.h"
#include "states_process.h"
#include "control_process.h"

//Configuracion del canal para el timer interrup
#define TC_CHANNEL_WAVEFORM		0
#define IRQ_PRIOR_TC			1

// Parametros de PWM
#define PWM_FREQUENCY		560247
#define PERIOD_VALUE		100
#define INIT_DUTY_VALUE		0
#define PWM_FC_OUTPUT		PIO_PC21_IDX //Pin C21 output pwm
#define PWM_FC_FLAGS		(PIO_PERIPH_B | PIO_DEFAULT)
#define PWM_FC_CHANNEL		PWM_CHANNEL_3

//Configuracion de los puertos para interrupcion de puerto.
#define GPIO_PORT_EVENT_GENERAL  (PIO_PC18_IDX|PIO_PC29_IDX|PIO_PC19_IDX|PIO_PC30_IDX|PIO_PC20_IDX|PIO_PC31_IDX)
        
#define GPIO_PORT_EVENT_1			(PIO_PC18_IDX)//PIN A ENC_POSPLATE
#define GPIO_PORT_EVENT_2			(PIO_PC19_IDX)//PIN B ENC_POSPLATE
#define GPIO_PORT_EVENT_3			(PIO_PC29_IDX)//PIN A ENC_MOTOR_DIFERENCIAL
#define GPIO_PORT_EVENT_4			(PIO_PC30_IDX)//PIN B ENC_MOTOR_DIFERENCIAL
#define GPIO_PORT_EVENT_5			(PIO_PC20_IDX)//PIN A ENC_FLOOR_DISTURBANCE
#define GPIO_PORT_EVENT_6			(PIO_PC31_IDX)//PIN B ENC_FLOOR_DISTURBANCE

#define PORT_EVENT_MASK_GENERAL		(PIO_PC18|PIO_PC29|PIO_PC19|PIO_PC30|PIO_PC20|PIO_PC31)

#define PORT_EVENT_MASK_1			PIO_PC18
#define PORT_EVENT_MASK_2			PIO_PC19
#define PORT_EVENT_MASK_3			PIO_PC29
#define PORT_EVENT_MASK_4			PIO_PC30
#define PORT_EVENT_MASK_5			PIO_PC20
#define PORT_EVENT_MASK_6			PIO_PC31

#define GPIO_PORT_EVENT_GENERAL_FLAGS    (PIO_INPUT | PIO_PULL | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define ALL_PORT_EVENT_PIO			PIOC
#define ALL_PORT_EVENT_ID			ID_PIOC
#define PORT_EVENT_GENERAL_TYPE		PIO_INPUT
#define PORT_EVENT_GENERAL_ATTR		PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_EDGE

#define IRQ_PRIOR_PIO			0

pwm_channel_t g_pwm_channel_fc;

const char ancii_code[14]={48,49,50,51,52,53,54,55,56,57,68,77,80,70};
				   //value  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, D, M, P, F
				     //pos  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13
					 

const int ten_pow[5]={1,10,100,1000,10000};	

enc_count encoder_count;
enc_count encoder_sample;
state_space actual_states;
state_space ant_states;
double FC_ant=0;
int duty_cicle=50;

int i=0;
long aux=0;
//int p1=0;
//int p2=0;
//int p3=0;

//long f1_p1=0;
//long f1_p2=0;
//long f2_p1=0;
//long f2_p2=0;




//Prototipos de funcion.


void EventPIO_Handler1(uint32_t, uint32_t);
void EventPIO_Handler2(uint32_t, uint32_t);
void EventPIO_Handler3(uint32_t, uint32_t);

void conf_pwm(void);
void conf_tc(void);
void conf_port_event(void);
void decode_data(long data,int n);
void s_uart(int dc);


//Manejo de la interrupcion el encoder del plato de arriba de la suspension para su conteo.
void EventPIO_Handler1(uint32_t id, uint32_t mask)
{

	
	
	if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_1 == mask) 
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_1)==pio_get_pin_value(GPIO_PORT_EVENT_2))
		{
			inc_count(&encoder_count,ENC_POSPLATE);
			
			
		} else{
			
			dec_count(&encoder_count,ENC_POSPLATE);
		}
		
		

	}
	
	if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_2 == mask)
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_1)==pio_get_pin_value(GPIO_PORT_EVENT_2))
		{
			dec_count(&encoder_count,ENC_POSPLATE);
		
			
		} else{
			
			inc_count(&encoder_count,ENC_POSPLATE);
		}
		
   
	}
	


}

//Manejo de la interrupcion el encoder del actuador de la suspension para su conteo.

void EventPIO_Handler2(uint32_t id, uint32_t mask)
{
	
	
		if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_3 == mask) 
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_3)==pio_get_pin_value(GPIO_PORT_EVENT_4))
		{
			inc_count(&encoder_count,ENC_MOTOR_DIFERENCIAL);
				
			
		} else{
			
			dec_count(&encoder_count,ENC_MOTOR_DIFERENCIAL);
		}
		

	}
	
	if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_4 == mask)
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_3)==pio_get_pin_value(GPIO_PORT_EVENT_4))
		{
			dec_count(&encoder_count,ENC_MOTOR_DIFERENCIAL);
			
			
		} else{
			
			inc_count(&encoder_count,ENC_MOTOR_DIFERENCIAL);
		}
		

	}
	
}

//Manejo de la interrupcion el encoder del generador de perturbaciones de la suspension para su conteo.
void EventPIO_Handler3(uint32_t id, uint32_t mask)
{
	
	
		if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_5 == mask) 
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_5)==pio_get_pin_value(GPIO_PORT_EVENT_6))
		{
			inc_count(&encoder_count,ENC_FLOOR_DISTURBANCE);
			
		} else{
			
			dec_count(&encoder_count,ENC_FLOOR_DISTURBANCE);
		}
		

	}
	
	if (ALL_PORT_EVENT_ID == id && PORT_EVENT_MASK_6 == mask)
	{
		
		if (pio_get_pin_value(GPIO_PORT_EVENT_5)==pio_get_pin_value(GPIO_PORT_EVENT_6))
		{
			dec_count(&encoder_count,ENC_FLOOR_DISTURBANCE);
			
		} else{
			
			inc_count(&encoder_count,ENC_FLOOR_DISTURBANCE);
		}
		

	}
}

//Manejo de la interrupcion de timer que indica cuando se debe ejecutar la ley de control.
void TC0_Handler(void)
{	
	volatile uint32_t ul_dummy;

	/* Clear status bit to acknowledge interrupt */
	ul_dummy = tc_get_status(TC0, 0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);
	encoder_sample=encoder_count;
	process_states(&actual_states,&ant_states,&encoder_sample);
	#ifdef MPC_RN_CONTROL
	mpc_rn_control(&actual_states,&ant_states,&FC_ant);
	#endif
	#ifdef MPC_GN_CONTROL
	mpc_gn_control(&actual_states,&ant_states,&FC_ant);
	#endif
	#ifdef MPC_MP_CONTROL
	mpc_mp_control(&actual_states,&ant_states,&FC_ant);
	#endif
	#ifdef MPC_SS_CONTROL
	mpc_ss_control(&actual_states,&ant_states,&FC_ant);
	#endif
	duty_cicle=force2dutycicle(FC_ant);
	
	//Actualizacion de la señal de control en el pwm.
	pwm_channel_update_duty(PWM, &g_pwm_channel_fc,duty_cicle);



		//s_uart(10);	
		//aux=encoder_sample.motor_diferencial;
		//decode_data(aux,2);
		//decode_data(duty_cicle,2);
		//aux=encoder_sample.floor_disturbance;
		//decode_data(aux,2);
		//aux=encoder_sample.posplate;
		//decode_data(aux,2);
		//s_uart(11);


}


//Configuracion del modulo de timer.
void conf_tc(void)
{

	pmc_enable_periph_clk(ID_TC0);
	tc_init(TC0,TC_CHANNEL_WAVEFORM,TC_CMR_TCCLKS_TIMER_CLOCK2|TC_CMR_CPCTRG);
	tc_write_rc(TC0,TC_CHANNEL_WAVEFORM,12000);
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	NVIC_SetPriority((IRQn_Type) ID_TC0, IRQ_PRIOR_TC);
	tc_enable_interrupt(TC0,TC_CHANNEL_WAVEFORM,TC_IER_CPCS);
	
	
	
	tc_start(TC0,TC_CHANNEL_WAVEFORM);

}

//Configuracion de los vectores del manejo de la interrupcion de los pines de encoder.
void conf_port_event(void){
	
	pmc_enable_periph_clk(ALL_PORT_EVENT_ID);
	pio_set_debounce_filter(ALL_PORT_EVENT_PIO, PORT_EVENT_MASK_GENERAL, 10);
	
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_1, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler1);
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_2, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler1);
	
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_3, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler2);
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_4, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler2);
	 
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_5, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler3);
	pio_handler_set(ALL_PORT_EVENT_PIO, ALL_PORT_EVENT_ID,
	PORT_EVENT_MASK_6, PORT_EVENT_GENERAL_ATTR, EventPIO_Handler3);

	NVIC_EnableIRQ((IRQn_Type) ALL_PORT_EVENT_ID);
	pio_handler_set_priority(ALL_PORT_EVENT_PIO,
	(IRQn_Type) ALL_PORT_EVENT_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(ALL_PORT_EVENT_PIO, PORT_EVENT_MASK_GENERAL);

	
}

static void conf_uart(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

void conf_pwm(void){
	
	gpio_configure_pin(PWM_FC_OUTPUT, PWM_FC_FLAGS);
	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);

	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PWM_FC_CHANNEL);

	/* Set PWM clock A as PWM_FREQUENCY*PERIOD_VALUE (clock B is not used) */
	pwm_clock_t clock_setting = {
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);

	/* Initialize PWM channel for LED0 */
	/* Period is left-aligned */
	g_pwm_channel_fc.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	g_pwm_channel_fc.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	g_pwm_channel_fc.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_fc.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_fc.ul_duty = INIT_DUTY_VALUE;
	g_pwm_channel_fc.channel = PWM_FC_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_fc);

	pwm_channel_enable(PWM, PWM_FC_CHANNEL);
	
	
}

void decode_data(long data,int n){
	
	int ndigit=0;
	int tx_data=0;
	int pow=0;
	
	if (data==0)
	{
					int	b_uart=1;
			while(b_uart!=0){
				b_uart= uart_write(UART0,ancii_code[0]);
			}
		
		}else{
		
		ndigit=n;
		
		pow=ten_pow[(ndigit-1)];
		
		while(ndigit!=0)
		{
			tx_data=(int)((data/pow)%(10));
			
			int	b_uart=1;
			while(b_uart!=0){
				b_uart= uart_write(UART0,ancii_code[tx_data]);
			}
			
			
			pow/=10;
			ndigit--;
			

			
		}
		
	}
	
	
}

void s_uart(int dc){
	
	int	b_uart=1;
			while(b_uart!=0){
				b_uart= uart_write(UART0,ancii_code[dc]);
			}
	
}


//Programa principal
int main (void)
{
	
	//Inicializacion de estructuras.
	init_count(&encoder_count);
	init_count(&encoder_sample);
	init_states(&actual_states);
	init_states(&ant_states);
		
	//Definicion del uso de los pines
	pio_set_output(PIOB, PIO_PB14, LOW, DISABLE, ENABLE);
	pio_set_output(PIOA, PIO_PA23, LOW, DISABLE, ENABLE);
	pio_set_output(PIOA, PIO_PA25, HIGH, DISABLE, ENABLE);

	//Inicializacion del reloj del sistema.
	sysclk_init();
	board_init();
	
	//Activacion de los perifericos usados.
	conf_pwm();
	conf_tc();
	conf_port_event();
	conf_uart();
	

		
	while(1)
	{	

	}
	
}
