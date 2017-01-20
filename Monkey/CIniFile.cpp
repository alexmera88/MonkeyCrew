#include "CIniFile.h"

#include <fstream>
#include <boost/algorithm/string.hpp>

bool CIniFile::load( const std::string &vFilePath )
{
	clear();
	return include( vFilePath );
}

void CIniFile::splitNameValue( const std::string &vLine, std::string &rName, std::string &rValue ) const
{
	size_t lPos( vLine.find( "=" ) );

	if ( lPos != std::string::npos ) {
		rName = vLine.substr( 0, lPos );
		boost::trim_right( rName );

		rValue = vLine.substr( lPos + 1 );
		boost::trim_left( rValue );
	}
	else {
		rName = vLine;
		rValue = "";
	}
}

bool CIniFile::include( const std::string &vFilePath )
{
	std::ifstream lSource( vFilePath.c_str() );

	if ( !lSource.is_open() )
		return false;

	std::string lSectionName;
	TSection lSectionValues;

	// Assumes INI File format: https://en.wikipedia.org/wiki/INI_file

	// Look for sections...
	std::string lLine;
	while( std::getline( lSource, lLine ) ) {
		boost::trim( lLine );

		// Ignore empty lines
		if ( lLine.empty() )
			continue;

		// Ignore remarks
		if ( boost::starts_with( lLine, ";" ) || boost::starts_with( lLine, "#" ) )
			continue;

		// Section found
		if ( boost::starts_with( lLine, "[" ) && boost::ends_with( lLine, "]" ) ) {
			lSectionName = lLine;			
			continue;
		}

		if ( !lSectionName.empty() ) {
			std::string lName;
			std::string lValue;

			splitNameValue( lLine, lName, lValue );
			
			mSections[lSectionName][lName] = lValue;
		}
	}

	return true;
}

void CIniFile::clear()
{
	mSections.clear();
}

bool CIniFile::empty() const
{
	return mSections.empty();
}

int CIniFile::sectionCount() const
{
	return mSections.size();
}

void CIniFile::getSectionList( TSectionList &rSections ) const
{
	rSections.clear();

	for( TIniSections::const_iterator pSection = mSections.begin(); pSection != mSections.end(); pSection ++ ) {
		rSections.push_back( pSection->first );
	}
}

void CIniFile::getSection( const std::string &vSectionName, TSection &rNameValues ) const
{
	rNameValues.clear();

	TIniSections::const_iterator lPos = mSections.find( vSectionName );
	if ( lPos == mSections.end() )
		return;
	
	for( TSection::const_iterator pNameValue = lPos->second.begin(); pNameValue != lPos->second.end(); pNameValue ++ ) {
		rNameValues[ pNameValue->first ] = pNameValue->second;
	}
}
