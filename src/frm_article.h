//---------------------------------------------------------------------------

#ifndef frm_articleH
#define frm_articleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------


#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>

#include "db/article.h"

class TFrmArticle : public TForm
{
__published:	// Composants gérés par l'EDI
	TLabel *lbl_deposant;
	TLabel *lbl_reference;
	TLabel *lbl_libelle;
	TLabel *lbl_lot;
	TLabel *lbl_dlv;
	TComboBox *cb_deposant;
	TEdit *ed_reference;
	TEdit *ed_libelle;
	TCheckBox *chk_lot;
	TCheckBox *chk_dlv;
	TGroupBox *gb_palette;
	TLabel *lbl_p_dimensions;
	TLabel *lbl_p_quantite;
	TLabel *lbl_p_poids;
	TEdit *ed_p_largeur;
	TEdit *ed_p_hauteur;
	TEdit *ed_p_profondeur;
	TEdit *ed_p_quantite;
	TLabel *lbl_p_x1;
	TLabel *lbl_p_x2;
	TButton *btn_valider;
	TButton *btn_annuler;
	TEdit *ed_p_poids;
	TLabel *lbl_p_libelle;
	TEdit *ed_p_libelle;
	TGroupBox *gb_contionnement;
	TLabel *lbl_c_dimensions;
	TLabel *lbl_c_quantite;
	TLabel *lbl_c_poids;
	TLabel *lbl_c_x1;
	TLabel *lbl_c_x2;
	TLabel *lbl_c_libelle;
	TEdit *ed_c_largeur;
	TEdit *ed_c_hauteur;
	TEdit *ed_c_profondeur;
	TEdit *ed_c_quantite;
	TEdit *ed_c_poids;
	TEdit *ed_c_libelle;
	TGroupBox *gb_sousconditionnement;
	TLabel *lbl_s_dimensions;
	TLabel *lbl_s_quantite;
	TLabel *lbl_s_poids;
	TLabel *lbl_s_x1;
	TLabel *lbl_s_x2;
	TLabel *lbl_s_libelle;
	TEdit *ed_s_largeur;
	TEdit *ed_s_hauteur;
	TEdit *ed_s_profondeur;
	TEdit *ed_s_quantite;
	TEdit *ed_s_poids;
	TEdit *ed_s_libelle;
	TGroupBox *gb_unité;
	TLabel *lbl_u_dimensions;
	TLabel *lbl_u_quantite;
	TLabel *lbl_u_poids;
	TLabel *lbl_u_x1;
	TLabel *lbl_u_x2;
	TLabel *lbl_u_libelle;
	TEdit *ed_u_largeur;
	TEdit *ed_u_hauteur;
	TEdit *ed_u_profondeur;
	TEdit *ed_u_quantite;
	TEdit *ed_u_poids;
	TEdit *ed_u_libelle;
	TLabel *lbl_nbjour;
	TEdit *ed_nbjour;
	TActionList *ActionList1;
	TAction *Action_article_enregistrer;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btn_annulerClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall chk_dlvClick(TObject *Sender);
	void __fastcall Action_article_enregistrerExecute(TObject *Sender);


private:	// Déclarations utilisateur

	Article* m_Article;
	bool     m_ReadOnly;
	TEdit*   m_CondEdits[4][6];

	int validate();

public:		// Déclarations utilisateur
	__fastcall TFrmArticle(TComponent* Owner);


	void init( Article* article );

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmArticle *FrmArticle;
//---------------------------------------------------------------------------
#endif
