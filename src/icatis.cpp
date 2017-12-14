//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


#include "error.h"
#include "util.h"


//---------------------------------------------------------------------------
USEFORM("frm_article.cpp", FrmArticle);
USEFORM("frm_gestion_articles.cpp", FrmGestionArticles);
USEFORM("frm_login.cpp", FrmLogin);
USEFORM("frm_main.cpp", FrmMain);
USEFORM("frm_stock.cpp", FrmStock);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFrmLogin), &FrmLogin);
		Application->CreateForm(__classid(TFrmMain), &FrmMain);
		Application->CreateForm(__classid(TFrmGestionArticles), &FrmGestionArticles);
		Application->CreateForm(__classid(TFrmArticle), &FrmArticle);
		Application->CreateForm(__classid(TFrmStock), &FrmStock);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
