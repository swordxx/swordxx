/******************************************************************************
 *  zverse2.h   - code for class 'zVerse2'- a module that reads raw text
 *				files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *				and provides lookup and parsing functions based on
 *				class VerseKey2 for compressed modules
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

#include <utilfuns.h>
#include <versekey2.h>
#include <zverse2.h>
#include <sysdata.h>
#include <swbuf.h>


#ifndef O_BINARY
#define O_BINARY 0
#endif

SWORD_NAMESPACE_START

/******************************************************************************
 * zVerse2 Statics
 */

int zVerse2::instance = 0;

const char zVerse2::uniqueIndexID[] = {'X', 'r', 'v', 'c', 'b', 'i'};

/******************************************************************************
 * zVerse2 Constructor - Initializes data for instance of zVerse2
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 *		be sure to include the trailing separator (e.g. '/' or '\')
 *		(e.g. 'modules/texts/rawtext/webster/')
 *		fileMode - open mode for the files (O_RDONLY, etc.)
 *		blockType - verse, chapter, book, etc.
 */

zVerse2::zVerse2(const char *ipath, int fileMode, int blockType, 
	SWCompress *icomp, int indexesPerBlock)
{
	SWBuf buf;

	nl = '\n';
	path = 0;
	cacheBufIdx = -1;
	cacheBuf = 0;
	m_idxPerBlock = indexesPerBlock;
	dirtyCache = false;
	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	compressor = (icomp) ? icomp : new SWCompress();

	if (fileMode == -1) { // try read/write if possible
		fileMode = O_RDWR;
	}
		
	buf.setFormatted("%s/text.%czs", path, uniqueIndexID[blockType]);
	idxfp = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	buf.setFormatted("%s/text.%czz", path, uniqueIndexID[blockType]);
	textfp = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	buf.setFormatted("%s/text.%czv", path, uniqueIndexID[blockType]);
	compfp = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	buf.setFormatted("%s/text.%czm", path, uniqueIndexID[blockType]);
	markupfp = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	buf.setFormatted("%s/text.%czr", path, uniqueIndexID[blockType]);
	midxfp = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	
	instance++;
}


/******************************************************************************
 * zVerse2 Destructor - Cleans up instance of zVerse2
 */

zVerse2::~zVerse2()
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

	FileMgr::getSystemFileMgr()->close(idxfp);
	FileMgr::getSystemFileMgr()->close(textfp);
	FileMgr::getSystemFileMgr()->close(compfp);
	FileMgr::getSystemFileMgr()->close(markupfp);
	FileMgr::getSystemFileMgr()->close(midxfp);
}


