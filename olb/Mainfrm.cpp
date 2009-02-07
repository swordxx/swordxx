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

#include "Mainfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


 extern "C" {
#include "online.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __GNUC__
#include <io.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <versekey.h>





TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	
}
//---------------------------------------------------------------------------

int TForm1::ProcessOLB(char *filepath)
{
	long pos, offset;
	int num1, num2, rangemax, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];
	short size, tmp;
	extern struct zonline online;

//	checkparams(argc, argv);
	startflag = 0;

	inckey = key1 = key2 = key3 = "Genesis 1:1";
	olbvnum = 0;

	openfiles();

	num1    = key1.Chapter();
	num2    = key1.Verse();

	DOINIT(filepath);   /* initialize OLB system */

	while(!findbreak(fp, &offset, &num1, &num2, &rangemax, &size)) {
		if (!startflag) {
			startflag = 1;
		}
		else {
			if (num2 < key2.Verse()) {            // new chapter
				if (num1 <= key2.Chapter()) { // new book
					key2.Verse(1);
					key2.Chapter(1);
					key2.Book(key2.Book()+1);
				}
				printf("Found Chapter Break: %d ('%s')\n", num1, (const char *)key2);
				chapoffset = offset;
				chapsize = size;
//				continue;
			}
		}
		key2.Verse(1);
		key2.Chapter(num1);
		key2.Verse(num2);

		key3 = key2;
//		key3 += (rangemax - key3.Verse());

		writeidx(key1, key2, key3, offset, size);
	}
	close(vfp);
	close(cfp);
	close(bfp);
	close(fp);
	DOEND();		// close OLB system
}


/**************************************************************************
 * ENT:	key1	- current location of index
 * 	key2	- minimum keyval for which this offset is valid
 * 	key3	- maximum keyval for which this offset is valid
 */

void TForm1::writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size)
{
	long pos;
	short tmp;

		if (key1.Verse() == 1) {	// new chapter
			if (key1.Chapter() == 1) {	// new book
				pos = lseek(cfp, 0, SEEK_CUR);
				write(bfp, &pos, 4);
				pos = lseek(vfp, 0, SEEK_CUR); /* Book intro (cps) */
				write(cfp, &pos, 4);
				write(vfp, &chapoffset, 4);  /* Book intro (vss)  set to same as chap for now(it should be chap 1 which usually contains the book into anyway)*/
				write(vfp, &chapsize, 2);
			}
			pos = lseek(vfp, 0, SEEK_CUR);
			write(cfp, &pos, 4);
			write(vfp, &chapoffset, 4);  /* Chapter intro */
			write(vfp, &chapsize, 2);
		}
		if (key1 >= key2) {
			write(vfp, &offset, 4);
			write(vfp, &size, 2);
		}
		else	{
			pos = 0;
			tmp = 0;
			write(vfp, &pos, 4);
			write(vfp, &tmp, 2);
		}
		key1++;
}



char TForm1::findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size)
{
	char buf[7];
	char buf2[20];
	char ch;
	char loop;
	long offset2;
	int ch2, vs2, rm2;
	bool flag;
	long chapstart = 0; 
	short versedata[MAX_VERSE];
	char data[MAX_DATA];


	if (++olbvnum <= 31102) {

		if (olbvnum == 23146) {	// "Matthew 1:1"	
			close(vfp);
			close(cfp);
			close(bfp);
			close(fp);
			key1 = key2 = key3 = inckey = "Matthew 1:1";
			openfiles();
			startflag = 0;
		}


		*offset = lseek(fp, 0, SEEK_CUR);

		if ((olbvnum!=1) && (olbvnum != 23146))
			inckey++;
			
		*num1 = inckey.Chapter();
		*num2 = inckey.Verse();
	
	// output verse from olb
//		printf("%s\n", (const char *)inckey);
		GETEXT(olbvnum, versedata, 1, 1, 1, 1);
		BLDVERSE(versedata, data);
		charsetconvert(data);
		write(fp, data, strlen(data));

		*size = lseek(fp, 0, SEEK_CUR) - *offset;
		write(fp, "\n", 1);
		return 0;
	}
	return 1;
}

void TForm1::charsetconvert(char *data)
{
	for (;*data; data++) {
//		printf("%c %d", *data, *data);
		switch((unsigned char)*data) {
		case 129: *data = 252; break;
		case 130: *data = 233; break;
		case 131: *data = 226; break;
		case 133: *data = 224; break;
		case 135: *data = 231; break;
		case 136: *data = 234; break;
		case 137: *data = 235; break;
		case 138: *data = 232; break;
		case 139: *data = 239; break;
		case 140: *data = 238; break;
		case 144: *data = 201; break;
		case 147: *data = 244; break;
		case 150: *data = 251; break;
		case 151: *data = 249; break;
		case 160: *data = 225; break;
		case 161: *data = 237; break;
		case 162: *data = 243; break;
		case 163: *data = 250; break;
		case 164: *data = 241; break;
		case 168: *data = 191; break;
		case 173: *data = 161; break;
		default:
			if ((unsigned char)*data > 126)
				fprintf(stderr, "Suspicious character: %c (%d); %s\n", *data, (unsigned char )*data, (const char *)inckey);
			break;
		}
	}
}


void TForm1::openfiles()
{
	char buf[255];
	char fname[5];
	long pos;
	short size;
	
	testmnt = key1.Testament();

	strcpy(fname, (testmnt==2) ? "nt" : "ot");
	unlink(fname);
	if ((fp = open(fname, O_CREAT|O_RDWR|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", fname);
		MessageBox(this->Handle, buf, "openfiles()", MB_OK);
		exit(1);
	}

	sprintf(buf, "%s.vss", fname);
	unlink(buf);
	if ((vfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", fname);
	unlink(buf);
	if ((cfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", fname);
	unlink(buf);
	if ((bfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	pos  = 0;
	write(bfp, &pos, 4);  /* Book    offset for testament intros */
	pos = 4;
	write(cfp, &pos, 4);  /* Chapter offset for testament intro */


/*	Right now just zero out intros until parsing correctly */
	pos = 0;
	size = 0;
	write(vfp, &pos, 4);  /* Module intro */
	write(vfp, &size, 2);
	write(vfp, &pos, 4);  /* Testament intro */
	write(vfp, &size, 2);

}


void TForm1::checkparams(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s <path to OLB module directory (e.g. \"G:\\BIBLE\\AV\")>\n", argv[0]);
		exit(1);
	}
}
void __fastcall TForm1::FSButtonClick(TObject *Sender)
{
	FSDialog->FileName = "Text.dat";
	char *fname;

	if (FSDialog->Execute()) {
		fname = new char [ strlen(FSDialog->FileName.c_str()) + 1 ];
		strcpy(fname, FSDialog->FileName.c_str());
		for (int i = strlen(fname) - 1; i; i--) {
			if ((fname[i] != '\\') && (fname[i] != '/'))
				fname[i] = 0;
			else break;
		}
		FilePathEdit->Text = fname;
		delete [] fname;
		try {
			RichEdit1->Lines->LoadFromFile(FilePathEdit->Text + "Copyrite.dat");
		}
		catch (...) {}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	ProcessOLB(FilePathEdit->Text.c_str());	
}
//---------------------------------------------------------------------------
