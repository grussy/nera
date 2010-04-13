#include "motor.h"
#include <avr/io.h>

void motorInit( void ) {
// 	// First set all used Pins to Outputs.
// 	RUNDDR |= (1<<M1CLK | 1<<M2CLK | 1<<M3CLK | 1<<M1CW | 1<<M2CW | 1<<M3CW);
// 	CTLDDR |= (1<<M1EN | 1<<M2EN | 1<<M3EN | 1<<M1HF | 1<<M2HF | 1<<M3HF);
// 	ADDDDR |= (1<<M1CTL | 1<<M2CTL | 1<<M3CTL | 1<<M1RS | 1<<M2RS | 1<<M3RS);
	// Now Set The CTL and The ADD Port
	// All Motors Powered and Fullstep
	CTLPORT |= ( 1<<M1EN | 1<<M2EN | 1<<M3EN | 1<<M1HF | 1<<M2HF | 1<<M3HF );
	// Inhibit Chopping, Reset High
	ADDPORT |= ( 1<<M1RS | 1<<M2RS | 1<<M3RS);
	// Set RUNPORT CLK Pins to high since step is falling edge triggered
	RUNPORT |= ( 1<<M1CLK | 1<<M2CLK | 1<<M3CLK);
}

void motorSetCtl( int en1, int en2, int en3, int hf1, int hf2, int hf3 ){
	char setval = 0x00;
	if ( en1 ) {
		setval |= (1<<M1EN);
	}
	if ( en2 ) {
		setval |= (1<<M2EN);
	}
	if ( en3 ) {
		setval |= (1<<M3EN);
	}
	if ( hf1 ) {
		setval |= (1<<M1HF);
	}
	if ( hf2 ) {
		setval |= (1<<M2HF);
	}
	if ( hf3 ) {
		setval |= (1<<M3HF);
	}
	CTLPORT &= ~( 1<<M1EN | 1<<M2EN | 1<<M3EN | 1<<M1HF | 1<<M2HF | 1<<M3HF ); //reset all except the unused ports
	CTLPORT |= setval; // set the wanted settings
}

void motorSetEnable(int en1, int en2, int en3) {
	char setval = 0x00;
	if ( en1 ) {
		setval |= (1<<M1EN);
	}
	if ( en2 ) {
		setval |= (1<<M2EN);
	}
	if ( en3 ) {
		setval |= (1<<M3EN);
	}
	CTLPORT &= ~( M1EN | M2EN | M3EN ); //reset all except the unused ports
	CTLPORT |= setval; // set the wanted settings
}

void motorSetStep(int hf1, int hf2, int hf3) {
	char setval = 0x00;
	if ( hf1 ) {
		setval |= (1<<M1HF);
	}
	if ( hf2 ) {
		setval |= (1<<M2HF);
	}
	if ( hf3 ) {
		setval |= (1<<M3HF);
	}
	CTLPORT &= ~( M1HF | M2HF | M3HF ); //reset all except the unused ports
	CTLPORT |= setval; // set the wanted settings
}

void motorSetAdd( int ctl1, int ctl2, int ctl3, int rs1, int rs2, int rs3 ){
	char setval = 0x00;
	if ( ctl1 ) {
		setval |= (1<<M1CTL);
	}
	if ( ctl2 ) {
		setval |= (1<<M2CTL);
	}
	if ( ctl3 ) {
		setval |= (1<<M3CTL);
	}
	if ( rs1 ) {
		setval |= (1<<M1RS);
	}
	if ( rs2 ) {
		setval |= (1<<M2RS);
	}
	if ( rs3 ) {
		setval |= (1<<M3RS);
	}
	ADDPORT &= ~( 1<<M1CTL | 1<<M2CTL | 1<<M3CTL | 1<<M1RS | 1<<M2RS | 1<<M3RS ); //reset all except the unused ports
	ADDPORT |= setval; // set the wanted settings
}

void motorSetChopper( int ctl1, int ctl2, int ctl3 ){
	char setval = 0x00;
	if ( ctl1 ) {
		setval |= (1<<M1CTL);
	}
	if ( ctl2 ) {
		setval |= (1<<M2CTL);
	}
	if ( ctl3 ) {
		setval |= (1<<M3CTL);
	}
	ADDPORT &= ~( 1<<M1CTL | 1<<M2CTL | 1<<M3CTL ); //reset all except the unused ports
	ADDPORT |= setval; // set the wanted settings
}

void motorSetReset( int rs1, int rs2, int rs3 ){
	char setval = 0x00;
	if ( rs1 ) {
		setval |= (1<<M1RS);
	}
	if ( rs2 ) {
		setval |= (1<<M2RS);
	}
	if ( rs3 ) {
		setval |= (1<<M3RS);
	}
	ADDPORT &= ~( 1<<M1RS | 1<<M2RS | 1<<M3RS ); //reset all except the unused ports
	ADDPORT |= setval; // set the wanted settings
}

inline void motorSetDirections(int cw1, int cw2, int cw3) {
	char setval = 0x00;
	if ( cw1 ) {
		setval |= (1<<M1CW);
	}
	if ( cw2 ) {
		setval |= (1<<M2CW);
	}
	if ( cw3 ) {
		setval |= (1<<M3CW);
	}
	RUNPORT &= ~( 1<<M1CW | 1<<M2CW | 1<<M3CW );
	RUNPORT |= setval;
}

inline void motorStep( int clk1, int clk2, int clk3 ){
	char setval = 0x00;
	if ( clk1 ) {
		setval |= (1<<M1CLK);
	}
	if ( clk2 ) {
		setval |= (1<<M2CLK);
	}
	if ( clk3 ) {
		setval |= (1<<M3CLK);
	}
	RUNPORT &= ~(setval);	//this generates the step
	int i = 0;
	for (i = 0; i < PULSE;i++) {} // wait a little
	// Set RUNPORT CLK Pins to high since step is falling edge triggered
	RUNPORT |= ( 1<<M1CLK | 1<<M2CLK | 1<<M3CLK);
	for (i = 0; i < PULSE;i++) {} // wait a little
}

inline void motorStepDir( int clk1, int clk2, int clk3, int cw1, int cw2, int cw3 ){
	motorSetDirections(cw1, cw2, cw3);
	motorStep(clk1, clk2, clk3);
}