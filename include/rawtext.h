/******************************************************************************
 *  rawtext.h   - code for class 'RawText'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
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

#ifndef RAWTEXT_H
#define RAWTEXT_H

#include <rawverse.h>
#include <rawstr.h>
#include <swtext.h>

#include <defs.h>

SWORD_NAMESPACE_START

class SWDLLEXPORT RawText : public SWText, public RawVerse {

	VerseKey &getVerseKey();
	RawStr *fastSearch[2];

public:
  
	RawText(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN,
	const char* ilang = 0);
	virtual ~RawText();
	virtual SWBuf &getRawEntryBuf();
	virtual void increment(int steps = 1);
	virtual void decrement(int steps = 1) { increment(-steps); }
	virtual signed char createSearchFramework(
			void (*percent) (char, void *) = &nullPercent,
			void *percentUserData = 0);
	virtual void deleteSearchFramework();
	virtual bool hasSearchFramework() { return true; }
	virtual ListKey &search(const char *istr, int searchType = 0, int flags = 0, SWKey * scope = 0, bool * justCheckIfSupported = 0, void (*percent)(char, void *) = &SWModule::nullPercent, void *percentUserData = 0);
	// write interface ----------------------------
	virtual bool isWritable() { return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & O_RDWR) == O_RDWR)); }
	static char createModule(const char *path) { return RawVerse::createModule(path); }
	virtual void setEntry(const char *inbuf, long len = -1);	// Modify current module entry
	virtual void linkEntry(const SWKey *linkKey);	// Link current module entry to other module entry
	virtual void deleteEntry();	// Delete current module entry
	// end write interface ------------------------

	SWMODULE_OPERATORS

};

SWORD_NAMESPACE_END
#endif
