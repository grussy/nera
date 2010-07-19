#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "motor.h"

// constant definitions
#define RX_BUFFER_LENGTH 30
#define TX_BUFFER_LENGTH 30
#define FW_VER "0.0.1 pre-alpha"
#define US_PER_OVERFLOW 65535000000UL / F_CPU
#define PULSE_WIDTH 10

/* Variable Declaration */
volatile char cmd [RX_BUFFER_LENGTH];		//Command buffer for receiving from USART
char fields [][RX_BUFFER_LENGTH];
char buffer [TX_BUFFER_LENGTH];	//Send Buffer for sending via USART
volatile int cmd_recieved, overflows; 			//flag for USART '\n'.
unsigned int running, state, speed, steps;
struct { unsigned x:1; unsigned y:1; unsigned z:1; } move;
struct { unsigned x:1; unsigned y:1; unsigned z:1; } dir;

ISR(USART_RXC_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR;
	if (ReceivedByte == '\r'){
		cmd_recieved = 1;
	} else {
	UDR = ReceivedByte;
        int len = strlen(cmd);
	if (len > RX_BUFFER_LENGTH) {
		sprintf(cmd, "");
	}
        cmd[len] = ReceivedByte;
        cmd[len+1] = '\0';
	}
}

ISR(TIMER1_OVF_vect)
{
	overflows++;
}

void timerInit( void )
{
	TIMSK |= (1 << TOIE1); // enable Interrupt on Overlow
	TCCR1A &= ~((1 << COM1A1)|(1<<COM1B1)|(1 << COM1A0)|(1<<COM1B0)); // normal Mode
}

inline void startTimer( void )
{
	TCCR1B |= (1<<CS10); // no prescaler, start timer
}

inline void stopTimer( void )
{
	TCCR1B &= ~(1<<CS10); // stop timer
	TCNT1 = 0;
	overflows = 0;
}

inline unsigned long getTime ( void )
{
	int timer = TCNT1;
	return ((overflows*US_PER_OVERFLOW)+(timer/16))/1000;
}

inline void setDirections( void ) {
	char setval = 0x00;
	setval |= ( dir.x<<M1CW | dir.y<<M2CW | dir.z<<M3CW );
	RUNPORT &= ~( M1CW | M2CW | M3CW );
	RUNPORT |= setval;
}

inline void step( void ){
	RUNPORT &= ~( move.x<<M1CLK | move.y<<M2CLK | move.z<<M3CLK );	//this generates the step
	int i = 0;
	//for (i = 0; i < PULSE;i++) {} // wait a little
	// Set RUNPORT CLK Pins to high since step is falling edge triggered
	RUNPORT |= ( M1CLK | M2CLK | M3CLK);
	//for (i = 0; i < PULSE;i++) {} // wait a little
}

