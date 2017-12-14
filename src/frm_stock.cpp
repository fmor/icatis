//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <assert.h>

#include "frm_stock.h"
#include "frm_main.h"

#include "db/db.h"
#include "util.h"
#include "error.h"
#include "config.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define NODE_LEVEL_DEPOSANT	0
#define NODE_LEVEL_MAGASIN  1
#define NODE_LEVEL_ZONE		2

#define COL_REFXART 0
#define	COL_LIBXART 1
#define COL_ADRXDAR 2
#define COL_ZONXDAR 3
#define COL_QTEXDAR 4
#define COL_NB_C 5
#define COL_SCCXDAR 6
#define COL_LOTXDAR 7
#define COL_DLVXDAR 8
#define COL_DLPXDAR 9

#define STRETCH_COLUMN COL_LIBXART


#define SEARCH_FIELD_CODXREF	0
#define SEARCH_FIELD_CODXADR	1
#define SEARCH_FIELD_ALL		2

TFrmStock *FrmStock;

static void reset_filter( TFrmStock* frm )
{
	TNotifyEvent ev = frm->cb_adresse->OnClick;

	frm->cb_adresse->OnClick   = NULL;
	frm->cb_reference->OnClick = NULL;
	frm->cb_libelle->OnClick   = NULL;
	frm->cb_adresse->Checked   = true;
	frm->cb_libelle->Checked   = true;
	frm->cb_reference->Checked = true;
	frm->cb_adresse->OnClick   = ev;
	frm->cb_libelle->OnClick   = ev;
	frm->cb_reference->OnClick = ev;

}



