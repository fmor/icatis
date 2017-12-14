#pragma once

#include "db.h"


typedef struct
{
	char CODXDEP[5+1];
	char LIBXDEP[30+1];
	char CODXMAG[5+1];
	char LIBXMAG[30+1];
	char CODXZON[10+1];
	char LIBXZON[30+1];
} StockTree;


typedef struct
{
	char REFXART[30+1];
	char LIBXART[50+1];
	char CODXADR[15+1];
	char LIBXZON[30+1];
	double QTEXDAR;
	double NB_C;
	char SCCXDAR[18+1];
	char LOTXDAR[20+1];
	ISC_TIMESTAMP DLVXDAR;
	ISC_TIMESTAMP DLPXDAR;
	double NB_S;

	char DLVXART;
	char LOTXART;

} StockLine;


int db_select_stocktree( DB* db, std::vector<StockTree*>* result );
int db_select_stockline( DB* db, std::vector<StockLine*>* result, const char* CODXDEP, const char* CODXMAG, const char* CODXZON, const char* LIKE_CODXREF, const char* LIKE_CODXADR, const char* LIKE_LIBXART );
