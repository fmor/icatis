//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frm_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;

#include "error.h"
#include "util.h"
#include "config.h"


#include "frm_article.h"
#include "frm_gestion_articles.h"
#include "frm_stock.h"
#include "frm_login.h"




//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
	this->Position = poMainFormCenter;
	this->BorderStyle = bsDialog;
	this->Caption = APP_NAME;
	this->db = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
	if( this->db )
		db_close( &db );
}

void __fastcall TFrmMain::FormShow( TObject *Sender )
{

	char buffer[256];
	int r;

	r  = db_version( this->db );
	if( r < 0 )
		goto LBL_FAIL;

	if( r != APP_VERSION )
	{
		show_info( "La base de données va être mise à jour.\n Il est conseillé de faire une sauvegarde de celle-ci\navant de procéder" );
		r = db_upgrade( this->db );
		if( r != 0 )
		{
			show_last_error();
			show_error( "Echec de la mise à jour de base" );
			form_post_close_message( this );
			return;
		}
		show_info( "Mise à jour réussie");
	}

	// Statis
	sprintf( buffer, "%s@%s", DEFAULT_DB_USERNAME, DEFAULT_DB_FILE );
	this->sb->SimplePanel = true;
	this->sb->SimpleText = buffer;

	return;

LBL_FAIL:
	this->sb->SimpleText = "Echec de la connection à la base de données";
}


void __fastcall TFrmMain::btn_articlesClick(TObject *Sender)
{
	form_show_modal( this, FrmGestionArticles );
}
void __fastcall TFrmMain::btn_stockClick(TObject *Sender)
{
	form_show_modal( this, FrmStock );
}

void __fastcall TFrmMain::btn_quitterClick(TObject *Sender)
{
	int r;

	TMsgDlgButtons buttons;
	buttons << mbYes << mbNo;


	r = MessageDlg( "Se déconnecter et quitter ?", mtConfirmation, buttons , 0 );
	if( r == mrYes )
		this->ModalResult = mrAbort;
}