//---------------------------------------------------------------------------
__fastcall TFrmStock::TFrmStock(TComponent* Owner)
	: TForm(Owner)
{
	this->Timer1->Enabled = false;
	this->Timer1->Interval = DEFAULT_BLINK_INTERVAL;

}
//---------------------------------------------------------------------------
void __fastcall TFrmStock::FormShow(TObject *Sender)
{
	int r;
	std::vector<StockTree*> stocktrees;
	TTreeNode* nodes[2] = { NULL, NULL };

	// Input
	this->cb_recherche_auto->Checked = true;

	this->ed_recherche->Clear();

	// Checkbox
	this->cb_adresse->OnClick = this->cb_click;
	reset_filter( this );




	// Tree Filtres
	// --> ORDER BY <--
	r = db_select_stocktree( FrmMain->db, &stocktrees );
	if( r < 0 )
		goto LBL_FAIL;

	for( int i =0; i < r; ++i )
	{
		StockTree* st = stocktrees[i];
		TTreeNode* n;
		int l;

		// DEPOSANT
		if( (nodes[0] == NULL) || ( strcmp( st->CODXDEP, (char*) nodes[0]->Data) != 0) )
		{
			n = this->tv_filtre->Items->Add( NULL, st->LIBXDEP );

			l = strlen( st->CODXDEP );
			n->Data = malloc( sizeof(char)*l +1 );
			if( n->Data == NULL )
			{
				error_set( ERR_NO_MEMORY );
				goto LBL_FAIL;
			}
			((char*)n->Data)[l] = 0;
			memcpy( n->Data, st->CODXDEP, l );

			nodes[0] = n;
		}
		// MAGASIN
		if( (nodes[1] == NULL) || ( strcmp( st->CODXMAG, (char*) nodes[1]->Data) != 0) )
		{
			n = this->tv_filtre->Items->AddChild( nodes[0], st->LIBXMAG );
			l = strlen( st->CODXMAG );
			n->Data = malloc( sizeof(char)*l+1 );
			if( n->Data == NULL )
			{
				error_set( ERR_NO_MEMORY );
				goto LBL_FAIL;
			}
            ((char*)n->Data)[l] = 0;
			memcpy( n->Data, st->CODXMAG, l );
			nodes[1] = n;
		}

		// Zone
		n = this->tv_filtre->Items->AddChild( nodes[1], st->LIBXZON );
		l = strlen( st->LIBXZON );
		n->Data = malloc( sizeof(char)*l+1 );
		if( n->Data == NULL )
		{
			error_set( ERR_NO_MEMORY );
			goto LBL_FAIL;
		}
		((char*) n->Data)[l] = 0;
		memcpy( n->Data, st->CODXZON, l );
	}

	for( int i =0; i < this->tv_filtre->Items->Count; ++i )
		this->tv_filtre->Items->Item[i]->Expand(true);

	this->tv_filtre->ReadOnly = true;





	// Grille
	stringgrid_defaults_options( this->sg_stocklines );
	this->sg_stocklines->ColCount = 10;
	this->sg_stocklines->RowCount = 1;
	this->sg_stocklines->FixedCols = 0;
	this->sg_stocklines->ColWidths[COL_REFXART] = 100;
	this->sg_stocklines->ColWidths[COL_LIBXART] = 150;
	this->sg_stocklines->ColWidths[COL_ADRXDAR] = 50;
	this->sg_stocklines->ColWidths[COL_ZONXDAR] = 100;
	this->sg_stocklines->ColWidths[COL_QTEXDAR] = 50;
	this->sg_stocklines->ColWidths[COL_NB_C] = 40;
	this->sg_stocklines->ColWidths[COL_SCCXDAR] = 130;
	this->sg_stocklines->ColWidths[COL_LOTXDAR] = 60;
	this->sg_stocklines->ColWidths[COL_DLVXDAR] = 80;
	this->sg_stocklines->ColWidths[COL_DLPXDAR] = 80;



	// Status bar
	this->sb_form->SimplePanel = true;


	//
	this->tv_filtre->Selected = NULL;
	this->tv_filtreChange( NULL, NULL );

	return;

LBL_FAIL:
	show_error( error_last() );
	this->ModalResult = mrAbort;
	form_post_close_message( this );
}
//---------------------------------------------------------------------------
void __fastcall TFrmStock::FormClose(TObject *Sender, TCloseAction &Action)
{
	this->clear();

    // Filtres
	for( int i = 0; i < this->tv_filtre->Items->Count; ++i )
		free( this->tv_filtre->Items->Item[i]->Data );
	this->tv_filtre->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmStock::sg_stocklinesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	if( (ARow == 0) && (this->sg_stocklines->Objects[0][0] == NULL) )
	{
		this->sg_stocklines->Cells[COL_REFXART][0] = "Référence";
		this->sg_stocklines->Cells[COL_LIBXART][0] = "Libellé";
		this->sg_stocklines->Cells[COL_ADRXDAR][0] = "Adresse";
		this->sg_stocklines->Cells[COL_ZONXDAR][0] = "Zone";
		this->sg_stocklines->Cells[COL_QTEXDAR][0] = "Quantité";
		this->sg_stocklines->Cells[COL_NB_C][0]    = "Cond.";
		this->sg_stocklines->Cells[COL_SCCXDAR][0] = "SSEE";
		this->sg_stocklines->Cells[COL_LOTXDAR][0] = "Lot";
		this->sg_stocklines->Cells[COL_DLVXDAR][0] = "DLV";
		this->sg_stocklines->Cells[COL_DLPXDAR][0] = "DLP";

		this->sg_stocklines->Objects[0][0] = (TObject*) 1;
	}
	else if( ARow && (this->sg_stocklines->Objects[0][ARow] == NULL) )
	{
		StockLine* sl = this->m_StockLineVector[ARow-1];
		this->sg_stocklines->Cells[COL_REFXART][ARow] = sl->REFXART;
		this->sg_stocklines->Cells[COL_LIBXART][ARow] = sl->LIBXART;
		this->sg_stocklines->Cells[COL_ADRXDAR][ARow] = sl->CODXADR;
		this->sg_stocklines->Cells[COL_ZONXDAR][ARow] = sl->LIBXZON;
		this->sg_stocklines->Cells[COL_QTEXDAR][ARow] = sl->QTEXDAR;
		this->sg_stocklines->Cells[COL_NB_C][ARow] = sl->NB_C;
		this->sg_stocklines->Cells[COL_SCCXDAR][ARow] = sl->SCCXDAR;
		this->sg_stocklines->Cells[COL_LOTXDAR][ARow] = sl->LOTXDAR;

		char buffer[64];
		struct tm t;

		if( sl->DLVXDAR.timestamp_date )
		{
			isc_decode_timestamp( &sl->DLVXDAR, &t );
//			sprintf( buffer, "%04d-%02d-%02d %02d:%02d:%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec  );
			sprintf( buffer, "%04d-%02d-%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday );

			this->sg_stocklines->Cells[COL_DLVXDAR][ARow] = buffer;
		}
		else
			this->sg_stocklines->Cells[COL_DLVXDAR][ARow] = "";

		if( sl->DLPXDAR.timestamp_date )
		{
			isc_decode_timestamp( &sl->DLPXDAR, &t );
//			sprintf( buffer, "%04d-%02d-%02d %02d:%02d:%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec  );
			sprintf( buffer, "%04d-%02d-%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday);
			this->sg_stocklines->Cells[COL_DLPXDAR][ARow] = buffer;
		}
		else
			this->sg_stocklines->Cells[COL_DLPXDAR][ARow] = "";


		this->sg_stocklines->Objects[0][ARow] = (TObject*) 1;
	}


	if( ARow == 0 )
		return;

	StockLine* sl = this->m_StockLineVector[ARow-1];
	switch( ACol )
	{
		case COL_DLPXDAR :
		if( sl->DLVXART && (sl->DLPXDAR.timestamp_date == 0)  )
		{
			stringgrid_drawcell( this->sg_stocklines, Rect, ACol, ARow, blink_colors[ blink_color_id], "Invalid" );
            this->Timer1->Enabled = true;
        }
		break;

		case COL_DLVXDAR :
		if( sl->DLVXART && (sl->DLVXDAR.timestamp_date == 0)  )
		{
			stringgrid_drawcell( this->sg_stocklines, Rect, ACol, ARow, blink_colors[ blink_color_id], "Invalid" );
            this->Timer1->Enabled = true;
        }
		break;



		case COL_LOTXDAR :
		if( sl->LOTXART && (sl->LOTXDAR[0] == 0) )
		{
			stringgrid_drawcell( this->sg_stocklines, Rect, ACol, ARow, blink_colors[ blink_color_id], "Invalid" );
			this->Timer1->Enabled = true;

        }
		break;
	}









}
//---------------------------------------------------------------------------
void __fastcall TFrmStock::tv_filtreChange(TObject *Sender, TTreeNode *Node)
{
	bool b;

//	show_info( "tv_filtreChange" );


	b = (Node != NULL) && (Node->Level != NODE_LEVEL_DEPOSANT);

	this->lbl_recherche->Enabled     = b;
	this->ed_recherche->Enabled      = b;
	this->btn_rechercher->Enabled    = b;
	this->cb_adresse->Enabled        = b;
	this->cb_libelle->Enabled		 = b;
	this->cb_reference->Enabled      = b;
	this->btn_effacer->Enabled       = b;
	this->cb_recherche_auto->Enabled = b;
	this->sg_stocklines->Enabled     = b;

	if( b )
		this->rechercher();
	else
		this->clear();
}
//---------------------------------------------------------------------------


void TFrmStock::clear()
{
	this->sg_stocklines->Cols[0]->Clear();
	this->sg_stocklines->RowCount = 1;

	const int sz = this->m_StockLineVector.size();
	for( int i = 0; i < sz; ++i )
		free( this->m_StockLineVector[i] );
	this->m_StockLineVector.clear();

	this->sb_form->SimpleText = "";

}
void __fastcall TFrmStock::btn_rechercherClick(TObject *Sender)
{
	this->rechercher();
}
void TFrmStock::rechercher()
{
	char buffer[256];
	int r;
	TTreeNode* n;
	AnsiString s;
	char* CODXDEP;
	char* CODXMAG;
	char* CODXZON;
	char* CODXREF;
	char* CODXADR;
	char* LIBXART;

	int NB[3];


	this->clear();


	n = this->tv_filtre->Selected;

	if( n == NULL )
		return;

	switch( n->Level )
	{
		case NODE_LEVEL_MAGASIN:
			CODXDEP = (char*) n->Parent->Data;
			CODXMAG = (char*) n->Data;
			CODXZON = NULL;
			break;

		case NODE_LEVEL_ZONE:
			CODXDEP = (char*) n->Parent->Parent->Data;
			CODXMAG = (char*) n->Parent->Data;
			CODXZON = (char*) n->Data;
			break;

		default:
			assert(0);
	}




	s = this->ed_recherche->Text.Trim();

	CODXADR = NULL;
	CODXREF = NULL;
	LIBXART = NULL;

	if( s.Length() )
	{
		if( this->cb_adresse->Checked )
			CODXADR = s.c_str();
		if( this->cb_reference->Checked )
			CODXREF = s.c_str();
		if( this->cb_libelle->Checked )
			LIBXART = s.c_str();
	}



	r = db_select_stockline( FrmMain->db, &this->m_StockLineVector, CODXDEP, CODXMAG, CODXZON, CODXREF, CODXADR, LIBXART );
	if( r < 0 )
	{
		show_last_error();
		r = 0;
	}

	this->sg_stocklines->RowCount = r +  1;
	if( r )
		this->sg_stocklines->FixedRows = 1;
	if( r == 1 )
        this->sg_stocklines->Row = 1;


	// Status Info
	NB[0] = 0;
	NB[1] = 0;
	NB[2] = 0;
	for( int i = 0; i < r; ++i )
	{
		StockLine* sl = this->m_StockLineVector[i];
		NB[0] += sl->NB_C;
		NB[1] += sl->NB_S;
		NB[2] += sl->QTEXDAR;
	}


	snprintf( buffer, 1024, "Lignes : %d, Cartons : %d, Sachets : %d, Unités : %d", r, NB[0], NB[1], NB[2] );
	this->sb_form->SimpleText = buffer;


}
//---------------------------------------------------------------------------

void __fastcall TFrmStock::btn_effacerClick(TObject *Sender)
{
	this->ed_recherche->Clear();

	// Checkbox
	reset_filter( this );
}
//---------------------------------------------------------------------------

void __fastcall TFrmStock::ed_rechercheChange(TObject *Sender)
{
	if( this->cb_recherche_auto->Checked )
		this->rechercher();
}
//---------------------------------------------------------------------------

void __fastcall TFrmStock::ed_rechercheKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if( Key == VK_RETURN )
		this->rechercher();

}
//---------------------------------------------------------------------------




void __fastcall TFrmStock::cb_click(TObject *Sender)
{
	if( (this->cb_adresse->Checked == false ) && (this->cb_reference->Checked == false) && (this->cb_libelle->Checked == false) )
	{
		assert( Sender );
		TCheckBox* cbs[3] = { this->cb_adresse, this->cb_libelle, this->cb_reference };
		checkbox_at_least_one_checked( cbs, 2, (TCheckBox*) Sender);
	}
	else if( this->cb_recherche_auto->Checked )
		this->rechercher();
}
//---------------------------------------------------------------------------


void __fastcall TFrmStock::FormResize(TObject *Sender)
{
	stringgrid_stretch_col( this->sg_stocklines, STRETCH_COLUMN );
}
//---------------------------------------------------------------------------

void __fastcall TFrmStock::Timer1Timer(TObject *Sender)
{
    blink_color_id = ! blink_color_id;
	this->Timer1->Enabled = false;
	this->sg_stocklines->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmStock::btn_fermerClick(TObject *Sender)
{
	this->ModalResult = mrOk;
}

