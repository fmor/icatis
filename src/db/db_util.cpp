#include "db_util.h"

#include <stdlib.h>

#include "../util.h"
#include "../error.h"


void likify( char* dst, const char* src, int maxlen )
{
	int l = strlen( src );
	int j;

	dst[0] = '%';
	j = 1;
	for( int i = 0; i < l; ++i )
	{
		if( isalnum(src[i]) )
		{
			dst[j] = toupper( src[i] );
			++j;
		}
		else if( src[i] == '%' )
		{
			if( dst[j-1] != '%' )
			{
				dst[j] = '%';
				++j;
			}
		}
		else
		{
			dst[j] = src[i];
            ++j;
		}

		if( (j+2) == maxlen )
			break;
	}
	dst[j]   = '%';
	dst[j+1] = 0;
}
