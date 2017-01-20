// Wait.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

const std::string TXT_COMMAND_HELP( "Usage: Wait <Miliseconds>\n" );

#include <iomanip>
std::string formatSeconds( size_t vSeconds  )
{
	int lMinutes = vSeconds / 60;
	int lHours = lMinutes / 60;

	std::stringstream lRes;
	lRes << std::setfill('0') << lHours << ":" << std::setw(2) << ( lMinutes % 60 ) << ":" << std::setw(2) << ( vSeconds % 60 );
	return lRes.str();
}

int main(int argc, char* argv[])
{
	if ( argc < 2 ) {
		std::cout << TXT_COMMAND_HELP;
		return 1;
	}

	size_t lMilis( atoi( argv[1] ) );

	Sleep( lMilis % 1000 );

	std::string lTimeStr;

	for ( int i = ( lMilis / 1000 ); i > 0 ; i-- ) {
		lTimeStr = formatSeconds( i );
		std::cout << lTimeStr;
		Sleep( 1000 );
		std::cout << std::string( lTimeStr.length(), '\b' ) + 
			std::string( lTimeStr.length(), ' ' ) + 
			std::string( lTimeStr.length(), '\b' );
	}

	return 0;
}

