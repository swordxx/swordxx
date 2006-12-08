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

#include <utilstr.h>
#include <versekey.h>
#include <zverse.h>
#include <sysdata.h>
#include <swbuf.h>
#include <filemgr.h>
#include <swcomprs.h>


SWORD_NAMESPACE_START

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
 *		fileMode - open mode for the files (FileMgr::RDONLY, etc.)
 *		blockType - verse, chapter, book, etc.
 */

zVerse::zVerse(const char *ipath, int fileMode, int blockType, SWCompress *icomp)
{
	// this line, instead of just defaulting, to keep FileMgr out of header
	if (fileMode == -1) fileMode = FileMgr::RDONLY;

	SWBuf buf;

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
		fileMode = FileMgr::RDWR;
	}
		
	buf.setFormatted("%s/ot.%czs", path, uniqueIndexID[blockType]);
	idxfp[0] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);

	buf.setFormatted("%s/nt.%czs", path, uniqueIndexID[blockType]);
	idxfp[1] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);

	buf.setFormatted("%s/ot.%czz", path, uniqueIndexID[blockType]);
	textfp[0] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);

	buf.setFormatted("%s/nt.%czz", path, uniqueIndexID[blockType]);
	textfp[1] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);

	buf.setFormatted("%s/ot.%czv", path, uniqueIndexID[blockType]);
	compfp[0] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);

	buf.setFormatted("%s/nt.%czv", path, uniqueIndexID[blockType]);
	compfp[1] = FileMgr::getSystemFileMgr()->open(buf, fileMode, true);
	
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
		FileMgr::getSystemFileMgr()->close(idxfp[loop1]);
		FileMgr::getSystemFileMgr()->close(textfp[loop1]);
		FileMgr::getSystemFileMgr()->close(compfp[loop1]);
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

void zVerse::findOffset(char testmt, long idxoff, long *start, unsigned short *size)
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

	*start = *size = 0;
	//printf ("Finding offset %ld\n", idxoff);
	idxoff *= 10;
	if (!testmt) {
		testmt = ((idxfp[0]) ? 1:2);
	}
	
	// assert we have and valid file descriptor
	if (compfp[testmt-1]->getFd() < 1)
		return;
		
	long newOffset = compfp[testmt-1]->seek(idxoff, SEEK_SET);
	if (newOffset == idxoff) {
		if (compfp[testmt-1]->read(&ulBuffNum, 4) != 4) {
			printf ("Error reading ulBuffNum\n");
			return;
		}
	}
	else return;

	ulBuffNum = swordtoarch32(ulBuffNum);

	if (compfp[testmt-1]->read(&ulVerseStart, 4) < 2)
	{
		printf ("Error reading ulVerseStart\n");
		return;
	}
	if (compfp[testmt-1]->read(&usVerseSize, 2) < 2)
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


		if (idxfp[testmt-1]->seek(ulBuffNum*12, SEEK_SET)!=(long) ulBuffNum*12)
		{
			printf ("Error seeking compressed file index\n");
			return;
		}
		if (idxfp[testmt-1]->read(&ulCompOffset, 4)<4)
		{
			printf ("Error reading ulCompOffset\n");
			return;
		}
		if (idxfp[testmt-1]->read(&ulCompSize, 4)<4)
		{
			printf ("Error reading ulCompSize\n");
			return;
		}
		if (idxfp[testmt-1]->read(&ulUnCompSize, 4)<4)
		{
			printf ("Error reading ulUnCompSize\n");
			return;
		}

		ulCompOffset  = swordtoarch32(ulCompOffset);
		ulCompSize  = swordtoarch32(ulCompSize);
		ulUnCompSize  = swordtoarch32(ulUnCompSize);

		if (textfp[testmt-1]->seek(ulCompOffset, SEEK_SET)!=(long)ulCompOffset)
		{
			printf ("Error: could not seek to right place in compressed text\n");
			return;
		}
		SWBuf pcCompText;
		pcCompText.setSize(ulCompSize+5);

		if (textfp[testmt-1]->read(pcCompText.getRawData(), ulCompSize)<(long)ulCompSize) {
			printf ("Error reading compressed text\n");
			return;
		}
		pcCompText.setSize(ulCompSize);
		rawZFilter(pcCompText, 0); // 0 = decipher
		
		compressor->zBuf(&ulCompSize, pcCompText.getRawData());

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

