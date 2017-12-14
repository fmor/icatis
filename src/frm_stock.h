//---------------------------------------------------------------------------

#ifndef frm_stockH
#define frm_stockH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>

#include <Vcl.ExtCtrls.hpp>

#include "db/stock.h"


//---------------------------------------------------------------------------
class TFrmStock : public TForm
{
__published:	// Composants gérés par l'EDI
	TStatusBar *sb_form;
	TLabel *lbl_recherche;
	TEdit *ed_recherche;
	TButton *btn_rechercher;
	TStringGrid *sg_stocklines;
	TTreeView *tv_filtre;
	TButton *btn_effacer;
	TCheckBox *cb_recherche_auto;
	TCheckBox *cb_reference;
	TCheckBox *cb_adresse;
	TCheckBox *cb_libelle;
	TTimer *Timer1;
	TButton *btn_fermer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall sg_stocklinesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall tv_filtreChange(TObject *Sender, TTreeNode *Node);
	void __fastcall btn_rechercherClick(TObject *Sender);
	void __fastcall btn_effacerClick(TObject *Sender);
	void __fastcall ed_rechercheChange(TObject *Sender);
	void __fastcall ed_rechercheKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);

    void __fastcall cb_click( TObject* cb );
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall btn_fermerClick(TObject *Sender);


private:	// Déclarations utilisateur
	std::vector<StockLine*> m_StockLineVector;

	void rechercher();
	void clear();


public:		// Déclarations utilisateur
	__fastcall TFrmStock(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmStock *FrmStock;
//---------------------------------------------------------------------------
#endif
