/******************************************************************************
 *  zverse.h   - code for class 'zVerse'- a module that reads raw text
 *				files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *				and provides lookup and parsing functions based on
 *				class VerseKey for compressed modules
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
#include <versekey.h>
#include <zverse.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif


/******************************************************************************
 * zVerse Statics
 */

int zVerse::instance = 0;

const int zVerse::VERSEBLOCKS = 2;
const int zVerse::CHAPTERBLOCKS = 3;
const int zVerse::BOOKBLOCKS = 4;

/******************************************************************************
 * zVerse Constructor - Initializes data for instance of zVerse
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 *		be sure to include the trailing separator (e.g. '/' or '\')
 *		(e.g. 'modules/texts/rawtext/webster/')
 *		fileMode - open mode for the files (O_RDONLY, etc.)
 *		blockType - verse, chapter, book, etc.
 */

zVerse::zVerse(const char *ipath, int fileMode, int blockType, SWCompress *icomp)
{
	char buf[127];

	printf("init zverse\n");

	nl = '\n';
	path = 0;
	ulcache=-1;
	//lgBugffNum=0;
	stdstr(&path, ipath);

	compressor = (icomp) ? icomp : new SWCompress();

	switch ( blockType) {
		case 1 :
			sprintf(buf, "%sot.rzs", path);
			idxfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.rzs", path);
			idxfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.rzz", path);
			textfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.rzz", path);
			textfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.rzv", path);
			compfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.rzv", path);
			compfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);
			break;
		case VERSEBLOCKS :
			sprintf(buf, "%sot.vzs", path);
			idxfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.vzs", path);
			idxfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.vzz", path);
			textfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.vzz", path);
			textfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.vzv", path);
			compfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.vzv", path);
			compfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);
			break;
		case CHAPTERBLOCKS :
			sprintf(buf, "%sot.czs", path);
			idxfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.czs", path);
			idxfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.czz", path);
			textfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.czz", path);
			textfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.czv", path);
			compfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.czv", path);
			compfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);
			break;
		case BOOKBLOCKS :
			sprintf(buf, "%sot.bzs", path);
			idxfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.bzs", path);
			idxfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.bzz", path);
			textfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.bzz", path);
			textfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%sot.bzv", path);
			compfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);

			sprintf(buf, "%snt.bzv", path);
			compfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY);
			break;
	}

	instance++;
}


/******************************************************************************
 * zVerse Destructor - Cleans up instance of zVerse
 */

zVerse::~zVerse()
{
	int loop1;

	if (path)
		delete [] path;

	if (compressor)
		delete compressor;

	--instance;

	for (loop1 = 0; loop1 < 2; loop1++) {
		FileMgr::systemFileMgr.close(idxfp[loop1]);
		FileMgr::systemFileMgr.close(textfp[loop1]);
		FileMgr::systemFileMgr.close(compfp[loop1]);
	}
}


/******************************************************************************
 * zVerse::findoffset	- Finds the offset of the key verse from the indexes
 *
 *
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	book	- book      to find (0 - testament    introduction)
 *	chapter	- chapter   to find (0 - book         introduction)
 *	verse	- verse     to find (0 - chapter      introduction)
 *	start	- address to store the starting offset
 *	size	- address to store the size of the entry
 */

