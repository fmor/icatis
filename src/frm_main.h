//---------------------------------------------------------------------------

#ifndef frm_mainH
#define frm_mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <Vcl.ComCtrls.hpp>


#include "db/db.h"


//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *btn_articles;
	TButton *btn_stock;
	TStatusBar *sb;
	TButton *btn_quitter;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btn_articlesClick(TObject *Sender);
	void __fastcall btn_stockClick(TObject *Sender);
	void __fastcall btn_quitterClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TFrmMain(TComponent* Owner);


	DB* db;

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
