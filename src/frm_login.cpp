//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frm_login.h"
#include "frm_main.h"

#include "config.h"
#include "db/db.h"
#include "util.h"
#include "error.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmLogin *FrmLogin;



__fastcall TFrmLogin::TFrmLogin(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmLogin::FormCreate(TObject *Sender)
{
	this->Position = poMainFormCenter;
	this->BorderStyle = bsDialog;

	this->lbl_title->Caption = APP_NAME;

	this->ed_username->Text = DEFAULT_DB_USERNAME;
	this->ed_password->Text = DEFAULT_DB_PASSWORD;
	this->ed_database->Text = DEFAULT_DB_FILE;

}
void __fastcall TFrmLogin::btn_quitClick(TObject *Sender)
{
	Application->Terminate();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TFrmLogin::btn_loginClick(TObject *Sender)
{
	int r;
	AnsiString u;
	AnsiString p;
	AnsiString d;

	u = AnsiString( this->ed_username->Text );
	p = AnsiString( this->ed_password->Text );
	d = AnsiString( this->ed_database->Text );

	r = db_open( &FrmMain->db, d.c_str(), u.c_str(), p.c_str() );
	if( r != 0 )
	{
		show_last_error();
		return;
	}
	r = form_show_modal( this, FrmMain );
	if( r == mrAbort )
		Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmLogin::btn_select_bddClick(TObject *Sender)
{
	int r;

	this->OpenDialog1->Filter = "Base de données Interbase/Firebird (*.gdb)|*.GDB";
	r = this->OpenDialog1->Execute( this->Handle );
	if( r )
    	this->ed_database->Text = this->OpenDialog1->FileName;

}
//---------------------------------------------------------------------------

