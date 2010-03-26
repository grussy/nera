#include "commands.h"

// WIZARD CODE Do not edit the lines below.
// Otherwise the wizard may be unable to reread the settings.
// WIZARD SEVEN_SEGMENT_WIZARD 23104567
char segmentsc[] = { 0x3f, 0x5, 0x5e, 0x4f, 0x65, 0x6b, 0x7b,
                    0xd, 0x7f, 0x6f };
// END OF WIZARD CODE
void identify_command()
{
	char command[17];      // String mit maximal 12 zeichen 
  	if (uart_gets( command, sizeof( command ) )) return;
		
	if( strcmp( command, "TG" ) == 0 )
	{
		uart_puts("TG AVR Testboard v0.0.1\n");
		//uart_puts(command);
	}
	else if	( strcmp( command, "LED1" ) == 0 )
	{
		DDRD |= ( 1 << PD6 );
		PORTD = PIND ^ ( 1 << PD6 );
		uart_puts(command);
	}
	else if	( strcmp( command, "FET" ) == 0 )
	{
		DDRA |= ( 1 << PA7 );
		PORTA = PINA ^ ( 1 << PA7 );
		uart_puts(command);
	}
	else if	( strcmp( command, "LCDW1" ) == 0 )
	{
		uart_puts("Type...\n");
		char lcdtext[17];      // String mit maximal 16 zeichen 
  		while (uart_gets( lcdtext, sizeof( lcdtext )));
		set_cursor(0,1);
		lcd_string(lcdtext);
	}
	else if	( strcmp( command, "LCDW2" ) == 0 )
	{
		uart_puts("Type...\n");
		char lcdtext[17];      // String mit maximal 16 zeichen 
  		while (uart_gets( lcdtext, sizeof( lcdtext )));
		set_cursor(0,2);
		lcd_string(lcdtext);
	}
		else if	( strcmp( command, "CLK" ) == 0 )
	{
		segmentDataClk();
		uart_puts(command);
	}
		else if	( strcmp( command, "STR" ) == 0 )
	{
		segmentStorageClk();
		uart_puts(command);
	}
		else if	( strcmp( command, "7" ) == 0 )
	{
		uart_puts("Type a number < 10...\n");
		char ledtext[2];      // String mit maximal 1 zeichen 
  		while (uart_gets( ledtext, sizeof( ledtext )));
		unsigned int number = atoi(ledtext);
		segmentSetNumber(number);
	}
	else if (strcmp( command, "HELP" ) == 0 )
	{
		uart_puts("Available commands:\n TG - print Device and Version Info\n LEDx - Turn on/off LED number x\n			LCDWx - Prompt for Text to write on Line x on LCD\n\n HELP - Print this Info\n");
	}
}
