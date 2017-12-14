//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <vector>
#include <stdlib.h>

#include "frm_main.h"
#include "frm_article.h"

#include "util.h"
#include "error.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmArticle *FrmArticle;


#define IDX_EDIT_LIBELLE	0
#define IDX_EDIT_LARGEUR    1
#define IDX_EDIT_HAUTEUR    2
#define IDX_EDIT_PROFONDEUR 3
#define IDX_EDIT_QUANTITE   4
#define IDX_EDIT_POIDS	    5

#include "db/deposant.h"



//---------------------------------------------------------------------------
__fastcall TFrmArticle::TFrmArticle(TComponent* Owner) :
	TForm(Owner),
    m_Article( NULL )
{
}

void __fastcall TFrmArticle::FormCreate(TObject *Sender)
{

	this->cb_deposant->Clear();
	this->cb_deposant->Style = csDropDownList;

	this->ed_reference->MaxLength = 30;
	this->ed_libelle->MaxLength = 50;

	this->ed_nbjour->NumbersOnly = True;
	this->ed_nbjour->Text = 1;

	// P
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_LIBELLE] 	= this->ed_p_libelle;
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_LARGEUR] 	= this->ed_p_largeur;
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_HAUTEUR] 	= this->ed_p_hauteur;
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_PROFONDEUR]= this->ed_p_profondeur;
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_QUANTITE] 	= this->ed_p_quantite;
	this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_POIDS] 	= this->ed_p_poids;
	// C
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_LIBELLE] 	= this->ed_c_libelle;
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_LARGEUR] 	= this->ed_c_largeur;
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_HAUTEUR] 	= this->ed_c_hauteur;
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_PROFONDEUR]= this->ed_c_profondeur;
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_QUANTITE] 	= this->ed_c_quantite;
	this->m_CondEdits[IDX_COND_CONDITIONNEMENT][IDX_EDIT_POIDS] 	= this->ed_c_poids;
	// S
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_LIBELLE] 	= this->ed_s_libelle;
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_LARGEUR] 	 = this->ed_s_largeur;
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_HAUTEUR] 	 = this->ed_s_hauteur;
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_PROFONDEUR]= this->ed_s_profondeur;
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_QUANTITE]  = this->ed_s_quantite;
	this->m_CondEdits[IDX_COND_SOUS_CONDITIONNEMENT][IDX_EDIT_POIDS] 	 = this->ed_s_poids;
	// U
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_LIBELLE] 	= this->ed_u_libelle;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_LARGEUR] 	= this->ed_u_largeur;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_HAUTEUR] 	= this->ed_u_hauteur;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_PROFONDEUR]	= this->ed_u_profondeur;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_QUANTITE] 	= this->ed_u_quantite;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_POIDS] 		= this->ed_u_poids;

	for( int i = 0; i < 4; ++i )
	{
		this->m_CondEdits[IDX_COND_PALETTE][IDX_EDIT_LIBELLE]->MaxLength = 10;

		for( int j=1; j < 6; ++j )
		{
			this->m_CondEdits[i][j]->NumbersOnly = true;
		}

	}

	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_QUANTITE]->Text = 1;
	this->m_CondEdits[IDX_COND_UNITE][IDX_EDIT_QUANTITE]->ReadOnly = true;



}
void __fastcall TFrmArticle::FormShow(TObject *Sender)
{
	int r;
	std::vector<Deposant*> deposants;

	// Déposants
	r = db_select_deposants( FrmMain->db, &deposants );
	if( r < 0 )
		goto LBL_FAIL;

	for( unsigned int i =0; i < deposants.size(); ++i )
	{
		Deposant* d = deposants[i];
		this->cb_deposant->AddItem( d->LIBXDEP, (TObject*) d );
		if( this->m_Article && (strcmp(d->CODXDEP, this->m_Article->DEPXART) == 0) )
			this->cb_deposant->ItemIndex = i;
	}



	return;

LBL_FAIL:
	show_last_error();
	form_post_close_message( this );
}


void __fastcall TFrmArticle::FormClose(TObject *Sender, TCloseAction &Action)
{

    // Déposants
	for( int i = 0; i < this->cb_deposant->Items->Count; ++i )
		free( this->cb_deposant->Items->Objects[i] );
	this->cb_deposant->Clear();
}

void TFrmArticle::init( Article* article )
{
	static const char* cond_format = "%0.2lf";
	int r;
	char buffer[64];


	// Article
	this->m_Article = article;



	this->ed_reference->Text = article->REFXART;
	this->ed_libelle->Text = article->LIBXART;
	this->chk_lot->Checked = article->LOTXART;

	this->chk_dlv->Checked = article->DLVXART;
	sprintf( buffer, "%d", article->NBJXART );
	this->ed_nbjour->Text = buffer;
	this->chk_dlvClick( NULL );

	// Cond
	for( int i = 0; i < 4; ++i )
	{
		this->m_CondEdits[i][IDX_EDIT_LIBELLE]->Text = article->conds[i].LIBXARC;

		sprintf( buffer, cond_format, article->conds[i].LARXARC );
		this->m_CondEdits[i][IDX_EDIT_LARGEUR]->Text 	=  buffer;

		sprintf( buffer, cond_format, article->conds[i].HAUXARC );
		this->m_CondEdits[i][IDX_EDIT_HAUTEUR]->Text 	= buffer;

		sprintf( buffer, cond_format, article->conds[i].PROXARC );
		this->m_CondEdits[i][IDX_EDIT_PROFONDEUR]->Text = buffer;

		sprintf( buffer, cond_format, article->conds[i].QTEXARC );
		this->m_CondEdits[i][IDX_EDIT_QUANTITE]->Text 	= buffer;

		sprintf( buffer, cond_format, article->conds[i].POIXARC );
		this->m_CondEdits[i][IDX_EDIT_POIDS]->Text 		= buffer;

	}


	return;

LBL_FAIL:
	show_last_error();
	form_post_close_message( this );
}

