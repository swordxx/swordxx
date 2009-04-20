/******************************************************************************
 *  ztext.cpp - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
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
#include <sysdata.h>
#include <versekey.h>
#include <filemgr.h>

#include <ztext.h>

#include <regex.h>	// GNU


SWORD_NAMESPACE_START

/******************************************************************************
 * zText Constructor - Initializes data for instance of zText
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		iblockType - verse, chapter, book, etc. of index chunks
 *		icomp - Compressor object
 *		idisp - Display object to use for displaying
 */

zText::zText(const char *ipath, const char *iname, const char *idesc, int iblockType, SWCompress *icomp, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char *ilang, const char *versification)
		: zVerse(ipath, FileMgr::RDWR, iblockType, icomp), SWText(iname, idesc, idisp, enc, dir, mark, ilang, versification) {
	blockType = iblockType;
	lastWriteKey = 0;
}


/******************************************************************************
 * zText Destructor - Cleans up instance of zText
 */

zText::~zText()
{
	flushCache();

	if (lastWriteKey)
		delete lastWriteKey;

}


bool zText::isWritable() { return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR)); }


/******************************************************************************
 * zText::getRawEntry	- Returns the current verse buffer
 *
 * RET: buffer with verse
 */

SWBuf &zText::getRawEntryBuf() {
	long  start = 0;
	unsigned short size = 0;
	unsigned long buffnum = 0;
	VerseKey &key = getVerseKey();

	findOffset(key.Testament(), key.TestamentIndex(), &start, &size, &buffnum);
	entrySize = size;        // support getEntrySize call
			  
	entryBuf = "";
	
	zReadText(key.Testament(), start, size, buffnum, entryBuf);
	rawFilter(entryBuf, &key);

//	if (!isUnicode())
		prepText(entryBuf);

	return entryBuf;
}


bool zText::sameBlock(VerseKey *k1, VerseKey *k2) {
	if (k1->Testament() != k2->Testament())
		return false;

	switch (blockType) {
	case VERSEBLOCKS:
		if (k1->Verse() != k2->Verse())
			return false;
	case CHAPTERBLOCKS:
		if (k1->Chapter() != k2->Chapter())
			return false;
	case BOOKBLOCKS:
		if (k1->Book() != k2->Book())
			return false;
	}
	return true;
}


void zText::setEntry(const char *inbuf, long len) {
	VerseKey &key = getVerseKey();

	// see if we've jumped across blocks since last write
	if (lastWriteKey) {
		if (!sameBlock(lastWriteKey, &key)) {
			flushCache();
		}
		delete lastWriteKey;
	}

	doSetText(key.Testament(), key.TestamentIndex(), inbuf, len);

	lastWriteKey = (VerseKey *)key.clone();	// must delete
}


void zText::linkEntry(const SWKey *inkey) {
	VerseKey &destkey = getVerseKey();
	const VerseKey *srckey = &getVerseKey(inkey);
	doLinkEntry(destkey.Testament(), destkey.TestamentIndex(), srckey->TestamentIndex());
}


/******************************************************************************
 * zFiles::deleteEntry	- deletes this entry
 *
 */

void zText::deleteEntry() {

	VerseKey &key = getVerseKey();

	doSetText(key.Testament(), key.TestamentIndex(), "");
}


/******************************************************************************
 * zText::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 */

void zText::increment(int steps) {
	long start;
	unsigned short size;
	unsigned long buffnum;
	VerseKey *tmpkey = &getVerseKey();

	findOffset(tmpkey->Testament(), tmpkey->TestamentIndex(), &start, &size, &buffnum);

	SWKey lastgood = *tmpkey;
	while (steps) {
		long laststart = start;
		unsigned short lastsize = size;
		SWKey lasttry = *tmpkey;
		(steps > 0) ? ++(*key) : --(*key);
		tmpkey = &getVerseKey();

		if ((error = key->Error())) {
			*key = lastgood;
			break;
		}
		long index = tmpkey->TestamentIndex();
		findOffset(tmpkey->Testament(), index, &start, &size, &buffnum);

		if (
			(((laststart != start) || (lastsize != size))	// we're a different entry
//				&& (start > 0)
				&& (size))	// and we actually have a size
				||(!skipConsecutiveLinks)) {	// or we don't want to skip consecutive links
			steps += (steps < 0) ? 1 : -1;
			lastgood = *tmpkey;
		}
	}
	error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

bool zText::isLinked(const SWKey *k1, const SWKey *k2) const {
	long start1, start2;
	unsigned short size1, size2;
	unsigned long buffnum1, buffnum2;
	VerseKey *vk1 = &getVerseKey(k1);
	VerseKey *vk2 = &getVerseKey(k2);
	if (vk1->Testament() != vk2->Testament()) return false;

	findOffset(vk1->Testament(), vk1->TestamentIndex(), &start1, &size1, &buffnum1);
	findOffset(vk2->Testament(), vk2->TestamentIndex(), &start2, &size2, &buffnum2);
	return start1 == start2 && buffnum1 == buffnum2;
}

bool zText::hasEntry(const SWKey *k) const {
	long start;
	unsigned short size;
	unsigned long buffnum;
	VerseKey *vk = &getVerseKey(k);

	findOffset(vk->Testament(), vk->TestamentIndex(), &start, &size, &buffnum);
	return size;
}


SWORD_NAMESPACE_END
