#include "CActionQueue.h"

#include <iostream>
#include <sstream>
#include "StrUtils.h"

#define EXEC_OK 0
#define DO_ECHO( S, C ) { std::stringstream __s; __s << S; doEcho( __s.str(), C ); }
#define DO_LOG( C, P ) { std::stringstream __p; __p << P; logCommand( C, __p.str() ); }

const std::string TXT_LINE( "[==========] " );
const std::string TXT_ABOUT_TO_EXEC( "[Monkey say] " );
const std::string TXT_DO_EXEC( "[Monkey do ] ");
const std::string TXT_EXEC_RESULT_OK( "[        OK] ");
const std::string TXT_EXEC_RESULT_ERROR( "[     ERROR] ");

CActionQueue::CActionQueue()
	: mTerminate( false ), mTimer( NULL ), mVerbose( false ), mOutput( NULL ), mReplay( NULL )
{
	// Nothing more
}

CActionQueue::CActionQueue( CActionList &vActions )
	: mTerminate( false ), mTimer( NULL ), mVerbose( false ), mOutput( NULL ), mReplay( NULL )
{
	load( vActions );
}

CActionQueue::~CActionQueue()
{
	if ( mOutput ) {
		mOutput->close();
		delete mOutput;
	}

	if ( mReplay ) {
		mReplay->close();
		delete mReplay;
	}
}

bool CActionQueue::empty() const
{
	return mQueue.empty();
}

void CActionQueue::load( CActionList &vActions )
{
	for( CActionList::iterator pAction = vActions.begin(); pAction != vActions.end(); pAction ++ ) {
		schedule( &(*pAction) );
	}
}

void CActionQueue::start()
{
	std::string lLastPath;
	char lPath[MAX_PATH] = {0x0};
	__int64 lStep( 1 );

	/* initialize random seed: */
	srand (time(NULL));

	mTerminate = false;
	
	timerStart( mTimer );
	
	DO_ECHO( TXT_LINE << "\n", CL_GREEN );
	
	while( !mTerminate ) {
		time_t  lWait;
		CAction	*lAction;

		// Get next action
		if ( !pop( lAction, lWait ) ) 
			break;		

		// Enabled?
		if ( !lAction->enabled() ) 
			continue;

		DO_ECHO( TXT_ABOUT_TO_EXEC, CL_GREEN );
		DO_ECHO( "(" << lStep << ") " << lAction->execStatus() << " " << lAction->name() << " ", CL_LIGHT_GRAY );
		
		if ( lWait ) {
			clock_t   lWaitStart( clock( ) );

			// Wait until next execution
			if ( ! timerWait( mTimer, lWait ) ) 
				break;
		
			DO_LOG( "WAIT", clock( ) - lWaitStart  );
		}

		DO_ECHO( "\n", CL_LIGHT_GRAY );

		DO_LOG( "ECHO", "(" << lStep << ") " << lAction->execStatus() << " " << lAction->name() );

		lStep++;

		// Execution dir
		GetCurrentDirectory( MAX_PATH, lPath );
		if ( lLastPath.empty() ) {
			lLastPath = lPath;
		}

		if ( !strEquals( lPath, lLastPath ) ) {				
			DO_LOG( "CD", quotedString( lPath ) );
			lLastPath = lPath;
		}

		// Execute
		DO_LOG( lAction->execCommand(), "" );

		if ( mVerbose ) {
			DO_ECHO( TXT_DO_EXEC << "\n", CL_GREEN );
			DO_ECHO( lPath << ">" << lAction->execCommand() << "\n", CL_LIGHT_GRAY );
		}

		int lRes( EXEC_OK );

		if ( mVerbose ) {
			std::stringstream lCmdOut;			
			lRes = lAction->exec( &lCmdOut );
			DO_ECHO( lCmdOut.str(), CL_LIGHT_GRAY );
		}
		else {
			lRes = lAction->exec( NULL );
		}
		
		if ( lRes == EXEC_OK ) {
			DO_ECHO( TXT_EXEC_RESULT_OK, CL_GREEN );
		}
		else {
			DO_ECHO( TXT_EXEC_RESULT_ERROR, CL_RED );
		}
		
		DO_ECHO( "ERRORLEVEL = " << lRes << "\n", CL_LIGHT_GRAY );
		DO_LOG( "REM", "ERRORLEVEL = " << lRes << "\n" );
		
		// Schedule next execution
		schedule( lAction );					
	}	

	DO_ECHO( TXT_LINE << "\n", CL_GREEN );

	timerStop( mTimer );	
}

