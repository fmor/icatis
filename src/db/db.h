#pragma once

#include <vector>
#include <string>

#define _INTPTR_T_DEFINED

extern "C" {
#include <stdint.h>
#include <ibase.h>
}


typedef struct
{
	isc_db_handle handle;
} DB;




#define SET_SQLDA_CHAR(      sqlda, index, var ) sqlda->sqlvar[index].sqldata =         var; sqlda->sqlvar[index].sqllen = 1;           		  sqlda->sqlvar[index].sqltype = SQL_TEXT
#define SET_SQLDA_TEXT(      sqlda, index, var ) sqlda->sqlvar[index].sqldata =         var; sqlda->sqlvar[index].sqllen = strlen(var);  		  sqlda->sqlvar[index].sqltype = SQL_TEXT
#define SET_SQLDA_VARYING(   sqlda, index, var ) sqlda->sqlvar[index].sqldata = (char*) var; sqlda->sqlvar[index].sqllen = (var)->length;  		  sqlda->sqlvar[index].sqltype = SQL_VARYING;
#define SET_SQLDA_INT64(     sqlda, index, var ) sqlda->sqlvar[index].sqldata = (char*) var; sqlda->sqlvar[index].sqllen = sizeof(std::int64_t);  sqlda->sqlvar[index].sqltype = SQL_INT64
#define SET_SQLDA_DOUBLE(    sqlda, index, var ) sqlda->sqlvar[index].sqldata = (char*) var; sqlda->sqlvar[index].sqllen = sizeof(double);		  sqlda->sqlvar[index].sqltype = SQL_DOUBLE
#define SET_SQLDA_TIMESTAMP( sqlda, index, var ) sqlda->sqlvar[index].sqldata = (char*) var; sqlda->sqlvar[index].sqllen = sizeof(ISC_TIMESTAMP); sqlda->sqlvar[index].sqltype = SQL_TIMESTAMP;
#define _SQL_DIALECT_   SQL_DIALECT_V6


#define FETCH_EOF 100L

typedef void(*callback)(void*);


int  db_open( DB** db, const char* dbfile, const char* username, const char* password );
void db_close( DB** db );
int  db_version( DB* db );
int  db_upgrade( DB* db );
int  db_exec( DB* db, isc_tr_handle* tr, const char* sql, int sqllen, XSQLDA* input );
int  db_select_one_fetch( DB* db, isc_tr_handle* tr, const char* sql, XSQLDA* input, XSQLDA* output );


