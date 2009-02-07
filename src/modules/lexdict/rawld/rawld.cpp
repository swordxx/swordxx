/******************************************************************************
 *  rawld.cpp - code for class 'RawLD'- a module that reads raw lexicon and
 *				dictionary files: *.dat *.idx
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#include <utilstr.h>
#include <rawstr.h>
#include <rawld.h>
#include <filemgr.h>

SWORD_NAMESPACE_START

 /******************************************************************************
 * RawLD Constructor - Initializes data for instance of RawLD
 *
 * ENT:	ipath	- path and filename of files (no extension)
 *		iname	- Internal name for module
 *		idesc	- Name to display to user for module
 *		idisp	- Display object to use for displaying
 */

RawLD::RawLD(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawStr(ipath), SWLD(iname, idesc, idisp, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

RawLD::~RawLD()
{
}


bool RawLD::isWritable() {
	 return ((idxfd->getFd() > 0) && ((idxfd->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawLD::strongsPad	- Pads a key if (it-1) is 100% digits to 5 places
 *						allows for final to be alpha, e.g. '123B'
 *
 * ENT: buf -	buffer to check and pad
 */

void RawLD::strongsPad(char *buf)
{
	char *check;
	int size = 0;
	int len = strlen(buf);
	char subLet = 0;
	bool bang = false;
	if ((len < 8) && (len > 0)) {
		for (check = buf; *(check+1); check++) {
			if (!isdigit(*check))
				break;
			else size++;
		}

		if (size && ((size == (len-1)) || (size == (len-2)))) {
			if (*check == '!') {
				bang = true;
				check++;
			}
			if (isalpha(*check)) {
				subLet = toupper(*check);
				*(check-(bang?1:0)) = 0;
			}
			sprintf(buf, "%.5d", atoi(buf));
			if (subLet) {
				check = buf+(strlen(buf));
				if (bang) {
					*check++ = '!';
				}
				*check++ = subLet;
				*check = 0;
			}
		}
	}
}


/******************************************************************************
 * RawLD::getEntry	- Looks up entry from data file.  'Snaps' to closest
 *				entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char RawLD::getEntry(long away)
{
	long  start = 0;
	unsigned short size = 0;
	char *idxbuf = 0;
	char retval = 0;

	char *buf = new char [ strlen(*key) + 6 ];
	strcpy(buf, *key);

	strongsPad(buf);

	if (!(retval = findOffset(buf, &start, &size, away))) {
		readText(start, &size, &idxbuf, entryBuf);
		rawFilter(entryBuf, 0);	// hack, decipher
		rawFilter(entryBuf, key);
		entrySize = size;        // support getEntrySize call
		if (!key->Persist())			// If we have our own key
			*key = idxbuf;				// reset it to entry index buffer

		stdstr(&entkeytxt, idxbuf);	// set entry key text that module 'snapped' to.
		delete [] idxbuf;
	}
	else {
		entryBuf = "";
	}
		
	delete [] buf;
	return retval;
}


/******************************************************************************
 * RawLD::getRawEntry	- Returns the correct entry when char * cast
 *							is requested
 *
 * RET: string buffer with entry
 */

SWBuf &RawLD::getRawEntryBuf() {

	char ret = getEntry();
	if (!ret) {
//		if (!isUnicode())
			prepText(entryBuf);
	}
	else error = ret;

	return entryBuf;
}


/******************************************************************************
 * RawLD::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

void RawLD::increment(int steps) {
	char tmperror;

	if (key->isTraversable()) {
		*key += steps;
		error = key->Error();
		steps = 0;
	}
	
	tmperror = (getEntry(steps)) ? KEYERR_OUTOFBOUNDS : 0;
	error = (error)?error:tmperror;
	*key = entkeytxt;
}


void RawLD::setEntry(const char *inbuf, long len) {
	doSetText(*key, inbuf, len);
}


void RawLD::linkEntry(const SWKey *inkey) {
	doLinkEntry(*key, *inkey);
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawLD::deleteEntry() {
	doSetText(*key, "");
}

SWORD_NAMESPACE_END
