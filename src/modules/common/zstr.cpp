/******************************************************************************
 *  zstr.cpp   - code for class 'zStr'- a module that reads compressed text
 *		files and provides lookup and parsing functions based on
 *		class StrKey
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <utilfuns.h>
#include <zstr.h>
#include <swcomprs.h>

#include <sysdata.h>
#include <entriesblk.h>

/******************************************************************************
 * zStr Statics
 */

int zStr::instance = 0;
const int zStr::IDXENTRYSIZE = 8;
const int zStr::ZDXENTRYSIZE = 8;


/******************************************************************************
 * zStr Constructor - Initializes data for instance of zStr
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 */

zStr::zStr(const char *ipath, int fileMode, long blockCount, SWCompress *icomp) {
	char buf[127];

	nl = '\n';
	lastoff = -1;
	path = 0;
	stdstr(&path, ipath);

	compressor = (icomp) ? icomp : new SWCompress();
	this->blockCount = blockCount;
#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.

	if (fileMode == -1) { // try read/write if possible
		fileMode = O_RDWR;
	}
		
	sprintf(buf, "%s.idx", path);
	idxfd = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s.dat", path);
	datfd = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s.zdx", path);
	zdxfd = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s.zdt", path);
	zdtfd = FileMgr::systemFileMgr.open(buf, fileMode|O_BINARY, true);

	if (datfd <= 0) {
		sprintf(buf, "Error: %d", errno);
		perror(buf);
	}

	cacheBlock = 0;
	cacheBlockIndex = -1;
	cacheDirty = false;

	instance++;
}


/******************************************************************************
 * zStr Destructor - Cleans up instance of zStr
 */

zStr::~zStr() {

	flushCache();

	if (path)
		delete [] path;

	--instance;

	FileMgr::systemFileMgr.close(idxfd);
	FileMgr::systemFileMgr.close(datfd);
	FileMgr::systemFileMgr.close(zdxfd);
	FileMgr::systemFileMgr.close(zdtfd);


	if (compressor)
		delete compressor;

}


/******************************************************************************
 * zStr::getidxbufdat	- Gets the index string at the given dat offset
 *				NOTE: buf is calloc'd, or if not null, realloc'd and must
 *					be free'd by calling function
 *
 * ENT:	ioffset	- offset in dat file to lookup
 *		buf		- address of pointer to allocate for storage of string
 */

void zStr::getKeyFromDatOffset(long ioffset, char **buf) {
	int size;
	char ch;
	if (datfd > 0) {
		lseek(datfd->getFd(), ioffset, SEEK_SET);
		for (size = 0; read(datfd->getFd(), &ch, 1) == 1; size++) {
			if ((ch == '\\') || (ch == 10) || (ch == 13))
				break;
		}
		*buf = (*buf) ? (char *)realloc(*buf, size + 1) : (char *)malloc(size + 1);
		if (size) {
			lseek(datfd->getFd(), ioffset, SEEK_SET);
			read(datfd->getFd(), *buf, size);
		}
		(*buf)[size] = 0;
		for (size--; size > 0; size--)
			(*buf)[size] = SW_toupper((*buf)[size]);
	}
	else {
		*buf = (*buf) ? (char *)realloc(*buf, 1) : (char *)malloc(1);
		**buf = 0;
	}
}


/******************************************************************************
 * zStr::getidxbuf	- Gets the index string at the given idx offset
 *						NOTE: buf is calloc'd, or if not null, realloc'd
 *							and must be freed by calling function
 *
 * ENT:	ioffset	- offset in idx file to lookup
 *		buf		- address of pointer to allocate for storage of string
 */

void zStr::getKeyFromIdxOffset(long ioffset, char **buf) {
	__u32 offset;
	
	if (idxfd > 0) {
		lseek(idxfd->getFd(), ioffset, SEEK_SET);
		read(idxfd->getFd(), &offset, sizeof(__u32));
		offset = swordtoarch32(offset);
		getKeyFromDatOffset(offset, buf);
	}
}


