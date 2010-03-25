#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "commands.h"
#include "lcd.h"
#include "segment.h"



int int1, int2 = 0;
unsigned int saved_timer = 0;
unsigned long overflows, saved_overflows=0;


void init_Interrupts( void )
{
	// interrupt on change on INT0 and INT1
	MCUCR = (0<<ISC01) |(1<<ISC00);
	// turn on interrupts!
	GIMSK  |= (1<<INT0);
}

ISR(TIMER0_OVF_vect)
{
	if (OCR1A < 1023)
	{
		OCR1A++;
		OCR1B++;
	}
	else
	{
		OCR1A = 0;
		OCR1B = 0;

	}
}

void pwmSet( unsigned int number )
{
	//stop Timer
	//TCCR1B &= ~(1<<CS10);
	//set PWM
	
	OCR1A = number;

}

void pwmInc ( void )
{
	
	OCR1A = OCR1A + 10;
}

void init_Timer( void )
{
	TCCR1A |= (1<<COM1A1) | (1<<WGM10)|(1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<CS10);

	OCR1A = 650;
}
void dac(int value){
	PORTB=value & 0xFF; // mask out 8 bits
	// 0x300=1100000000
	value=(value & 0x300)>>8; // mask out 2 msb bits and shift by 6
	// F3=11110011
	// FC=11111100
	PORTC=(PORTC & 0xFC)|value; // change PC2 and PC3 only
}


unsigned  char v= 0;
signed char r=0;
unsigned char t=0;

int main(void)
{
	char buffer [20];
	USART_Init();
	//lcd_init();
	//init_Interrupts();
	//initmaus();
	
	init_Timer();
	sei();

	
 
    	set_cursor(0,2);
 
    	//lcd_string("Hello World!");

	DDRD |= ( 1 << PD5 );
	DDRB = 0xFF;
	DDRC |= (1<<PC0) | (1<<PC1);
	
	dac(275);
 
	//DDRA |= (1 << PA4 );
	//initSegment();
	
	//segmentSetNumber(2);

	
	while(1)
	{
		//USART abfragen ob zeichen verfügbar
		if (UCSRA & (1<<RXC)) 
		{
			//uart_puts("zeichen vorhanden\n");
			identify_command();
		}
		//pwmInc();
		//PORTD = PIND ^ ( 1 << PD5 );
		/*r=(signed char)getdy();	// und gibt diese an POROTC aus 
		if((v+r)>255)
		{
			v=255;
		}
		else
		{
			if((v+r)<0)
			{
				v=0;
			}
			else
			{
				v=v+r;
			}
		}
		
		//uart_puts(v);*/
	}
}