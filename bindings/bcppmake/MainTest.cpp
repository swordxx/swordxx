//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainTest.h"
extern "C" {
#include <flatapi.h>
}
//using namespace sword;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
	SWHANDLE mgr = SWMgr_new();
	SWHANDLE it = SWMgr_getModulesIterator(mgr);
	SWHANDLE module = 0;
	String modlist;
	do {
		module = ModList_iterator_val(it);
		if (module) {
			modlist += SWModule_getName(module) + (String)"\r\n";
			ModList_iterator_next(it);
		}
	} while (module);
	Memo1->Text = modlist;
	SWMgr_delete(mgr);
}
//---------------------------------------------------------------------------