/******************************************************************************
 * zStr::findoffset	- Finds the offset of the key string from the indexes
 *
 * ENT:	key		- key string to lookup
 *		offset	- address to store the starting offset
 *		size		- address to store the size of the entry
 *		away		- number of entries before of after to jump
 *					(default = 0)
 *
 * RET: error status
 */

signed char zStr::findKeyIndex(const char *ikey, long *idxoff, long away) {
	char *trybuf = 0, *key = 0, quitflag = 0;
	signed char retval = 0;
	__s32 headoff, tailoff, tryoff = 0, maxoff = 0;
	__u32 start, size;

	if (idxfd->getFd() >= 0) {
		tailoff = maxoff = lseek(idxfd->getFd(), 0, SEEK_END) - IDXENTRYSIZE;
		if (*ikey) {
			headoff = 0;
			stdstr(&key, ikey);
			toupperstr(key);

			while (headoff < tailoff) {
				tryoff = (lastoff == -1) ? headoff + (((((tailoff / IDXENTRYSIZE) - (headoff / IDXENTRYSIZE))) / 2) * IDXENTRYSIZE) : lastoff;
				lastoff = -1;

				getKeyFromIdxOffset(tryoff, &trybuf);

				if (!*trybuf && tryoff) {		// In case of extra entry at end of idx (not first entry)
					tryoff += (tryoff > (maxoff / 2))?-IDXENTRYSIZE:IDXENTRYSIZE;
					retval = -1;
					break;
				}

				int diff = strcmp(key, trybuf);
				if (!diff)
					break;

				if (diff < 0)
					tailoff = (tryoff == headoff) ? headoff : tryoff;
				else headoff = tryoff;
				if (tailoff == headoff + IDXENTRYSIZE) {
					if (quitflag++)
						headoff = tailoff;
				}
			}
			if (headoff >= tailoff)
				tryoff = headoff;
			if (trybuf)
				free(trybuf);
			delete [] key;
		}
		else	{ tryoff = 0; }

		lseek(idxfd->getFd(), tryoff, SEEK_SET);

		start = size = 0;
		retval = (read(idxfd->getFd(), &start, sizeof(__u32))==sizeof(__u32)) ? retval : -1;
		retval = (read(idxfd->getFd(), &size, sizeof(__u32))==sizeof(__u32)) ? retval : -1;
		start = swordtoarch32(start);
		size  = swordtoarch32(size);

		if (idxoff)
			*idxoff = tryoff;

		while (away) {
			__u32 laststart = start;
			__u32 lastsize = size;
			__s32 lasttry = tryoff;
			tryoff += (away > 0) ? IDXENTRYSIZE : -IDXENTRYSIZE;

			bool bad = false;
			if (((long)(tryoff + (away*IDXENTRYSIZE)) < -IDXENTRYSIZE) || (tryoff + (away*IDXENTRYSIZE) > (maxoff+IDXENTRYSIZE)))
				bad = true;
			else	if (lseek(idxfd->getFd(), tryoff, SEEK_SET) < 0)
				bad = true;
			if (bad) {
				retval = -1;
				start = laststart;
				size = lastsize;
				tryoff = lasttry;
				if (idxoff)
					*idxoff = tryoff;
				break;
			}
			read(idxfd->getFd(), &start, sizeof(__u32));
			read(idxfd->getFd(), &size, sizeof(__u32));
			start = swordtoarch32(start);
			size  = swordtoarch32(size);

			if (idxoff)
				*idxoff = tryoff;


			if (((laststart != start) || (lastsize != size)) && (start >= 0) && (size)) 
				away += (away < 0) ? 1 : -1;
		}
	
		lastoff = tryoff;
	}
	else {
		if (idxoff)
			*idxoff = 0;
		retval = -1;
	}
	return retval;
}


/******************************************************************************
 * zStr::preptext	- Prepares the text before returning it to external
 *					objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *				text.
 */

void zStr::prepText(char *buf) {
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

	while (to > (buf+1)) {			// remove trailing excess
		to--;
		if ((*to == 10) || (*to == ' '))
			*to = 0;
		else break;
	}
}


