#include "stock.h"


#include <sstream>

#include <assert.h>
#include <stdlib.h>

#include "../error.h"
#include "../util.h"

#include "db_util.h"
#include "varying.h"


int db_select_stocktree( DB* db, std::vector<StockTree*>* result )
{
	static const char* SQL = "SELECT V.CODXDEP, V.LIBXDEP, V.CODXMAG, V.LIBXMAG, V.CODXZON, V.LIBXZON FROM V_STOCK_TREE V";

	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_stmt_handle st = NULL;
	isc_tr_handle tr = NULL;
	XSQLDA* sqlda;

	Varying<5>  CODXDEP;
	Varying<30> LIBXDEP;
	Varying<5>  CODXMAG;
	Varying<30> LIBXMAG;
	Varying<10> CODXZON;
	Varying<30> LIBXZON;

	error_clear();

	sqlda = (XSQLDA*) malloc( XSQLDA_LENGTH(6) );
	if( sqlda == NULL )
	{
		error_set( ERR_NO_MEMORY );
		return -1;
	}
	sqlda->sqln = 6;
	sqlda->sqld = 0;
	sqlda->version = 1;


	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_allocate_statement( status, &db->handle, &st );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare( status, &tr, &st, 0, SQL, _SQL_DIALECT_, sqlda );
	if( r != 0 )
		goto LBL_DB_FAIL;


	SET_SQLDA_VARYING( sqlda, 0, &CODXDEP );
	SET_SQLDA_VARYING( sqlda, 1, &LIBXDEP );
	SET_SQLDA_VARYING( sqlda, 2, &CODXMAG );
	SET_SQLDA_VARYING( sqlda, 3, &LIBXMAG );
	SET_SQLDA_VARYING( sqlda, 4, &CODXZON );
	SET_SQLDA_VARYING( sqlda, 5, &LIBXZON );

	r = isc_dsql_execute( status, &tr, &st, 1, NULL );
	if( r != 0 )
		goto LBL_FAIL;

	while( (r=isc_dsql_fetch( status, &st, 1, sqlda)) == 0 )
	{
		StockTree* sto = (StockTree*) malloc( sizeof(StockTree) );
		if( sto == NULL )
		{
			error_set( ERR_NO_MEMORY );
			goto LBL_FAIL;
		}

		memcpy( sto->CODXDEP, CODXDEP.str, CODXDEP.length ); sto->CODXDEP[CODXDEP.length] = 0;
		memcpy( sto->LIBXDEP, LIBXDEP.str, LIBXDEP.length ); sto->LIBXDEP[LIBXDEP.length] = 0;
		memcpy( sto->CODXMAG, CODXMAG.str, CODXMAG.length ); sto->CODXMAG[CODXMAG.length] = 0;
		memcpy( sto->LIBXMAG, LIBXMAG.str, LIBXMAG.length ); sto->LIBXMAG[LIBXMAG.length] = 0;
		memcpy( sto->CODXZON, CODXZON.str, CODXZON.length ); sto->CODXZON[CODXZON.length] = 0;
		memcpy( sto->LIBXZON, LIBXZON.str, LIBXZON.length ); sto->LIBXZON[LIBXZON.length] = 0;

		result->push_back( sto );
	}

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	return result->size();

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	r = result->size();
	for( int i=0; i < r; ++i )
		free( (*result)[i] );

	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	if( tr )
		isc_rollback_transaction( status, &tr );

	free( sqlda );
	return -1;

}






