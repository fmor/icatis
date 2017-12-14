#include "error.h"


#include "config.h"
#include "util.h"

#include <string>
#include <sstream>


extern "C" {
#include <stdio.h>
#include <assert.h>
#include <ibase.h>
#include <iberror.h>
}



#define BUFFER_SIZE 1024

static char  buffer[BUFFER_SIZE];
static long  last_error_code;
static char* last_error_string;

void error_clear()
{
	last_error_string = "Pas d'erreur";
	last_error_code = 0;
	buffer[0] = 0;
}

const char* error_last()
{
	return last_error_string;
}
void error_set( long code )
{
	error_set( code, NULL );
}
void error_set( const char* msg )
{
	error_set( ERR_CUSTOM, msg );
}
void error_set( intptr_t* iscstatus )
{
	error_set( ERR_ISC_STATUS, (const char*) iscstatus );
}

void error_set( long code, const char* msg )
{
	last_error_code = code;

	switch( code )
	{
		// Dev errors
		case ERR_ISC_MSG_INFO		  : assert( 0 && "STUPIDO : Message ISC Malformé" );

		//
		case ERR_NO_MEMORY			  :
			last_error_string = "Mémoire insuffisant";
		break;

		case ERR_BAD_ARGUMENTS 		  :
			last_error_string = "Arguments incorrects";
		break;

		case ERR_DB_INFO_CORRUPT	  :
			last_error_string = "La table d'information de la base de donnée est corrompue";
		break;



		case ERR_CUSTOM 			  :
		{
			int l = strlen(msg);
			if( l > (BUFFER_SIZE-1) )
				l = BUFFER_SIZE-1;
			memcpy( buffer, msg, l );
			buffer[l] = 0;

			last_error_string = buffer;
		}
		break;

		case ERR_ISC_STATUS :
		{
			intptr_t* _msg = (intptr_t*) msg;
			switch( _msg[1] )
			{

				case isc_foreign_key 			: last_error_string = "Des enregistrements connexes existent"; break;
				case isc_unique_key_violation 	: last_error_string = "Un enregistrement avec ces identifiants existe"; break;
				case isc_bad_db_handle			: last_error_string = "Erreur de connexion à la la base de données"; break;
				case isc_no_dup					: last_error_string = "Un enregistrement ayant une ou plusieurs caractérisques existe déjà dans la base de donnée"; break;

				default:
				{
					std::stringstream ss;
					std::string str;
					int l;

					ss << "Erreur de base de donnée :\n\n";
					while( isc_interprete( buffer, &_msg )  )
						ss << buffer << std::endl;

					str = ss.str();
					l = str.size();
					if( l > (BUFFER_SIZE-1) )
						l = BUFFER_SIZE -1;
					memcpy( buffer, str.c_str(), l );
					buffer[l] = 0;

					last_error_string = buffer;
				}



			}

		}
		break;

		default :
			assert( 0 && "error_set()" );
    }

}

void show_last_error()
{
	show_msg( MSG_TYPE_ERROR, last_error_string );
}

