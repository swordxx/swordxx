/*****************************************************************************
 *
 *	This code reeks but works (sometimes).  Good luck!
 * Modified for zText purposes
 */

//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//#ifndef __GNUC__
#include <io.h>
//#else
//#include <unistd.h>
//#endif

#include <fcntl.h>
#include <versekey.h>


void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size);
char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, unsigned short *size);
void openfiles(char *fname);
void checkparams(int argc, char **argv);


VerseKey key1, key2, key3;
int fp=0, vfp=0, cfp=0, bfp=0;
long chapoffset=0;
unsigned short chapsize=0;
long bookoffset=0;
unsigned short booksize=0;
long testoffset=0;
unsigned short testsize=0;
long verseoffset=0;
unsigned short versesize=0;
long nextoffset=0;
char testmnt=0;
int deadcount = 0;
int chapmark=-4, bookmark=-1;
ofstream cfile;


int main(int argc, char **argv)
{
	long pos, offset;
	int num1, num2, rangemax;//, curbook = 0, curchap = 0, curverse = 0;
	//char buf[127],
	char startflag = 0;
	unsigned short size;//, tmp;

	checkparams(argc, argv);

	openfiles(argv[1]);
	//key1 = "Matthew 1:1";
	//key2 = "Matthew 1:1";
	//key3 = "Matthew 1:1";

	testmnt = key1.Testament();
	cfile << "testament" << (int) testmnt << "\n";
	num1 = key1.Chapter();
	num2 = key1.Verse();
	pos  = 0;
	write(bfp, &pos, 4);  /* Book    offset for testament intros */
	pos = 4;
	write(cfp, &pos, 4);  /* Chapter offset for testament intro */


/*	Right now just zero out intros until parsing correctly */
	/*pos = 0;
	size = 0;
	write(vfp, &pos, 4);  // Module intro
	write(vfp, &size, 2);
	cfile << "modintro pos{" << pos << "} size{" << size << "}\n";
	write(vfp, &pos, 4);  // Testament intro
	write(vfp, &size, 2);
	cfile << "test intro pos{" << pos << "} size{" << size << "}\n";
	*/
	cout << "GBFIDX Running\n";
	cout.flush();
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
				cfile << "Found Chapter Break: " << num1 << " ('" << (const char *)key2 << "')\n";
				//chapoffset = offset;
				//chapsize = chapsize - size;
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

	return 1;
}


/**************************************************************************
 * writeidx:	key1	- current location of index
 * 	key2	- minimum keyval for which this offset is valid
 * 	key3	- maximum keyval for which this offset is valid
 */

