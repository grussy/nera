#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

#define DATAPIN 4
#define DATAPORT PINA
#define DATAPULL PORTA
#define DATADDR DDRA

#define CLKPIN 2
#define CLKPORT PIND
#define CLKPULL PORTD
#define CLKDDR DDRD

#define del100 100000
#define delaymy(d) ( _delay_loop_2( (d/1000000)*(F_CPU/4) ) )

#define SEND 1
#define RECIVE 2 //ist der ruhe zustand.
#define BUSY 3   // zustand während des empfangs

unsigned char initmaus(void);
unsigned char mouse_getbit(void);
void mouse_setbit(unsigned char);
void mouse_sendbits(void);
void getbits(void);
void updatevalues(void);
unsigned char readbuttons(void);
signed int getdx(void);
signed int getdy(void);

volatile unsigned char parity,bitcount,sendbyte, recivebyte, recivedbyte, mousestatus, mouseerror, bytecount;
volatile unsigned char mbyte[4];


unsigned char initmaus(void)
{
	cli();
	uart_puts("[DEBUG] Entering initmaus Interrupts disabled\n");
	sendmaus(0xFF);	//reset
	//uart_puts("[DEBUG] CAll of sendmaus1 finished\n");
	while(mousestatus == SEND){}
	uart_puts("[DEBUG] send trough\n");
	while(mousestatus == RECIVE){}
	uart_puts("[DEBUG] recive trough\n");
	while(mousestatus == BUSY){}
	
	uart_puts("[DEBUG] send recive busy through\n");	

	sendmaus(0xf6);	// defaultwerte auswählen 
	
	while(mousestatus == SEND){}
	while(mousestatus == RECIVE){}
	while(mousestatus == BUSY){}	
	
	sendmaus(0xf4);	// protokoll einschalten 
	
	while(mousestatus == SEND){}
	while(mousestatus == RECIVE){}
	while(mousestatus == BUSY){}
	bytecount=0;
	
	sei();
	return (0);
}


unsigned char mouse_getbit(void)
{
	//uart_puts("[DEBUG] getbit called, bit was:\n");
	char buffer;
	


	buffer =  DATAPORT & ( 1 << DATAPIN );
	uart_puts(buffer);
	return buffer;
}

void mouse_setbit(unsigned char bit)
{
	//uart_puts("[DEBUG] mouse_setbit called\n");
	bit = bit & 0x01; // nur das erste bit  im parameter beachten 
	DATADDR |= (1 << DATAPIN);
	if(bit==0)
	{
		DATAPULL &= ~(1 << DATAPIN);
		//uart_puts("[DEBUG] cleared bit\n");
	}
	else
	{
		DATAPULL |= (1 << DATAPIN);
		//uart_puts("[DEBUG] set bit\n");
		parity++;//parität errechnen;
	}
	DATADDR &= ~(1 << DATAPIN);
}

void mouse_sendbits(void) // sendet die einzelnen bits an die maus
{
	char buffer[12];
	sprintf(buffer, "%u\n", bitcount);
	uart_puts("[DEBUG]Bitcount in mouse_sendbits");
	uart_puts(buffer);
	if(bitcount<8)		//bits setzen und dann um eine position nach rechts schieben
	{
		mouse_setbit(sendbyte);
		sendbyte=sendbyte/2;
	}
	else
	{
		if(bitcount==8) //paritäts bit setzen 
		{
		 	mouse_setbit(~parity);
		 	parity=0;
		}
		if(bitcount==9)//stopbit setzen
		{
			 mouse_setbit(0x01);
		}
		if(bitcount==10)//acknowledge abfangen
		{
			DATADDR &= ~(1 << DATAPIN)
;
			mouseerror = mouse_getbit();
			mousestatus = RECIVE;
			bitcount = 0xff;
			sendbyte = 0;
			recivebyte = 0;
		}
	}
	bitcount++;
}

void getbits(void) // liest die einzelnen bits zu sammen und fast sie zu einem byte
{
	if(bitcount==0)
	{
		mousestatus = BUSY;
	}
	else
	{
		if(bitcount<9)
		{
			recivebyte=recivebyte|(mouse_getbit()<<(bitcount-1));
		}
	 	else
	 	{
		  	if(bitcount==9)
		  	{
		  		parity=mouse_getbit();
		  		recivedbyte=recivebyte;
		  	}
		  	if(bitcount==10)
		  	{
		  		updatevalues();
		  		mousestatus = RECIVE;
		  		bitcount=0xFF;
		  		recivebyte = 0;
		  	}
		}
	}
	bitcount++;
}

void updatevalues(void)
{
	mbyte[bytecount++]=recivedbyte;
	if (bytecount==3)
	{
		bytecount=0;
	}
}

unsigned char readbuttons(void)
{
	return((mbyte[0]&0xF7));
}

signed int getdx(void)
{
	signed int v;
	unsigned char f;
	
	f=mbyte[0];
	f=f>>4;
	f=f&0x01;
	
	if(f==0x01)
	{
		v=mbyte[1];
		v=v|0xff00;
	}
	else
	{
		v=0x0000;
		v=v|mbyte[1];
	}
	mbyte[1]=0x00;
	mbyte[0]=mbyte[0]&0xEF;
	return v;
}

signed int getdy(void)
{
	signed int v;
	unsigned char f;
	
	f=mbyte[0];
	f=f>>5;
	f=f&0x01;
	
	if(f==0x01)
	{
		v=mbyte[2];
		v=v|0xff00;
	}
	else
	{
		v=0x0000;
		v=v|mbyte[2];
	}
	
	mbyte[2]=0x00;
	mbyte[0]=mbyte[0]&0x6F;
	return v;
}


//****************************************************************************

void sendmaus(unsigned char mausbyte)
{
cli();
uart_puts("[DEBUG] Just entered sendmaus() Interrupts disabled\n");
bytecount=0;
bitcount=0;

DATADDR |= (1 << DATAPIN);
CLKDDR |= (1 << CLKPIN);
sendbyte=mausbyte;
mousestatus = SEND;
CLKPULL &= ~(1 << CLKPIN);
DATAPULL |= (1 << DATAPIN);
//delaymy(300);
_delay_ms(1000);
DATAPULL &= ~(1 << DATAPIN);
//CLKPULL |= (1 << CLKPIN);
CLKDDR &= ~(1 << CLKPIN);
//CLKPULL &= ~(1 << CLKPIN);
//uart_puts("[DEBUG] Messing around with PINS DIRECTIONS and ... now settig Interruptflag\n");
GIFR |= (1 << INTF0); //Setze Interruptflag
//uart_puts("[DEBUG] Flag set\n");
//sei();
_delay_ms(20);
uart_puts("[DEBUG] Interrupts enabled exiting sendmaus()\n");
sei();
}
