/******************************************************************************
 *  zcom.h   - code for class 'zCom'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id: zcom.h,v 1.13 2002/03/14 16:56:49 scribe Exp $
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

#ifndef ZCOM_H
#define ZCOM_H

#include <rawverse.h>
#include <swcom.h>
#include <swcomprs.h>

#include <defs.h>

class SWDLLEXPORT zCom:public zVerse, public SWCom {
  VerseKey *lastWriteKey;
  bool sameBlock (VerseKey * lastWriteKey, VerseKey * key);
  int blockType;
public:
  
    
    zCom (const char *ipath, const char *iname = 0, const char *idesc =
	   0, int blockType = CHAPTERBLOCKS, SWCompress * icomp =
	   0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN,
           const char* ilang = 0);
    virtual ~ zCom ();
  virtual char *getRawEntry ();
  virtual SWModule & operator += (int increment);
  virtual SWModule & operator -= (int decrement)
  {
    return this->operator += (-decrement);
  }

  // write interface ----------------------------
	virtual bool isWritable () { return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & O_RDWR) == O_RDWR)); }
  static char createModule (const char *path, int blockBound)
  {
    return zVerse::createModule (path, blockBound);
  }
  virtual SWModule & setentry (const char *inbuf, long len);	// Modify current module entry
  virtual SWModule & operator << (const char *inbuf);	// Modify current module entry
  virtual SWModule & operator << (const SWKey * linkKey);	// Link current module entry to other module entry
  virtual void deleteEntry ();	// Delete current module entry
  // end write interface ------------------------
};


#endif