/******************************************************************************
 * zStr::getText	- gets text at a given offset
 *
 * ENT:
 *	offset	- idxoffset where the key is located.
 *	buf		- buffer to store text
 *	idxbuf	- buffer to store index key
 *			NOTE: buffer will be alloc'd / realloc'd and 
 *			should be free'd by the client
 *
 */

void zStr::getText(long offset, char **idxbuf, char **buf) {
	char *ch;
	char *idxbuflocal = 0;
	getKeyFromIdxOffset(offset, &idxbuflocal);
	__u32 start;
	__u32 size;

	do {
		lseek(idxfd->getFd(), offset, SEEK_SET);
		read(idxfd->getFd(), &start, sizeof(__u32));
		read(idxfd->getFd(), &size, sizeof(__u32));
		start = swordtoarch32(start);
		size = swordtoarch32(size);

		*buf = (*buf) ? (char *)realloc(*buf, size + 1) : (char *)malloc(size + 1);
		*idxbuf = (*idxbuf) ? (char *)realloc(*idxbuf, size + 1) : (char *)malloc(size + 1);
		memset(*buf, 0, size + 1);
		memset(*idxbuf, 0, size + 1);
		lseek(datfd->getFd(), start, SEEK_SET);
		read(datfd->getFd(), *buf, (int)(size));

		for (ch = *buf; *ch; ch++) {		// skip over index string
			if (*ch == 10) {
				ch++;
				break;
			}
		}
		memmove(*buf, ch, size - (unsigned long)(ch-*buf));

		// resolve link
		if (!strncmp(*buf, "@LINK", 5)) {
			for (ch = *buf; *ch; ch++) {		// null before nl
				if (*ch == 10) {
					*ch = 0;
					break;
				}
			}
			findKeyIndex(*buf + IDXENTRYSIZE, &offset);
		}
		else break;
	}
	while (true);	// while we're resolving links

	if (idxbuflocal) {
		__u32 localsize = strlen(idxbuflocal);
		localsize = (localsize < (size - 1)) ? localsize : (size - 1);
		strncpy(*idxbuf, idxbuflocal, localsize);
		(*idxbuf)[localsize] = 0;
		free(idxbuflocal);
	}
	__u32 block = 0;
	__u32 entry = 0;
	memmove(&block, *buf, sizeof(__u32));
	memmove(&entry, *buf + sizeof(__u32), sizeof(__u32));
	block = swordtoarch32(block);
	entry = swordtoarch32(entry);
	getCompressedText(block, entry, buf);
}


/******************************************************************************
 * zStr::getCompressedText - Get text entry from a compressed index / zdata
 * 		file.
 */

void zStr::getCompressedText(long block, long entry, char **buf) {

	__u32 size = 0;

	if (cacheBlockIndex != block) {
		__u32 start = 0;

		lseek(zdxfd->getFd(), block * ZDXENTRYSIZE, SEEK_SET);
		read(zdxfd->getFd(), &start, sizeof(__u32));
		read(zdxfd->getFd(), &size, sizeof(__u32));
		start = swordtoarch32(start);
		size = swordtoarch32(size);

		*buf = (*buf) ? (char *)realloc(*buf, size + 1) : (char *)malloc(size + 1);

		lseek(zdtfd->getFd(), start, SEEK_SET);
		read(zdtfd->getFd(), *buf, size);

		flushCache();

		unsigned long len = size;
		compressor->zBuf(&len, *buf);
		char * rawBuf = compressor->Buf(0, &len);
		cacheBlock = new EntriesBlock(rawBuf, len);
		cacheBlockIndex = block;
	}
	size = cacheBlock->getEntrySize(entry);
	*buf = (*buf) ? (char *)realloc(*buf, size + 1) : (char *)malloc(size + 1);
	strcpy(*buf, cacheBlock->getEntry(entry));
}


