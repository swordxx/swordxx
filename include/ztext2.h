/******************************************************************************
 *  ztext.h   - code for class 'zText2'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id$
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

#ifndef ZTEXT2_H
#define ZTEXT2_H

#include <zverse2.h>
#include <versekey2.h>
#include <rawstr.h>
#include <swtext.h>
//#include <swcomprs.h>

#include <defs.h>

namespace lucene { namespace index {
class IndexReader;
}}

namespace lucene { namespace search {
class IndexSearcher;
}}


SWORD_NAMESPACE_START

/*** SWModule implementation for compressed modules
* This class handles compressed modules.
* It should not be used in frontends, unless you are doing very special things.
*/
class SWDLLEXPORT zText2:public zVerse2, public SWText {

	VerseKey2 *lastWriteKey;
	bool sameBlock(VerseKey2 * lastWriteKey, VerseKey2 * key);
	int blockType;
	lucene::index::IndexReader *ir;
	lucene::search::IndexSearcher *is;
	VerseKey2 &getVerseKey();


public:
    
	zText2(const char *ipath, const char *iname = 0, const char *idesc = 0,
			int blockType = CHAPTERBLOCKS, int idxPerBlock = 10,
			SWCompress * icomp = 0,
			SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN,
			SWTextDirection dir = DIRECTION_LTR,
			SWTextMarkup markup = FMT_UNKNOWN, const char* ilang = 0);

	virtual ~zText2();
	virtual SWBuf &getRawEntryBuf();

	virtual void increment(int steps = 1);
	virtual void decrement(int steps = 1) { increment(-steps); }

  // write interface ----------------------------
	virtual bool isWritable() { return ((idxfp->getFd() > 0) && ((idxfp->mode & O_RDWR) == O_RDWR)); }
	static char createModule(const char *path, int blockBound, int indxPerBlock = 10) {
		return zVerse2::createModule(path, blockBound, indxPerBlock);
	}

     virtual void setEntry(const char *inbuf, long len = -1);	// Modify current module entry
     virtual void linkEntry(const SWKey *linkKey);	// Link current module entry to other module entry
     virtual void deleteEntry();	// Delete current module entry
  // end write interface ------------------------
  
	virtual void rawZFilter(SWBuf &buf, char direction = 0) { rawFilter(buf, (SWKey *)(long)direction); }// hack, use key as direction for enciphering

	// swcacher interface ----------------------
	virtual void flush() { flushCache(); }
	// end swcacher interface ----------------------

	virtual signed char createSearchFramework();
	virtual bool hasSearchFramework() { return true; }
	virtual ListKey &search(const char *istr, int searchType = 0, int flags = 0, SWKey * scope = 0, bool * justCheckIfSupported = 0, void (*percent)(char, void *) = &SWModule::nullPercent, void *percentUserData = 0);


	SWMODULE_OPERATORS

};

SWORD_NAMESPACE_END
#endif
