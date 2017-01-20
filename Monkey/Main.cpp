// Monkey.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <conio.h>
#include <string>
#include <iosfwd>
#include <fstream>
#include <iostream>

#include "CIniFile.h"
#include "CActionList.h"
#include "CActionQueue.h"

const std::string TXT_COMMAND_HELP( "Usage: Monkey <Actions File Path> [-v] [-o <Output File Path>]\n\n"
								   "\t<Actions File Path> : tPath to a file describing monkey actions.\n"
									"\t-v : Verbose action execution.\n"
									"\t-o : Copy Output to file at <Output File Path>.\n"
									"\t-r : Generate Replay command file at <Replay File Path>.\n"
									);

const std::string TXT_CANNT_OPEN_FILE( "> Monkey can't play " );
const std::string TXT_FILE_OPENED( "> Monkey plays " );
const std::string TXT_PRESS_ESC_TO_END( "> Press <N> to play Now, <Esc> to let the monkey Escape.\n\n" );

const DWORD RESULT_OK( 0 );
const DWORD RESULT_ERROR_SYSTEM( 100 );
const DWORD RESULT_ERROR_UNEXPECTED( 101 );
const DWORD RESULT_ERROR_UNKNOWN( 102 );

DWORD WINAPI runActionQueue( LPVOID vActionQueue )
{
	try {
		CActionQueue *lQueue( (CActionQueue *) vActionQueue );
		lQueue->start();
		return RESULT_OK;
	}
	catch( const std::exception &e) {
		std::cerr << "ERROR " << RESULT_ERROR_UNEXPECTED << ": " << e.what();
		return RESULT_ERROR_UNEXPECTED;
	}
	catch(...) {
		std::cerr << "ERROR " << RESULT_ERROR_UNKNOWN << ": Unknown error!";
		return RESULT_ERROR_UNKNOWN;
	}
}

std::string GetSysErrorMessage( DWORD vErrorCode ) 
{ 
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, vErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

	std::string lRes( (LPCTSTR) lpMsgBuf );		
	LocalFree(lpMsgBuf);
	
	return lRes;
}

WORD getConsoleInput()
{
	static HANDLE lConsole( GetStdHandle(STD_INPUT_HANDLE) );
	INPUT_RECORD lEventBuffer;
	DWORD lEventCount(0);

	if ( !PeekConsoleInput( lConsole, &lEventBuffer, 1, &lEventCount ) )
		return 0;	
		
	if ( !lEventCount )
		return 0;

	if ( !ReadConsoleInput( lConsole, &lEventBuffer, 1, &lEventCount ) )
		return 0;

	if ( !lEventBuffer.Event.KeyEvent.bKeyDown ) {	
		return lEventBuffer.Event.KeyEvent.wVirtualKeyCode;	
	}
	return 0;
}

std::string getFileName( const std::string &vFilePath )
{
	std::string lRes( vFilePath );

	size_t lStart = lRes.find_last_of( "/\\" );
	if ( lStart != std::string::npos ) {
		lRes.erase( 0, lStart + 1 );
	}

	size_t lFinish = lRes.find_last_of( "." );
	if ( lFinish != std::string::npos ) {
		lRes.erase( lFinish );
	}

	return lRes;
}

int main(int argc, char* argv[])
{
	if ( argc < 2 ) {
		std::cout << TXT_COMMAND_HELP;
		return 1;
	}

	CActionQueue lActionQueue;
	CIniFile lActions;
	
	for( int i = 1; i < argc; i++ ) {
		if ( *argv[i] == '-' ) {
			if ( !strcmp( "-v", argv[i] ) ) {
				lActionQueue.verbose( true );
			}
			
			else if ( !strcmp( "-o", argv[i] ) ) {
				i++;
				if ( i < argc ) {
					lActionQueue.output( argv[i] );
				}
			}

			else if ( !strcmp( "-r", argv[i] ) ) {
				i++;
				if ( i < argc ) {
					lActionQueue.replay( argv[i] );
				}
			}
		}

		else if ( lActions.include( argv[i] ) ) {
			setForeColor( CL_WHITE );
			std::cout << TXT_FILE_OPENED << getFileName( argv[i] ) << "!\n";
		} 
		else {
			setForeColor( CL_RED );
			std::cerr << TXT_CANNT_OPEN_FILE << getFileName( argv[i] ) << "\n";
		}
	}

	if ( lActions.empty() ) {		
		return 2;
	}

	CActionList	 lAvailableActions( lActions );
	lActionQueue.load( lAvailableActions );	
	
	setForeColor( CL_YELLOW );
	std::cout << TXT_PRESS_ESC_TO_END;
	setForeColor( CL_LIGHT_GRAY );

	HANDLE lProcessingThread = CreateThread( NULL, 0, runActionQueue, &lActionQueue, 0, NULL );

	if ( !lProcessingThread ) {
		setForeColor( CL_RED );
		std::cerr << "ERROR " << RESULT_ERROR_SYSTEM << ": " << GetSysErrorMessage( GetLastError() );
		return RESULT_ERROR_SYSTEM ;
	}

	while( WaitForSingleObject( lProcessingThread, 250 ) ) {
		WORD lCmd( getConsoleInput() );

		if ( lCmd == VK_ESCAPE ) {
			lActionQueue.stop(); 
			break;
		}
		else if ( toupper( lCmd ) == 'N' ) {
			lActionQueue.cancelTimer(); 
		}
	}

	return 0;
}