#include "article.h"
#include <sstream>

#include <assert.h>
#include <stdlib.h>

#include "../error.h"
#include "../util.h"

#include "db_util.h"
#include "varying.h"


static const char  TypeCondionnement[] 			= { 'P', 		'C', 		'S',		'U' };
static const char* TypeCondionnementLibelle[] 	= { "PALETTE",	"CARTON",	"SACHET",	"UNITE" };
static const ArticleCond DefaultArticleCond[4] =
{
	{ "PALETTE", 0, 0, 0, 1, 0 },
	{ "CARTON",  0, 0, 0, 1, 0 },
	{ "SACHET",  0, 0, 0, 1, 0 },
	{ "UNITE",   0, 0, 0, 1, 0 },
};



void article_init( Article* a )
{
	memset( a, 0, sizeof(Article) );
	strcpy( a->LIBXART, "Nouvel article" );

	a->ARTXRES = -1;

	memcpy( a->conds, DefaultArticleCond, sizeof(ArticleCond) * 4 );
}


int db_select_article_par_deposant( DB* db, std::vector<Article_par_deposant*>* results, const char* _CODXDEP, const char* _LIKE_REFXART, const char* _LIKE_LIBXART )
{
	static const char* SQL = "SELECT V.ARTXRES, V.REFXART, V.LIBXART, V.LARXARC, V.HAUXARC, V.PROXARC, V.QTEXARC FROM V_ARTICLE_PAR_DEPOSANT V";

	typedef struct
	{
		int64_t ARTXRES;
		Varying<30> REFXART;
		Varying<50> LIBXART;
		double LARXARC;
		double HAUXARC;
		double PROXARC;
		double QTEXARC;
	} RS;


	std::stringstream ss;
	XSQLDA* output = NULL;
	XSQLDA* input = NULL;
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr = NULL;
	isc_stmt_handle st = NULL;
	RS rs;
	int i;
	char REFXART[31];
	char LIBXART[51];





	error_clear();

	// SQL
	ss << SQL;
	ss << " WHERE DEPXART = ?";
	i = 1;
	if( _LIKE_REFXART && _LIKE_LIBXART )
	{
		ss << " AND ( UPPER(REFXART) LIKE ? OR UPPER(LIBXART) LIKE ? )";
		i += 2;
	}
	else if( _LIKE_REFXART )
	{
		ss << " AND UPPER(REFXART) LIKE ? ";
		++i;
	}
	else if( _LIKE_LIBXART )
	{
		ss << " AND UPPER(LIBXART) LIKE ? ";
		i++;
	}

	ss << " ORDER BY LIBXART ASC";



	// Input
	input = (XSQLDA*) malloc(XSQLDA_LENGTH(i) );
	if( input == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	input->sqld = i;
	input->sqln = i;
	input->version = 1;


	SET_SQLDA_TEXT( input, 0, const_cast<char*>(_CODXDEP) );





	i = 1;
	if( _LIKE_REFXART )
	{
		likify( REFXART, _LIKE_REFXART, 31 );
		SET_SQLDA_TEXT( input, i, REFXART );
		++i;
	}
	if( _LIKE_LIBXART )
	{
		likify( LIBXART, _LIKE_LIBXART, 51 );
		SET_SQLDA_TEXT( input, i, LIBXART );
		++i;
	}



	// Output
	output = (XSQLDA*) malloc( XSQLDA_LENGTH(6) );
	if( output == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	output->sqld = 0;
	output->sqln = 7;
	output->version = 1;

	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_allocate_statement( status, &db->handle, &st );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare( status, &tr, &st, 0, ss.str().c_str() , _SQL_DIALECT_, output );
	if( r != 0 )
		goto LBL_DB_FAIL;

	SET_SQLDA_INT64(   output, 0, &rs.ARTXRES );
	SET_SQLDA_VARYING( output, 1, &rs.REFXART );
	SET_SQLDA_VARYING( output, 2, &rs.LIBXART );
	SET_SQLDA_DOUBLE(  output, 3, &rs.LARXARC );
	SET_SQLDA_DOUBLE(  output, 4, &rs.HAUXARC );
	SET_SQLDA_DOUBLE(  output, 5, &rs.PROXARC );
	SET_SQLDA_DOUBLE(  output, 6, &rs.QTEXARC );

	r = isc_dsql_execute( status, &tr, &st, 1, input );
	if( r != 0 )
		goto LBL_DB_FAIL;

	while( (r = isc_dsql_fetch( status, &st, 1, output)) == 0 )
	{
		Article_par_deposant* a = (Article_par_deposant*) malloc( sizeof(Article_par_deposant) );
		assert( rs.ARTXRES < 0xFFFFFFFF );
        a->ARTXRES = rs.ARTXRES;
		COPY_VARYING( a->REFXART, rs.REFXART );
		COPY_VARYING( a->LIBXART, rs.LIBXART );
		a->LARXARC = rs.LARXARC;
		a->HAUXARC = rs.HAUXARC;
		a->PROXARC = rs.PROXARC;
		a->QTEXARC = rs.QTEXARC;

		results->push_back( a );
	}

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( input );
	free( output );

	return results->size();

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	r = results->size();
	for( int i =0; i < r; ++i )
		free( (*results)[i] );
	results->clear();

	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	if( tr )
		isc_rollback_transaction( status, &tr );

	if( input )
		free( input );

	if( output )
		free( output );

	return -1;
}




int db_select_article( DB* db, Article* result, const char* _DEPXART, const char* _REFXART )
{
	static const char* SQL = "SELECT V.DEPXART, V.REFXART, V.LIBXART, V.DLVXART, V.LOTXART, V.NBJXART, V.ARTXRES,"
	" V.P_LIBXARC, V.P_LARXARC, V.P_HAUXARC, V.P_PROXARC, V.P_QTEXARC, V.P_POIXARC,"
	" V.C_LIBXARC, V.C_LARXARC, V.C_HAUXARC, V.C_PROXARC, V.C_QTEXARC, V.C_POIXARC,"
	" V.S_LIBXARC, V.S_LARXARC, V.S_HAUXARC, V.S_PROXARC, V.S_QTEXARC, V.S_POIXARC,"
	" V.U_LIBXARC, V.U_LARXARC, V.U_HAUXARC, V.U_PROXARC, V.U_QTEXARC, V.U_POIXARC"
	" FROM V_ARTICLE V  WHERE V.DEPXART = ? AND V.REFXART = ?";

	typedef struct {
		Varying<10> LIBXARC;
		double LARXARC;
		double HAUXARC;
		double PROXARC;
		double QTEXARC;
		double POIXARC;
	} RSCond;

	typedef struct
	{
		// rs
		Varying<5>  DEPXART;
		Varying<30> REFXART;
		Varying<50> LIBXART;
		char		DLVXART;
		char  		LOTXART;
		int64_t	    NBJXART;
		int64_t     ARTXRES;
		RSCond conds[4];
	} RS;

	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr = NULL;
	isc_stmt_handle st = NULL;
	int f;
	Article a;
	RS rs;
	XSQLDA* input;
	XSQLDA* output;

	error_clear();

	// input
	input = (XSQLDA*) malloc( XSQLDA_LENGTH(2) );
	if( input == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	input->sqln = 2;
	input->sqld = 2;
	input->version = 1;

	SET_SQLDA_TEXT( input, 0, const_cast<char*>(_DEPXART) );
	SET_SQLDA_TEXT( input, 1, const_cast<char*>(_REFXART) );

	// output
	output = (XSQLDA*) malloc( XSQLDA_LENGTH(7 + 4*6) );
	if( output == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	output->sqln = 7 + 4*6;
	output->sqld = 0;
	output->version = 1;

	r = isc_start_transaction(status, &tr, 1, &db->handle, 0, NULL);
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_allocate_statement(status, &db->handle, &st);
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare(status, &tr, &st, 0, SQL, _SQL_DIALECT_, output );
	if( r != 0 )
		goto LBL_DB_FAIL;


	SET_SQLDA_VARYING(output, 0, &rs.DEPXART );
	SET_SQLDA_VARYING(output, 1, &rs.REFXART );
	SET_SQLDA_VARYING(output, 2, &rs.LIBXART );
	SET_SQLDA_CHAR(   output, 3, &rs.DLVXART );
	SET_SQLDA_CHAR(   output, 4, &rs.LOTXART );
	SET_SQLDA_INT64(  output, 5, &rs.NBJXART);
	SET_SQLDA_INT64(  output, 6, &rs.ARTXRES);
	f = 7;
	for( int i = 0; i < 4; ++i )
	{
		SET_SQLDA_VARYING(output, f, &rs.conds[i].LIBXARC );
		f++;
		SET_SQLDA_DOUBLE( output, f, &rs.conds[i].LARXARC);
		f++;
		SET_SQLDA_DOUBLE( output, f, &rs.conds[i].HAUXARC);
		f++;
		SET_SQLDA_DOUBLE( output, f, &rs.conds[i].PROXARC);
		f++;
		SET_SQLDA_DOUBLE( output, f, &rs.conds[i].QTEXARC);
		f++;
		SET_SQLDA_DOUBLE( output, f, &rs.conds[i].POIXARC);
		f++;
	}

	r = isc_dsql_execute(status, &tr, &st, 1, input );
	if( r != 0 )
		goto LBL_DB_FAIL;


	r = isc_dsql_fetch(status, &st, 1, output );
	if( r == FETCH_EOF )
		goto LBL_NO_RESULT;

	COPY_VARYING( a.DEPXART, rs.DEPXART );
	COPY_VARYING( a.REFXART, rs.REFXART );
	COPY_VARYING( a.LIBXART, rs.LIBXART );
	assert( rs.NBJXART <= 0xFFFFFFFF );
	assert( rs.ARTXRES <= 0xFFFFFFFF );
	a.NBJXART = rs.NBJXART;
	a.ARTXRES = rs.ARTXRES;
	a.DLVXART = rs.DLVXART == 'Y' ? 1 : 0;
	a.LOTXART = rs.LOTXART == 'Y' ? 1 : 0;
	for( int i =0; i < 4; ++i )
	{
		COPY_VARYING( a.conds[i].LIBXARC, rs.conds[i].LIBXARC );
		a.conds[i].LARXARC = rs.conds[i].LARXARC;
		a.conds[i].HAUXARC = rs.conds[i].HAUXARC;
		a.conds[i].PROXARC = rs.conds[i].PROXARC;
		a.conds[i].QTEXARC = rs.conds[i].QTEXARC;
		a.conds[i].POIXARC = rs.conds[i].POIXARC;
	}


	r = isc_dsql_fetch(status, &st, 1, output );
	if( r != FETCH_EOF )
		goto LBL_DB_FAIL;

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( input );
	free( output );

	memcpy( result, &a, sizeof(Article) );

	return 1;

LBL_NO_RESULT:
	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_FAIL;
	r = isc_rollback_transaction( status, &tr );
	if( r != 0 )
		goto LBL_FAIL;
	free( input );
	free( output );
	return 0;
LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );
	if( tr )
		isc_rollback_transaction( status, &tr );
	if( input )
		free( input );
	if( output )
		free( output );
	return -1;
}


int db_delete_article( DB* db, const char* DEPXART, const char* REFXART )
{
	static const char* SQL = "DELETE FROM ARTXRES WHERE DEPXART = ? AND REFXART = ?";

	ISC_STATUS r;
	ISC_STATUS_ARRAY status;
	XSQLDA* sqlda = NULL;
	isc_tr_handle tr = NULL;

	error_clear();

	sqlda = (XSQLDA*) malloc( XSQLDA_LENGTH(2) );
	if( sqlda == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	sqlda->sqln = 2;
	sqlda->sqld = 2;
	sqlda->version = 1;

	SET_SQLDA_TEXT( sqlda, 0, const_cast<char*>(DEPXART) );
	SET_SQLDA_TEXT( sqlda, 1, const_cast<char*>(REFXART) );

	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL);
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = db_exec( db, &tr, SQL, 0, sqlda );
	if( r < 0 ) // -- 0 ou 1 : Un autre utilisateur peut l'avoir delete entre la construction de la grille et le click delete
		goto LBL_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( sqlda );

	return 1;
LBL_DB_FAIL:
	error_set( status );

LBL_FAIL:
	if( tr )
		isc_rollback_transaction( status, &tr );

	if( sqlda )
		free( sqlda );
	return -1;

}




int db_update_article( DB* db, const char* _DEPXART, const char* _REFXART, const Article* article  )
{
	static const char* SQL_ARTXRES = "UPDATE ARTXRES SET DEPXART = ?, REFXART = ?, LIBXART = ?, LOTXART = ?, DLVXART = ?, NBJXART = ? WHERE DEPXART = ? AND REFXART = ?";
	static const char* SQL_ARCXRES = "UPDATE ARCXRES SET LIBXARC = ?, QTEXARC = ?, LARXARC = ?, PROXARC = ?, HAUXARC = ?, POIXARC = ? WHERE ART_ARC = ? AND TYPXARC = ?";

	ISC_STATUS_ARRAY status;
	ISC_STATUS r;

	isc_tr_handle tr = NULL;
	XSQLDA* input[2] = { NULL, NULL };

	char LOTXART;
	char DLVXART;
	double NBJXART;
	int ARTXRES;



	error_clear();

	LOTXART = article->LOTXART ? 'Y' : 'N';
	DLVXART = article->DLVXART ? 'Y' : 'N';
	NBJXART = article->NBJXART;
	ARTXRES = article->ARTXRES;


	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL);
	if( r != 0 )
		goto LBL_FAIL;


	// ARTXREC
	input[0] = (XSQLDA*) malloc( XSQLDA_LENGTH(8) );
	if( input[0] == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	input[0]->sqld = 8;
	input[0]->sqln = 8;
	input[0]->version = 1;

	SET_SQLDA_TEXT( input[0], 0, const_cast<char*>(article->DEPXART) );
	SET_SQLDA_TEXT( input[0], 1, const_cast<char*>(article->REFXART) );
	SET_SQLDA_TEXT( input[0], 2, const_cast<char*>(article->LIBXART) );
	SET_SQLDA_CHAR( input[0], 3, &LOTXART );
	SET_SQLDA_CHAR( input[0], 4, &DLVXART );
	SET_SQLDA_DOUBLE(input[0], 5,&NBJXART );
	SET_SQLDA_TEXT( input[0], 6, const_cast<char*>(_DEPXART) );
	SET_SQLDA_TEXT( input[0], 7, const_cast<char*>(_REFXART) );

	r = db_exec( db, &tr, SQL_ARTXRES, 0, input[0] );
	if( r != 1 )
		goto LBL_FAIL;


	// ARCXRE
	input[1] = (XSQLDA*) malloc(XSQLDA_LENGTH(8) );
	if( input[1] == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	input[1]->sqld = 8;
	input[1]->sqln = 8;
	input[1]->version = 1;

	for( int i = 0; i < 4; ++i )
	{
		SET_SQLDA_TEXT(   input[1], 0, const_cast<char*>(   article->conds[i].LIBXARC) );
		SET_SQLDA_DOUBLE( input[1], 1, const_cast<double*>(&article->conds[i].QTEXARC) );
		SET_SQLDA_DOUBLE( input[1], 2, const_cast<double*>(&article->conds[i].LARXARC) );
		SET_SQLDA_DOUBLE( input[1], 3, const_cast<double*>(&article->conds[i].PROXARC) );
		SET_SQLDA_DOUBLE( input[1], 4, const_cast<double*>(&article->conds[i].HAUXARC) );
		SET_SQLDA_DOUBLE( input[1], 5, const_cast<double*>(&article->conds[i].POIXARC) );

		SET_SQLDA_INT64(  input[1], 6, &ARTXRES );
		SET_SQLDA_CHAR(   input[1], 7, const_cast<char*>(  &TypeCondionnement[i] ) );

		r = db_exec( db, &tr, SQL_ARCXRES, 0, input[1] );
		if( r != 1 )
			goto LBL_FAIL;
	}


	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_FAIL;

	free( input[0] );
	free( input[1] );

	return 1;

LBL_DB_FAIL:
	error_set( status );

LBL_FAIL:
	if( tr )
		isc_rollback_transaction( status, &tr );

	if( input[0] )
		free( input[0] );

	if( input[1] )
		free( input[1] );

	return -1;
}



int db_insert_article( DB* db, Article* article )
{
	static const char* SQL_ARTXRES   = "INSERT INTO ARTXRES ( DEPXART, REFXART, LIBXART, LOTXART, DLVXART, NBJXART ) VALUES ( ?, ?, ?, ?, ?, ? )";
	static const char* SQL_ARCXRES   = "INSERT INTO ARCXRES ( ART_ARC, TYPXARC, LIBXARC, QTEXARC, LARXARC, PROXARC, HAUXARC, POIXARC ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ? )";
	static const char* SQL_SELECT_ID = "SELECT A.ARTXRES FROM ARTXRES A WHERE A.DEPXART = ? AND A.REFXART = ?";





	ISC_STATUS_ARRAY status;
	ISC_STATUS r;

	isc_tr_handle tr = NULL;
	isc_tr_handle st = NULL;
	XSQLDA* input = NULL;
    XSQLDA* output = NULL;

	double NBJXART;
	char LOTXART;
	char DLVXART;
	int64_t ART_ARC;


	error_clear();


	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;


	// Insert ARTXRES
	input = (XSQLDA*) calloc( 1, XSQLDA_LENGTH(6) );
	if( input == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}

	input->sqld = 6;
	input->sqln = 6;
	input->version = 1;

	NBJXART = article->NBJXART;
	LOTXART = article->LOTXART ? 'Y' : 'N';
	DLVXART = article->DLVXART ? 'Y' : 'N';
	SET_SQLDA_TEXT(  input, 0, article->DEPXART );
	SET_SQLDA_TEXT(  input, 1, article->REFXART );
	SET_SQLDA_TEXT(  input, 2, article->LIBXART );
	SET_SQLDA_CHAR(  input, 3, &LOTXART );
	SET_SQLDA_CHAR(  input, 4, &DLVXART );
	SET_SQLDA_DOUBLE(input, 5, &NBJXART );
	r = db_exec( db, &tr, SQL_ARTXRES, 0, input );
	if( r != 1 )
		goto LBL_FAIL;



	// Select ARTXRES ID;
	input->sqld = 2;
	input->sqln = 2;

	output = (XSQLDA*) malloc( XSQLDA_LENGTH(1) );
	if( output == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	output->sqld = 0;
	output->sqln = 1;
	output->version = 1;
	SET_SQLDA_INT64( output, 0, &ART_ARC );

	ART_ARC = 0;

	r = db_select_one_fetch( db, &tr, SQL_SELECT_ID, input, output );
	if( r != 1 )
		goto LBL_FAIL;
	free( output );
	output = NULL;

	free( input );


	// ARCXRES
	input = (XSQLDA*) malloc( XSQLDA_LENGTH(8) );
	if( input == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	input->sqld = 8;
	input->sqln = 8;
	input->version = 1;

	SET_SQLDA_INT64( input, 0, &ART_ARC );


	for( int i =0; i < 4; ++i )
	{

		SET_SQLDA_CHAR(   input, 1,  const_cast<char*>(&TypeCondionnement[i]) );
		SET_SQLDA_TEXT(   input, 2,  article->conds[i].LIBXARC );
		SET_SQLDA_DOUBLE( input, 3, &article->conds[i].QTEXARC );
		SET_SQLDA_DOUBLE( input, 4, &article->conds[i].LARXARC );
		SET_SQLDA_DOUBLE( input, 5, &article->conds[i].PROXARC );
		SET_SQLDA_DOUBLE( input, 6, &article->conds[i].HAUXARC );
		SET_SQLDA_DOUBLE( input, 7, &article->conds[i].POIXARC );

		r = db_exec( db, &tr, SQL_ARCXRES, 0, input );
		if( r != 1 )
			goto LBL_DB_FAIL;
    }

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	return 1;

LBL_DB_FAIL:
	error_set( status );

LBL_FAIL:
	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	if( tr )
		isc_rollback_transaction( status, &tr );

	if( output )
		free( output );

	if(input )
		free( input );

	return -1;
}

