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
#ifndef MainfrmH
#define MainfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <versekey.h>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *FSDialog;
	TPanel *Panel1;
	TPanel *Panel2;
	TButton *Button1;
	TLabel *Label1;
	TEdit *FilePathEdit;
	TButton *FSButton;
	TRichEdit *RichEdit1;
	void __fastcall FSButtonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations

void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size);
char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size);
void openfiles();
void checkparams(int argc, char **argv);
void charsetconvert(char *data);
int ProcessOLB(char *filepath);


VerseKey key1, key2, key3;
int fp, vfp, cfp, bfp;
long chapoffset;
short chapsize;
char testmnt;
char startflag;
VerseKey inckey;
int olbvnum;

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
