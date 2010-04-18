#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "drehgeber.h"



void encode_init( void )
{
  int8_t new;
  int8_t new2;

  new = 0;
  new2 = 0;
  if( PHASE_A )
    new = 3;
  if( PHASE_A2 )
	new2 = 3;
  if( PHASE_B )
    new ^= 1;					// convert gray to binary
  if( PHASE_B2)
	new2 ^= 1;
  last = new;					// power on state
  last2 = new2;
  enc_delta = 0;
  enc_delta2 = 0;
  TCCR0 = 1<<WGM01^1<<CS01^1<<CS00;		// CTC, XTAL / 64
  //OCR0 = (uint8_t)(XTAL / 64.0 * 0.5e-3 - 0.5);	// 1ms
  OCR0 = (uint8_t)250;	// 1ms
  TIMSK |= 1<<OCIE0;
}


ISR( TIMER0_COMP_vect )				// 1ms for manual movement
{
  int8_t new, diff, new2, diff2;

  new = 0;
  new2 = 0;

  if( PHASE_A )
    new = 3;
  if( PHASE_A2 )
    new2 = 3;
  if( PHASE_B )
    new ^= 1;					// convert gray to binary
  if( PHASE_B2 )
    new2 ^= 1;
  diff = last - new;				// difference last - new
  diff2 = last2 - new2;
  if( diff & 1 ){				// bit 0 = value (1)
    last = new;					// store new as next last
    enc_delta += (diff & 2) - 1;		// bit 1 = direction (+/-)
  }
  if( diff2 & 1 ){				// bit 0 = value (1)
    last2 = new2;					// store new as next last
    enc_delta2 += (diff2 & 2) - 1;		// bit 1 = direction (+/-)
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
  val = enc_delta2;
  enc_delta2 = val & 1;
  sei();
  return val >> 1;
}

int8_t encode_read4( void )      // read four step encoders
{
  int8_t val;

  cli();
  val = enc_delta;
  enc_delta &= 3;
  sei();
  return val >> 2;
}
