#include "segment.h"

// WIZARD CODE Do not edit the lines below.
// Otherwise the wizard may be unable to reread the settings.
// WIZARD SEVEN_SEGMENT_WIZARD 23104567
char segments[] = { 0x3f, 0x5, 0x5e, 0x4f, 0x65, 0x6b, 0x7b,
                    0xd, 0x7f, 0x6f };
// END OF WIZARD CODE


void segmentDataClk( void )
{
	CLKPORT |= ( 1 << CLKPIN );
	_delay_us(1);
	CLKPORT &= ~(1 << CLKPIN );
	_delay_us(1);
}

void segmentStorageClk( void )
{
	STORAGEPORT |= ( 1 << STORAGEPIN );
	_delay_us(1);
	STORAGEPORT &= ~(1 << STORAGEPIN );
	_delay_us(1);
}
	
void segmentSetBit( unsigned char bit )
{
	bit = bit & 0x01; // nur das erste bit  im parameter beachten 
	
	if(bit==0)
	{
		DATAPORT &= ~(1 << DATAPIN);
	}
	else
	{
		DATAPORT |= (1 << DATAPIN);
	}
	segmentDataClk();
}

void segmentSet( char seg )
{
	int i;
	for (i = 0; i<8; i++)
	{
		segmentSetBit(seg);
		seg = seg/2;
	}
	segmentStorageClk();
}

void segmentSetNumber(unsigned int number)
{
	if (number < 10) segmentSet( segments[number] );
}
void initSegment( void )
{
	CLKDDR |= ( 1 << CLKPIN );
	DATADDR |= ( 1 << DATAPIN );
	STORAGEDDR |= ( 1 << STORAGEPIN );
	OEDDR |= ( 1 << OEPIN );
	MRDDR |= ( 1 << MRPIN );

	OEPORT &= ~( 1 << OEPIN ); // Output enable
	MRPORT |= ( 1 << MRPIN );  // MClear off
}
