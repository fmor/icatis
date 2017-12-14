#pragma once

#include <vector>

#include "db.h"

typedef struct
{
	char CODXDEP[5+1];
	char LIBXDEP[30+1];
} Deposant;


int db_select_deposants( DB* db, std::vector<Deposant*>* results );

