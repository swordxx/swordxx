/******************************************************************************
 *  zverse2.h   - code for class 'zVerse2'- a module that reads raw text
 *			files:  text using indexs ??.bks ??.cps ??.vss
 *			and provides lookup and parsing functions based on
 *			class VerseKey2
 */

#ifndef ZVERSE2_H
#define ZVERSE2_H

#include <filemgr.h>
#include <fcntl.h>
//#include <rawverse.h>
#include <swcomprs.h>

#include <defs.h>

SWORD_NAMESPACE_START

class SWDLLEXPORT zVerse2 {
	SWCompress *compressor;

protected:
	static int instance;		// number of instantiated zVerse2 objects or derivitives

	FileDesc *idxfp;
	FileDesc *textfp;
	FileDesc *compfp;
	FileDesc *markupfp;
	FileDesc *midxfp;
	char *path;
	#if 0
	void prepText(SWBuf &buf);
	#endif
	void doSetText(long idxoff, const char *buf, long len = 0);
	void doLinkEntry(long destidxoff, long srcidxoff);
	void flushCache();
	char *cacheBuf;
	long cacheBufIdx;
	bool dirtyCache;
	int m_idxPerBlock;

public:

#define	VERSEBLOCKS 2
#define	CHAPTERBLOCKS 3
#define	BOOKBLOCKS 4
#define	INDEXBLOCKS 5

	static const char uniqueIndexID[];
	char nl;


	zVerse2(const char *ipath, int fileMode = O_RDONLY, int blockType = CHAPTERBLOCKS, 
		SWCompress * icomp = 0, int indexesPerBlock = 10);
	virtual ~zVerse2();
	void findOffsetText(long idxoff, long *start, unsigned short *end);
	void findOffsetMarkup(long idxoff, long *start, unsigned short *end);
	void zReadText(long start, unsigned short size, SWBuf &buf);
	#if 0
	virtual void rawZFilter(SWBuf &buf, char direction = 0) {}
	#endif
	static char createModule(const char *path, int blockBound, int indxPerBlock);
};

SWORD_NAMESPACE_END
#endif
