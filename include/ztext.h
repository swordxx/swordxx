/******************************************************************************
 *  ztext.h   - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id: ztext.h,v 1.19 2002/03/04 17:52:24 scribe Exp $
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

#include <defs.h>

/*** SWModule implementation for compressed modules
* This class handles compressed modules.
* It should not be used in frontends, unless you are doing very special things.
*/
class SWDLLEXPORT zText:public zVerse, public SWText {
     VerseKey *lastWriteKey;
     bool sameBlock (VerseKey * lastWriteKey, VerseKey * key);
	int blockType;
public:
  
    
	zText(const char *ipath, const char *iname = 0, const char *idesc =
	   0, int blockType = CHAPTERBLOCKS, SWCompress * icomp =
	   0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN,
           const char* ilang = 0);
	virtual ~zText ();
 	virtual char *getRawEntry();

     virtual SWModule & operator += (int increment);
     virtual SWModule & operator -= (int decrement) { return this->operator += (-decrement); }

  // write interface ----------------------------
	virtual bool isWritable() { return true; }
	static char createModule (const char *path, int blockBound) {
		return zVerse::createModule (path, blockBound);
	}

     virtual SWModule & setentry (const char *inbuf, long len);	// Modify current module entry
     virtual SWModule & operator << (const char *inbuf);	// Modify current module entry
     virtual SWModule & operator << (const SWKey * linkKey);	// Link current module entry to other module entry
     virtual void deleteEntry ();	// Delete current module entry
  // end write interface ------------------------
};

#endif
