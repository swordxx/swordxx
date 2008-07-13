/******************************************************************************
 *  rawcom.cpp - code for class 'zCom'- a module that reads raw commentary
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#include <zverse.h>
#include <versekey.h>
#include <zcom.h>
#include <filemgr.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * zCom Constructor - Initializes data for instance of zCom
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		iblockType - verse, chapter, book, etc. of index chunks
 *		icomp - Compressor object
 *		idisp - Display object to use for displaying
 */

zCom::zCom(const char *ipath, const char *iname, const char *idesc, int iblockType, SWCompress *icomp, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : zVerse(ipath, -1, iblockType, icomp), SWCom(iname, idesc, idisp, enc, dir, mark, ilang)/*, SWCompress()*/
{
	blockType = iblockType;
	lastWriteKey = 0;
}

/******************************************************************************
 * zCom Destructor - Cleans up instance of zCom
 */

zCom::~zCom() {
	flushCache();

	if (lastWriteKey)
		delete lastWriteKey;
}


bool zCom::isWritable() {
	return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * zCom::getRawEntry	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */
SWBuf &zCom::getRawEntryBuf() {
	long  start = 0;
	unsigned short size = 0;
	VerseKey *key = &getVerseKey();

	findOffset(key->Testament(), key->TestamentIndex(), &start, &size);
	entrySize = size;        // support getEntrySize call

	entryBuf = "";
	zReadText(key->Testament(), start, size, entryBuf);

	rawFilter(entryBuf, key);

//	if (!isUnicode())
		prepText(entryBuf);

	return entryBuf;
}


bool zCom::sameBlock(VerseKey *k1, VerseKey *k2) {
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

void zCom::setEntry(const char *inbuf, long len) {
	VerseKey *key = &getVerseKey();

	// see if we've jumped across blocks since last write
	if (lastWriteKey) {
		if (!sameBlock(lastWriteKey, key)) {
			flushCache();
		}
		delete lastWriteKey;
	}

	doSetText(key->Testament(), key->TestamentIndex(), inbuf, len);

	lastWriteKey = (VerseKey *)key->clone();	// must delete
}


void zCom::linkEntry(const SWKey *inkey) {
	VerseKey *destkey = &getVerseKey();
	const VerseKey *srckey = 0;

	// see if we have a VerseKey * or decendant
	SWTRY {
		srckey = (const VerseKey *) SWDYNAMIC_CAST(VerseKey, inkey);
	}
	SWCATCH ( ... ) {
	}
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	doLinkEntry(destkey->Testament(), destkey->TestamentIndex(), srckey->TestamentIndex());

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;
}

/******************************************************************************
 * zCom::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void zCom::deleteEntry() {

	VerseKey *key = &getVerseKey();
	doSetText(key->Testament(), key->TestamentIndex(), "");
}


/******************************************************************************
 * zCom::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

void zCom::increment(int steps) {
	long  start;
	unsigned short size;
	VerseKey *tmpkey = &getVerseKey();

	findOffset(tmpkey->Testament(), tmpkey->TestamentIndex(), &start, &size);

	SWKey lastgood = *tmpkey;
	while (steps) {
		long laststart = start;
		unsigned short lastsize = size;
		SWKey lasttry = *tmpkey;
		(steps > 0) ? (*key)++ : (*key)--;
		tmpkey = &getVerseKey();

		if ((error = key->Error())) {
			*key = lastgood;
			break;
		}
		long index = tmpkey->TestamentIndex();
		findOffset(tmpkey->Testament(), index, &start, &size);
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


SWORD_NAMESPACE_END
