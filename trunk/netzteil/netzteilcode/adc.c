#include "adc.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"
#include <avr/io.h>

volatile int adc_u = 0;
volatile long adc_middle_u = 0;
volatile int adc_numbers_u = 0;

volatile int adc_i = 0;
volatile long adc_middle_i = 0;
volatile int adc_numbers_i = 0;

volatile int dac = 0;
volatile long voltage = 0;
volatile long current = 0;
volatile int countoverflows = 0;

ISR(ADC_vect)
{
	if (ADMUX & (1<<MUX0)) // We just measured the Current
	{
		adc_middle_i = adc_middle_i + ADC;
		adc_numbers_i++;
		ADMUX &= ~(1<<MUX0);
	}
	else					// Just measured the Voltage
	{
		adc_middle_u = adc_middle_u + ADC;
		adc_numbers_u++;
		ADMUX |= (1<<MUX0);
	}

	ADCSRA |= (1<<ADSC); // Start new ADConversion
}

ISR( TIMER2_COMP_vect )				// 1ms for manual movement
{
	if (1)
	{
		if (adc_numbers_u)			//Voltage Mittelwert bilden
		{
			adc_u = adc_middle_u / adc_numbers_u;
			adc_middle_u = 0;
			adc_numbers_u = 0;
		}
		if (adc_numbers_i)			//Current Mittelwert bilden
		{
			adc_i = adc_middle_i / adc_numbers_i;
			adc_middle_i = 0;
			adc_numbers_i = 0;
		}
		long abstand = getadc_Volts() - voltage;

		if (abs(abstand) > 1)
		{
			if (abstand > 1 && dac > 0)
			{
				setdac(dac - 1);
			}
			else if(dac < DACMAXINC)
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
		//uart_puts(buffer);
		}

}

void setdac(int value){
	dac = value;
	PORTB=value & 0xFF; // mask out 8 bits
	// 0x300=1100000000
	value=(value & 0x300)>>8; // mask out 2 msb bits and shift by 6
	// F3=11110011
	// FC=11111100
	PORTA=(PORTA & 0xFC)|value; // change PA0 and PA1 only
}

void init_ADC(void)
{
	ADMUX |= (1<<REFS0) | (1<<MUX2); // Reference: AVCC internal 5V | Single ended ADC4
	ADCSRA |= (1<<ADIE) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) | (1<<ADEN); // AD-Interrupt enabled | FCLK/128 | ADC enable
	// Timer 2 for adc average
	TCCR2 = 1<<WGM21^1<<CS21^1<<CS20^1<<CS22;		// CTC, XTAL /1024
	OCR2 = 255;	//
	TIMSK |= 1<<OCIE2;

	ADCSRA |= (1<<ADSC); // Start ADConversion

}

int getadc_u(void)
{
	return adc_u;
}

int getadc_i(void)
{
	return adc_i;
}

long getadc_Volts(void)
{
	long out;
	out = adc_u * (long)ADCMAXVOL;
	out = out / (long)ADCMAXINC;
	return out;
}

long getadc_Current(void)
{
	long out;
	out = adc_i * (long)ADCMAXCUR;
	out = out / (long)ADCMAXCURINC;
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

void setCurrent(long current100)
{
	current = current100;
}

long getCurrent(void)
{
	return current;
}
