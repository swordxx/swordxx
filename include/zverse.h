/******************************************************************************
 *  rawverse.h   - code for class 'RawVerse'- a module that reads raw text
 *			files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *			and provides lookup and parsing functions based on
 *			class VerseKey
 */

#ifndef ZVERSE_H
#define ZVERSE_H

#include <filemgr.h>
#include <fcntl.h>
#include <rawverse.h>
#include <swcomprs.h>

#include <defs.h>

class SWDLLEXPORT zVerse {
	SWCompress *compressor;

protected:
	static int instance;		// number of instantiated RawVerse objects or derivitives

	FileDesc *idxfp[2];
	FileDesc *textfp[2];
	FileDesc *compfp[2];
	char *path;
	void preptext (char *buf);
	void settext (char testmt, long idxoff, const char *buf, long len = 0);
	void linkentry (char testmt, long destidxoff, long srcidxoff);
	void flushCache ();
	//RawVerse();
	char *cacheBuf;
	char cacheTestament;
	long cacheBufIdx;
	bool dirtyCache;

public:

#define	VERSEBLOCKS 2
#define	CHAPTERBLOCKS 3
#define	BOOKBLOCKS 4

	static const char uniqueIndexID[];
	char nl;


	zVerse (const char *ipath, int fileMode = O_RDONLY, int blockType = CHAPTERBLOCKS, SWCompress * icomp = 0);
	virtual ~ zVerse ();
	void findoffset (char testmt, long idxoff, long *start, unsigned short *end);
	void zreadtext (char testmt, long start, unsigned short size, char *buf);
	static char createModule (const char *path, int blockBound);
};


#endif
