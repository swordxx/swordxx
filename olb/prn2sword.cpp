//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("prn2sword.res");
USEFORM("prn2swordfrm.cpp", Form2);
USEUNIT("..\src\keys\versekey.cpp");
USEUNIT("..\src\keys\strkey.cpp");
USEUNIT("..\src\keys\swkey.cpp");
USEUNIT("..\src\keys\listkey.cpp");
USEUNIT("..\src\utilfuns\utilstr.cpp");
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
