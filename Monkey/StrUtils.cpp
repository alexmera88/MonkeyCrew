#include "StrUtils.h"

size_t strLen( const char *vStr  )
{
	return strlen( vStr );
}

size_t strLen( const wchar_t *vStr  )
{
	return wcslen( vStr );
}

char upCase( const char C )
{
	return toupper( C );
}

wchar_t upCase( const wchar_t C )
{
	return towupper( C );
}

char loCase( const char C )
{
	return tolower( C );
}

wchar_t loCase( const wchar_t C )
{
	return towlower( C );
}

bool isDigit( const char C )
{
	return isdigit( C );
}

bool isDigit( const wchar_t C )
{
	return iswdigit( C );
}

bool strEquals( const char *vStr, const char *vValue )
{
	return !_stricmp( vStr, vValue );
}

bool strEquals( const wchar_t *vStr, const wchar_t *vValue )
{
	return !_wcsicmp( vStr, vValue );
}