void zVerse::findoffset(char testmt, long idxoff, long *start, unsigned short *size)
{
	/*
	idxoff *= 6;
	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	lseek(idxfp[testmt-1]->getFd(), idxoff, SEEK_SET);
	read(idxfp[testmt-1]->getFd(), start, 4);
	if (read(idxfp[testmt-1]->getFd(), size, 2) < 2) { 		// read size
		*size = (unsigned short)(lseek(textfp[testmt-1]->getFd(), 0, SEEK_END) - (long)start);	// if for some reason we get an error reading size, make size to end of file
	}
	 */
	// set start to offset in
	// set size to
	// set
	unsigned long ulBuffNum=0;	          // buffer number
	unsigned long ulVerseStart=0;	       // verse offset within buffer
	unsigned short usVerseSize=0;	       // verse size
	unsigned long ulCompOffset=0;	       // compressed buffer start
	unsigned long ulCompSize=0;	             // buffer size compressed
	unsigned long ulUnCompSize=0;	          // buffer size uncompressed
	char *pcCompText=NULL;					 // compressed text


	//printf ("Finding offset %ld\n", idxoff);
	idxoff *= 10;
	if (!testmt)
	{
		testmt = ((idxfp[1]) ? 1:2);
	}
	lseek(compfp[testmt-1]->getFd(), idxoff, SEEK_SET);
	if (read(compfp[testmt-1]->getFd(), &ulBuffNum, 4) < 4)
	{
		printf ("Error reading ulBuffNum\n");
		return;
	}

	if (read(compfp[testmt-1]->getFd(), &ulVerseStart, 4) < 2)
	{
		printf ("Error reading ulVerseStart\n");
		return;
	}
	if (read(compfp[testmt-1]->getFd(), &usVerseSize, 2) < 2)
	{
		printf ("Error reading usVerseSize\n");
		return;
	}
	if (((long) ulBuffNum == ulcache) && (compressor->Buf()))
	{
		// have the text buffered
		*start = ulVerseStart;
		*size = usVerseSize;
		return;
	}

	//printf ("Got buffer number{%ld} versestart{%ld} versesize{%d}\n", ulBuffNum, ulVerseStart, usVerseSize);

	if (lseek(idxfp[testmt-1]->getFd(), ulBuffNum*12, SEEK_SET)!=(long) ulBuffNum*12)
	{
		printf ("Error seeking compressed file index\n");
		return;
	}
	if (read(idxfp[testmt-1]->getFd(), &ulCompOffset, 4)<4)
	{
		printf ("Error reading ulCompOffset\n");
		return;
	}
	if (read(idxfp[testmt-1]->getFd(), &ulCompSize, 4)<4)
	{
		printf ("Error reading ulCompSize\n");
		return;
	}
	if (read(idxfp[testmt-1]->getFd(), &ulUnCompSize, 4)<4)
	{
		printf ("Error reading ulUnCompSize\n");
		return;
	}

	if (lseek(textfp[testmt-1]->getFd(), ulCompOffset, SEEK_SET)!=(long)ulCompOffset)
	{
		printf ("Error: could not seek to right place in compressed text\n");
		return;
	}
	//printf ("Got text offset{%ld} compsize{%ld} uncompsize{%ld}\n", ulCompOffset, ulCompSize, ulUnCompSize);
	pcCompText = new char[ulCompSize];
	//printf("made buffer to read compressed text\n");
	if (read(textfp[testmt-1]->getFd(), pcCompText, ulCompSize)<(long)ulCompSize)
	{
		printf ("Error reading compressed text\n");
		return;
	}
	//printf("read compressed text into buffer\n");
	//printf("got compressed buffer:\n%s\n", pcCompText);
	compressor->zBuf(&ulCompSize, pcCompText);
	//printf("deleting the buffer\n");
	//delete[] pcCompText;

	//printf ("Got zstart{%ld} zsize{%d} zlen{%d}\n", *start, *size, tempzlen);

	//return ulVerseStart as start
	//return ulVerseSize as size
	*start = ulVerseStart;
	*size = usVerseSize;
//	blen = ulUnCompSize;
	//need to store ulUnCompSize somewhere?
	//printf("finished finding offset\n");
}


/******************************************************************************
 * zVerse::swgettext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry + 1 (null)
 *	buf	- buffer to store text
 *
 */

void zVerse::swgettext(char testmt, long start, unsigned short size, char *inbuf)
{
	char *mybuf;
	/*
	memset(buf, 0, size);
	lseek(textfp[testmt-1]->getFd(), start, SEEK_SET);
	read(textfp[testmt-1]->getFd(), buf, (int)size - 1);
	*/
	/*
	printf ("initialising buffer start{%ld} size{%d}\n", start, size);
	memset(inbuf, 0, size);
	printf ("seeking in text\n");
	lseek(textfp[testmt-1]->getFd(), start, SEEK_SET);
	//read(textfp[testmt-1]->getFd(), buf, (int)size - 1);
	printf ("reading in text\n");
	read(textfp[testmt-1]->getFd(), inbuf, tempzlen);
	printf ("setting compressed buffer %d\n", tempzlen);
	zBuf(&tempzlen, inbuf);
	printf ("copying uncompressed\n%s\n", Buf());
	memcpy(inbuf, Buf(), size);
	printf ("done\n");
	inbuf[size]=0;
	printf ("got\n");
	*/
	//printf("swgettext start{%ld} size{%d}\n", start, size);
	memset(inbuf, 0, size);
	mybuf = compressor->Buf();
	//printf("got full buffer\n%s\n", mybuf);

	strncpy(inbuf, &(mybuf[start]), size-1);

	inbuf[size]=0;
}

void zVerse::swsettext(char testmt, long idxoff, const char *buf)
{
	// can't set it yet in a compressed module
}


/******************************************************************************
 * zVerse::preptext	- Prepares the text before returning it to external
 *				objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *			text.
 */

void zVerse::preptext(char *buf)
{
	char *to, *from, space = 0, cr = 0, realdata = 0, nlcnt = 0;

	for (to = from = buf; *from; from++) {
		switch (*from) {
		case 10:
			if (!realdata)
				continue;
			space = (cr) ? 0 : 1;
			cr = 0;
			nlcnt++;
			if (nlcnt > 1) {
//				*to++ = nl;
				*to++ = nl;
//				nlcnt = 0;
			}
			continue;
		case 13:
			if (!realdata)
				continue;
			*to++ = nl;
			space = 0;
			cr = 1;
			continue;
		}
		realdata = 1;
		nlcnt = 0;
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

	for (to--; to > buf; to--) {			// remove training excess
		if ((*to == 10) || (*to == ' '))
			*to = 0;
		else break;
	}
}
