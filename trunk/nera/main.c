#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "commands.h"
#include "lcd.h"
#include "segment.h"
#include "drehgeber.h"
#include "adc.h"
#include <util/delay.h>


int main(void)
{
	// Initialize Uart ..........................................
	char buffer [20];
	USART_Init();
	//...........................................................

	// Initialize LCD............................................
	lcd_init();
	//...........................................................

	// Initialize ADC / DAC .....................................
	init_ADC();
	DDRB = 0xFF; //DAC Pins
	DDRC |= (1<<PC0) | (1<<PC1); // DAC Pins
	//...........................................................

	// Initialize Drehgeber .....................................
	int32_t drehgeber = 0;
	DDRD &= ~((1<<DDD3) | (1<<DDD2)); // Set PORTD Pins 2 and 3 as input (external interupt pins)
	PORTD |= (1<<PD3) | (1<<PD2); // Internal pullups
	encode_init();	
	//...........................................................



	DDRD |= ( 1 << PD5 ); // LED
	
	
	// Everything initialized enable Interrupts now
	sei();

	while(1)
	{
		//USART abfragen ob zeichen verfügbar
		if (UCSRA & (1<<RXC)) 
		{
			//uart_puts("zeichen vorhanden\n");
			identify_command();
		}

		drehgeber += encode_read2();
		dac(drehgeber);
		sprintf(buffer, "Out: %li          ", drehgeber);
		set_cursor(0,1);
		lcd_string(buffer);

		if (getadc_ready())
		{
			set_cursor(0,2);
			sprintf(buffer, "In: %u          ", getadc());
			lcd_string(buffer);
			ADCSRA |= (1<<ADSC); // Start ADConversion
			setadc_ready(0);
		}
	}
}
