/******************************************************************************
 *  rawverse.h   - code for class 'RawVerse'- a module that reads raw text
 *			files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *			and provides lookup and parsing functions based on
 *			class VerseKey
 */

#ifndef RAWVERSE_H
#define RAWVERSE_H

#include <filemgr.h>
#include <fcntl.h>

class RawVerse {
	static int instance;	// number of instantiated RawVerse objects or derivitives
	FileDesc *idxfp[2];
	FileDesc *textfp[2];

protected:
	char *path;
	void preptext(char *buf);
	void settext(char testmt, long idxoff, const char *buf);
	
public:
	char nl;
	RawVerse(const char *ipath, int fileMode = O_RDONLY);
	virtual ~RawVerse();
	void findoffset(char testmt, long idxoff, long *start, unsigned short *end);
	void gettext(char testmt, long start, unsigned short size, char *buf);
	static char CreateModule(char *path);
};


#endif
