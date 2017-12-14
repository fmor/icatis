#pragma once

#include <string>
#include <Vcl.Grids.hpp>
#include <Vcl.Forms.hpp>




#define MSG_TYPE_INFO  0
#define MSG_TYPE_ERROR 1

extern int blink_color_id;
extern TColor blink_colors[];

void stringgrid_defaults_options( TStringGrid* sq );
void stringgrid_drawcell( TStringGrid* sg, const TRect& rect, int col, int row, TColor color, const char* txt );
void stringgrid_clear( TStringGrid* stringgrid );




void show_msg( int type, const char* msg );
inline void show_info( const char* msg ) { show_msg( MSG_TYPE_INFO,  msg ); }
inline void show_error(const char* msg ) { show_msg( MSG_TYPE_ERROR, msg ); }
void show_balloon( TControl* control, const char* msg );


void form_post_close_message( TForm* form );
int form_show_modal( TForm* from, TForm* to );


void checkbox_at_least_one_checked( TCheckBox** cbs, int cnt, TCheckBox* selected );

void stringgrid_stretch_col(  TStringGrid* sg, int col );
void stringgrid_clear_sel( TStringGrid* sg );

int check_length( const wchar_t* str, int min, int max );
int unicodestring_as_double( double* d, UnicodeString* _str );

char* read_tiny_file( const char* filename );
char* resource_pointer( const wchar_t* name );


