/******************************************************************************
 *  ztext.h   - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id: ztext.h,v 1.8 2000/12/15 09:59:16 scribe Exp $
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

#ifndef ZTEXT_H
#define ZTEXT_H

#include <zverse.h>
#include <swtext.h>
//#include <swcomprs.h>

class zText : public zVerse, public SWText {
	char *versebuf;
	VerseKey *lastWriteKey;
	bool sameBlock(VerseKey *lastWriteKey, VerseKey *key);
	int blockType;
public:
	zText(const char *ipath, const char *iname = 0, const char *idesc = 0, int blockType = CHAPTERBLOCKS, SWCompress *icomp = 0, SWDisplay *idisp = 0);
	virtual ~zText();
	virtual char *getRawEntry();

	// write interface ----------------------------
	virtual bool isWritable() { return true; }
	static char createModule(const char *path, int blockBound) { return zVerse::createModule(path, blockBound); }
	virtual SWModule &operator <<(const char *inbuf);    // Modify current module entry
	virtual SWModule &operator <<(const SWKey *linkKey); // Link current module entry to other module entry
	virtual void deleteEntry(); // Delete current module entry
	// end write interface ------------------------
};


#endif