//---------------------------------------------------------------------------





int TFrmArticle::validate()
{
	if( this->cb_deposant->ItemIndex == -1 )
	{
		show_balloon( this->cb_deposant, "Un déposant est requis" );
		return -1;
	}

	if( check_length( this->ed_reference->Text.c_str(), 1, 30) != 0 )
	{
		show_balloon( this->ed_reference, "Référence invalide" );
		return -1;
	}
	if( check_length(this->ed_libelle->Text.c_str(), 1, 50) != 0 )
	{
		show_balloon( this->ed_libelle, "Libellé invalide" );
		return -1;
	}

	if( this->chk_dlv->Checked && ( StrToInt64(this->ed_nbjour->Text) < 1 ) )
	{
		show_balloon( this->ed_nbjour, "Nombre de jour de vente invalide" );
		return -1;
	}

	for( int i =0; i < 4; ++i )
	{
		TEdit* ed =  this->m_CondEdits[i][IDX_EDIT_LIBELLE];
		if( check_length( ed->Text.c_str(), 1 , 10) != 0 )
		{
			show_balloon( ed, "Libellé invalide" );
			return -1;
		}
	}



	return 0;

}
void __fastcall TFrmArticle::chk_dlvClick(TObject *Sender)
{
	this->ed_nbjour->Enabled  = this->chk_dlv->Checked;
	this->ed_nbjour->Visible  = this->chk_dlv->Checked;
	this->lbl_nbjour->Visible = this->chk_dlv->Checked;
}


void __fastcall TFrmArticle::btn_annulerClick(TObject *Sender)
{
	this->ModalResult = mrCancel;
}



void __fastcall TFrmArticle::Action_article_enregistrerExecute(TObject *Sender)
{
	int r;
	Article a;
	Deposant* d;

	r = this->validate();
	if( r != 0 )
		return;

	memset( &a, 0, sizeof(Article) );

	d = (Deposant*) this->cb_deposant->Items->Objects[ this->cb_deposant->ItemIndex ];

	/*
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
*/


	strcpy( a.DEPXART, d->CODXDEP );
	strcpy( a.REFXART, AnsiString(this->ed_reference->Text).c_str() );
	strcpy( a.LIBXART, AnsiString(this->ed_libelle->Text).c_str() );
	a.DLVXART = (this->chk_dlv->Checked) ? 1 : 0;
	a.LOTXART = (this->chk_lot->Checked) ? 1 : 0;
	a.NBJXART = StrToInt64(this->ed_nbjour->Text);
	a.ARTXRES = this->m_Article->ARTXRES;

	// Conditionnements
	for( int i =0; i < 4; ++i )
	{
		strcpy( a.conds[i].LIBXARC, AnsiString(this->m_CondEdits[i][IDX_EDIT_LIBELLE]->Text).c_str() );

		r = unicodestring_as_double( &(a.conds[i].LARXARC), &(this->m_CondEdits[i][IDX_EDIT_LARGEUR]->Text) );
		if( r != 0 )
			goto LBL_FAIL;

		r = unicodestring_as_double( &(a.conds[i].HAUXARC), &(this->m_CondEdits[i][IDX_EDIT_HAUTEUR]->Text) );
		if( r != 0 )
			goto LBL_FAIL;

		r = unicodestring_as_double( &(a.conds[i].PROXARC), &(this->m_CondEdits[i][IDX_EDIT_PROFONDEUR]->Text) );
		if( r != 0 )
			goto LBL_FAIL;

		r = unicodestring_as_double( &(a.conds[i].QTEXARC), &(this->m_CondEdits[i][IDX_EDIT_QUANTITE]->Text) );
		if( r != 0 )
			goto LBL_FAIL;

		r = unicodestring_as_double( &(a.conds[i].POIXARC), &(this->m_CondEdits[i][IDX_EDIT_POIDS]->Text) );
		if( r != 0 )
			goto LBL_FAIL;
	}

	if( this->m_Article->ARTXRES == -1 )
		r = db_insert_article( FrmMain->db, &a );
	else
		r = db_update_article( FrmMain->db, this->m_Article->DEPXART, this->m_Article->REFXART, &a );

	if( r < 0 )
		goto LBL_FAIL;


	this->ModalResult = mrOk;
	return;

LBL_FAIL:
	show_last_error();

}
//---------------------------------------------------------------------------