/******************************************************************************
 * zVerse2::findoffset	- Finds the offset of the key verse from the indexes
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

void zVerse2::findOffsetText(long idxoff, long *start, unsigned short *size)
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
	
	// assert we have and valid file descriptor
	if (compfp->getFd() < 1)
		return;
		
	long newOffset = lseek(compfp->getFd(), idxoff, SEEK_SET);
	if (newOffset == idxoff) {
		if (read(compfp->getFd(), &ulBuffNum, 4) != 4) {
			printf ("Error reading ulBuffNum from %d\n", idxoff);
			return;
		}
	}
	else {
		printf ("Error seeking to ulBuffNum from %d\n", idxoff);
		return;
	}

	ulBuffNum = swordtoarch32(ulBuffNum);

	if (read(compfp->getFd(), &ulVerseStart, 4) != 4)
	{
		printf ("Error reading ulVerseStartfrom %d\n", idxoff);
		return;
	}
	if (read(compfp->getFd(), &usVerseSize, 2) != 2)
	{
		printf ("Error reading usVerseSize from %d\n", idxoff);
		return;
	}

	*start = swordtoarch32(ulVerseStart);
	*size = swordtoarch16(usVerseSize);

	if (*size) {
		if (((long) ulBuffNum == cacheBufIdx) && (cacheBuf)) {
			// have the text buffered
			return;
		}

		//printf ("Got buffer number{%ld} versestart{%ld} versesize{%d}\n", ulBuffNum, ulVerseStart, usVerseSize);


		if (lseek(idxfp->getFd(), ulBuffNum*12, SEEK_SET)!=(long) ulBuffNum*12)
		{
			printf ("Error seeking compressed file index\n");
			return;
		}
		if (read(idxfp->getFd(), &ulCompOffset, 4)<4)
		{
			printf ("Error reading ulCompOffset\n");
			return;
		}
		if (read(idxfp->getFd(), &ulCompSize, 4)<4)
		{
			printf ("Error reading ulCompSize\n");
			return;
		}
		if (read(idxfp->getFd(), &ulUnCompSize, 4)<4)
		{
			printf ("Error reading ulUnCompSize\n");
			return;
		}

		ulCompOffset  = swordtoarch32(ulCompOffset);
		ulCompSize  = swordtoarch32(ulCompSize);
		ulUnCompSize  = swordtoarch32(ulUnCompSize);

		if (lseek(textfp->getFd(), ulCompOffset, SEEK_SET)!=(long)ulCompOffset)
		{
			printf ("Error: could not seek to right place in compressed text\n");
			return;
		}
		SWBuf pcCompText;
		pcCompText.setSize(ulCompSize+5);

		if (read(textfp->getFd(), pcCompText.getRawData(), ulCompSize)<(long)ulCompSize) {
			printf ("Error reading compressed text\n");
			return;
		}
		pcCompText.setSize(ulCompSize);
		#if 0 // it's a nullop - dunno what it was supposed to be?
		rawZFilter(pcCompText, 0); // 0 = decipher
		#endif
		
		compressor->zBuf(&ulCompSize, pcCompText.getRawData());

		if (cacheBuf) {
			flushCache();
			free(cacheBuf);
		}
		
		unsigned long len = 0;
		compressor->Buf(0, &len);
		cacheBuf = (char *)calloc(len + 1, 1);
		memcpy(cacheBuf, compressor->Buf(), len);

		cacheBufIdx = ulBuffNum;
	}
}


void zVerse2::findOffsetMarkup(long idxoff, long *start, unsigned short *size)
{
	if (midxfp->getFd() < 1)
		return;
	idxoff *= 6;
		
	if (midxfp->getFd() >= 0) {
		lseek(midxfp->getFd(), idxoff, SEEK_SET);
		read(midxfp->getFd(), start, 4);							// read start
		unsigned short len = read(idxfp->getFd(), size, 2); 		// read size

		*start = swordtoarch32(*start);
		*size  = swordtoarch16(*size);

		if (len < 2) {
			*size = (unsigned short)((*start) ? (lseek(markupfp->getFd(), 0, SEEK_END) - (long)*start) : 0);	
				// if for some reason we get an error reading size, make size to end of file
		}
	}
	else {
		*start = 0;
		*size = 0;
	}
}

/******************************************************************************
 * zVerse2::zreadtext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry + 1 (null)
 *	buf	- buffer to store text
 *
 */

void zVerse2::zReadText(long start, unsigned short size, SWBuf &inBuf) {
	inBuf = "";
	inBuf.setFillByte(0);
	inBuf.setSize(size+1);
	if (size > 0) {
		if (cacheBuf)
			strncpy(inBuf.getRawData(), &(cacheBuf[start]), size);
	}
	inBuf.setSize(strlen(inBuf.c_str()));
}


