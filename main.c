#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "commands.h"
#include "lcd.h"
#include "segment.h"
#include <util/delay.h>

#define	XTAL		16e6			// 16MHz
 
#define PHASE_A		(PIND & 1<<PA3)
#define PHASE_B		(PIND & 1<<PA2)

volatile int adc = 0;
volatile int adc_ready = 1;	
unsigned int prell_timer = 0;
unsigned long overflows, saved_overflows=0;

volatile int8_t enc_delta;			// -128 ... 127
static int8_t last;

void encode_init( void )
{
  int8_t new;
 
  new = 0;
  if( PHASE_A )
    new = 3;
  if( PHASE_B )
    new ^= 1;					// convert gray to binary
  last = new;					// power on state
  enc_delta = 0;
  TCCR0 = 1<<WGM01^1<<CS01^1<<CS00;		// CTC, XTAL / 64
  OCR0 = (uint8_t)(XTAL / 64.0 * 1e-3 - 0.5);	// 1ms
  TIMSK |= 1<<OCIE0;
}
 
 
ISR( TIMER0_COMP_vect )				// 1ms for manual movement
{
  int8_t new, diff;
 
  new = 0;
  if( PHASE_A )
    new = 3;
  if( PHASE_B )
    new ^= 1;					// convert gray to binary
  diff = last - new;				// difference last - new
  if( diff & 1 ){				// bit 0 = value (1)
    last = new;					// store new as next last
    enc_delta += (diff & 2) - 1;		// bit 1 = direction (+/-)
  }
}

int8_t encode_read1( void )			// read single step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = 0;
  sei();
  return val;					// counts since last call
}

int8_t encode_read2( void )			// read two step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = val & 1;
  sei();
  return val >> 1;
}

void init_Interrupts( void )
{
	DDRD &= ~((1<<DDD3) | (1<<DDD2)); // Set PORTD Pins 2 and 3 as input (external interupt pins)
	PORTD |= (1<<PD3) | (1<<PD2); // Internal pullups
	// interrupt on change on INT0 and INT1
	//MCUCR = (0<<ISC01) |(1<<ISC00) | (0<<ISC11) | (1<<ISC10);
	// turn on interrupts!
	//GIMSK  |= (1<<INT0);
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
ISR(INT0_vect)
{
	if (PIND & (1<<PD3))
	{
		uart_puts("links\n");
	}
	else
	{
		uart_puts("rechts\n");
	}
	
}

ISR(ADC_vect)
{
	adc = ADC;
	adc_ready = 1;
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

void init_ADC(void)
{
	ADMUX |= (1<<REFS0) | (1<<MUX2); // Reference: AVCC internal 5V | Single ended ADC4
	ADCSRA |= (1<<ADIE) | (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) | (1<<ADEN); // AD-Interrupt enabled | FCLK/128 | ADC enable
	
}


unsigned  char v= 0;
signed char r=0;
unsigned char t=0;

int main(void)
{
	char buffer [20];
	USART_Init();
	lcd_init();
	init_Interrupts();
	init_ADC();
	//initmaus();
	int32_t val = 0;
 
	encode_init();	
	//init_Timer();
	sei();

	
 
    	set_cursor(0,2);
 
    	lcd_string("Hello World!");

	set_cursor(0,1);

	DDRD |= ( 1 << PD5 );
	DDRB = 0xFF;
	DDRC |= (1<<PC0) | (1<<PC1);
	
	dac(400);
 
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



		val += encode_read2();
		dac(val);
		sprintf(buffer, "Out: %u          ", val);
		set_cursor(0,1);
		lcd_string(buffer);

		if (adc_ready)
		{
			set_cursor(0,2);
			sprintf(buffer, "In: %u          ", adc);
			lcd_string(buffer);
			ADCSRA |= (1<<ADSC); // Start ADConversion
			adc_ready = 0;
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