/*****************************************************************************
 * zstr.h   - code for class 'zStr'- a module that reads compressed text
 *			files.
 *			and provides lookup and parsing functions based on
 *			class StrKey
 *
 * $Id: zstr.h,v 1.2 2002/01/06 22:27:03 chrislit Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef ZSTR_H
#define ZSTR_H

#include <filemgr.h>

#include <defs.h>

class SWCompress;
class EntriesBlock;

class SWDLLEXPORT zStr {

private:
	static int instance;		// number of instantiated zStr objects or derivitives
	FileDesc *idxfd;
	FileDesc *datfd;
	FileDesc *zdxfd;
	FileDesc *zdtfd;
	EntriesBlock *cacheBlock;
	long cacheBlockIndex;
	bool cacheDirty;
	char *path;
	long lastoff;
	long blockCount;
	SWCompress *compressor;

protected:
	static const int IDXENTRYSIZE;
	static const int ZDXENTRYSIZE;

	void getCompressedText(long block, long entry, char **buf);
	void flushCache();
	void prepText(char *buf);
	void getKeyFromDatOffset(long ioffset, char **buf);
	void getKeyFromIdxOffset(long ioffset, char **buf);

public:
	char nl;
	zStr(const char *ipath, int fileMode = -1, long blockCount = 100, SWCompress *icomp = 0);
	virtual ~zStr();
	signed char findKeyIndex(const char *ikey, long *idxoff, long away = 0);
	void getText(long index, char **idxbuf, char **buf);
	void setText(const char *ikey, const char *buf, long len = 0);
	void linkEntry(const char *destkey, const char *srckey);
	static signed char createModule (const char *path);
};

#endif