void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size)
{
	long pos;
	unsigned short tmp;

	for (; ((key1 <= key3) && (key1.Error() != KEYERR_OUTOFBOUNDS) && (key1.Testament() == testmnt)); key1+=1) {
		if (chapmark>=2)
		{
			if (bookmark==2)
			{
				//booksize = booksize - chapsize + 7;
				cfile << "versesize " << versesize << " booksize " << booksize << " chapsize " << chapsize << " size " << size << "\n";
				//cfile.flush();
				//assert(chapsize < size);
				//if (chapsize > size)           // At start of Psalms gets chapsize rather than chapsize+size ???
				//{
				//	versesize = versesize - (booksize - (chapsize - size) + 7);
				//}
				//else
				//{
				versesize = versesize - (booksize - (chapsize) + 7);
				//}
					cfile << "Last verse in book\n";
			}
			//chapsize = chapsize - size;
			cfile << "versesize " << versesize << " chapsize " << chapsize << " size " << size<< "\n";
			cfile.flush();
			//assert(chapsize > size);
			//if (chapsize > size)           // At start of Psalms gets chapsize rather than chapsize+size ???
			//{
			//	versesize = versesize - (chapsize - size);
			//}
			//else
			//{
			versesize = versesize - (chapsize);
			//}
			cfile << "Last verse in chapter\n";
		}
		if (chapmark>=2 && bookmark!=1)
		{
			cfile << "prev verse pos{" << verseoffset << "} size{" << versesize << "} nextoffset{" << nextoffset << "}\n";
			cfile.flush();
			assert(verseoffset==nextoffset);
			write(vfp, &verseoffset, 4);
			write(vfp, &versesize, 2);
			nextoffset = verseoffset+versesize;
			bookmark = 0;
			chapmark = 0;
		}
		if (key1.Verse() == 1) {	// new chapter
			cfile << "size??? " << size << "\n";
			cfile.flush();
			//assert(chapsize > size || key1.Chapter()==1);
			//assert(chapsize > size);
			//if (chapsize > size)           // At start of books gets chapsize rather than chapsize+size
			//{
			//	chapsize = chapsize - size;
			//}
			if (key1.Chapter() == 1) {	// new book
				booksize = booksize - chapsize + 7;
				if (key1.Book() == 1)
				{
					pos = 0;
					//tmp = testoffset;
					tmp = 0;            	// better just remember that it goes up to the testament intro to avoid 64k limit
												// AV exceeds that anyway!
					write(vfp, &pos, 4);  /* Module intro */
					write(vfp, &tmp, 2);
					assert(nextoffset==0);
					cfile << "modintro pos{" << pos << "} size{" << tmp << "}\n";
					testsize = testsize - booksize - chapsize + 7;
					if (testsize > 10000)
					{
						cerr << "Error: testament too big " << testsize << "\n";
						exit(-1);
					}
					//assert(testoffset==nextoffset);
					write(vfp, &testoffset, 4);  /* Testament intro (vss)  */
					write(vfp, &testsize, 2);
					nextoffset = testoffset+testsize;
					cfile << "test intro pos{" << testoffset << "} size{" << testsize << "}\n";
				}
				pos = lseek(cfp, 0, SEEK_CUR);
				write(bfp, &pos, 4);
				pos = lseek(vfp, 0, SEEK_CUR); /* Book intro (cps) */
				write(cfp, &pos, 4);
				if (booksize > 10000)
				{
					cerr << "Error: book too big " << booksize << "\n";
					exit(-1);
				}
				assert(bookoffset==nextoffset);
				write(vfp, &bookoffset, 4);  /* Book intro (vss)  */
				write(vfp, &booksize, 2);
				nextoffset = bookoffset+booksize;
				cfile << "book intro pos{" << bookoffset << "} size{" << booksize << "}\n";
				//offset += booksize;
				//bookmark = false;
			}
			pos = lseek(vfp, 0, SEEK_CUR);
			write(cfp, &pos, 4);
			assert(chapsize < 10000);
			write(vfp, &chapoffset, 4);  /* Chapter intro */
			write(vfp, &chapsize, 2);
			nextoffset = chapoffset+chapsize;
			cfile << "chapter intro pos{" << chapoffset << "} size{" << chapsize << "}\n";
			//offset += chapsize;
			//size -= chapsize;
			//chapmark = false;
		}
		if (key1 >= key2) {
			if (size > 10000)
			{
				cerr << "Error: verse too big " << size << "\n";
				exit(-1);
			}
			if (!chapmark && !bookmark)
			{
				write(vfp, &offset, 4);
				write(vfp, &size, 2);
				cfile << "verse pos{" << offset << "} size{" << size << "}\n";
				cfile.flush();
				assert(offset==nextoffset);
				nextoffset = offset+size;
				//cfile << "bookmark " << bookmark << " chapmark " << chapmark << "\n";
			}
			else
			{
				verseoffset = offset;
				versesize = size;
				cfile << "saving verse pos{" << offset << "} size{" << size << "}\n";
				cfile << "bookmark " << bookmark << " chapmark " << chapmark << "\n";
			}
		}
		else	{
			pos = 0;
			tmp = 0;
			write(vfp, &pos, 4);
			write(vfp, &tmp, 2);
			cfile << "blank pos{" << pos << "} size{" << tmp << "}\n";
		}
	}
}

char startmod(char *buf)
{
	//char loop;

	if (buf[0] != '<')
		return 0;
	if (buf[1] != 'H')
		return 0;
	if (buf[2] != '0')
		return 0;
/*
	if (!isdigit(buf[2]))
		return 0;
	for (loop = 3; loop < 7; loop++) {
		if (buf[loop] == ' ')
			break;
		if ((!isdigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
			return 0;
	}
*/
	return 1;
}


