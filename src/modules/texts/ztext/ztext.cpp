/******************************************************************************
 *  ztext.cpp - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <iostream.h>
#include <string.h>
#include <utilfuns.h>
//#include <rawverse.h>
#include <ztext.h>
//#include <zlib.h>


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

zText::zText(const char *ipath, const char *iname, const char *idesc, int iblockType, SWCompress *icomp, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : zVerse(ipath, -1, iblockType, icomp), SWText(iname, idesc, idisp, enc, dir, mark, ilang)/*, SWCompress()*/
{
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


/******************************************************************************
 * zText::getRawEntry	- Returns the current verse buffer
 *
 * RET: buffer with verse
 */

char *zText::getRawEntry()
{
/*
	long  start;
	unsigned long size;
	unsigned long destsize;
	char *tmpbuf;
	char *dest;
	VerseKey *lkey = (VerseKey *) SWModule::key;
	char sizebuf[3];

	lkey->Verse(0);
	if (chapcache != lkey->Index()) {
		findoffset(lkey->Testament(), lkey->Index(), &start, &((unsigned short) size));
		gettext(lkey->Testament(), start, 3, sizebuf);
		memcpy(&size, sizebuf, 2);
		tmpbuf = new char [ size + 1 ];
		gettext(lkey->Testament(), start + 2, size + 1 , tmpbuf);
		//zBuf(&size, tmpbuf);
		dest = new char [ (size*4) + 1 ];
		uncompress((Bytef *)dest, &destsize, (Bytef *) tmpbuf, size);
		chapcache = lkey->Index();
		delete [] tmpbuf;
	}

	//findoffset(key->Testament(), key->Index(), &start, &size);
	findoffset(lkey->Testament(), lkey->Index(), &start, &((unsigned short) size));

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ size + 1 ];
	//memcpy(versebuf, Buf(), size);
	memcpy(versebuf, dest, destsize);
	delete [] dest;

	preptext(versebuf);

	return versebuf;
*/

	long  start = 0;
	unsigned short size = 0;
	VerseKey *key = 0;

	//printf ("zText char *\n");

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

	//printf ("checking cache\n");
	//printf ("finding offset\n");
	findoffset(key->Testament(), key->Index(), &start, &size);
	entrySize = size;        // support getEntrySize call

	//printf ("deleting previous buffer\n");
	unsigned long newsize = (size + 2) * FILTERPAD;
	if (newsize > entrybufallocsize) {
		if (entrybuf)
			delete [] entrybuf;
		entrybuf = new char [ newsize ];
		entrybufallocsize = newsize;
	}
	*entrybuf = 0;

	//printf ("getting text\n");
	swgettext(key->Testament(), start, (size + 2), entrybuf);
	//printf ("got text\n");

	rawFilter(entrybuf, size, key);

	//printf ("preparing text\n");
        if (!isUnicode())
		preptext(entrybuf);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	//printf ("returning text\n");
	return entrybuf;

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


SWModule &zText::setentry(const char *inbuf, long len) {
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

SWModule &zText::operator <<(const char *inbuf) {
        return setentry(inbuf, 0);
}


SWModule &zText::operator <<(const SWKey *inkey) {
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
 * zFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void zText::deleteEntry() {

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
 * zText::operator +=	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWModule &zText::operator +=(int increment)
{
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

	SWKey lastgood = *tmpkey;
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
			*key = lastgood;
			break;
		}
		long index = tmpkey->Index();
		findoffset(tmpkey->Testament(), index, &start, &size);
		if ((((laststart != start) || (lastsize != size))||(!skipConsecutiveLinks)) && (start >= 0) && (size)) {
			increment += (increment < 0) ? 1 : -1;
			lastgood = *tmpkey;
		}
	}
	error = (error) ? KEYERR_OUTOFBOUNDS : 0;

	if (tmpkey != key)
		delete tmpkey;

	return *this;
}
