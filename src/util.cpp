#include "util.h"

#include <clipbrd.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "error.h"
#include "config.h"

int blink_color_id = 0;
TColor blink_colors[2] = {clWhite, clRed };

void stringgrid_defaults_options( TStringGrid* sg )
{
	TGridOptions s;
	sg->Options << goRowSelect << goDrawFocusSelected; //<< goFixedColClick;
	s << goRangeSelect;
	sg->Options -= s;
}

void stringgrid_drawcell( TStringGrid* sg, const TRect& rect, int col, int row, TColor color, const char* txt )
{
	sg->Canvas->Brush->Color = color;
	sg->Canvas->FillRect( rect );
	if( txt )
		sg->Canvas->TextOutW( rect.Left + 2, rect.Top +2, txt );
	else
		sg->Canvas->TextOutW( rect.Left + 2, rect.Top +2, sg->Cells[col][row]  );
}


void stringgrid_clear( TStringGrid* sg )
{
	for( int i=0; i < sg->ColCount; ++i )
		sg->Cols[i]->Clear();
}

void show_msg( int type, const char* msg )
{
	Clipboard()->Clear();
	Clipboard()->AsText = msg;
	switch( type )
	{
		case MSG_TYPE_ERROR :
			MessageBoxA( NULL, msg , "Error", MB_ICONERROR| MB_OK );
		break;

		case MSG_TYPE_INFO :
		default:
			MessageBoxA( NULL, msg , "Info", MB_ICONINFORMATION | MB_OK );
			break;
	}

}


void show_balloon( TControl* control, const char* msg )
{
	static TBalloonHint* balloon = new TBalloonHint( NULL );

	MessageBeep( 0xFFFFFFFF );
	balloon->Delay = 0;
	balloon->HideAfter = 2500;
	balloon->Description = msg;
	balloon->ShowHint( control );
}



void form_post_close_message( TForm* form )
{
	PostMessageA( form->Handle, WM_CLOSE, 0, 0 );
}
void form_show_center( TForm* form )
{
	form->Position = poDesktopCenter;
	form->Show();
}
void form_post_hide_message( TForm* form )
{
	PostMessageA( form->Handle, SW_HIDE, 0, 0 );
}

void checkbox_at_least_one_checked( TCheckBox** cbs, int cnt, TCheckBox* selected )
{
	TNotifyEvent ev;

	if( selected->Checked )
		return;

	for( int i = 0; i < cnt; ++i )
	{
		if( cbs[i]->Checked )
			return;
	}

	ev = selected->OnClick;
	selected->OnClick = NULL;
	selected->Checked = true;
	selected->OnClick = ev;

	show_balloon( selected, "Au moins 1 critère est requis");
}


void stringgrid_stretch_col(  TStringGrid* sg, int col )
{
	int w;

	if( col < 0)
		return;

	if( col >= sg->ColCount )
		return;

	tagSCROLLINFO sbinfo;

	/*
	Win32Check( GetScrollInfo(sg->Handle, SB_VERT, &sbinfo) );
	sbinfo.cbSize
	*/

	w = sg->Width -  sg->BevelWidth - GetSystemMetrics(SM_CXVSCROLL);
	for( int i = 0;i < sg->ColCount; ++i )
	{
		if( i == col )
			continue;
		w -= sg->ColWidths[i];
	}
	sg->ColWidths[ col ] = w;
}

void stringgrid_clear_sel( TStringGrid* sg )
{
	static const TGridRect rect = {0, 0, 0, 0};
	sg->Selection = rect;
}


int check_length( const wchar_t* str, int min, int max )
{
	int l = wcslen( str );
	if( l < min )
		return -1;
	if( l > max )
		return -1;

	return 0;
}

int unicodestring_as_double( double* d, UnicodeString* _str )
{
	AnsiString str;
	int r;

	str = AnsiString( *_str );

	r = sscanf( str.c_str(), "%lf", d );
	if( r != 1 )
	{
		char buffer[512];
		sprintf( buffer, "Impossible de convertir en double :\n%s", str.c_str() );
		error_set( buffer );
		return -1;
	}
	return 0;
}

int form_show_modal( TForm* from, TForm* to )
{
	int r;

	to->Position = poMainFormCenter;

	if( from )
		from->Hide();

	r = to->ShowModal();

	if( from )
		from->Show();
	return r;

}


char* read_tiny_file( const char* filename )
{
	FILE* f ;
	int r;
	char* buffer;
    int64_t L;

	f = NULL;
	buffer = NULL;

	f = fopen( filename , "rb" );
	if( f == NULL )
		goto LBL_FAIL;

	r = fseek( f, 0, SEEK_END );
	if( r != 0 )
		goto LBL_FAIL;

	L = ftell( f );
	if( L < 0 )
		goto LBL_FAIL;

	buffer = (char*) malloc( L );
	if( buffer == NULL )
		goto LBL_FAIL;

	r = fseek( f, 0, SEEK_SET );
	if( r != 0 )
		goto LBL_FAIL;

	r = fread( buffer, L, 1, f );
	if( r != 1 )
		goto LBL_FAIL;

	fclose( f );

	return buffer;

LBL_FAIL:
    show_error( strerror(errno) );

	if( f )
		fclose( f );
	if( buffer )
		free( buffer );

	return NULL;
}


char* resource_pointer( const wchar_t* name )
{
	char* p;

	HRSRC i;
	HGLOBAL g;
	uint32_t sz;

	i = FindResourceW( NULL, name , RT_RCDATA );
	assert( i != NULL );

	g = LoadResource( NULL, i );
	assert( g != NULL );

 	sz = SizeofResource( NULL, i );
	assert( sz != 0 );

	p = (char*) LockResource( g );
	assert( p != NULL );

	return p;
}