char starttest(char *buf)
{
	//char loop;

	if (buf[0] != '<')
		return 0;
	if (buf[1] != 'B')
		return 0;
	if (testmnt==2)
	{
		if (buf[2] != 'N')
			return 0;
	}
	else
	{
		if (buf[2] != 'O')
			return 0;
	}
	//if (buf[3] != '>')
	//	return 0;
/*
	if (!isdigit(buf[2]))
		return 0;
	for (loop = 3; loop < 7; loop++) {
		if (buf[loop] == ' ')
			break;
		if ((!isdigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
			return 0;
	}
*/
	return 1;
}


char startbook(char *buf)
{
	//char loop;

	if (buf[0] != '<')
		return 0;
	if (buf[1] != 'S')
		return 0;
	if (buf[2] != 'B')
		return 0;
/*
	if (!isdigit(buf[2]))
		return 0;
	for (loop = 3; loop < 7; loop++) {
		if (buf[loop] == ' ')
			break;
		if ((!isdigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
			return 0;
	}
*/
	return 1;
}


char startchap(char *buf)
{
	//char loop;

	if (buf[0] != '<')
		return 0;
	if (buf[1] != 'S')
		return 0;
	if (buf[2] != 'C')
		return 0;
/*
	if (!isdigit(buf[2]))
		return 0;
	for (loop = 3; loop < 7; loop++) {
		if (buf[loop] == ' ')
			break;
		if ((!isdigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
			return 0;
	}
*/
	return 1;
}


char startentry(char *buf)
{
	//char loop;
	//cfile << "{SV}";

	if (buf[0] != '<')
	{
		//cfile << "{no<}";
		return 0;
	}
	if (buf[1] != 'S')
	{
		//cfile << "\n{noS}\n";
		return 0;
	}
	if (buf[2] != 'V')
	{
		//cfile << "\n{noV}\n";
		return 0;
	}
/*
	if (!isdigit(buf[2]))
		return 0;
	for (loop = 3; loop < 7; loop++) {
		if (buf[loop] == ' ')
			break;
		if ((!isdigit(buf[loop])) && (buf[loop] != ',') && (buf[loop] != '-'))
			return 0;
	}
*/
	return 1;
}


