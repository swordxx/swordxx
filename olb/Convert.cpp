//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Convert.res");
USEFORM("Mainfrm.cpp", Form1);
USEUNIT("all.c");
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
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
