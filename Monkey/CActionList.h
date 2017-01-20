#ifndef CActionList_h__
#define CActionList_h__

#include <string>
#include <deque>
#include "CAction.h"

class CActionList : public std::deque< CAction > {
public:
	CActionList( );
	CActionList( const CIniFile &vSource );

	void load( std::istream &vSource );	
	void load( const CIniFile &vSource );	
};

#endif // CActionList_h__
