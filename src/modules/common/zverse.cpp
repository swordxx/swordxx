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
#include <stdlib.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <versekey.h>
#include <zverse.h>
#include <sysdata.h>


#ifndef O_BINARY
#define O_BINARY 0
#endif


/******************************************************************************
 * zVerse Statics
 */

int zVerse::instance = 0;

const char zVerse::uniqueIndexID[] = {'X', 'r', 'v', 'c', 'b'};

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

	nl = '\n';
	path = 0;
	cacheBufIdx = -1;
	cacheTestament = 0;
	cacheBuf = 0;
	dirtyCache = false;
	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	compressor = (icomp) ? icomp : new SWCompress();

	if (fileMode == -1) { // try read/write if possible
		fileMode = O_RDWR;
	}
		
	sprintf(buf, "%s/ot.%czs", path, uniqueIndexID[blockType]);
	idxfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/nt.%czs", path, uniqueIndexID[blockType]);
	idxfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/ot.%czz", path, uniqueIndexID[blockType]);
	textfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/nt.%czz", path, uniqueIndexID[blockType]);
	textfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/ot.%czv", path, uniqueIndexID[blockType]);
	compfp[0] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/nt.%czv", path, uniqueIndexID[blockType]);
	compfp[1] = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);
	
	instance++;
}


/******************************************************************************
 * zVerse Destructor - Cleans up instance of zVerse
 */