int main(void)
{
	move.x = 0; move.y = 0; move.z = 0; overflows = 0;
	dir.x = 0; dir.y = 0; dir.z = 0; running = 0;
	state = 1; speed = 10, steps = -1;
	USART_Init();
	uart_puts("\n#########################################################");
	uart_puts("\n#              Welcome on AtMega32's USART              #");
	uart_puts("\n#                this is a Stepper Test.                #");
	uart_puts("\n#########################################################\n");
	motorInit();
	timerInit();
	sei();
	while(1) {
		if (running) {
			switch(state) {
				case 1:	
					if (steps > 0) {
						steps--;
					} else if (steps <= 0) {
						steps--;
						running = 0;
						break;
					}
					RUNPORT &= ~( move.x<<M1CLK | move.y<<M2CLK | move.z<<M3CLK );	//this generates the step
					startTimer();
					state = 2;
					break;
				case 2:
					if ( getTime() >= PULSE_WIDTH ) {
						stopTimer();
						// Set RUNPORT CLK Pins to high since step is falling edge triggered
						RUNPORT |= ( 1<<M1CLK | 1<<M2CLK | 1<<M3CLK);
						state = 3;
						startTimer();
					}
					break;
				case 3:
					if ( getTime() >= 10000/speed ) {
					stopTimer();
					state = 1;
					}
			}
		}
		if (cmd_recieved) {
			char *ptr; ptr = cmd;
			int len = 0; int n;
			while (*ptr != '\0') {
				int items_read = sscanf(ptr, "%31[^;]%n", fields[len], &n);
				len ++;
				if (items_read == 1) {
					ptr += n; /* advance the pointer by the number of characters read */
				}
				if ( *ptr != ';' ) {
					break; /* didn't find an expected delimiter, done? */
				}
				++ptr; /* skip the delimiter */
			}
			uart_putc('\r');
			if ((strcmp(fields[0] ,"help")==0)|(strcmp(cmd ,"?")==0)){
				uart_puts("\n[Usage] Enter Command:");
				uart_puts("\n                 help or ? - for this Help");
				uart_puts("\n                     ver   - Display Firmware Version");
				uart_puts("\n                    step   - moving Motors make x steps");
				uart_puts("\n                   start   - Start contimous Stepping");
				uart_puts("\n                    stop   - Stop Contimous Stepping");
				uart_puts("\n                setSpeed   - set the Speed of Stepping");
				uart_puts("\n                 setMove   - set the Motors to move");
				uart_puts("\n                  setDir   - set the Direction of the Motors");
				uart_puts("\n                  setCtl   - set the Control Settings, wich are EN1-3 and HF1-3");
				uart_puts("\n                  setAdd   - set the Additional Settings, namely CTL1-3 and RS1-3");
			} else if (strcmp(fields[0], "ver") == 0){
				sprintf(buffer, "\n Firmware Version:   %s", FW_VER);
				uart_puts(buffer);
			} else if (strcmp(fields[0], "start") == 0){
				running = 1;
				uart_puts("\nOK.");
// 			} else if (strcmp(fields[0], "step") == 0){
// 				if (len == 2) {
// 					steps = atoi(fields[1]);
// 					running = 1;
// 				} else {
// 					uart_puts("[Unknown Parameters] Usage: step <number>");
// 				}
// 				sprintf(buffer, "\n[Motor] Steping %i times.", steps);
// 				uart_puts(buffer);
			} else if (strcmp(fields[0], "stop") == 0){
				stopTimer();
				running = 0; state = 1; steps = 0;
				RUNPORT |= ( 1<<M1CLK | 1<<M2CLK | 1<<M3CLK);
				uart_puts("\nOK.");
			} else if (strcmp(fields[0], "setSpeed") == 0){
				if (len == 2) {
					speed = atoi(fields[1]);
				} else {
					uart_puts("[Unknown Parameters] Usage: setSpeed <Speed>");
				}
				sprintf(buffer, "\n[Motor] Speed Settings: %i", speed);
				uart_puts(buffer);
			} else if (strcmp(fields[0], "setMove") == 0){
				if (len == 4) {
					move.x = atoi(fields[1]); move.y = atoi(fields[2]); move.z = atoi(fields[3]);
				} else {
					uart_puts("[Unknown Parameters] Usage: setMove <MoveX> <MoveY> <MoveZ>");
				}
				sprintf(buffer, "\n[Motor] Movement settings: %i %i %i", move.x, move.y, move.z);
				uart_puts(buffer);
			} else if (strcmp(fields[0], "setDir") == 0){
				if (len == 4) {
					dir.x = atoi(fields[1]); dir.y = atoi(fields[2]); dir.z = atoi(fields[3]);
				} else {
					uart_puts("[Unknown Parameters] Usage: setDir <DirectionX> <DirectionY> <DirectionZ>");
				}
				sprintf(buffer, "\n[Motor] Direction Settings: %i %i %i", dir.x, dir.y, dir.z);
				uart_puts(buffer);
			} else if (strcmp(fields[0], "setCtl") == 0){
				if (len == 7) {
					motorSetCtl(atoi(fields[1]), atoi(fields[2]), atoi(fields[3]), atoi(fields[4]), 
						atoi(fields[5]), atoi(fields[6]));
				} else {
					uart_puts("[Unknown Parameters] Usage: setCtl <EN1> <EN2> <EN3> <HF1> <HF2> <HF3>");
				}
				sprintf(buffer, "\n[Motor] Ctl Settings: %i %i %i %i %i %i", atoi(fields[1]), atoi(fields[2]),
					atoi(fields[3]), atoi(fields[4]), atoi(fields[5]), atoi(fields[6]));
				uart_puts(buffer);
			} else if (strcmp(fields[0], "setAdd") == 0){
				if (len == 7) {
					motorSetCtl(atoi(fields[1]), atoi(fields[2]), atoi(fields[3]), atoi(fields[4]), 
						atoi(fields[5]), atoi(fields[6]));
				} else {
					uart_puts("[Unknown Parameters] Usage: setCtl <CTL1> <CTL2> <CTL3> <RS1> <RS2> <RS3>");
				}
				sprintf(buffer, "\n[Motor] Add Settings: %i %i %i %i %i %i", atoi(fields[1]), atoi(fields[2]),
					atoi(fields[3]), atoi(fields[4]), atoi(fields[5]), atoi(fields[6]));
				uart_puts(buffer);
			} else {
				sprintf(buffer, "\n[Unknown Command]: %s", fields[0]);
				uart_puts(buffer);
				uart_puts("\n Enter \"?\" or \"help\" for a list of commands ...");
			}
			sprintf(cmd, "");
			cmd_recieved = 0;
			uart_putc('\r');
		}
	}
}