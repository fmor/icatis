//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <assert.h>

#include "frm_main.h"
#include "frm_gestion_articles.h"
#include "frm_article.h"

#include "db/db.h"
#include "util.h"
#include "error.h"
#include "config.h"

#define COL_REFXART 0
#define COL_LIBXART 1
#define COL_QTEXARC 2
#define COL_LARXARC 3
#define COL_HAUXARC 4
#define COL_PROXARC 5

#define STRETCH_COLUMN COL_LIBXART



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmGestionArticles *FrmGestionArticles;
//---------------------------------------------------------------------------
__fastcall TFrmGestionArticles::TFrmGestionArticles(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TFrmGestionArticles::FormCreate(TObject *Sender)
{

	this->Timer1->Interval = DEFAULT_BLINK_INTERVAL;
	this->Timer1->Enabled = false;

	// Status
	this->sb_form->SimplePanel = true;


	// Grille
	stringgrid_defaults_options( this->sg_articles );
	this->sg_articles->PopupMenu = this->PopupMenu_Articles;
	this->sg_articles->ColCount = 6;
	this->sg_articles->RowCount = 1;
	this->sg_articles->FixedCols = 0;
	this->sg_articles->ColWidths[COL_REFXART] = 100;
	this->sg_articles->ColWidths[COL_LIBXART] = 150;
	this->sg_articles->ColWidths[COL_LARXARC] = 60;
	this->sg_articles->ColWidths[COL_HAUXARC] = 60;
	this->sg_articles->ColWidths[COL_PROXARC] = 60;
	this->sg_articles->ColWidths[COL_QTEXARC] = 60;

}


void __fastcall TFrmGestionArticles::FormShow(TObject *Sender)
{
	int r;
	std::vector<Deposant*> deposants;

	// Déposants
	this->m_DeposantIndex = -1;
	this->lb_deposant->MultiSelect = false;
	this->lb_deposant->Clear();
	r = db_select_deposants( FrmMain->db, &deposants );
	if( r < 0 )
		goto LBL_FAIL;
	if( r == 0 )
	{
		error_set( "Veuillez créer un déposant");
		goto LBL_FAIL;
	}
	for( int i =0; i < r; ++i )
	{
		Deposant* d = deposants[i];
		this->lb_deposant->AddItem( d->LIBXDEP, (TObject*) d );
	}

	// Search
	this->ed_recherche->OnChange = NULL;
	this->ed_recherche->Clear();
	this->ed_recherche->OnChange = this->ed_rechercheChange;

	// Chb
	this->cb_recherche_auto->Checked = 1;


	this->cb_reference->OnClick = NULL;
	this->cb_libelle->OnClick 	= NULL;
	this->cb_reference->Checked = true;
	this->cb_libelle->Checked   = true;
	this->cb_reference->OnClick = this->cb_onclick;
	this->cb_libelle->OnClick 	= this->cb_onclick;


	//
	this->lb_deposant->Selected[0] = true;
	this->m_DeposantIndex = 0;
	this->rechercher();

	return;

LBL_FAIL:
    show_last_error();
	PostMessageA( this->Handle, WM_CLOSE, 0, 0 );
	return;
}

void __fastcall TFrmGestionArticles::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Deposants
	for( int i = 0; i < this->lb_deposant->Items->Count; ++i )
		free( this->lb_deposant->Items->Objects[i] );
	this->lb_deposant->Clear();

	this->clear();
}


void __fastcall TFrmGestionArticles::FormResize(TObject *Sender)
{
	stringgrid_stretch_col( this->sg_articles, STRETCH_COLUMN );
}


void TFrmGestionArticles::clear()
{
	int nb_articles;

	nb_articles = this->m_ArticlesVector.size();

	for( int i = 0; i < nb_articles; ++i )
		free( this->m_ArticlesVector[i] );
	this->m_ArticlesVector.clear();

	this->sg_articles->Cols[0]->Clear();
	this->sg_articles->RowCount = 1;

}
void TFrmGestionArticles::rechercher()
{
	char buffer[256];
	AnsiString s;
	Deposant* d;
	char* REFXART;
	char* LIBXART;
	int r;

	d = NULL;

	this->clear();


	r = this->lb_deposant->Items->Count;
	for( int i = 0; i < r; ++i )
	{
		if( this->lb_deposant->Selected[i] )
		{
			d = (Deposant*) this->lb_deposant->Items->Objects[i];
			break;
		}
	}

	assert( d != NULL );

	REFXART = NULL;
    LIBXART = NULL;

	s = this->ed_recherche->Text;
	if( s.Length() )
	{
		if(  this->cb_reference->Checked )
			REFXART = s.c_str();

		if( this->cb_libelle->Checked )
			LIBXART = s.c_str();
	}

	r = db_select_article_par_deposant( FrmMain->db, &this->m_ArticlesVector, d->CODXDEP, REFXART, LIBXART );
	if( r < 0 )
		goto LBL_FAIL;

	// Grid
	this->sg_articles->RowCount = 1 + r;
	if( r )
		this->sg_articles->FixedRows = 1;
	if( r == 1 )
		this->sg_articles->Row = 1;
	else
		stringgrid_clear_sel( this->sg_articles );

	// Status
	sprintf( buffer, "Articles : %d", r );
	this->sb_form->SimpleText = buffer;


	return;

LBL_FAIL:
	show_last_error();

	this->sg_articles->RowCount = 1;
	this->sb_form->SimpleText = "Articles : 0";
}

