/******************************************************************************
 *  rawstr.cpp   - code for class 'RawStr'- a module that reads raw text
 *				files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *				and provides lookup and parsing functions based on
 *				class StrKey
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
#include <stdlib.h>
#include <utilfuns.h>
#include <rawstr.h>

#ifdef BIGENDIAN
#include <sys/pctypes.h>
#endif

/******************************************************************************
 * RawStr Statics
 */

int RawStr::instance = 0;


/******************************************************************************
 * RawStr Constructor - Initializes data for instance of RawStr
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 *		be sure to include the trailing separator (e.g. '/' or '\')
 *		(e.g. 'modules/texts/rawtext/webster/')
 */

RawStr::RawStr(const char *ipath)
{
	char buf[127];

	nl = '\n';
	lastoff = -1;
	path = 0;
	stdstr(&path, ipath);

#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.

	sprintf(buf, "%s.idx", path);
	idxfd = FileMgr::systemFileMgr.open(buf, O_RDONLY|O_BINARY);

	sprintf(buf, "%s.dat", path);
	if ((datfd = FileMgr::systemFileMgr.open(buf, O_RDONLY|O_BINARY)) < 0) {
		sprintf(buf, "Error: %d", errno);
		perror(buf);
	}

	instance++;
}


/******************************************************************************
 * RawStr Destructor - Cleans up instance of RawStr
 */

RawStr::~RawStr()
{
	if (path)
		delete [] path;

	--instance;

	FileMgr::systemFileMgr.close(idxfd);
	FileMgr::systemFileMgr.close(datfd);
}


/******************************************************************************
 * RawStr::getidxbufdat	- Gets the index string at the given idx offset
 *						NOTE: buf is allocated and must be freed by
 *							calling function
 *
 * ENT:	ioffset	- offset to lookup
 *		buf		- address of pointer to allocate for storage of string
 */

void RawStr::getidxbufdat(long ioffset, char **buf)
{
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
			(*buf)[size] = toupper((*buf)[size]);
	}
}


/******************************************************************************
 * RawStr::getidxbuf	- Gets the index string at the given idx offset
 *						NOTE: buf is allocated and must be freed by
 *							calling function
 *
 * ENT:	ioffset	- offset to lookup
 *		buf		- address of pointer to allocate for storage of string
 */

void RawStr::getidxbuf(long ioffset, char **buf)
{
	char *trybuf, *targetbuf;
	long offset;
	
	if (idxfd > 0) {
		lseek(idxfd->getFd(), ioffset, SEEK_SET);
		read(idxfd->getFd(), &offset, 4);
#ifdef BIGENDIAN
		offset = lelong(offset);
#endif
		getidxbufdat(offset, buf);
		for (trybuf = targetbuf = *buf; *trybuf; trybuf++, targetbuf++) {
/*
			if (*trybuf == '-') {		// ignore '-' because alphabetized silly in file
				targetbuf--;
				continue;
			}
*/
			*targetbuf = toupper(*trybuf);
		}
		*targetbuf = 0;
		trybuf = 0;
	}
}


/******************************************************************************
 * RawStr::findoffset	- Finds the offset of the key string from the indexes
 *
 * ENT:	key		- key string to lookup
 *		start	- address to store the starting offset
 *		size		- address to store the size of the entry
 *		away		- number of entries before of after to jump
 *					(default = 0)
 *
 * RET: error status
 */

char RawStr::findoffset(const char *ikey, long *start, unsigned short *size, long away)
{
	char *trybuf, *targetbuf, *key, quitflag = 0, retval = 0;
	long headoff, tailoff, tryoff = 0, maxoff = 0;

	if (*ikey) {
		headoff = 0;
		tailoff = maxoff = lseek(idxfd->getFd(), 0, SEEK_END) - 6;

		key = new char [ strlen(ikey) + 1 ];
		strcpy(key, ikey);

		for (trybuf = targetbuf = key; *trybuf; trybuf++, targetbuf++) {
/*
			if (*trybuf == '-') {		// ignore '-' because alphabetized silly in file
				targetbuf--;
				continue;
			}
*/
			*targetbuf = toupper(*trybuf);
		}
		*targetbuf = 0;
		trybuf = 0;

		while (headoff < tailoff) {
			tryoff = (lastoff == -1) ? headoff + ((((tailoff / 6) - (headoff / 6))) / 2) * 6 : lastoff; 
			lastoff = -1;
			getidxbuf(tryoff, &trybuf);

			if (!*trybuf) {		// In case of extra entry at end of idx
				tryoff += (tryoff > (maxoff / 2))?-6:6;
				retval = -1;
				break;
			}
					
			if (!strcmp(key, trybuf))
				break;

			int diff = strcmp(key, trybuf);
			if (diff < 0)
				tailoff = (tryoff == headoff) ? headoff : tryoff;
			else headoff = tryoff;
			if (tailoff == headoff + 6) {
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
	else	tryoff = 0;

	lseek(idxfd->getFd(), tryoff, SEEK_SET);
	read(idxfd->getFd(), start, 4);
	read(idxfd->getFd(), size, 2);

#ifdef BIGENDIAN
		*start = lelong(*start);
		*size  = leshort(*size);
#endif

	while (away) {
		long laststart = *start;
		unsigned short lastsize = *size;
		long lasttry = tryoff;
		tryoff += (away > 0) ? 6 : -6;
		
		if ((lseek(idxfd->getFd(), tryoff, SEEK_SET) < 0) || ((tryoff + (away*6)) < -6) || (tryoff + (away*6) > (maxoff+6))) {
			retval = -1;
			*start = laststart;
			*size = lastsize;
			tryoff = lasttry;
			break;
		}
		read(idxfd->getFd(), start, 4);
		read(idxfd->getFd(), size, 2);

#ifdef BIGENDIAN
		*start = lelong(*start);
		*size  = leshort(*size);
#endif

		if (((laststart != *start) || (lastsize != *size)) && (*start >= 0) && (*size)) 
			away += (away < 0) ? 1 : -1;
	}
	
	lastoff = tryoff;
	return retval;
}


/******************************************************************************
 * RawStr::preptext	- Prepares the text before returning it to external
 *					objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *				text.
 */

void RawStr::preptext(char *buf)
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


/******************************************************************************
 * RawStr::gettext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry
 *	buf	- buffer to store text
 *
 */

void RawStr::gettext(long start, unsigned short size, char *idxbuf, char *buf)
{
	char *ch;
	char *idxbuflocal = 0;
	getidxbufdat(start, &idxbuflocal);

	memset(buf, 0, size);
	lseek(datfd->getFd(), start, SEEK_SET);
	read(datfd->getFd(), buf, (int)(size - 1));

	for (ch = buf; *ch; ch++) {		// skip over index string
		if (*ch == 10) {
			ch++;
			break;
		}
	}
	memmove(buf, ch, strlen(ch) + 1);
	if (idxbuflocal) {
		int localsize = strlen(idxbuflocal);
		localsize = (localsize < (size - 1)) ? localsize : (size - 1);
		strncpy(idxbuf, idxbuflocal, localsize);
		idxbuf[localsize] = 0;
		free(idxbuflocal);
	}
}