/******************************************************************************
 * zLD::settext	- Sets text for current offset
 *
 * ENT: key	- key for this entry
 *	buf	- buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void zStr::setText(const char *ikey, const char *buf, long len) {

	__u32 start, outstart;
	__u32 size, outsize;
	__s32 endoff;
	long idxoff = 0;
	__s32 shiftSize;
	static const char nl[] = {13, 10};
	char *tmpbuf = 0;
	char *key = 0;
	char *dbKey = 0;
	char *idxBytes = 0;
	char *outbuf = 0;
	char *ch = 0;

	stdstr(&key, ikey);
	toupperstr(key);

	char notFound = findKeyIndex(ikey, &idxoff, 0);
	if (!notFound) {
		getKeyFromIdxOffset(idxoff, &dbKey);
		int diff = strcmp(key, dbKey);
		if (diff < 0) {
		}
		else if (diff > 0) {
			idxoff += IDXENTRYSIZE;
		}
		else if ((!diff) && (len || strlen(buf) /*we're not deleting*/)) { // got absolute entry
			do {
				lseek(idxfd->getFd(), idxoff, SEEK_SET);
				read(idxfd->getFd(), &start, sizeof(__u32));
				read(idxfd->getFd(), &size, sizeof(__u32));
				start = swordtoarch32(start);
				size = swordtoarch32(size);

				tmpbuf = new char [ size + 2 ];
				memset(tmpbuf, 0, size + 2);
				lseek(datfd->getFd(), start, SEEK_SET);
				read(datfd->getFd(), tmpbuf, size);

				for (ch = tmpbuf; *ch; ch++) {		// skip over index string
					if (*ch == 10) {
						ch++;
						break;
					}
				}
				memmove(tmpbuf, ch, size - (unsigned long)(ch-tmpbuf));

				// resolve link
				if (!strncmp(tmpbuf, "@LINK", 5) && (len ? len : strlen(buf))) {
					for (ch = tmpbuf; *ch; ch++) {		// null before nl
						if (*ch == 10) {
							*ch = 0;
							break;
						}
					}
					findKeyIndex(tmpbuf + IDXENTRYSIZE, &idxoff);
					delete [] tmpbuf;
				}
				else break;
			}
			while (true);	// while we're resolving links
		}
	}

	endoff = lseek(idxfd->getFd(), 0, SEEK_END);

	shiftSize = endoff - idxoff;

	if (shiftSize > 0) {
	        idxBytes = new char [ shiftSize ];
		lseek(idxfd->getFd(), idxoff, SEEK_SET);
		read(idxfd->getFd(), idxBytes, shiftSize);
	}

	outbuf = new char [ (len ? len : strlen(buf)) + strlen(key) + 5 ];
	sprintf(outbuf, "%s%c%c", key, 13, 10);
	size = strlen(outbuf);
	if (len ? len : strlen(buf)) {	// NOT a link
		if (!cacheBlock) {
			flushCache();
			cacheBlock = new EntriesBlock();
			cacheBlockIndex = (lseek(zdxfd->getFd(), 0, SEEK_END) / ZDXENTRYSIZE);
		}
		else if (cacheBlock->getCount() >= blockCount) {
			flushCache();
			cacheBlock = new EntriesBlock();
			cacheBlockIndex = (lseek(zdxfd->getFd(), 0, SEEK_END) / ZDXENTRYSIZE);
		}
		__u32 entry = cacheBlock->addEntry(buf);
		cacheDirty = true;
		outstart = archtosword32(cacheBlockIndex);
		outsize = archtosword32(entry);
		memcpy (outbuf + size, &outstart, sizeof(__u32));
		memcpy (outbuf + size + sizeof(__u32), &outsize, sizeof(__u32));
		size += (sizeof(__u32) * 2);
	}
	else {	// link
		memcpy(outbuf + size, buf, len ? len : strlen(buf));
		size += (len ? len : strlen(buf));
	}

	start = lseek(datfd->getFd(), 0, SEEK_END);

	outstart = archtosword32(start);
	outsize  = archtosword32(size);

	lseek(idxfd->getFd(), idxoff, SEEK_SET);
	if (len ? len : strlen(buf)) {
		lseek(datfd->getFd(), start, SEEK_SET);
		write(datfd->getFd(), outbuf, size);

		// add a new line to make data file easier to read in an editor
		write(datfd->getFd(), &nl, 2);
		
		write(idxfd->getFd(), &outstart, sizeof(__u32));
		write(idxfd->getFd(), &outsize, sizeof(__u32));
		if (idxBytes) {
			write(idxfd->getFd(), idxBytes, shiftSize);
		}
	}
	else {	// delete entry
		if (idxBytes) {
			write(idxfd->getFd(), idxBytes+IDXENTRYSIZE, shiftSize-IDXENTRYSIZE);
			lseek(idxfd->getFd(), -1, SEEK_CUR);	// last valid byte
			FileMgr::systemFileMgr.trunc(idxfd);	// truncate index
		}
	}

	if (idxBytes)
		delete [] idxBytes;
	delete [] key;
	delete [] outbuf;
	free(dbKey);
}


