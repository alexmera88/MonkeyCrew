#if !defined( _QSTRING_H_ )
#define _QSTRING_H_

#include <assert.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
size_t strLen( const char *vStr  );
size_t strLen( const wchar_t *vStr  );

char	upCase( const char C );
wchar_t upCase( const wchar_t C );

char	loCase( const char C );
wchar_t loCase( const wchar_t C );

bool isDigit( const char C );
bool isDigit( const wchar_t C );

//////////////////////////////////////////////////////////////////////////
template <typename CharT> 
bool strStartsWith( const CharT *vStr, const CharT *vValue )
{
	if ( vStr == vValue ) return true;
	if ( ! ( vStr && vValue ) ) return false;

	while( ( upCase( *vValue ) == upCase( *vStr ) ) && (*vValue) && (*vStr) ) {
		vValue++;
		vStr++;
	}

	return !( *vValue );
}

template <typename CharT> 
bool strStartsWith( const std::basic_string<CharT> &vStr, const std::basic_string<CharT> &vValue )
{
	return strStartsWith( vStr.c_str(), vValue.c_str() );
}

template <typename CharT> 
bool strStartsWith( const std::basic_string<CharT> &vStr, const CharT *vValue )
{
	return strStartsWith( vStr.c_str(), vValue );
}

template <typename CharT> 
bool strStartsWith( const CharT *vStr, const std::basic_string<CharT> &vValue )
{
	return strStartsWith( vStr, vValue.c_str() );
}

//////////////////////////////////////////////////////////////////////////
template <typename CharT> 
bool strEndsWith( const CharT *vStr, const CharT *vValue )
{
	if ( vStr == vValue ) return true;
	if ( ! (vStr && vValue) ) return false;

	const CharT *v = vValue + strlen( vValue ); 
	const CharT *s = vStr + strlen( vStr ); 

	while( ( upCase( *v ) == upCase( *s ) ) && (v >= vValue) && (s >= vStr) ) {
		v--;
		s--;
	}

	return ( v < vValue );
}

template <typename CharT> 
bool strEndsWith( const std::basic_string<CharT> &vStr, const std::basic_string<CharT> &vValue )
{
	return strEndsWith( vStr.c_str(), vValue.c_str() );
}

template <typename CharT> 
bool strEndsWith( const std::basic_string<CharT> &vStr, const CharT *vValue )
{
	return strEndsWith( vStr.c_str(), vValue );
}

template <typename CharT> 
bool strEndsWith( const CharT *vStr, const std::basic_string<CharT> &vValue )
{
	return strEndsWith( vStr, vValue.c_str() );
}

//////////////////////////////////////////////////////////////////////////
bool strEquals( const char *vStr, const char *vValue );
bool strEquals( const wchar_t *vStr, const wchar_t *vValue );

template <typename CharT> 
bool strEquals( const std::basic_string<CharT> &vStr, const std::basic_string<CharT> &vValue )
{
	return strEquals( vStr.c_str(), vValue.c_str() );
}

template <typename CharT> 
bool strEquals( const std::basic_string<CharT> &vStr, const CharT *vValue )
{
	return strEquals( vStr.c_str(), vValue );
}

template <typename CharT> 
bool strEquals( const CharT *vStr, const std::basic_string<CharT> &vValue )
{
	return strEquals( vStr, vValue.c_str() );
}

//////////////////////////////////////////////////////////////////////////
template <typename CharT> 
time_t strToTime( const std::basic_string<CharT> &vValue )
{
	if ( vValue.empty() ) return 0;

	time_t lRes( 0 );
	int n(0), p(0);

	for( const CharT *c = vValue.c_str(); *c; c++ ) {
		if ( isDigit( *c ) ) {
			n = (10 * n) + ( *c - '0' );
		}

		else {
			// Hours
			if ( ( *c == 'h' ) ||
				(( *c == ':' ) && ( p == 0 ))) {
					lRes += ( n * 60 * 60 );				
			}
			// Minutes
			else if ( ( *c == '\'' ) ||
				(( *c == ':' ) && ( p == 1 ))) {
					lRes += ( n * 60 );				
			}		

			else if ( ( *c == '"' ) ||
				(( *c == ':' ) && ( p == 2 ))) {
					lRes += ( n );				
			}

			n = 0;
			p++;			
		}
	}

	lRes += n;
	return lRes;	
}

