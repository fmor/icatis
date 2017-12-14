#pragma once

#include <stdint.h>

#define ERR_CUSTOM			1L
#define ERR_NO_MEMORY	  	2L
#define ERR_BAD_ARGUMENTS 	3L
#define ERR_DB_INFO_CORRUPT 4L
#define ERR_ISC_MSG_INFO    5L
#define ERR_ISC_STATUS		6L



const char* error_last();
void error_clear();

void error_set( long code, const char* msg );
void error_set( long code );
void error_set( const char* msg );
void error_set( intptr_t* iscstatus );


void show_last_error();

