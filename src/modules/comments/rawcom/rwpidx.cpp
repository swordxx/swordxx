/*****************************************************************************
 *
 *	This code wreaks but works (at least for RWP).  Good luck!
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


void writeidx(VerseKey &key1, VerseKey &key2, VerseKey &key3, long offset, short size);
char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size);
void openfiles(char *fname);
void checkparams(int argc, char **argv);


VerseKey key1, key2, key3;
int fp, vfp, cfp, bfp;
long chapoffset;
short chapsize;
char testmnt;


main(int argc, char **argv)
{
	long pos, offset;
	int num1 = 0, num2 = 0, rangemax, curbook = 0, curchap = 0, curverse = 0;
	char buf[127], startflag = 0;
	short size, tmp;

	checkparams(argc, argv);

	openfiles(argv[1]);

	testmnt = key1.Testament();
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

	while(!findbreak(fp, &offset, &num1, &num2, &rangemax, &size)) {
		if (num2) {				
			key2.Verse(1);
			key2.Chapter(num1);
			key2.Verse(num2);
		}
		else {
			key2.Verse(1);
			if (!startflag) {
				startflag = 1;
			}
			else {
				if (num1 <= key2.Chapter()) { // new book
					key2.Chapter(1);
					key2.Book(key2.Book()+1);
				}
			}
			key2.Chapter(num1);
			printf("Found Chapter Break: %d ('%s')\n", num1, (const char *)key2);
			chapoffset = offset;
			chapsize = size;
			continue;
		}

		key3 = key2;
		key3 += (rangemax - key3.Verse());

		printf("Found verse Break: ('%s')\n", (const char *)key2);
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

	for (; ((key1 <= key3) && (key1.Error() != KEYERR_OUTOFBOUNDS) && (key1.Testament() == testmnt)); key1+=1) {
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
	}
}


char startentry(char *buf)
{
	char colon = 0;

	if (buf[0] != 10)
		return 0;
	if (buf[1] != 10)
		return 0;
	if (!isdigit(buf[2]))
		return 0;
	if (!isdigit(buf[3])) {
		if (buf[3]!= ':')
			return 0;
		else	colon++;
	}
	if (!isdigit(buf[4])) {
		if (buf[4]!= ':')
			return 0;
		else	colon++;
	}
	if (colon != 1)
		return 0;
	return 1;
}


char findbreak(int fp, long *offset, int *num1, int *num2, int *rangemax, short *size)
{
	char buf[7];
	char buf2[20];
	char ch;
	char loop;
	long offset2;
	int ch2, vs2, rm2;
	
	memset(buf, ' ', 7);

	while (1) {
		if (startentry(buf)) {
			buf[0] = ' ';
			buf[1] = ' ';
			sscanf(buf, "%d:%d", num1, num2);
			*rangemax = *num2;
			*offset = lseek(fp, 0, SEEK_CUR) - 5;
			if (size) {
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					*size = (short) (lseek(fp, 0, SEEK_END) - (*offset));
				}
				else {
					if (vs2) {
						*size = (offset2 - (*offset)) - 2;
					}
					else {
						*size = (offset2 - (*offset)) - 6;
					}
				}
				lseek(fp, *offset, SEEK_SET);
			}
			return 0;
		}
					
		if (!strncmp(buf, "$-$-$-", 6)) {
			*offset = lseek(fp, 0, SEEK_CUR) - 1;
			*num2 = 0;
			(*num1)++;
			printf("Book marker: %s\n", buf2);
			if (size) {
				if (findbreak(fp, &offset2, &ch2, &vs2, &rm2, 0)) {
					*size = (short) (lseek(fp, 0, SEEK_END) - (*offset));
				}
				else {
					if (vs2) {
						*size = (offset2 - (*offset)) - 2;
					}
					else {
						*size = (offset2 - (*offset)) - 6;
					}
				}
				lseek(fp, *offset, SEEK_SET);
			}
			return 0;
		}
				 
				
		memmove(buf, &buf[1], 6);
		if (read(fp, &buf[6], 1) != 1)
			return 1;
	}
}


void openfiles(char *fname)
{
	char buf[255];

	if ((fp = open(fname, O_RDONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", fname);
		exit(1);
	}

	sprintf(buf, "%s.vss", fname);
	if ((vfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", fname);
	if ((cfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", fname);
	if ((bfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}
}


void checkparams(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s <file to process> [nt - for new testmt file]\n", argv[0]);
		exit(1);
	}
	if (argc == 3)
		key1 = key2 = key3 = "Matthew 1:1";
	else	key1 = key2 = key3 = "Genesis 1:1";
}
