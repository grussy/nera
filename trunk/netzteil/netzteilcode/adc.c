#include "adc.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"
#include <avr/io.h>

volatile int adc = 0;
volatile int adc_ready = 1;
volatile long adc_middle = 0;
volatile int adc_numbers = 0;
volatile int dac = 500;
volatile long voltage = 0;
volatile int countoverflows = 0;

ISR(ADC_vect)
{
	//adc = ADC;
	//adc_ready = 1;
	adc_middle = adc_middle + ADC;
	adc_numbers++;
}

ISR( TIMER2_COMP_vect )				// 1ms for manual movement
{
	if (countoverflows == 5)
	{
		if (adc_numbers)
		{
			adc = adc_middle / adc_numbers;
			adc_middle = 0;
			adc_numbers = 0;
		}
		long abstand = getadc_Volts() - voltage;

		if (abs(abstand) > 1)
		{
			if (abstand > 1)
			{
				setdac(dac - 1);
			}
			else
			{
				setdac(dac + 1);
			}
		}
		countoverflows = 0;
	}
	else
		{
		countoverflows++;
		char buffer [20];
		sprintf(buffer,"%i",countoverflows);
		uart_puts(buffer);
		}

}

void setdac(int value){
	dac = value;
	PORTB=value & 0xFF; // mask out 8 bits
	// 0x300=1100000000
	value=(value & 0x300)>>8; // mask out 2 msb bits and shift by 6
	// F3=11110011
	// FC=11111100
	PORTC=(PORTC & 0xFC)|value; // change PC2 and PC3 only
}

void init_ADC(void)
{
	ADMUX |= (1<<REFS0) | (1<<MUX2); // Reference: AVCC internal 5V | Single ended ADC4
	ADCSRA |= (1<<ADIE) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) | (1<<ADEN); // AD-Interrupt enabled | FCLK/128 | ADC enable
	// Timer 2 for adc average
	TCCR2 = 1<<WGM21^1<<CS21^1<<CS20^1<<CS22;		// CTC, XTAL /1024
	OCR2 = 255;	//
	TIMSK |= 1<<OCIE2;

}

int getadc(void)
{
	return adc;
}

int getadc_ready(void)
{
	return adc_ready;
}
void setadc_ready(int ready)
{
	adc_ready = ready;
}

long getadc_Volts(void)
{
	long out;
	out = adc * (long)ADCMAXVOL;
	out = out / (long)ADCMAXINC;
	return out;
}

void setVoltage(long voltage100)
{
	voltage = voltage100;
}

long getVoltage(void)
{
	return voltage;
}