/******************************************************************************
 * zVerse2::settext	- Sets text for current offset
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	idxoff	- offset into .vss
 *	buf	- buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void zVerse2::doSetText(long idxoff, const char *buf, long len) {

	len = (len < 0) ? strlen(buf) : len;
	if ((!dirtyCache) || (cacheBufIdx < 0)) {
		cacheBufIdx = lseek(idxfp->getFd(), 0, SEEK_END) / 12;
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

	lseek(compfp->getFd(), idxoff, SEEK_SET);
	write(compfp->getFd(), &outBufIdx, 4);
	write(compfp->getFd(), &outstart, 4);
	write(compfp->getFd(), &outsize, 2);
	strcat(cacheBuf, buf);
}


void zVerse2::flushCache() {
	if (dirtyCache) {
		unsigned long idxoff;
		unsigned long start, outstart;
		unsigned long size, outsize;
		unsigned long zsize, outzsize;

		idxoff = cacheBufIdx * 12;
		if (cacheBuf) {
			size = outsize = zsize = outzsize = strlen(cacheBuf);
			if (size) {
	//			if (compressor) {
	//				delete compressor;
	//				compressor = new LZSSCompress();
	//			}
				compressor->Buf(cacheBuf);
				compressor->zBuf(&zsize);
				outzsize = zsize;

				SWBuf buf;
				buf.setSize(zsize + 5);
				memcpy(buf.getRawData(), compressor->zBuf(&zsize), zsize);
				buf.setSize(zsize);
				#if 0 // it's a nullop - dunno what it was supposed to be?
				rawZFilter(buf, 1); // 1 = encipher
				#endif

				start = outstart = lseek(textfp->getFd(), 0, SEEK_END);

				outstart  = archtosword32(start);
				outsize   = archtosword32(size);
				outzsize  = archtosword32(zsize);

				write(textfp->getFd(), buf, zsize);

				lseek(idxfp->getFd(), idxoff, SEEK_SET);
				write(idxfp->getFd(), &outstart, 4);
				write(idxfp->getFd(), &outzsize, 4);
				write(idxfp->getFd(), &outsize, 4);
			}
			free(cacheBuf);
			cacheBuf = 0;
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

void zVerse2::doLinkEntry(long destidxoff, long srcidxoff) {
	long bufidx;
	long start;
	unsigned short size;

	destidxoff *= 10;
	srcidxoff  *= 10;

	// get source
	lseek(compfp->getFd(), srcidxoff, SEEK_SET);
	read(compfp->getFd(), &bufidx, 4);
	read(compfp->getFd(), &start, 4);
	read(compfp->getFd(), &size, 2);

	// write dest
	lseek(compfp->getFd(), destidxoff, SEEK_SET);
	write(compfp->getFd(), &bufidx, 4);
	write(compfp->getFd(), &start, 4);
	write(compfp->getFd(), &size, 2);
	
	// !!WDG do we want to link the markup as well?
}


/******************************************************************************
 * RawVerse::CreateModule	- Creates new module files
 *
 * ENT: path	- directory to store module files
 * RET: error status
 */

char zVerse2::createModule(const char *ipath, int blockBound, int indxPerBlock)
{
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd, *fd2;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s/text.%czs", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/text.%czz", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/text.%czm", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/text.%czr", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();

	sprintf(buf, "%s/text.%czv", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd2 = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();

	VerseKey2 vk;
	vk.Headings(1);
	long offset = 0;
	short size = 0;
	for (vk = TOP; !vk.Error(); vk++) {
		//printf("vk %s %d\n", vk.getText(), vk.Index());
		write(fd->getFd(), &offset, 4);	//compBufIdxOffset
		write(fd->getFd(), &offset, 4);
		write(fd->getFd(), &size, 2);
		write(fd2->getFd(), &offset, 4);	//compBufIdxOffset
		write(fd2->getFd(), &offset, 4);
		write(fd2->getFd(), &size, 2);
	}

	FileMgr::getSystemFileMgr()->close(fd);
	FileMgr::getSystemFileMgr()->close(fd2);

	delete [] path;
	delete [] buf;
/*
	RawVerse rv(path);
	VerseKey2 mykey("Rev 22:21");
*/
	
	return 0;
}


/******************************************************************************
 * zVerse2::preptext	- Prepares the text before returning it to external
 *				objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *			text.
 */
#if 0 // should we really need this?
void zVerse2::prepText(SWBuf &buf) {
	unsigned int to, from; 
	char space = 0, cr = 0, realdata = 0, nlcnt = 0;
	char *rawBuf = buf.getRawData();
	for (to = from = 0; rawBuf[from]; from++) {
		switch (rawBuf[from]) {
		case 10:
			if (!realdata)
				continue;
			space = (cr) ? 0 : 1;
			cr = 0;
			nlcnt++;
			if (nlcnt > 1) {
//				*to++ = nl;
				rawBuf[to++] = 10;
//				*to++ = nl[1];
//				nlcnt = 0;
			}
			continue;
		case 13:
			if (!realdata)
				continue;
//			*to++ = nl[0];
			rawBuf[to++] = 10;
			space = 0;
			cr = 1;
			continue;
		}
		realdata = 1;
		nlcnt = 0;
		if (space) {
			space = 0;
			if (rawBuf[from] != ' ') {
				rawBuf[to++] = ' ';
				from--;
				continue;
			}
		}
		rawBuf[to++] = rawBuf[from];
	}
	buf.setSize(to);

	while (to > 1) {			// remove trailing excess
		to--;
		if ((rawBuf[to] == 10) || (rawBuf[to] == ' '))
			buf.setSize(to);
		else break;
	}
}
#endif

SWORD_NAMESPACE_END