/******************************************************************************
 * zLD::linkentry	- links one entry to another
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	destidxoff	- dest offset into .vss
 *	srcidxoff		- source offset into .vss
 */

void zStr::linkEntry(const char *destkey, const char *srckey) {
	char *text = new char [ strlen(destkey) + 7 ];
	sprintf(text, "@LINK %s", destkey);
	setText(srckey, text);
	delete [] text;
}


void zStr::flushCache() {
	if (cacheBlock) {
		if (cacheDirty) {
			__u32 start = 0;
			unsigned long size = 0;
			__u32 outstart = 0, outsize = 0;

			const char *rawBuf = cacheBlock->getRawData(&size);
			compressor->Buf(rawBuf, &size);
			compressor->zBuf(&size);

			long zdxSize = lseek(zdxfd->getFd(), 0, SEEK_END);
			long zdtSize = lseek(zdtfd->getFd(), 0, SEEK_END);

			if ((cacheBlockIndex * ZDXENTRYSIZE) > (zdxSize - ZDXENTRYSIZE)) {	// New Block
				start = zdtSize;
			}
			else {
				lseek(zdxfd->getFd(), cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
				read(zdxfd->getFd(), &start, sizeof(__u32));
				read(zdxfd->getFd(), &outsize, sizeof(__u32));
				start = swordtoarch32(start);
				outsize = swordtoarch32(outsize);
				if (start + outsize >= zdtSize) {	// last entry, just overwrite
					// start is already set
				}
				else	if (size < outsize) {	// middle entry, but smaller, that's fine and let's preserve bigger size
					size = outsize;
				}
				else {	// middle and bigger-- we have serious problems, for now let's put it at the end = lots of wasted space
					start = zdtSize;
				}
			}



			outstart = archtosword32(start);
			outsize  = archtosword32((__u32)size);

			lseek(zdxfd->getFd(), cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
			lseek(zdtfd->getFd(), start, SEEK_SET);
			rawBuf = compressor->zBuf(&size);
			write(zdtfd->getFd(), rawBuf, size);

			// add a new line to make data file easier to read in an editor
			write(zdtfd->getFd(), &nl, 2);
			
			write(zdxfd->getFd(), &outstart, sizeof(__u32));
			write(zdxfd->getFd(), &outsize, sizeof(__u32));

			delete cacheBlock;
		}
	}
	cacheBlockIndex = -1;
	cacheBlock = 0;
	cacheDirty = false;
}


/******************************************************************************
 * zLD::CreateModule	- Creates new module files
 *
 * ENT: path	- directory to store module files
 * RET: error status
 */

signed char zStr::createModule(const char *ipath) {
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd, *fd2;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s.dat", path);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::systemFileMgr.close(fd);

	sprintf(buf, "%s.idx", path);
	unlink(buf);
	fd2 = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();
	FileMgr::systemFileMgr.close(fd2);

	sprintf(buf, "%s.zdt", path);
	unlink(buf);
	fd2 = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();
	FileMgr::systemFileMgr.close(fd2);

	sprintf(buf, "%s.zdx", path);
	unlink(buf);
	fd2 = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();
	FileMgr::systemFileMgr.close(fd2);

	delete [] path;
	
	return 0;
}