char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, unsigned short *size)
{
	char buf[8];
	//char buf2[20];
	//char ch;
	char loop=0;
	long offset2;
	int ch2, vs2, rm2;
	bool flag;
	long versestart = 0;
	long chapstart = 0;
	long bookstart = 0;
	long teststart = 0;

	memset(buf, ' ', 8);

	while (1) {
		//cfile << "#" << buf << "#";
		//if (lseek(fp, 0, SEEK_CUR) > 2000000)
		//{
		//	cfile << lseek(fp, 0, SEEK_CUR) << "\n";
		//}
		if (starttest(buf)) {
			cfile << "\n{start of testament}\n";
			//chapstart = lseek(fp, 0, SEEK_CUR) - 7;
			teststart = lseek(fp, 0, SEEK_CUR) - 7;
			testoffset = teststart;
			memset(buf, ' ', 3);
			flag = false;
			for (loop = 3; loop < 6; loop++) {
				if (buf[loop]!='>')
					flag = true;
				else {
					buf[loop] = 0;
					break;
				}
			}
			ch2 = *num1;
			vs2 = 1;
			if (size) {
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					testsize = (unsigned short) (lseek(fp, 0, SEEK_END) - teststart-7);
				}
				else {
					if (vs2) {
						testsize = (offset2 - teststart - 7);
					}
				}
				lseek(fp, teststart+7, SEEK_SET);
				cfile << "\nGot testsize " << testsize << "\n";
			}
		}


		if (startbook(buf)) {
			cfile << "\n{start of book}\n";
			bookmark++;
			//chapstart = lseek(fp, 0, SEEK_CUR) - 7;
			bookstart = lseek(fp, 0, SEEK_CUR) - 7;
			bookoffset = bookstart;
			memset(buf, ' ', 3);
			flag = false;
			for (loop = 3; loop < 6; loop++) {
				if (buf[loop]!='>')
					flag = true;
				else {
					buf[loop] = 0;
					break;
				}
			}
			if (size) {
				ch2 = *num1;
				vs2 = 1;
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					booksize = (unsigned short) (lseek(fp, 0, SEEK_END) - bookstart - 7);
				}
				else {
					if (vs2) {
						booksize = (offset2 - bookstart - 7);
					}
				}
				lseek(fp, bookstart+7, SEEK_SET);
				cfile << "\nGot booksize " << booksize << "\n";
			}
		}

		if (startchap(buf)) {
			cfile << "{start of chapter}";
			chapmark++;
			//chapstart = lseek(fp, 0, SEEK_CUR) - 7;
			chapstart = lseek(fp, 0, SEEK_CUR) - 7;
			chapoffset = chapstart;
			memset(buf, ' ', 3);
			flag = false;
			for (loop = 3; loop < 6; loop++) {
				if (isdigit(buf[loop]))
					flag = true;
				else {
					buf[loop] = 0;
					break;
				}
			}
			if (flag)
				*num1 = atoi(buf);
			else	(*num1)++;

			if (size) {
				ch2 = *num1;
				vs2 = 1;
				lseek(fp, chapstart, SEEK_SET);
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					chapsize = (unsigned short) (lseek(fp, 0, SEEK_END) - chapstart);
					cfile << "getting chapsizeend{" << chapsize << "} = " << lseek(fp, 0, SEEK_END) << " - " << chapstart << "\n";
				}
				else {
					if (vs2) {
						chapsize = (offset2 - chapstart);
						cfile << "getting chapsize{" << chapsize << "} = " << offset2 << " - " << chapstart << "\n";
					}
				}
				lseek(fp, chapstart + 7, SEEK_SET);
				cfile << "\nGot chapsize " << chapsize << " loop{" << (int) loop << "}\n";
			}
			//return 0;

		}
		if (startentry(buf)) {
			//cfile << "{start of verse}";
			memset(buf, ' ', 3);
			flag = false;
			for (loop = 3; loop < 6; loop++) {
				if (isdigit(buf[loop]))
					flag = true;
				else {
					buf[loop] = 0;
					break;
				}
			if (flag)
				*num2 = atoi(buf);
			else	(*num2)++;
			}
			loop++;
			/*
			if (size)
			{
				// *offset = lseek(fp, 0, SEEK_CUR) - (7 - loop);
				 *offset = lseek(fp, 0, SEEK_CUR) - 7;
			}
			//else	*offset = (chapstart) ? chapstart : lseek(fp, 0, SEEK_CUR) - 7;
			else	*offset = (chapstart) ? chapstart : lseek(fp, 0, SEEK_CUR) - 7;
			*/
			/*if (chapstart)
			{
				chapsize = *offset-chapstart;
			}
			else
			{
				chapsize = 0;
			}*/
			*offset = lseek(fp, 0, SEEK_CUR) - 7;
			versestart = *offset;
			if (size) {
				ch2 = *num1;
				vs2 = *num2;
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					*size = (unsigned short) (lseek(fp, 0, SEEK_END) - versestart);
					cfile << "getting sizeend{" << *size << "} = " << lseek(fp, 0, SEEK_END) << " - " << versestart << "\n";
				}
				else {
					if (vs2) {
						*size = (offset2 - versestart);
						cfile << "getting size{" << *size << "} = " << offset2 << " - " << versestart << "\n";
					}
				}
				lseek(fp, *offset+1, SEEK_SET);
			}
			else
			{
				cfile << "got offset{" << *offset << "}\n";
			}
			return 0;
		}
		//cfile << "{ng}";
		//deadcount++;
		//if (deadcount==1000) exit(-1);
		//if (!size)
		//{
		//	cfile << "not bound offset{" << *offset << "}\n";
		//}
		memmove(buf, &buf[1], 6);
		if (read(fp, &buf[6], 1) != 1)
			return 1;
	}
}


void openfiles(char *fname)
{
#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.
	char buf[255];

	if ((fp = open(fname, O_RDONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", fname);
		exit(1);
	}

	sprintf(buf, "%s.vss", fname);
	if ((vfp = open(buf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", fname);
	if ((cfp = open(buf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", fname);
	if ((bfp = open(buf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}
	cfile.open("gbfidx.log", ios::out);
	if (!cfile.is_open())
	{
		cerr << "Failed to open log file\n";
		exit(-1);
	}
}


void checkparams(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s <file to process> [nt - for new testmt file]\n", argv[0]);
		exit(1);
	}
	if (!strcmp(argv[1], "nt"))
		key1 = key2 = key3 = "Matthew 1:1";
	else if (!strcmp(argv[1], "ot"))
	{
			key1 = key2 = key3 = "Genesis 1:1";
	}
	else
	{
		cerr << "File must be ot or nt\n";
		exit(-1);
	}
}