void CActionQueue::stop()
{
	if ( mTerminate )
		return;

	mTerminate = true;
	WaitForSingleObject( mTimer, 0 ) ;
}

void CActionQueue::cancelTimer()
{
	mCancelTimer = true;
}

void CActionQueue::schedule( CAction *vAction, time_t vWhen )
{
	if ( !vAction->enabled( ) )
		return;

	if ( vWhen == -1 ) {
		vWhen = vAction->nextExecTime();				
	}

	mQueue.insert( std::pair<time_t, CAction *>( vWhen, vAction ) );
}

bool CActionQueue::pop( CAction *&rAction, time_t &rWhen )
{
	if ( mQueue.empty() ) return false;

	TActionQueue::iterator lTop( mQueue.begin() );

	rWhen = max( lTop->first - time( NULL ), 0 );
	rAction = lTop->second;
	mQueue.erase( lTop );
	return true;
}

void CActionQueue::timerStart( HANDLE &vTimer )
{
	vTimer = CreateSemaphore( NULL, 0, 1, NULL );
}

#include <iomanip>
std::string formatSeconds( size_t vSeconds  )
{
	int lMinutes = vSeconds / 60;
	int lHours = lMinutes / 60;

	std::stringstream lRes;
	lRes << std::setfill('0') << lHours << ":" << std::setw(2) << ( lMinutes % 60 ) << ":" << std::setw(2) << ( vSeconds % 60 );
	return lRes.str();
}

std::string formatTime( time_t vTime )
{
	char lBuf[256] = {0x0};

	time_t rawtime( vTime);
	struct tm * timeinfo( localtime( &rawtime ) );

	strftime( lBuf, 256, "%x %X", timeinfo );

	return lBuf;
}

bool CActionQueue::timerWait( HANDLE &vTimer, size_t vSeconds )
{	
	mCancelTimer = false;

	std::string lTimeStr;

	for ( int i = vSeconds; i > 0 ; i-- ) {
		lTimeStr = "in " + formatSeconds( i );
		std::cout << lTimeStr;
		if ( WaitForSingleObject( vTimer, 1000 ) == WAIT_OBJECT_0 )
			return false;

		std::cout << std::string( lTimeStr.length(), '\b' ) + 
					std::string( lTimeStr.length(), ' ' ) + 
					std::string( lTimeStr.length(), '\b' );

		if ( mCancelTimer )
			break;
	}

	doEcho( "at " + formatTime( time( NULL ) ) );
	
	return true;
}

void CActionQueue::timerStop( HANDLE &vTimer )
{
	ReleaseSemaphore( vTimer, 1, NULL );
	CloseHandle( vTimer );
	vTimer = NULL;
}

void CActionQueue::logCommand( const std::string &vCommand, const std::string &vParams ) const
{
	if ( mReplay ) {
		(*mReplay) << vCommand;
		if ( !vParams.empty() ) {
			(*mReplay) << " " << vParams;
		}
		(*mReplay) << "\n";		
		(*mReplay).flush();
	}
}

void CActionQueue::doEcho( const std::string &vText, TAnsiColor vColor ) const
{
	if ( mOutput ) {
		(*mOutput) << vText;
		(*mOutput).sync();
	}

	setForeColor( vColor );
	std::cout << vText;	
}

bool CActionQueue::verbose() const
{
	return mVerbose;
}

void CActionQueue::verbose( bool vValue )
{
	mVerbose = vValue;
}

void CActionQueue::output( const std::string &vOutputFile )
{
	if ( mOutput ) {
		mOutput->close();
		delete mOutput;
	}

	mOutput = new std::fstream( );
	mOutput->open( vOutputFile.c_str(), std::ofstream::out, SH_DENYNO );	
}

void CActionQueue::replay( const std::string &vReplayFile )
{
	if ( mReplay ) {
		mReplay->close();
		delete mReplay;
	}

	mReplay = new std::fstream( );
	mReplay->open( vReplayFile.c_str(), std::ofstream::out, SH_DENYNO );
}