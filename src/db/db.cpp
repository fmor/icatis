#include "db.h"



#include <sstream>

#include <Windows.h>


#include "error.h"
#include "util.h"
#include "config.h"


#define isc_info_sql_records 23



/*
stinfo response
{
0	: u8  : req
1	: u16 : L data_length
3	: ... : data
3+L : u8  : isc_info_end
}*/

static int stinfo_sttype( isc_stmt_handle* st )
{
/*
	revoie :
	isc_dsql_sql_stmt_select
	isc_dsql_sql_stmt_insert
	isc_dsql_sql_stmt_update
	isc_dsql_sql_stmt_delete
    .....
*/
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	char req;
	char res[256];
    int i;

	req = isc_info_sql_stmt_type;
	memset( res, 0, 256 );

	r = isc_dsql_sql_info( status, st, 1, &req, 256, res );
	if( r != 0 )
		goto LBL_FAIL;

	if( res[0] != isc_info_sql_stmt_type )
		goto LBL_FAIL;

	i = isc_vax_integer( &res[1], 2 );

	if( res[3+i] != isc_info_end )
		goto LBL_FAIL;

	i = isc_vax_integer( &res[3], i );

	return i;

LBL_FAIL:
	error_set( ERR_ISC_STATUS, NULL );
	return -1;
}

static int stinfo_affected_rows( isc_stmt_handle* st, char type )
{
/*
	type :
	isc_info_req_select
	isc_info_req_update
	isc_info_req_insert
	isc_info_req_delete
*/
	char req;
	char res[256];
	char* p;
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	int l;

	req = isc_info_sql_records;
	memset( res, 0, 256 );

	r = isc_dsql_sql_info( status, st, 1, &req, 256, res );
	if( r != 0 )
		goto LBL_FAIL;

	if( res[0] != isc_info_sql_records )
		goto LBL_FAIL;

	l = isc_vax_integer( &res[1], 2 );

	if( res[3+l] != isc_info_end )
		goto LBL_FAIL;

	p = &res[3];

	while( (*p) != isc_info_end )
	{
		char t;
		int cnt;

		t = *p;
		p++;

		l = isc_vax_integer( p, 2);
		p += 2;

		cnt = isc_vax_integer( p, l );
		p += l;

		if( t == type )
			return cnt;
	}

LBL_FAIL:
	error_set( ERR_ISC_MSG_INFO );
	return -1;
}


int db_exec( DB* db, isc_tr_handle* tr, const char* sql, int sqllen, XSQLDA* input )
{
	int status[20];
	ISC_STATUS r;
	isc_stmt_handle st = NULL;
	int cnt;
	int sttype;

	st = NULL;

	error_clear();

	r = isc_dsql_allocate_statement(status, &db->handle, &st );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare( status, tr, &st, sqllen, sql, _SQL_DIALECT_, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_execute(status, tr, &st, 1, input );
	if( r != 0 )
		goto LBL_DB_FAIL;

	switch( stinfo_sttype( &st ) )
	{
		case isc_info_sql_stmt_update : cnt = stinfo_affected_rows( &st, isc_info_req_update_count ); break;
		case isc_info_sql_stmt_delete : cnt = stinfo_affected_rows( &st, isc_info_req_delete_count ); break;
		case isc_info_sql_stmt_insert : cnt = stinfo_affected_rows( &st, isc_info_req_insert_count ); break;

		case isc_info_sql_stmt_commit :
			// a partir d'ici *tr = NULL
		default:
        	cnt = 0;
	}

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_FAIL;

	return cnt;
LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );
	return -1;
}
int db_exec_script( DB* db, const char* script )
{
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr;
	int len;
	int sz;
	int idx;

#ifdef _DEBUG_
	assert( script != NULL );
#endif


	tr = NULL;
	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	len = strlen( script );
    idx = 0;
	while( len )
	{
		sz = 0;
		for( int i = 0; i < len; ++i )
		{
			if( script[idx+i] == ';' )
			{
				sz = i;
				break;
			}
		}
		if( sz == 0 )
			break;

		r = db_exec( db, &tr, &script[idx], sz, NULL );
		if( r < 0 )
		{
			#ifdef _DEBUG_
			char buffer[2500];
			snprintf( buffer, sz, "%s", &script[idx] );
			buffer[sz] = 0;
			show_info( buffer );
			#endif

			goto LBL_FAIL;
		}

		if( tr == NULL )
		{
			// commit script
			r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
			if( r != 0 )
				goto LBL_DB_FAIL;
		}

		len -= sz;
		idx += sz + 1;

	}


	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	return 0;

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	if( tr )
		isc_rollback_transaction( status, &tr );

	return -1;
}





