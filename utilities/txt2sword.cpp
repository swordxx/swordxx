#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <versekey.h>
#include <sys/stat.h>
#include <iostream.h>
const long MAXVLEN = 8192;

int fp, vfp, cfp, bfp, dfp;
short size;

void ProcessFile (char *fileName, char *testament);
char getVerse (int fp, int *verseNum, char *verseBuf, char testament);
void filterVerse (char *text, char lang);
void openFiles (char *fileName);

int main (int argc, char **argv) {
  if (argc == 3)
    ProcessFile(argv[1], argv[2]);
  else
    ProcessFile(argv[1], "ot");
  return 0;
};

void ProcessFile(char *fileName, char *testament)
{
	unsigned long pos = 0L;
	unsigned short size = 0, tmp;
	int verseNum, curbook = 0, curchap = 0, curverse = 0, curTestament = 1;
	char buf[127];
	VerseKey mykey;
	char verseBuf[MAXVLEN];
	bool fail = false;
	char end = 0;

	if ((fp = open(fileName, O_RDONLY)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", fileName);
		printf(buf);
		return;
	}
	
	if (!strcmp(testament, "nt")) {
	  openFiles("nt");
	
	  mykey = "Matthew 1:1";
	} else {
	  openFiles("ot");
	  
	  mykey = "Genesis 1:1";
	}

	// getVerse(fp, &verseNum, verseBuf, 1);
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
			sprintf(buf, "Error: Found chaptures out of sequence\n");
			printf(buf);
			fail = true;
			break;
		}
		if ((mykey.Verse() != curverse) || (curverse != verseNum)) {
			printf("Error: Found verses out of sequence");
			sprintf(buf, "Expected %d (%s), but got %d\n\n", curverse, (const char *)mykey, verseNum);
			printf(buf);
			printf("   Currently SWORD only supports KJV verse numbering and book ordering.\n");
			printf("   Try combining extra verses with the last valid verse just before.\n");
			printf("   This way, even though they will display in SWORD along with the previous verse, all data will be displayed.\n");
			printf("   Where verses are missing, add a blank entry.\n");
			printf("   Then try rerunning\n");
			fail = true;
			break;
		}
		
		pos = lseek(dfp, 0, SEEK_CUR);
		write(vfp, &pos, 4);
		size = strlen(verseBuf);
		write(vfp, &size, 2);
		sprintf(buf, "%2d:%3d:%3d (%s) found at offset: %7ld; size: %d\n", curbook, mykey.Chapter(), mykey.Verse(), (const char *)mykey, pos, size);
		printf(buf);
		if (size > 1000) {
			printf("WARNING: Strange Size.");
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
		printf("SUCCESS!!!!!!!");
}


char getVerse(int fp, int *verseNum, char *verseBuf, char testament)
{
  int retVal = 1;
  int tvn = 0;
  char * temp_char = new char[2];
  temp_char[1] = 0;
  *verseBuf = 0;

  while (1) {
    retVal = read(fp, temp_char, 1);

    if (isdigit(temp_char[0]))
      tvn = (10 * tvn) + temp_char[0] - '0';
    else
      break;
  }
  cout << tvn << " "; 
  *verseNum = tvn;

  while (1) {
    retVal = read(fp, temp_char, 1);
    if (temp_char[0] == 10 || temp_char[0] == 13)
      break;
    else
      strcat (verseBuf, temp_char);
  }
  strcat (verseBuf, "\n");

  cout << verseBuf; 

  if (retVal) 
    retVal = 0;
  else
    retVal = 1;

  delete temp_char;
  return retVal;
}

void openFiles(char *fileName)
{
	char buf[128];
	long pos;
	short size;

	unlink(fileName);
	if ((dfp = open(fileName, O_CREAT|O_WRONLY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", fileName);
		printf(buf);
		return;
	}

	sprintf(buf, "%s.vss", fileName);
	unlink(buf);
	if ((vfp = open(buf, O_CREAT|O_WRONLY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		printf(buf);
		return;
	}

	sprintf(buf, "%s.cps", fileName);
	unlink(buf);
	if ((cfp = open(buf, O_CREAT|O_WRONLY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		printf(buf);
		return;
	}

	sprintf(buf, "%s.bks", fileName);
	unlink(buf);
	if ((bfp = open(buf, O_CREAT|O_WRONLY, S_IREAD|S_IWRITE)) == -1) {
		sprintf(buf, "Couldn't open file: %s\n", buf);
		printf(buf);
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












