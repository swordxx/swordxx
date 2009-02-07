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

#include "prn2swordfrm.h"
#include <stdio.h>
#include <fcntl.h>
#include <versekey.h>
#include <io.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
const MAXVLEN = 8192;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FSButtonClick(TObject *Sender)
{
	FSDialog->FileName = "*.rtf";
	char *fname;

	if (FSDialog->Execute()) {
		FilePathEdit->Text = FSDialog->FileName;
	}
}
//---------------------------------------------------------------------------


void TForm2::ProcessFile(char *fileName)
{
	unsigned long pos = 0L;
	unsigned short size = 0, tmp;
	int verseNum, curbook = 0, curchap = 0, curverse = 0, curTestament = 1;
	char buf[127];
	VerseKey mykey;
	char verseBuf[MAXVLEN];
	bool fail = false;
	char end = 0;
	

	if ((fp = open(fileName, O_RDONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", fileName);
		MessageBox(this->Handle, buf, "ProcessFile", MB_OK);
		return;
	}

	openFiles("ot");
	
	mykey = "Genesis 1:1";
	
	getVerse(fp, &verseNum, verseBuf, 1);
	do {
		end = getVerse(fp, &verseNum, verseBuf, mykey.Testament());
		if (mykey.Verse() == 1) {  		/* if we're at a new chapter */
			if (mykey.Chapter() == 1) {	/* if we're at a new book */
				if (mykey.Testament() != curTestament) { // we've made it to new testament
					curTestament = mykey.Testament();
					close(vfp);	// verse
					close(cfp);	// chapter
					close(bfp);	// book
					close(dfp);	// data
					openFiles("nt");
				}
				Memo1->Text = "";
				pos = lseek(cfp, 0, SEEK_CUR);
				write(bfp, &pos, 4);
				pos = lseek(vfp, 0, SEEK_CUR); /* Book intro (cps) */
				write(cfp, &pos, 4);
				pos  = 0L;
				tmp = 0;
				write(vfp, &pos, 4);  /* Book intro (vss) */
				write(vfp, &tmp, 2);
				curbook++;
				curchap = 0;
			}
			pos = lseek(vfp, 0, SEEK_CUR);
			write(cfp, &pos, 4);
			curverse = 1;
			pos  = 0L;
			tmp = 0;
			write(vfp, &pos, 4);  /* Chapter intro */
			write(vfp, &tmp, 2);
			curchap++;
		}
		else curverse++;
	
		if (mykey.Chapter() != curchap) {
			sprintf(buf, "Error: Found chaptures out of sequence");
			Memo1->Lines->Append(buf);
			fail = true;
			break;
		}
		if ((mykey.Verse() != curverse) || (curverse != verseNum)) {
			Memo1->Lines->Append("Error: Found verses out of sequence");
			sprintf(buf, "Expected %d (%s), but got %d\n\n", curverse, (const char *)mykey, verseNum);
			Memo1->Lines->Append(buf);
			Memo1->Lines->Append("   Currently SWORD only supports KJV verse numbering and book ordering.");
			Memo1->Lines->Append("   Try combining extra verses with the last valid verse just before, including it in footnote markers <RF><Rf>(eg. <RF>(24) verse text. (25) verse text<Rf>)");
			Memo1->Lines->Append("   This way, even though they will display in SWORD along with the previous verse, all data will be displayed.");
			Memo1->Lines->Append("   Where verses are missing, add a blank entry.");
			Memo1->Lines->Append("   Then try rerunning");
			fail = true;
			break;
		}
		
		pos = lseek(dfp, 0, SEEK_CUR);
		write(vfp, &pos, 4);
		size = strlen(verseBuf);
		write(vfp, &size, 2);
		sprintf(buf, "%2d:%3d:%3d (%s) found at offset: %7ld; size: %d", curbook, mykey.Chapter(), mykey.Verse(), (const char *)mykey, pos, size);
		Memo1->Lines->Append(buf);
		if ((size < 0) || (size > 1000)) {
			Memo1->Lines->Append("WARNING: Strange Size.");
		}
		write(dfp, verseBuf, size);
		mykey++;
	} while (!end);

	close(vfp);	// verse
	close(cfp);	// chapter
	close(bfp);	// book
	close(dfp);	// data
	close(fp);	// source
	if (!fail)
		Memo1->Lines->Append("SUCCESS!!!!!!!");
}


char TForm2::getVerse(int fp, int *verseNum, char *verseBuf, char testament)
{
	char buf[21];
	char retVal = 0;
	*verseNum=-1;
	char *finalBuf = verseBuf;
	int paridx;
	const char *parchar = "\\par";

	memset(buf, 0, 21);

	while (1) {

		if (!memcmp(buf, "\\f1 \\b0 \\fs22  \\f", 17)) {
                        if (isdigit(buf[18])) {
                                lseek(fp, -1, SEEK_CUR);
                                break;
                        }
                        else if (isdigit(buf[17])) {
	        		lseek(fp, -2, SEEK_CUR);
        			break;
                        }
		}
		if (!memcmp(buf, "\\f1 \\b0 \\fs22 ", 14)) {
			lseek(fp, -7, SEEK_CUR);
			break;
		}
		if (!memcmp(buf, "\\li0\\fi0 \\par \\u", 16)) {
			lseek(fp, -4, SEEK_CUR);
                        while(read(fp, buf, 1) == 1) {
				if (*buf == 10)
					break;
			}
			memset(buf, 0, 21);
		}
		memmove(buf, &buf[1], 20);
		if (*buf) {
			*verseBuf++ = *buf;
			if (*verseNum == -1) {
				if (isdigit(*buf)) {
					sscanf(buf, "%d", verseNum);
				}
			}
		}
		if (read(fp, &buf[20], 1) != 1) {
			retVal = 1;
			for (int i = 1; i < 12; i++)
				*verseBuf++ = buf[i];
			break;
		}
	}
	*--verseBuf = 0;
	paridx = 3;
	// trim trailing junk
	do {
		verseBuf--;
		if (paridx > -1) {
			if (*verseBuf == parchar[paridx]) {
				*verseBuf = 0;
				paridx--;
				continue;
			}
			if (paridx < 3) {
				for (;paridx < 4;paridx++)
					*verseBuf++ = parchar[paridx];
				break;
			}
		}
		if ((*verseBuf != ' ') && (*verseBuf != 10) && (*verseBuf != 13) && (*verseBuf != '\\'))
			break;
		*verseBuf = 0;
	} while (verseBuf > finalBuf);
	*++verseBuf = 0;
	
	// check for Paragraph marker on next verse
	for (int i = 7; i < 48; i++) {
		if (buf[i] == '¶') {
			*verseBuf++ = '<';
			*verseBuf++ = 'C';
			*verseBuf++ = 'M';
			*verseBuf++ = '>';
			*verseBuf++ = 0;
			break;
		}
	}

	filterVerse(finalBuf, (testament == 2)?'G':'H');
	
	return retVal;
}







// lang	- 'G' | 'H'
void TForm2::filterVerse(char *text, char lang)
{
	char *to, *from, token[MAXVLEN], tokpos = 0;
	bool intoken = false;
	bool intoken2 = false;
	bool deref = false;
	bool lastspace = false;
	int len;
	int maxlen = MAXVLEN - 1;
	

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------

	while (*from) {
		if ((*from != ' ') && (!isdigit(*from)) && (*from != 10) && (*from !=13))
			break;
		from++;
	}
	
	for (to = text; *from; from++) {
		if ((*from == '\\') && (!deref)) {
			deref = true;
			continue;
		}
		if (*from == '<') {
			if (!isdigit(from[1])) {	// Not a strongs number
				if ((from[1] != 'C') || (from[2] != 'M') || (from[3] != '>')) {
					*to++ = '<';
					*to++ = 'F';
					*to++ = 'O';
					*to++ = '>';
					continue;
				}
			}
		}
		if (*from == '>') {
			if (!isdigit(from[-1])) {	// Not a strongs number
				if ((from[-2] != 'C') || (from[-1] != 'M') || (from[-3] != '<')) {
					*to++ = '<';
					*to++ = 'F';
					*to++ = 'o';
					*to++ = '>';
					continue;
				}
			}
		}
		if (deref) {
			deref = false;
			if (*from == '{') {
				*to++ = '<';
				*to++ = 'R';
				*to++ = 'F';
				*to++ = '>';
				continue;
			}
			
			if (*from == '}') {
				*to++ = '<';
				*to++ = 'R';
				*to++ = 'f';
				*to++ = '>';
                                if (from[1] == '\\' && from[2] == 'i' && from[3] == '0') {
                                        from += 3;
					if (from[1] == ' ') {
						if (lastspace)
							from++;
					}
                                }
				continue;
			}
			if (*from == 'i') {
				if (from[1] == '1') {
                                        if (from[2] == ' ' && from[3] == '\\' && from[4] == '{') {
                                                from++;
						if (lastspace)
							from++;
        					continue;
                                        }
					*to++ = '<';
					*to++ = 'F';
					*to++ = 'I';
					*to++ = '>';
					from++;
					if (from[1] == ' ') {
						if (lastspace)
							from++;
					}
					continue;
				}
				if (from[1] == '0') {
					*to++ = '<';
					*to++ = 'F';
					*to++ = 'i';
					*to++ = '>';
					from++;
					if (from[1] == ' ') {
						if (lastspace)
							from++;
					}
					if ((from[1] == ' ') && (from[2] == ' ')){	// dorky, but OLB places 2 spaces after italics off
						from++;
					}
					continue;
				}
			}
                        if (*from == 'f' && from[1] == 's') {
                                from++;
                                while (isdigit(from[1])) {
                                        from++;
                                }
				if (from[1] == ' ') {
                                        if (lastspace)
        					from++;
				}
                                continue;
                        }
			*to++ = '\\';
			*to++ = *from;
			lastspace = (*from == ' ');
			continue;
		}
			
		if (*from == '<') {
			if (isdigit(from[1])) {
				intoken = true;
				tokpos = 0;
				memset(token, 0, 20);
				continue;
			}
		}
		if (*from == '(') {
			if (isdigit(from[1])) {
				intoken2 = true;
				tokpos = 0;
				memset(token, 0, 20);
				continue;
			}
		}
		if ((*from == '>') && (intoken)) {
			intoken = false;
			if (*(to - 1) == ' ')
				to--;
			*to++ = '<';
			*to++ = 'W';
			*to++ = lang;
			for (int i = 0; i < strlen(token); i++)
				*to++ = token[i];
			*to++ = '>';
			continue;
		}
		if ((*from == ')') && (intoken2)) {
			intoken2 = false;
			if (*(to - 1) == ' ')
				to--;
			*to++ = '<';
			*to++ = 'W';
			*to++ = 'T';
			*to++ = lang;
			for (int i = 0; i < strlen(token); i++)
				*to++ = token[i];
			*to++ = '>';
			continue;
		}
		
		if ((*from == 13) || (*from == 10)) {
			continue;
		}

		if (*from == '¶') {
                        if (from[1] == ' ') {
                                from++;
                        }
			continue;
		}

		if (intoken || intoken2)
			token[tokpos++] = *from;
		else
                	*to++ = *from;
		lastspace = (*from == ' ');
	}
	*to-- = 0;
	*++to = '\n';
	*++to = 0;
}


void TForm2::openFiles(char *fileName)
{
	char buf[128];
	long pos;
	short size;

	unlink(fileName);
	if ((dfp = open(fileName, O_CREAT|O_WRONLY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", fileName);
		MessageBox(this->Handle, buf, "ProcessFile", MB_OK);
		return;
	}

	sprintf(buf, "%s.vss", fileName);
	unlink(buf);
	if ((vfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		MessageBox(this->Handle, buf, "ProcessFile", MB_OK);
		return;
	}

	sprintf(buf, "%s.cps", fileName);
	unlink(buf);
	if ((cfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		MessageBox(this->Handle, buf, "ProcessFile", MB_OK);
		return;
	}

	sprintf(buf, "%s.bks", fileName);
	unlink(buf);
	if ((bfp = open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		MessageBox(this->Handle, buf, "ProcessFile", MB_OK);
		return;
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

void __fastcall TForm2::Button1Click(TObject *Sender)
{
	ProcessFile(FilePathEdit->Text.c_str());	
}
//---------------------------------------------------------------------------