int db_select_stockline( DB* db, std::vector<StockLine*>* result, const char* _CODXDEP, const char* _CODXMAG, const char* _CODXZON, const char* _LIKE_CODXREF, const char* _LIKE_CODXADR, const char* _LIKE_LIBXART )
{
	typedef struct
	{
		Varying<30> REFXART;
		Varying<50> LIBXART;
		Varying<15> CODXADR;
		Varying<30> LIBXZON;
		double QTEXDAR;
		double NB_C;
		Varying<18> SCCXDAR;
		Varying<20> LOTXDAR;
		ISC_TIMESTAMP DLVXDAR;
		ISC_TIMESTAMP DLPXDAR;
		double NB_S;
		char DLVXART;
		char LOTXART;
	} RS;

	static const char* SQL = "SELECT V.REFXART, V.LIBXART, V.CODXADR, V.LIBXZON, V.QTEXDAR, V.NB_C, V.SCCXDAR, V.LOTXDAR, V.DLVXDAR, V.DLPXDAR, V.NB_S, V.DLVXART, V.LOTXART  FROM V_STOCK V";

	std::stringstream ss;


	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr = NULL;
	isc_stmt_handle st = NULL;
	XSQLDA* sqlda_output = NULL;
	XSQLDA* sqlda_input = NULL;
	int i;
	RS rs;

	char REFXART[31];
	char CODXADR[16];
	char LIBXART[51];

	error_clear();
	result->clear();

	ss << SQL << " WHERE V.CODXDEP = ? AND V.CODXMAG = ?" ;
	i = 2;
	if( _CODXZON )
	{
		ss << " AND V.CODXZON = ?";
		++i;
	}

	if( _LIKE_CODXREF || _LIKE_CODXADR || _LIKE_LIBXART )
	{
		ss << " AND ( ";
		if( _LIKE_CODXREF )
		{
			ss <<  " UPPER(V.REFXART) LIKE ?";
			++i;
		}
		if( _LIKE_CODXADR )
		{
			if( _LIKE_CODXREF )
				ss << " OR ";
			ss << "  UPPER(V.CODXADR) LIKE ?";
			++i;
		}
		if( _LIKE_LIBXART )
		{
			if( _LIKE_CODXREF || _LIKE_CODXADR )
				ss << " OR ";
			ss << "  UPPER(V.LIBXART) LIKE ?";
			++i;
		}
		ss << " ) ";
	}

	ss << "ORDER BY V.LIBXART ASC";

//	show_info( ss.str().c_str() );

	// Input
	sqlda_input = (XSQLDA*) malloc( XSQLDA_LENGTH(i) );
	if( sqlda_input == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	sqlda_input->sqld = i;
	sqlda_input->sqln = i;
	sqlda_input->version = 1;


	SET_SQLDA_TEXT( sqlda_input, 0, const_cast<char*>(_CODXDEP) );
	SET_SQLDA_TEXT( sqlda_input, 1, const_cast<char*>(_CODXMAG) );
	i = 2;
	if( _CODXZON )
	{
		SET_SQLDA_TEXT( sqlda_input, i, const_cast<char*>(_CODXZON) );
		++i;
	}

	if( _LIKE_CODXREF )
	{
		likify( REFXART, _LIKE_CODXREF, 31 );
		SET_SQLDA_TEXT( sqlda_input, i, REFXART );
		++i;
	}
	if( _LIKE_CODXADR )
	{
		likify( CODXADR, _LIKE_CODXADR, 16 );
		SET_SQLDA_TEXT( sqlda_input, i, CODXADR );
		++i;
	}
	if( _LIKE_LIBXART )
	{
		likify( LIBXART, _LIKE_LIBXART, 51 );
		SET_SQLDA_TEXT( sqlda_input, i, LIBXART );
		++i;
	}

	// output
	sqlda_output = (XSQLDA*) malloc( XSQLDA_LENGTH(10) );
	if( sqlda_output == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	sqlda_output->sqld = 0;
	sqlda_output->sqln = 13;
	sqlda_output->version = 1;

	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_allocate_statement( status, &db->handle, &st );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare( status, &tr, &st, 0, ss.str().c_str(), _SQL_DIALECT_, sqlda_output );
	if( r != 0 )
		goto LBL_DB_FAIL;

	SET_SQLDA_VARYING(   sqlda_output, 0, &rs.REFXART );
	SET_SQLDA_VARYING(   sqlda_output, 1, &rs.LIBXART );
	SET_SQLDA_VARYING(   sqlda_output, 2, &rs.CODXADR );
	SET_SQLDA_VARYING(   sqlda_output, 3, &rs.LIBXZON );
	SET_SQLDA_DOUBLE(    sqlda_output, 4, &rs.QTEXDAR );
	SET_SQLDA_DOUBLE(    sqlda_output, 5, &rs.NB_C    );
	SET_SQLDA_VARYING(   sqlda_output, 6, &rs.SCCXDAR );
	SET_SQLDA_VARYING(   sqlda_output, 7, &rs.LOTXDAR );
	SET_SQLDA_TIMESTAMP( sqlda_output, 8, &rs.DLVXDAR );
	SET_SQLDA_TIMESTAMP( sqlda_output, 9, &rs.DLPXDAR );
	SET_SQLDA_DOUBLE(    sqlda_output, 10,&rs.NB_S );
	SET_SQLDA_CHAR(		 sqlda_output, 11,&rs.LOTXART );
	SET_SQLDA_CHAR(		 sqlda_output, 12,&rs.DLVXART );


	r = isc_dsql_execute( status, &tr, &st, 1, sqlda_input );
	if( r != 0 )
		goto LBL_DB_FAIL;


	while( (r=isc_dsql_fetch( status, &st, 1, sqlda_output)) == 0 )
	{
		StockLine* l = (StockLine*) malloc( sizeof(StockLine) );
		if( l == NULL )
		{
			error_set( ERR_NO_MEMORY );
			goto LBL_FAIL;
		}

		memcpy( l->REFXART, rs.REFXART.str, rs.REFXART.length ); l->REFXART[rs.REFXART.length] = 0;
		memcpy( l->LIBXART, rs.LIBXART.str, rs.LIBXART.length ); l->LIBXART[rs.LIBXART.length] = 0;
		memcpy( l->CODXADR, rs.CODXADR.str, rs.CODXADR.length ); l->CODXADR[rs.CODXADR.length] = 0;
		memcpy( l->LIBXZON, rs.LIBXZON.str, rs.LIBXZON.length ); l->LIBXZON[rs.LIBXZON.length] = 0;
		l->QTEXDAR = rs.QTEXDAR;
		l->NB_C = rs.NB_C;
		memcpy( l->SCCXDAR, rs.SCCXDAR.str, rs.SCCXDAR.length ); l->SCCXDAR[rs.SCCXDAR.length] = 0;
		memcpy( l->LOTXDAR, rs.LOTXDAR.str, rs.LOTXDAR.length ); l->LOTXDAR[rs.LOTXDAR.length] = 0;
		l->DLVXDAR = rs.DLVXDAR;
		l->DLPXDAR = rs.DLPXDAR;
		l->NB_S = rs.NB_S;
		l->LOTXART = rs.LOTXART == 'Y' ? 1 : 0;
		l->DLVXART = rs.DLVXART == 'Y' ? 1 : 0;

		result->push_back( l );
	}
	if( r != FETCH_EOF )
		goto LBL_DB_FAIL;



	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( sqlda_input );
	free( sqlda_output );

	return result->size();

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	for( unsigned int i =0; i < result->size(); ++i )
		free( (*result)[i] );
	result->clear();

	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	if( tr )
		isc_rollback_transaction( status, &tr );

	if( sqlda_output )
		free( sqlda_output );

	if( sqlda_input )
		free( sqlda_input );
	return -1;
}

