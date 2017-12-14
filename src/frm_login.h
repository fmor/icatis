//---------------------------------------------------------------------------

#ifndef frm_loginH
#define frm_loginH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrmLogin : public TForm
{
__published:	// Composants g�r�s par l'EDI
	TLabel *lbl_login;
	TEdit *ed_username;
	TLabel *lbl_password;
	TEdit *ed_password;
	TOpenDialog *OpenDialog1;
	TLabel *lbl_bdd;
	TEdit *ed_database;
	TButton *btn_select_bdd;
	TLabel *lbl_title;
	TButton *btn_login;
	TButton *btn_quit;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btn_quitClick(TObject *Sender);
	void __fastcall btn_loginClick(TObject *Sender);
	void __fastcall btn_select_bddClick(TObject *Sender);
private:	// D�clarations utilisateur
public:		// D�clarations utilisateur
	__fastcall TFrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmLogin *FrmLogin;
//---------------------------------------------------------------------------
#endif
