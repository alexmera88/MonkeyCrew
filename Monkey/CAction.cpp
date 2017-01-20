#include "CAction.h"

//#include <boost/algorithm/string.hpp>
#include <cmath>
#include <sstream>
#include "StatVars.h"
#include "StrUtils.h"



CAction::TStatVar::TStatVar() 
	: Distrib( StatVars::TStatDist::SD_Exponential ), Min( 0 ), Avg( 0 ) 
{
	// Nothing
}
	
void CAction::TStatVar::loadValue( const std::string &vName, const std::string &vValue )
{
		 if ( strEndsWith( vName, ".Dist" ) )	Distrib = StatVars::strToStatDist( vValue );
	else if ( strEndsWith( vName, ".Min" ) )	Min = strToTime( vValue );
	else if ( strEndsWith( vName, ".Avg" ) )	Avg = strToTime( vValue );
}

CAction::CAction( )
	: mEnabled( false ), mPerformed( false )
{
	// Nothing
}

CAction::CAction( const CAction &vOther )
{
	mName = vOther.mName;
	mPerformCommand = vOther.mPerformCommand;
	mRevertCommand = vOther.mRevertCommand;
	mTBA = vOther.mTBA;
	mTTR = vOther.mTTR;
	mEnabled = vOther.mEnabled;
	mPerformed = vOther.mPerformed;
	mAlertOk = vOther.mAlertOk;
	mAlertError = vOther.mAlertError;
}

CAction::CAction( const std::string &vName, const CIniFile &vSource )
	: mName( vName ), mEnabled( false ), mPerformed( false )
{
	load( vSource );
}

void CAction::name( const std::string &vValue )
{
	mName = vValue;
}

void CAction::performCommand( const std::string &vValue )
{
	mPerformCommand = vValue;
}

void CAction::revertCommand( const std::string &vValue )
{
	mRevertCommand = vValue;
}

void CAction::enabled( bool vValue )
{
	mEnabled = vValue;
}

std::string CAction::name() const
{
	return mName;
}

std::string CAction::performCommand() const
{
	return mPerformCommand;
}

std::string CAction::revertCommand() const
{
	return mRevertCommand;
}

bool CAction::enabled() const
{
	return mEnabled;
}

bool CAction::isReversible() const
{
	return !mRevertCommand.empty();
}

void CAction::load( const CIniFile &vSource )
{
	CIniFile::TSection lValues;

	vSource.getSection( mName, lValues );
	
	for ( CIniFile::TSection::const_iterator pNameValue = lValues.begin(); pNameValue != lValues.end(); pNameValue ++ ) {
		std::string lName( pNameValue->first );
		std::string lValue( pNameValue->second );

			 if ( strEquals( lName, "Perform" ))	mPerformCommand = lValue;
		else if ( strEquals( lName, "Revert" ))		mRevertCommand = lValue;
		else if ( strEquals( lName, "Enabled" ))	mEnabled = strEquals( lValue, "true" );
		
		else if ( strEquals( lName, "Alert.Ok" ))		mAlertOk = lValue;
		else if ( strEquals( lName, "Alert.Error" ))	mAlertError = lValue;

		else if ( strStartsWith( lName, "TBA." ))	mTBA.loadValue( lName, lValue );
		else if ( strStartsWith( lName, "TTR." ))	mTTR.loadValue( lName, lValue );
	}
}

int CAction::exec( std::ostream *vOutput )
{
	int lRes( 0 );

	if ( !isReversible() ) {
		lRes = exec( mPerformCommand, vOutput );
	}
	else {
		if ( !mPerformed )
			lRes = exec( mPerformCommand, vOutput );
		else
			lRes = exec( mRevertCommand, vOutput );

		mPerformed = !mPerformed;
	}

	alert( lRes );
	
	return lRes;
}

std::string CAction::execCommand() const
{
	if ( !isReversible() )
		return mPerformCommand;	
	else if ( !mPerformed )
		return mPerformCommand;
	else
		return mRevertCommand;
}

std::string CAction::execStatus() const
{
	if ( !isReversible() || !mPerformed )
		return "Play";
	else
		return "Revert";
}

time_t CAction::nextExecTime() const
{
	#define round( x ) ( x < 0.0 ? ceil( x - 0.5 ) : floor( x + 0.5 ) );

	time_t lRes( time( NULL ) );

	if ( !isReversible() || !mPerformed )	{
		lRes += mTBA.Min;
		lRes += round( StatVars::getSample( mTBA.Distrib, ( mTBA.Avg - mTBA.Min ) ) );
	}
	else {
		lRes += mTTR.Min;
		lRes += round( StatVars::getSample( mTTR.Distrib, ( mTTR.Avg - mTTR.Min ) ) );
	}
	
	return lRes;
}

int CAction::exec( const std::string &vCommand, std::ostream *vOutput )
{
	std::string lCmd( vCommand );

	// Do we want STDERR?
	lCmd.append( " 2>&1" ); 

	if (FILE * stream = _popen( lCmd.c_str(), "r" ) ) {
		while ( !feof( stream ) ) {
			char buffer[256] = {0x0};
			if ( fgets( buffer, 256, stream ) ) {
				if ( vOutput ) {
					(*vOutput) << buffer;
				}
			}			
		}
		return ( _pclose( stream ) / 256 );
	}

	return errno;
}

void CAction::alert( int vResult )
{
	std::stringstream lAlertCmd;

	if ( !vResult && !mAlertOk.empty() ) {
		lAlertCmd << "MSG " << mAlertOk << " \"[Monkey says] " << mName << "!\"";
	}

	else if ( vResult && !mAlertError.empty() ) {
		lAlertCmd << "MSG " << mAlertError << " \"[Monkey says] " << mName << " FAILED!\"";
	}

	else {
		return;
	}

	exec( lAlertCmd.str(), NULL );
}