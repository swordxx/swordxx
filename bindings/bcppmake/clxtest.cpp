//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("clxtest.res");
USEFORM("MainCLXTest.cpp", Form2);
USEUNIT("..\clx\Sword.pas");
USELIB("sword.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TForm2), &Form2);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