//---------------------------------------------------------------------------
void __fastcall TFrmGestionArticles::cb_onclick(TObject *Sender)
{
	if( (this->cb_libelle->Checked == false) && (this->cb_reference->Checked == false) )
	{
		assert( Sender );
		TCheckBox* cbs[2] = { this->cb_libelle, this->cb_reference };
		checkbox_at_least_one_checked( cbs, 2, (TCheckBox*) Sender );
	}
	else
	{
		if( this->cb_recherche_auto->Checked )
			this->rechercher();
	}

}
void __fastcall TFrmGestionArticles::lb_deposantClick(TObject *Sender)
{
	for( int i = 0; i < this->lb_deposant->Items->Count; ++i )
	{
		if( this->lb_deposant->Selected[i] && (i !=  this->m_DeposantIndex ) )
		{
			this->m_DeposantIndex = i;

			if( this->cb_recherche_auto->Checked )
				this->rechercher();

			break;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrmGestionArticles::ed_rechercheChange(TObject *Sender)
{
	if( this->cb_recherche_auto->Checked )
		this->rechercher();
}

void __fastcall TFrmGestionArticles::ed_rechercheKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Key == VK_RETURN )
		this->rechercher();

}
//---------------------------------------------------------------------------
void __fastcall TFrmGestionArticles::btn_rechercherClick(TObject *Sender)
{
	this->rechercher();
}

void __fastcall TFrmGestionArticles::btn_effacerClick(TObject *Sender)
{
	this->ed_recherche->Clear();
}


void __fastcall TFrmGestionArticles::btn_fermerClick(TObject *Sender)
{
	this->ModalResult = mrOk;
}
//
void __fastcall TFrmGestionArticles::sg_articlesSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	CanSelect = true;
}
void __fastcall TFrmGestionArticles::sg_articlesKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch( Key )
	{
		case VK_RETURN:
			if( this->sg_articles->Row >= this->sg_articles->FixedRows )
				this->action_article_modifier->Execute();
			break;

		case VK_DELETE:
			if( this->sg_articles->Row >= this->sg_articles->FixedRows )
				this->action_articles_supprimer->Execute();
			break;
	}
}
void __fastcall TFrmGestionArticles::sg_articlesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

	if( (ARow == 0) && (this->sg_articles->Objects[0][0] == NULL) )
	{
		this->sg_articles->Cells[COL_REFXART][0] = "Référence";
		this->sg_articles->Cells[COL_LIBXART][0] = "Libellé";
		this->sg_articles->Cells[COL_QTEXARC][0] = "Quantité";
		this->sg_articles->Cells[COL_LARXARC][0] = "Largeur";
		this->sg_articles->Cells[COL_HAUXARC][0] = "Hauteur";
		this->sg_articles->Cells[COL_PROXARC][0] = "Profondeur";

		this->sg_articles->Objects[0][0] = (TObject*) 1;
	}
	else if( ARow && (this->sg_articles->Objects[0][ARow]== NULL) )
	{
		Article_par_deposant* article = this->m_ArticlesVector[ ARow -1 ];

		this->sg_articles->Cells[COL_REFXART][ARow] = article->REFXART;
		this->sg_articles->Cells[COL_LIBXART][ARow] = article->LIBXART;
		this->sg_articles->Cells[COL_LARXARC][ARow] = article->LARXARC;
		this->sg_articles->Cells[COL_HAUXARC][ARow] = article->HAUXARC;
		this->sg_articles->Cells[COL_PROXARC][ARow] = article->PROXARC;
		this->sg_articles->Cells[COL_QTEXARC][ARow] = article->QTEXARC;

		this->sg_articles->Objects[0][ARow] = (TObject*) article;
	}

	if( ARow == 0 )
		return;

	Article_par_deposant* article = this->m_ArticlesVector[ ARow -1 ];
	switch( ACol )
	{
		case COL_REFXART:
		if( article->REFXART[0] == 0 )
		{
			this->Timer1->Enabled = true;
			stringgrid_drawcell( this->sg_articles,  Rect, ACol, ARow, blink_colors[blink_color_id], "Invalid" );
		}
		break;

		case COL_LIBXART:
		if( article->LIBXART[0] == 0 )
		{
			this->Timer1->Enabled = true;
			stringgrid_drawcell( this->sg_articles,  Rect, ACol, ARow, blink_colors[blink_color_id], "Invalid" );
		}
		break;

	}




}

