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

class zVerse /*: public RawVerse*/ {
SWCompress *compressor;

protected:
	static int instance;	// number of instantiated RawVerse objects or derivitives

	FileDesc *idxfp[2];
	FileDesc *textfp[2];
	FileDesc *compfp[2];
	char *path;
	void preptext(char *buf);
	void swsettext(char testmt, long idxoff, const char *buf);
	//RawVerse();
	long ulcache;

public:
	static const int VERSEBLOCKS;
	static const int CHAPTERBLOCKS;
	static const int BOOKBLOCKS;
	char nl;
	zVerse(const char *ipath, int fileMode = O_RDONLY, int blockType = CHAPTERBLOCKS, SWCompress *icomp = 0);
	virtual ~zVerse();
	void findoffset(char testmt, long idxoff, long *start, unsigned short *end);
	void swgettext(char testmt, long start, unsigned short size, char *buf);
	//static char CreateModule(char *path);
};


#endif
