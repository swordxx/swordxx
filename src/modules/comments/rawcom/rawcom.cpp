/******************************************************************************
 *  rawcom.cpp - code for class 'RawCom'- a module that reads raw commentary
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
#include <rawverse.h>
#include <rawcom.h>


 /******************************************************************************
 * RawCom Constructor - Initializes data for instance of RawCom
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawCom::RawCom(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding encoding, SWTextDirection dir, SWTextMarkup markup, const char* ilang)
		: RawVerse(ipath),
            SWCom(iname, idesc, idisp, encoding, dir, markup, ilang){
}


/******************************************************************************
 * RawCom Destructor - Cleans up instance of RawCom
 */

RawCom::~RawCom()
{
}


/******************************************************************************
 * RawCom::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

char *RawCom::getRawEntry() {
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

	gettext(key->Testament(), start, (size + 2), entrybuf);

	rawFilter(entrybuf, size, key);

        if (!isUnicode())
		preptext(entrybuf);

	if (key != this->key)
		delete key;

	return entrybuf;
}


/******************************************************************************
 * RawCom::operator +=	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWModule &RawCom::operator +=(int increment)
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

SWModule &RawCom::setentry(const char *inbuf, long len) {
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

	settext(key->Testament(), key->Index(), inbuf, len);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return *this;
}

SWModule &RawCom::operator <<(const char *inbuf) {
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

	settext(key->Testament(), key->Index(), inbuf);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return *this;
}


SWModule &RawCom::operator <<(const SWKey *inkey) {
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
		srckey = SWDYNAMIC_CAST(VerseKey, inkey);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
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
 * RawCom::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawCom::deleteEntry() {

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
