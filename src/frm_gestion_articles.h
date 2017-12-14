//---------------------------------------------------------------------------

#ifndef frm_gestion_articlesH
#define frm_gestion_articlesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------


#include <vector>

#include "db/deposant.h"
#include "db/article.h"

class TFrmGestionArticles : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *btn_rechercher;
	TButton *btn_ajouter;
	TButton *btn_supprimer;
	TButton *btn_fermer;
	TStringGrid *sg_articles;
	TStatusBar *sb_form;
	TCheckBox *cb_recherche_auto;
	TButton *btn_effacer;
	TCheckBox *cb_reference;
	TCheckBox *cb_libelle;
	TEdit *ed_recherche;
	TLabel *lbl_recherche;
	TListBox *lb_deposant;
	TActionList *ActionList1;
	TAction *action_articles_supprimer;
	TAction *action_articles_ajouter;
	TPopupMenu *PopupMenu_Articles;
	TAction *action_article_modifier;
	TMenuItem *Crer1;
	TMenuItem *Ouvrir1;
	TMenuItem *Supprimer1;
	TButton *Button1;
	TTimer *Timer1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sg_articlesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btn_fermerClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall lb_deposantClick(TObject *Sender);
	void __fastcall ed_rechercheKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall btn_rechercherClick(TObject *Sender);
	void __fastcall btn_effacerClick(TObject *Sender);
	void __fastcall ed_rechercheChange(TObject *Sender);
	void __fastcall sg_articlesSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall sg_articlesKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall action_articles_supprimerExecute(TObject *Sender);
	void __fastcall action_articles_ajouterExecute(TObject *Sender);
	void __fastcall action_article_modifierExecute(TObject *Sender);


	void __fastcall cb_onclick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);





private:	// Déclarations utilisateur
	int m_DeposantIndex;
	std::vector<Article_par_deposant*> m_ArticlesVector;

	void rechercher();
	void clear();

	Deposant* selected_deposant() const;
	Article_par_deposant* selected_article() const;

public:		// Déclarations utilisateur
	__fastcall TFrmGestionArticles(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGestionArticles *FrmGestionArticles;
//---------------------------------------------------------------------------
#endif
