/******************************************************************************
 *  rawverse.h   - code for class 'RawVerse'- a module that reads raw text
 *				files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *				and provides lookup and parsing functions based on
 *				class VerseKey
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <rawverse.h>


/******************************************************************************
 * RawVerse Statics
 */

 int RawVerse::instance = 0;


/******************************************************************************
 * RawVerse Constructor - Initializes data for instance of RawVerse
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 *		be sure to include the trailing separator (e.g. '/' or '\')
 *		(e.g. 'modules/texts/rawtext/webster/')
 */

RawVerse::RawVerse(char *ipath)
{
	char buf[127];

 	nl = '\n';
	path = 0;
	stdstr(&path, ipath);

#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.

	sprintf(buf, "%sot.vss", path);
	idxfp[0] = open(buf, O_RDONLY|O_BINARY);

	sprintf(buf, "%snt.vss", path);
	idxfp[1] = open(buf, O_RDONLY|O_BINARY);

	sprintf(buf, "%sot", path);
	textfp[0] = open(buf, O_RDONLY|O_BINARY);

	sprintf(buf, "%snt", path);
	textfp[1] = open(buf, O_RDONLY|O_BINARY);
	instance++;
}


/******************************************************************************
 * RawVerse Destructor - Cleans up instance of RawVerse
 */

RawVerse::~RawVerse()
{
	int loop1;

	if (path)
		delete [] path;

	--instance;

	for (loop1 = 0; loop1 < 2; loop1++) {
		close(idxfp[loop1]);
		close(textfp[loop1]);
	}
}


/******************************************************************************
 * RawVerse::findoffset	- Finds the offset of the key verse from the indexes
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	book	- book      to find (0 - testament    introduction)
 *	chapter	- chapter   to find (0 - book         introduction)
 *	verse	- verse     to find (0 - chapter      introduction)
 *	start	- address to store the starting offset
 *	size	- address to store the size of the entry
 */

void RawVerse::findoffset(char testmt, long idxoff, long *start, short *size)
{
	idxoff *= 6;
	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	lseek(idxfp[testmt-1], idxoff, SEEK_SET);
	read(idxfp[testmt-1], start, 4);
	if (read(idxfp[testmt-1], size, 2) < 2) { 		// read size
		*size = (short)(lseek(textfp[testmt-1], 0, SEEK_END) - (long)start);	// if for some reason we get an error reading size, make size to end of file
	}
}


/******************************************************************************
 * RawVerse::preptext	- Prepares the text before returning it to external
 *				objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *			text.
 */

void RawVerse::preptext(char *buf)
{
	char *to, *from, space = 0;

	for (from = to = buf; *from; from++) {
		switch (*from) {
		case 10:
			space = 1;
			continue;
		case 13:
			*to++ = nl;
			space = 0;
			continue;
		}
		if (space) {
			space = 0;
			if (*from != ' ') {
				*to++ = ' ';
				from--;
				continue;
			}
		}
		*to++ = *from;
	}
	*to = 0;
}


/******************************************************************************
 * RawVerse::gettext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry + 1 (null)
 *	buf	- buffer to store text
 *
 */

void RawVerse::gettext(char testmt, long start, short size, char *buf)
{
	memset(buf, 0, size);
	lseek(textfp[testmt-1], start, SEEK_SET);
	read(textfp[testmt-1], buf, (int)size - 1);
}
