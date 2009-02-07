/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

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
USEUNIT("..\src\mgr\localemgr.cpp");
USEUNIT("..\src\mgr\swlocale.cpp");
USEUNIT("..\src\mgr\swconfig.cpp");
USEUNIT("..\src\mgr\swmgr.cpp");
USEUNIT("..\src\mgr\filemgr.cpp");
USEUNIT("..\src\utilfuns\roman.c");
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
