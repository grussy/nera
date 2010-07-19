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

#define VOLTAGE_MAX 3300
#define CURRENT_MAX 200


int main(void)
{
	// Initialize Uart ..........................................
	char buffer [20];
	char buffer2 [20];
	USART_Init();
	//...........................................................

	// Initialize LCD............................................
	//lcd_init(); LCD ist Kaputt
	//...........................................................

	// Initialize ADC / DAC .....................................
	init_ADC();
	DDRB = 0xFF; //DAC Pins
	DDRA |= (1<<PA0) | (1<<PA1); // DAC Pins
	//...........................................................

	// Initialize Drehgeber .....................................
	int32_t soll_voltage = 0;
	int32_t soll_current = 0;
	DDRC &= ~((1<<DDC7) | (1<<DDC6) | (1<<DDC0) | (1<<DDC1)); // Set PORTC Pins 6 and 7 as input
	PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC0) | (1<<PC1); // Internal pullups
	encode_init();
	//...........................................................



	//DDRD |= ( 1 << PD5 ); // LED
	//lcd_string("hallo");
	
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
		int test = encode_read4();
		int test2 = encode_read2();


		if(test | test2)
		{
			//int test = encode_read4();
			//sprintf(buffer, "test %i \n", test);
			//uart_puts(buffer);
			if (test > 1) test = 100;
			if (test < -1) test = -100;

			soll_voltage += test; //lese die Voltage Drehgeber Veränderung

			if (soll_voltage < 0) soll_voltage = 0; // keine negativen Werte zulassen
			if (soll_voltage > VOLTAGE_MAX) soll_voltage = VOLTAGE_MAX; // nicht größer als das Maximum
			setVoltage((long)soll_voltage);
			//setdac(drehgeber);
			//long voltagesoll = getVoltage();
			long voltageist = getadc_Volts();
			sprintf(buffer, "U: %li,%li[%li,%li] V       \n", soll_voltage / 100, soll_voltage % 100, voltageist / 100, voltageist % 100);
			//set_cursor(0,1);
			//lcd_string(buffer);
			uart_puts(buffer);
		//}



		//if(test2)
		//{
			//int test2 = encode_read2();
			if (test2 > 1) test2 = 100;
			if (test2 < -1) test2 = -100;

			soll_current += test2; //lese die Voltage Drehgeber Veränderung

			if (soll_current < 0) soll_current = 0; // keine negativen Werte zulassen
			if (soll_current > CURRENT_MAX) soll_current = CURRENT_MAX; // nicht größer als das Maximum
			setCurrent((long)soll_current);
			//setdac(drehgeber);*/
			//long currentsoll = getCurrent();
			long currentist = getadc_Current();

			sprintf(buffer2, "I: %li,%li[%li,%li] A       \n", soll_current / 100, soll_current % 100, currentist / 100, currentist % 100);
			//set_cursor(0,2);
			//lcd_string(buffer2);
			uart_puts(buffer2);
		}
	}
}
