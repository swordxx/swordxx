/*****************************************************************************
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <versekey.h>

#ifndef O_BINARY
	#define O_BINARY 0
#endif

void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size);
char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size);
void openfiles();
void checkparams(int argc, char **argv);
void charsetconvert(char *data);


VerseKey key1, key2, key3;
int fp, vfp, cfp, bfp;
long chapoffset;
short chapsize;
char testmnt;
char startflag = 0;


main(int argc, char **argv)
{
	long pos, offset;
	int num1, num2, rangemax, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];
	short size, tmp;
	extern struct zonline online;

	checkparams(argc, argv);

	key1 = key2 = key3 = "Genesis 1:1";

	openfiles();

	num1    = key1.Chapter();
	num2    = key1.Verse();

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
}


/**************************************************************************
 * ENT:	key1	- current location of index
 * 	key2	- minimum keyval for which this offset is valid
 * 	key3	- maximum keyval for which this offset is valid
 */

void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size)
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


static VerseKey inckey = "Genesis 1:1";

char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size)
{
	char buf[7];
	char buf2[20];
	char ch;
	char loop;
	long offset2;
	int ch2, vs2, rm2;
	bool flag;
	long chapstart = 0; 
	static int olbvnum = 0;
	char data[256];
	char *bookabrev[66] = {"Ge", "Ex", "Le", "Nu", "De", "Jos", "Jud", "Ru",
		"1Sa", "2Sa", "1Ki", "2Ki", "1Ch", "2Ch", "Ezr", "Ne", "Es",
		"Job", "Ps", "Pr", "Ec", "So", "Isa", "Jer", "La", "Eze", "Da",
		"Ho", "Joe", "Am", "Ob", "Jon", "Mic", "Na", "Heb", "Zep",
		"Hag", "Zec", "Mal",
		"Mt", "Mr", "Lu", "Joh", "Ac", "Ro", "1Co", "2Co", "Ga",
		"Eph", "Php", "Col", "1Th", "2Th", "1Ti", "2Ti", "Tit", "Phm",
		"Heb", "Jas", "1Pe", "2Pe", "1Jo", "2Jo", "3Jo", "Jude", "Re" };

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
	
		sprintf(data, "JFB%.2d.htm#%s%d_%d", inckey.Book() + ((inckey.Testament()>1)?39:0), bookabrev[inckey.Book() + ((inckey.Testament()>1)?39:0)-1], inckey.Chapter(), inckey.Verse());
		write(fp, data, strlen(data));

		*size = lseek(fp, 0, SEEK_CUR) - *offset;
		write(fp, "\n", 1);
		return 0;
	}
	return 1;
}


void openfiles()
{
	char buf[255];
	char fname[5];
	long pos;
	short size;
	
	testmnt = key1.Testament();

	strcpy(fname, (testmnt==2) ? "nt" : "ot");
	unlink(fname);
	if ((fp = open(fname, O_CREAT|O_RDWR|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", fname);
		exit(1);
	}

	sprintf(buf, "%s.vss", fname);
	unlink(buf);
	if ((vfp = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", fname);
	unlink(buf);
	if ((cfp = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", fname);
	unlink(buf);
	if ((bfp = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
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


void checkparams(int argc, char **argv)
{
	if (argc !=1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		exit(1);
	}
}