int db_open( DB** _db, const char* dbfile, const char* username, const char* password )
{
	ISC_STATUS_ARRAY status;
	char parameters[256];
	char* pp;
	char* p;
	short parameters_length;
	int len_username;
	int len_password;
	int i;
	ISC_STATUS r;
	DB* db;

	error_clear();

	*_db = NULL;

	db = (DB*) calloc( 1, sizeof(DB) );
	if( db == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
    }



	pp = parameters;
	len_username = strlen(username);
	len_password = strlen(password);

	*pp++ = isc_dpb_version1;
	*pp++ = isc_dpb_user_name;
	*pp++ = len_username;
	for( i =0; i < len_username; ++i)
		*pp++ = username[i];
	*pp++ = isc_dpb_password;
	*pp++ = len_password;
	for( i =0; i < len_password; ++i)
		*pp++ = password[i];
	parameters_length = pp - parameters;


	r = isc_attach_database( status, 0, dbfile, &db->handle, parameters_length, parameters );
	if( r != 0 )
		goto LBL_DB_FAIL;

   *_db = db;

	return 0;

LBL_DB_FAIL:
	error_set( status );

LBL_FAIL:
	if( db )
		free( db );
	return -1;
}


void db_close( DB** db )
{
	ISC_STATUS_ARRAY status;
	error_clear();
	isc_detach_database( status, &(*db)->handle );
	free( *db );
	*db = NULL;
}



int db_version( DB* db )
{
	static const char* SQL = "SELECT DB_VERSION, DB_DATE FROM DB_INFO";
	typedef struct
	{
        int64_t 	  DB_VERSION;
		ISC_TIMESTAMP DB_DATE;
	} RS;

	ISC_STATUS_ARRAY status;
	ISC_STATUS r;
	isc_tr_handle tr = NULL;
	isc_stmt_handle st = NULL;
	XSQLDA* sqlda = NULL;
	RS rs;



	sqlda = (XSQLDA*) calloc( 1, XSQLDA_LENGTH(2) );
	if( sqlda == NULL )
	{
		error_set( ERR_NO_MEMORY  );
		goto LBL_FAIL;
	}
	sqlda->sqld = 0;
	sqlda->sqln = 2;
	sqlda->version = 1;
	SET_SQLDA_INT64(     sqlda, 0, &rs.DB_VERSION );
	SET_SQLDA_TIMESTAMP( sqlda, 1, &rs.DB_DATE );


	r = isc_start_transaction( status, &tr, 1, &db->handle, 0, NULL );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = db_select_one_fetch(  db, &tr, SQL, NULL, sqlda );
	if( r != 1 )
	{
		error_set( ERR_DB_INFO_CORRUPT );
		goto LBL_FAIL;
	}

	r = isc_commit_transaction( status, &tr );
	if( r != 0 )
		goto LBL_DB_FAIL;

	if( rs.DB_VERSION > 0xFFFFFFFF )
	{
		error_set( ERR_DB_INFO_CORRUPT );
		goto LBL_FAIL;
	}

	return rs.DB_VERSION;

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );
	if( tr )
		isc_rollback_transaction( status, &tr );
	if( sqlda )
		free( sqlda );
	return 0; // L'upgrade échouera si ce n'est pas une base tiny stock
}

int db_upgrade( DB* db )
{
	static bool b = 1;
	static char* SQL_UPGRADE[1];

	int v;
	int r;


	// Init scripts
	if( b )
	{
		b = 0;
		SQL_UPGRADE[0] = resource_pointer( L"DB_0_SQL" );
	}


	for(;;)
	{
		v = db_version( db );
		if( v >= APP_VERSION )
			break;

		r = db_exec_script( db, SQL_UPGRADE[v] );
		if( r != 0 )
			goto LBL_FAIL;

	}

	return 0;

LBL_FAIL:
	return -1;
}



int db_select_one_fetch( DB* db, isc_tr_handle* tr, const char* sql, XSQLDA* input, XSQLDA* _output )
{
	ISC_STATUS_ARRAY status;
	ISC_STATUS r;

	XSQLDA* output = NULL;
	int output_length = XSQLDA_LENGTH(_output->sqln);
	isc_stmt_handle st = NULL;

    int cnt;



	error_clear();

	output = (XSQLDA*) calloc( 1, output_length );
	if( output == NULL )
	{
		error_set( ERR_NO_MEMORY );
		goto LBL_FAIL;
	}
	output->sqld = _output->sqld;
	output->sqln = _output->sqln;
	output->version = _output->version;

	r = isc_dsql_allocate_statement(status, &db->handle, &st);
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_prepare(status, tr, &st, 0, sql, _SQL_DIALECT_, output );
	if( r != 0 )
		goto LBL_DB_FAIL;

    memcpy( output->sqlvar, _output->sqlvar, sizeof(XSQLVAR) * _output->sqln );

	r = isc_dsql_execute(status, tr, &st, 1, input );
	if( r != 0 )
		goto LBL_DB_FAIL;

	r = isc_dsql_fetch(status, &st, 1, output );
	if( r == 0 )
		cnt = 1;
	else if( r == FETCH_EOF )
		cnt = 0;
	else
		goto LBL_DB_FAIL;

	r = isc_dsql_free_statement( status, &st, DSQL_drop );
	if( r != 0 )
		goto LBL_DB_FAIL;

	free( output );

	return cnt;

LBL_DB_FAIL:
	error_set( status );
LBL_FAIL:
	if( output )
		free( output );
	if( st )
		isc_dsql_free_statement( status, &st, DSQL_drop );

	return -1;
}




