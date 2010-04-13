#include <avr/io.h>

// calibration definitions
#define PULSE 1000

// Port definitions
// RUNPORT contains the CLK and CW Pins descriptions see below
#define RUNPORT PORTA
#define RUNDDR	DDRA

// CTLPORT contains the EN and HF Pins descriptions see below
#define CTLPORT PORTB
#define CTLDDR	DDRB

// ADDPORT contains the CTL and RS Pins descriptions see below
#define ADDPORT	PORTC
#define ADDDDR	DDRC

//Pin definitions
// The CLK Pins make the step (Falling Edge triggered)
#define M1CLK PA7
#define M2CLK PA6
#define M3CLK PA5

// The CW Pins decide Direction (1 = CW = clockwise
#define M1CW PA4
#define M2CW PA3
#define M3CW PA2

// The EN Pins are for Power saving (0 = no current
#define M1EN PB7
#define M2EN PB6
#define M3EN PB5

// The HF Pins are for Halfstep/Fullstep setting (1 = Fullstep
#define M1HF PB4
#define M2HF PB3
#define M3HF PB2

//The CTL Pins are for Chopper settings. Infos below.
// - Phase Chopping (1:
//	slow decrease of Current, good for current problems, only for Bipolar Motors.
// - Inhibit Chopiing (0:
//	fast decrease of Current, for Unipolar Motors too
#define M1CTL PC7
#define M2CTL PC6
#define M3CTL PC5

// The Reset Pins are not described further than "got to be high for the circuit to work"
#define M1RS PC4
#define M2RS PC3
#define M3RS PC2

// Prototype declarations
void motorInit( void );
void motorSetCtl( int en1, int en2, int en3, int hf1, int hf2, int hf3 );
void motorSetAdd( int ctl1, int ctl2, int ctl3, int rs1, int rs2, int rs3 );
void motorSetEnable(int en1, int en2, int en3);
void motorSetStep(int hf1, int hf2, int hf3);
void motorSetChopper( int ctl1, int ctl2, int ctl3 );
void motorSetReset( int rs1, int rs2, int rs3 );
inline void motorSetDirections(int cw1, int cw2, int cw3);
inline void motorStep( int clk1, int clk2, int clk3 );
inline void motorStepDir( int clk1, int clk2, int clk3, int cw1, int cw2, int cw3 );