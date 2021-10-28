/*
 * INF_PUMP.c
 *
 * Created: 10/28/2021 11:30:37 AM
 * Author : agva
 */ 

#define  F_CPU 4000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "UART_1_AVR128DA64.h"
#include "ADC_AVR128DA64.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define Stepper1PORT  PORTC
#define Stepper1_direction_pin  (1<<4)  //direction pin pc2
#define Stepper1_Step_pin       (1<<3)  // direction pin pc3

bool state = false;
unsigned long int sensor_value;
void timer_init();

int main(void)
{   sei();
	Stepper1PORT.DIR |= Stepper1_direction_pin; // setting direction pin as a output
	Stepper1PORT.DIR |= Stepper1_Step_pin;  //Setting step pin as a output
	PORTA.DIR |= (1<<4);
   // Stepper1PORT.OUTSET |= Stepper1_direction_pin;
    /* Replace with your application code */
    timer_init();
	USART1_init(9600);
	ADC0_init();
	ADC0_start();
	while (1) 
    {   //_delay_ms(10);
		int i=200;
		while(i)
		{
			sensor_value = ADC0_read(channel_1)+sensor_value;
			
			i--;
		}
		sensor_value = sensor_value/200;
		USART1_sendInt(sensor_value);
		 if(sensor_value <= 300)
		 {
			 
			 PORTA.OUTSET |= (1<<4);
		 }
		 if(sensor_value >= 900)
		 {
			
			 PORTA.OUTCLR |= (1<<4);
		 }
     }  
}

void timer_init()
{
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.PER = 100; //1 msec if prescaler is 64
	TCA0.SINGLE.CTRLA |= 0x0B; //Timer Enable 
	TCA0.SINGLE.INTCTRL |= (1<<0); //Ovf interrupt Enable
}

ISR(TCA0_OVF_vect)
{  
	state=!state;
	if(state)
	{
		Stepper1PORT.OUTSET |= Stepper1_Step_pin;
		
	}
	else
	{
		
	    Stepper1PORT.OUTCLR |= Stepper1_Step_pin;	
	
	}
	TCA0.SINGLE.INTFLAGS |= (1<<0); 
	
}