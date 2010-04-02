#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define CLKPORT		PORTA
#define CLKDDR		DDRA
#define CLKPIN		PA3

#define DATAPORT	PORTA
#define DATADDR		DDRA
#define DATAPIN		PA0

#define STORAGEPORT	PORTA
#define STORAGEDDR	DDRA
#define STORAGEPIN	PA2

#define OEPORT		PORTA         // inv. Output Enable Pin
#define OEDDR		DDRA
#define OEPIN		PA1

#define MRPORT		PORTA         // inv. Master Reset
#define MRDDR		DDRA
#define MRPIN		PA4


void segmentDataClk( void );
void segmentStorageClk( void );
void segmentSetBit( unsigned char bit );
void segmentSet( char seg );
void segmentSetNumber(unsigned int number);
void initSegment( void );


