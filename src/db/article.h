#pragma once

#include <vector>

#include "db.h"

#define IDX_COND_PALETTE 				0
#define IDX_COND_CONDITIONNEMENT 		1
#define IDX_COND_SOUS_CONDITIONNEMENT 	2
#define IDX_COND_UNITE 					3


typedef struct
{
	int32_t ARTXRES;
	char   REFXART[30+1];
	char   LIBXART[50+1];
	double LARXARC;
	double HAUXARC;
	double PROXARC;
	double QTEXARC;

} Article_par_deposant;

typedef struct
{
    char   LIBXARC[10+1];
	double LARXARC;
	double HAUXARC;
	double PROXARC;
	double QTEXARC;
	double POIXARC;
} ArticleCond;

typedef struct
{
	char DEPXART[5+1];
	char REFXART[30+1];
	char LIBXART[50+1];
	char DLVXART;
	char LOTXART;
	int32_t NBJXART;
	int32_t ARTXRES;
	ArticleCond  conds[4];
} Article;


void article_init( Article* a );

int db_select_article_par_deposant( DB* db, std::vector<Article_par_deposant*>* results, const char* _CODXDEP, const char* _REFXART, const char* _LIBXART );
int db_select_article( DB* db, Article* result, const char* DEPXART, const char* REFXART );

int db_insert_article( DB* db, Article* article );
int db_update_article( DB* db, const char* _DEPXART, const char* _REFXART, const Article* article  );


int db_delete_article( DB* db, const char* DEPXART, const char* REFXART );
