#include "StdOutUtils.h"

#include <Windows.h>

void setForeColor( TAnsiColor color )
{
	WORD wColor;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	
	if ( GetConsoleScreenBufferInfo( hStdOut, &csbi ) ) {
		//Mask out all but the background attribute, and add in the foreground color
		wColor = ( csbi.wAttributes & 0xF0 ) + ( (int)color & 0x0F );
		SetConsoleTextAttribute( hStdOut, wColor );
	}
}

void setBackColor( TAnsiColor color )
{
	WORD wColor;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	
	if ( GetConsoleScreenBufferInfo( hStdOut, &csbi ) ) {
		// Mask out all but the background attribute, and add in the foreground color
		wColor = ( csbi.wAttributes & 0x0F ) + ( ( (int) color & 0x0F ) << 4 );
		SetConsoleTextAttribute( hStdOut, wColor );
	}
	return;
}