#include "CActionList.h"

#include <string>
#include <boost/algorithm/string.hpp>

CActionList::CActionList( )
{
	// Nothing
}

CActionList::CActionList( const CIniFile &vSource )
{
	load( vSource );
}

void CActionList::load( const CIniFile &vSource )
{
	CIniFile::TSectionList lSections;

	vSource.getSectionList( lSections );

	for( int i = 0; i < lSections.size(); i++ ) {
		push_back( CAction( lSections[i], vSource ) );
	}
}