/******************************************************************************
 *  rawcom4.cpp - code for class 'RawCom4'- a module that reads raw commentary
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#include <filemgr.h>
#include <rawverse4.h>
#include <rawcom4.h>
#include <versekey.h>

SWORD_NAMESPACE_START

 /******************************************************************************
 * RawCom4 Constructor - Initializes data for instance of RawCom4
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawCom4::RawCom4(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding encoding, SWTextDirection dir, SWTextMarkup markup, const char* ilang)
		: RawVerse4(ipath),
            SWCom(iname, idesc, idisp, encoding, dir, markup, ilang){
}


/******************************************************************************
 * RawCom4 Destructor - Cleans up instance of RawCom4
 */

RawCom4::~RawCom4()
{
}


bool RawCom4::isWritable() {
	return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}
/******************************************************************************
 * RawCom4::getRawEntry()	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

SWBuf &RawCom4::getRawEntryBuf() {
	long  start = 0;
	unsigned long size = 0;
	VerseKey *key = &getVerseKey();

	findOffset(key->Testament(), key->Index(), &start, &size);
	entrySize = size;        // support getEntrySize call

	entryBuf = "";
	readText(key->Testament(), start, size, entryBuf);

	rawFilter(entryBuf, 0);	// hack, decipher
	rawFilter(entryBuf, key);

//	if (!isUnicode())
		prepText(entryBuf);

	return entryBuf;
}


/******************************************************************************
 * RawCom4::increment	- Increments module key a number of entries
 *
 * ENT:	steps	- Number of entries to jump forward
 *
 * RET: *this
 */

void RawCom4::increment(int steps) {
	long  start;
	unsigned long size;
	VerseKey *tmpkey = &getVerseKey();

	findOffset(tmpkey->Testament(), tmpkey->Index(), &start, &size);

	SWKey lastgood = *tmpkey;
	while (steps) {
		long laststart = start;
		unsigned long lastsize = size;
		SWKey lasttry = *tmpkey;
		(steps > 0) ? (*key)++ : (*key)--;
		tmpkey = &getVerseKey();

		if ((error = key->Error())) {
			*key = lastgood;
			break;
		}
		long index = tmpkey->Index();
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


void RawCom4::setEntry(const char *inbuf, long len) {
	VerseKey *key = &getVerseKey();
	doSetText(key->Testament(), key->Index(), inbuf, len);
}


void RawCom4::linkEntry(const SWKey *inkey) {
	VerseKey *destkey = &getVerseKey();
	const VerseKey *srckey = 0;

	// see if we have a VerseKey * or decendant
	SWTRY {
		srckey = SWDYNAMIC_CAST(VerseKey, inkey);
	}
	SWCATCH ( ... ) {}
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	doLinkEntry(destkey->Testament(), destkey->Index(), srckey->Index());

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;
}


/******************************************************************************
 * RawCom4::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawCom4::deleteEntry() {

	VerseKey *key = &getVerseKey();
	doSetText(key->Testament(), key->Index(), "");
}


SWORD_NAMESPACE_END
