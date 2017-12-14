#include "deposant.h"

#include <assert.h>
#include <stdlib.h>

#include "../error.h"
#include "../util.h"

#include "varying.h"


int db_select_deposants( DB* db, std::vector<Deposant*>* results )
{
	static const char* SQL = "SELECT CODXDEP, LIBXDEP FROM DEPXRES ORDER BY LIBXDEP ASC";
	typedef struct
	{
		Varying<5> CODXDEP;
		Varying<30> LIBXDEP;
	} RS;

	XSQLDA* sqlda = NULL;
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr = NULL;
	isc_stmt_handle st = NULL;
	RS rs;

	error_clear();

	results->clear();


	sqlda = (XSQLDA*) malloc( XSQLDA_LENGTH(2) );
	if( sqlda == NULL )
		goto LBL_FAIL;
	sqlda->sqld = 0;
	sqlda->sqln = 3;
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


	SET_SQLDA_VARYING( sqlda, 0, &rs.CODXDEP );
	SET_SQLDA_VARYING( sqlda, 1, &rs.LIBXDEP );

	r = isc_dsql_execute( status, &tr, &st, 1, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	while( (r= isc_dsql_fetch( status, &st, 1, sqlda)) == 0 )
	{
		Deposant* d = (Deposant*) malloc(sizeof(Deposant) );

		memcpy( d->CODXDEP, rs.CODXDEP.str, rs.CODXDEP.length ); d->CODXDEP[rs.CODXDEP.length] = 0;
		memcpy( d->LIBXDEP, rs.LIBXDEP.str, rs.LIBXDEP.length ); d->LIBXDEP[rs.LIBXDEP.length] = 0;

		results->push_back( d );
	}
	if( r != FETCH_EOF )
		goto LBL_DB_FAIL;

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( sqlda );

	return results->size();

LBL_DB_FAIL:
	error_set( status );

LBL_FAIL:

	for( unsigned int i =0; i < results->size(); ++i )
		free( (*results)[i] );
	results->clear();

	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	if( tr )
		isc_rollback_transaction( status, &tr );

	if( sqlda )
		free( sqlda );

	return -1;
}