//////////////////////////////////////////////////////////////////////////
template <typename CharT> 
bool isQuotedString( const CharT *vString, const CharT vQuote = '"'  )
{
	assert( vString != NULL );
	if ( vQuote != *vString ) return false;
	for( vString ++; *vString != 0x0; ++vString ) {
		if ( vQuote == *vString ) {
			++vString;
			if ( 0x0 == *vString ) return true;
			if ( vQuote != *vString  ) return false;			
		}
	}
	return false;
}

template <typename CharT> 
bool isQuotedString( const std::basic_string<CharT> &vString, const CharT vQuote = '"'  )
{
	return isQuotedString( vString.c_str(), vQuote );
}

template <typename CharT> 
CharT *strchrpos( const CharT *vStr, const CharT vChr )
{
	assert( vStr != NULL );
	
	for ( ; *vStr; ++vStr)	{ 	
		if ( vChr == *vStr ) return const_cast<CharT *>( vStr );
	}
	return NULL;
}

template <typename CharT> 
std::basic_string<CharT> quotedString( const CharT *vString, const CharT vQuote = '"' )
{
	assert( vString != NULL );

	std::basic_string<CharT> res;
	
	res += vQuote;
	CharT *next = strchrpos( vString, vQuote );

	while ( next )
	{
		res.append( vString, (size_t)( next - vString ) + 1 );
		res  += vQuote;
		vString = (next + 1);
		next = strchrpos( vString, vQuote );
	}
	
	res.append( vString );
	res += vQuote;

	return res;
}

template <typename CharT> 
std::basic_string<CharT> escapedString( const CharT *vString, const CharT vQuote = '"' )
{
	assert( vString != NULL );

	std::basic_string<CharT> res;

	res += vQuote;
	CharT *next = strchrpos( vString, vQuote );

	while ( next )
	{
		res.append( vString, (size_t)( next - vString ) );
		res  += '\\';
		vString = next;
		next = strchrpos( vString + 1, vQuote );
	}

	res.append( vString );
	res += vQuote;

	return res;
}

template <typename CharT> 
std::basic_string<CharT> quotedString( const std::basic_string<CharT> &vString, const CharT vQuote = '"' )
{
	return quotedString( vString.c_str(), vQuote );
}

template <typename CharT> 
std::basic_string<CharT> escapedString( const std::basic_string<CharT> &vString, const CharT vQuote = '"' )
{
	return escapedString( vString.c_str(), vQuote );
}

template <typename CharT> 
std::basic_string<CharT> dequotedString( const CharT *vString, const CharT vQuote = '"' )
{
	assert( vString != NULL );

	std::basic_string<CharT> res;
	
	if ( *vString != vQuote ) return vString;

	do
	{
		++vString;
		if ( *vString == vQuote ) {
			++vString;			
			if ( *vString != vQuote ) 
				break;	
		}
		res  += *vString;		
	} while ( *vString );
	
	return res;
}

template <typename CharT> 
std::basic_string<CharT> unescapedString( const CharT *vString, const CharT vQuote = '"' )
{
	assert( vString != NULL );
	assert( isQuotedString( vString, vQuote ) );

	std::basic_string<CharT> res;

	do
	{
		++vString;
		if ( *vString == '\\' ) {
			++vString;			
			if ( *vString != vQuote ) 
				break;	
		}
		res  += *vString;		
	} while ( *vString );

	return res;
}

template <typename CharT> 
std::basic_string<CharT> dequotedString( const std::basic_string<CharT> &vString, const CharT vQuote = '"' )
{
	return dequotedString( vString.c_str(), vQuote );
}

template <typename CharT> 
std::basic_string<CharT> unescapedString( const std::basic_string<CharT> &vString, const CharT vQuote = '"' )
{
	return unescapedString( vString.c_str(), vQuote );
}

#endif