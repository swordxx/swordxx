/******************************************************************************
 *  rawcom.cpp - code for class 'zCom'- a module that reads raw commentary
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <zverse.h>
#include <zcom.h>


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

/******************************************************************************
 * zCom::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */
char *zCom::getRawEntry() {
	long  start = 0;
	unsigned short size = 0;
	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);
	entrySize = size;        // support getEntrySize call

	unsigned long newsize = (size + 2) * FILTERPAD;
	if (newsize > entrybufallocsize) {
		if (entrybuf)
			delete [] entrybuf;
		entrybuf = new char [ newsize ];
		entrybufallocsize = newsize;
	}
	*entrybuf = 0;

	swgettext(key->Testament(), start, (size + 2), entrybuf);

	rawFilter(entrybuf, size, key);

	if (!isUnicode())
		preptext(entrybuf);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return entrybuf;
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

SWModule &zCom::setentry(const char *inbuf, long len) {
	VerseKey *key = 0;
	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);


	// see if we've jumped across blocks since last write
	if (lastWriteKey) {
		if (!sameBlock(lastWriteKey, key)) {
			flushCache();
		}
		delete lastWriteKey;
	}

	settext(key->Testament(), key->Index(), inbuf, len);

	lastWriteKey = (VerseKey *)key->clone();	// must delete

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return *this;
}

SWModule &zCom::operator <<(const char *inbuf) {
        return setentry(inbuf, 0);
}


SWModule &zCom::operator <<(const SWKey *inkey) {
	VerseKey *destkey = 0;
	const VerseKey *srckey = 0;
	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		destkey = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!destkey)
		destkey = new VerseKey(this->key);

	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		srckey = (const VerseKey *) SWDYNAMIC_CAST(VerseKey, inkey);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {
	}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	linkentry(destkey->Testament(), destkey->Index(), srckey->Index());

	if (this->key != destkey) // free our key if we created a VerseKey
		delete destkey;

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;

	return *this;
}

/******************************************************************************
 * zCom::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void zCom::deleteEntry() {

	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	settext(key->Testament(), key->Index(), "");

	if (key != this->key)
		delete key;
}


/******************************************************************************
 * zCom::operator +=	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWModule &zCom::operator +=(int increment) {
	long  start;
	unsigned short size;
	VerseKey *tmpkey = 0;

#ifndef _WIN32_WCE
	try {
#endif
		tmpkey = SWDYNAMIC_CAST(VerseKey, key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!tmpkey)
		tmpkey = new VerseKey(key);

	findoffset(tmpkey->Testament(), tmpkey->Index(), &start, &size);

	while (increment) {
		long laststart = start;
		unsigned short lastsize = size;
		SWKey lasttry = *tmpkey;
		(increment > 0) ? (*key)++ : (*key)--;
		if (tmpkey != key)
			delete tmpkey;
		tmpkey = 0;
#ifndef _WIN32_WCE
		try {
#endif
			tmpkey = SWDYNAMIC_CAST(VerseKey, key);
#ifndef _WIN32_WCE
		}
		catch ( ... ) {}
#endif
		if (!tmpkey)
			tmpkey = new VerseKey(key);

		if ((error = key->Error())) {
			*key = lasttry;
			break;
		}
		findoffset(tmpkey->Testament(), tmpkey->Index(), &start, &size);
		if (((laststart != start) || (lastsize != size)) && (start >= 0) && (size)) 
			increment += (increment < 0) ? 1 : -1;
	}
	error = (error) ? KEYERR_OUTOFBOUNDS : 0;

	if (tmpkey != key)
		delete tmpkey;

	return *this;
}

