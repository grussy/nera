#include "adc.h"
#include <avr/interrupt.h>

volatile int adc = 0;
volatile int adc_ready = 1;

ISR(ADC_vect)
{
	adc = ADC;
	adc_ready = 1;
}

void dac(int value){
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