zVerse::~zVerse()
{
	int loop1;

	if (cacheBuf) {
		flushCache();
		free(cacheBuf);
	}

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

	*start = *size = 0;
	//printf ("Finding offset %ld\n", idxoff);
	idxoff *= 10;
	if (!testmt) {
		testmt = ((idxfp[0]) ? 1:2);
	}
	
	// assert we have and valid file descriptor
	if (compfp[testmt-1]->getFd() < 1)
		return;
		
	long newOffset = lseek(compfp[testmt-1]->getFd(), idxoff, SEEK_SET);
	if (newOffset == idxoff) {
		if (read(compfp[testmt-1]->getFd(), &ulBuffNum, 4) != 4) {
			printf ("Error reading ulBuffNum\n");
			return;
		}
	}
	else return;

	ulBuffNum = swordtoarch32(ulBuffNum);

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

	*start = swordtoarch32(ulVerseStart);
	*size = swordtoarch16(usVerseSize);

	if (*size) {
		if (((long) ulBuffNum == cacheBufIdx) && (testmt == cacheTestament) && (cacheBuf)) {
			// have the text buffered
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

		ulCompOffset  = swordtoarch32(ulCompOffset);
		ulCompSize  = swordtoarch32(ulCompSize);
		ulUnCompSize  = swordtoarch32(ulUnCompSize);

		if (lseek(textfp[testmt-1]->getFd(), ulCompOffset, SEEK_SET)!=(long)ulCompOffset)
		{
			printf ("Error: could not seek to right place in compressed text\n");
			return;
		}
		pcCompText = new char[ulCompSize];

		if (read(textfp[testmt-1]->getFd(), pcCompText, ulCompSize)<(long)ulCompSize)
		{
			printf ("Error reading compressed text\n");
			return;
		}
		compressor->zBuf(&ulCompSize, pcCompText);

		if (cacheBuf) {
			flushCache();
			free(cacheBuf);
		}
		
		unsigned long len = 0;
		compressor->Buf(0, &len);
		cacheBuf = (char *)calloc(len + 1, 1);
		memcpy(cacheBuf, compressor->Buf(), len);

		cacheTestament = testmt;
		cacheBufIdx = ulBuffNum;
		if (pcCompText)
			delete [] pcCompText;
	}
}


/******************************************************************************
 * zVerse::zreadtext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry + 1 (null)
 *	buf	- buffer to store text
 *
 */

void zVerse::zreadtext(char testmt, long start, unsigned short size, char *inbuf)
{
	memset(inbuf, 0, size);
	if (size > 2) {
		strncpy(inbuf, &(cacheBuf[start]), size-2);
	}
}


/******************************************************************************
 * zVerse::settext	- Sets text for current offset
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	idxoff	- offset into .vss
 *	buf	- buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void zVerse::settext(char testmt, long idxoff, const char *buf, long len) {

	len = (len < 0) ? strlen(buf) : len;
	if (!testmt) 
		testmt = ((idxfp[0]) ? 1:2);
	if ((!dirtyCache) || (cacheBufIdx < 0)) {
		cacheBufIdx = lseek(idxfp[testmt-1]->getFd(), 0, SEEK_END) / 12;
		cacheTestament = testmt;
		if (cacheBuf)
			free(cacheBuf);
		cacheBuf = (char *)calloc(len + 1, 1);
	}
	else cacheBuf = (char *)((cacheBuf)?realloc(cacheBuf, strlen(cacheBuf)+(len + 1)):calloc((len + 1), 1));

	dirtyCache = true;

	unsigned long start, outstart;
	unsigned long outBufIdx = cacheBufIdx;
	unsigned short size;
	unsigned short outsize;

	idxoff *= 10;
	size = outsize = len;

	start = strlen(cacheBuf);

	if (!size)
		start = outBufIdx = 0;

	outBufIdx = archtosword32(outBufIdx);
	outstart  = archtosword32(start);
	outsize   = archtosword16(size);

	lseek(compfp[testmt-1]->getFd(), idxoff, SEEK_SET);
	write(compfp[testmt-1]->getFd(), &outBufIdx, 4);
	write(compfp[testmt-1]->getFd(), &outstart, 4);
	write(compfp[testmt-1]->getFd(), &outsize, 2);
	strcat(cacheBuf, buf);
}


void zVerse::flushCache() {
	if (dirtyCache) {
		unsigned long idxoff;
		unsigned long start, outstart;
		unsigned long size, outsize;
		unsigned long zsize, outzsize;

		idxoff = cacheBufIdx * 12;
		size = outsize = zsize = outzsize = strlen(cacheBuf);
		if (size) {
//			if (compressor) {
//				delete compressor;
//				compressor = new LZSSCompress();
//			}
			compressor->Buf(cacheBuf);
			compressor->zBuf(&zsize);
			outzsize = zsize;

			start = outstart = lseek(textfp[cacheTestament-1]->getFd(), 0, SEEK_END);

			outstart  = archtosword32(start);
			outsize   = archtosword32(size);
			outzsize  = archtosword32(zsize);

			write(textfp[cacheTestament-1]->getFd(), compressor->zBuf(&zsize), zsize);

			lseek(idxfp[cacheTestament-1]->getFd(), idxoff, SEEK_SET);
			write(idxfp[cacheTestament-1]->getFd(), &outstart, 4);
			write(idxfp[cacheTestament-1]->getFd(), &outzsize, 4);
			write(idxfp[cacheTestament-1]->getFd(), &outsize, 4);
		}
		dirtyCache = false;
	}
}

/******************************************************************************
 * RawVerse::linkentry	- links one entry to another
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	destidxoff	- dest offset into .vss
 *	srcidxoff		- source offset into .vss
 */

void zVerse::linkentry(char testmt, long destidxoff, long srcidxoff) {
	long bufidx;
	long start;
	unsigned short size;

	destidxoff *= 10;
	srcidxoff  *= 10;

	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	// get source
	lseek(compfp[testmt-1]->getFd(), srcidxoff, SEEK_SET);
	read(compfp[testmt-1]->getFd(), &bufidx, 4);
	read(compfp[testmt-1]->getFd(), &start, 4);
	read(compfp[testmt-1]->getFd(), &size, 2);

	// write dest
	lseek(compfp[testmt-1]->getFd(), destidxoff, SEEK_SET);
	write(compfp[testmt-1]->getFd(), &bufidx, 4);
	write(compfp[testmt-1]->getFd(), &start, 4);
	write(compfp[testmt-1]->getFd(), &size, 2);
}


/******************************************************************************
 * RawVerse::CreateModule	- Creates new module files
 *
 * ENT: path	- directory to store module files
 * RET: error status
 */

char zVerse::createModule(const char *ipath, int blockBound)
{
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd, *fd2;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s/ot.%czs", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::systemFileMgr.close(fd);

	sprintf(buf, "%s/nt.%czs", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::systemFileMgr.close(fd);

	sprintf(buf, "%s/ot.%czz", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::systemFileMgr.close(fd);

	sprintf(buf, "%s/nt.%czz", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd2 = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();
	FileMgr::systemFileMgr.close(fd);

	sprintf(buf, "%s/ot.%czv", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();

	sprintf(buf, "%s/nt.%czv", path, uniqueIndexID[blockBound]);
	unlink(buf);
	fd2 = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();

	VerseKey vk;
	vk.Headings(1);
	long offset = 0;
	short size = 0;
	for (vk = TOP; !vk.Error(); vk++) {
		write((vk.Testament() == 1) ? fd->getFd() : fd2->getFd(), &offset, 4);	//compBufIdxOffset
		write((vk.Testament() == 1) ? fd->getFd() : fd2->getFd(), &offset, 4);
		write((vk.Testament() == 1) ? fd->getFd() : fd2->getFd(), &size, 2);
	}

	FileMgr::systemFileMgr.close(fd);
	FileMgr::systemFileMgr.close(fd2);

	delete [] path;
/*
	RawVerse rv(path);
	VerseKey mykey("Rev 22:21");
*/
	
	return 0;
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

     if (to > buf) {
          for (to--; to > buf; to--) {			// remove trailing excess
               if ((*to == 10) || (*to == ' '))
                    *to = 0;
               else break;
          }
     }
}