void zVerse::zReadText(char testmt, long start, unsigned short size, SWBuf &inBuf) {
	inBuf = "";
	if ( (size > 0) && cacheBuf && ((unsigned)(start+size) <= strlen(cacheBuf)) ){ //TODO: optimize this, remove strlen
		inBuf.setFillByte(0);
		inBuf.setSize(size+1);
		strncpy(inBuf.getRawData(), &(cacheBuf[start]), size);
		inBuf.setSize(strlen(inBuf.c_str()));
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

void zVerse::doSetText(char testmt, long idxoff, const char *buf, long len) {

	len = (len < 0) ? strlen(buf) : len;
	if (!testmt) 
		testmt = ((idxfp[0]) ? 1:2);
	if ((!dirtyCache) || (cacheBufIdx < 0)) {
		cacheBufIdx = idxfp[testmt-1]->seek(0, SEEK_END) / 12;
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

	compfp[testmt-1]->seek(idxoff, SEEK_SET);
	compfp[testmt-1]->write(&outBufIdx, 4);
	compfp[testmt-1]->write(&outstart, 4);
	compfp[testmt-1]->write(&outsize, 2);
	strcat(cacheBuf, buf);
}


void zVerse::flushCache() {
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
				rawZFilter(buf, 1); // 1 = encipher

				start = outstart = textfp[cacheTestament-1]->seek(0, SEEK_END);

				outstart  = archtosword32(start);
				outsize   = archtosword32(size);
				outzsize  = archtosword32(zsize);

				textfp[cacheTestament-1]->write(buf, zsize);

				idxfp[cacheTestament-1]->seek(idxoff, SEEK_SET);
				idxfp[cacheTestament-1]->write(&outstart, 4);
				idxfp[cacheTestament-1]->write(&outzsize, 4);
				idxfp[cacheTestament-1]->write(&outsize, 4);
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

void zVerse::doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
	long bufidx;
	long start;
	unsigned short size;

	destidxoff *= 10;
	srcidxoff  *= 10;

	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	// get source
	compfp[testmt-1]->seek(srcidxoff, SEEK_SET);
	compfp[testmt-1]->read(&bufidx, 4);
	compfp[testmt-1]->read(&start, 4);
	compfp[testmt-1]->read(&size, 2);

	// write dest
	compfp[testmt-1]->seek(destidxoff, SEEK_SET);
	compfp[testmt-1]->write(&bufidx, 4);
	compfp[testmt-1]->write(&start, 4);
	compfp[testmt-1]->write(&size, 2);
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
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/nt.%czs", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/ot.%czz", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/nt.%czz", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd2 = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd2->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/ot.%czv", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd->getFd();

	sprintf(buf, "%s/nt.%czv", path, uniqueIndexID[blockBound]);
	FileMgr::removeFile(buf);
	fd2 = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
	fd2->getFd();

	VerseKey vk;
	vk.Headings(1);
	long offset = 0;
	short size = 0;
	for (vk = TOP; !vk.Error(); vk++) {
		if (vk.Testament() == 1) {
			fd->write(&offset, 4);	//compBufIdxOffset
			fd->write(&offset, 4);
			fd->write(&size, 2);
		}
		else {
			fd2->write(&offset, 4);	//compBufIdxOffset
			fd2->write(&offset, 4);
			fd2->write(&size, 2);
		}
	}

	FileMgr::getSystemFileMgr()->close(fd);
	FileMgr::getSystemFileMgr()->close(fd2);

	delete [] path;
	delete [] buf;
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

void zVerse::prepText(SWBuf &buf) {
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


SWORD_NAMESPACE_END
