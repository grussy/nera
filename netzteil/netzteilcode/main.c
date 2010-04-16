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
	DDRA |= (1<<PA0) | (1<<PA1); // DAC Pins
	//...........................................................

	// Initialize Drehgeber .....................................
	int32_t drehgeber = 0;
	DDRC &= ~((1<<DDC7) | (1<<DDC6)); // Set PORTC Pins 6 and 7 as input
	PORTC |= (1<<PC7) | (1<<PC6); // Internal pullups
	encode_init();
	//...........................................................



	//DDRD |= ( 1 << PD5 ); // LED
	lcd_string("hallo");
	
	//Light up both LEDS
	DDRA |= (1<<PA7) | (1<<PA6);
	PORTA |= (1<<PA7) | (1<<PA6);

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

		if(1)
		{
			drehgeber += encode_read2();
			setVoltage((long)drehgeber);
			//setdac(drehgeber);
			sprintf(buffer, "Soll: %li,%li V          ", getVoltage() / 100, getVoltage() % 100);
			set_cursor(0,1);
			lcd_string(buffer);
		}



		if (1)
		{
			set_cursor(0,2);
			sprintf(buffer, "Actual: %li,%li V          ", getadc_Volts() / 100, getadc_Volts() % 100);
			lcd_string(buffer);
			//uart_puts(buffer);
			ADCSRA |= (1<<ADSC); // Start ADConversion
			setadc_ready(0);
		}
	}
}