Article_par_deposant* TFrmGestionArticles::selected_article() const
{
	Article_par_deposant* a;

	if( this->sg_articles->Row < 1 )
		return NULL;

	a = (Article_par_deposant*) this->sg_articles->Objects[0][ this->sg_articles->Row ];
	return a;
}
Deposant* TFrmGestionArticles::selected_deposant() const
{
	Deposant* d;

	if( this->m_DeposantIndex < 0 )
		return NULL;

	d = (Deposant*) this->lb_deposant->Items->Objects[ this->m_DeposantIndex ];
    return d;
}

//---------------------------------------------------------------------------

void __fastcall TFrmGestionArticles::action_articles_ajouterExecute(TObject *Sender)
{
	int r;
	Article a;
	Deposant* d;

	d = this->selected_deposant();
	assert( d != NULL );

	article_init( &a );
	memcpy( a.DEPXART, d->CODXDEP, sizeof(a.DEPXART) );

	FrmArticle->init( &a );
	FrmArticle->Position = poMainFormCenter;
	r = FrmArticle->ShowModal();
	if( r == mrOk )
		this->rechercher();

}

void __fastcall TFrmGestionArticles::action_articles_supprimerExecute(TObject *Sender)
{
	Deposant* d;
	Article_par_deposant* a;
	int r;
	char buffer[256];
	TMsgDlgButtons buttons;

	d = this->selected_deposant();
	assert( d != NULL );

	a = this->selected_article();
	assert( a != NULL );

    sprintf( buffer, "Supprimer l'article %s ?", a->LIBXART );
	r = MessageBoxA( this->Handle, buffer, "Confirmation", MB_YESNO | MB_ICONQUESTION );

	if( r == IDNO )
		return;

	r = db_delete_article( FrmMain->db, d->CODXDEP, a->REFXART );
	if( r < 0 )
		goto LBL_FAIL;

	this->rechercher();
	return;

LBL_FAIL:
	show_last_error();
}
//---------------------------------------------------------------------------




void __fastcall TFrmGestionArticles::action_article_modifierExecute(TObject *Sender)
{
	int r;
	Article art;
	Article_par_deposant* a;
	Deposant* d;
	int idx;
	int cnt;

	if( this->sg_articles->Row == 0 )
		return;

	idx = this->sg_articles->Row - 1;

	d = this->selected_deposant();
	assert( d != NULL );

	a = this->selected_article();
	assert( a != NULL );

	r = db_select_article( FrmMain->db, &art, d->CODXDEP, a->REFXART );
	if( r < 0 )
		goto LBL_FAIL;
	else if( r == 0 )
	{
		show_info( "L'article a été supprimé");
        this->rechercher();
		return;
    }

	FrmArticle->init( &art );
	FrmArticle->Position = poMainFormCenter;
	r = FrmArticle->ShowModal();
	if( r != mrOk )
		return;

	// Recharger et sélection l'article
	this->rechercher();
	cnt = this->m_ArticlesVector.size();

	if( (idx < cnt) && (art.ARTXRES == this->m_ArticlesVector[idx]->ARTXRES) )
	{
		this->sg_articles->Row = idx + 1;
		return;
	}

	// Un autre utiilisateur a modifié la table
	for( int i = 0; i < cnt; ++i )
	{
		if( art.ARTXRES == this->m_ArticlesVector[i]->ARTXRES )
		{
			this->sg_articles->Row = i + 1;
			return;
		}
	}
	return;

LBL_FAIL:
	show_last_error();
}
//---------------------------------------------------------------------------

void __fastcall TFrmGestionArticles::Timer1Timer(TObject *Sender)
{
	blink_color_id = ! blink_color_id;
	this->sg_articles->Invalidate();
	this->Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

