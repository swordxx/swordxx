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

RawCom::RawCom(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : RawVerse(ipath), SWCom(iname, idesc, idisp)
{
	versebuf = 0;
}


/******************************************************************************
 * RawCom Destructor - Cleans up instance of RawCom
 */

RawCom::~RawCom()
{
	if (versebuf)
		delete [] versebuf;
}


/******************************************************************************
 * RawCom::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

RawCom::operator char*()
{
	long  start;
	unsigned short size;
	VerseKey *key = 0;
	FilterList::iterator it;

	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);


	findoffset(key->Testament(), key->Index(), &start, &size);

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ ++size * 3 ];

	gettext(key->Testament(), start, size, versebuf);

	for (it = rawfilters.begin(); it != rawfilters.end(); it++) {
		(*it)->ProcessText(versebuf, size, key);
	}

	preptext(versebuf);
	RenderText(versebuf, size * 3);

	if (key != this->key)
		delete key;

	return versebuf;
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

	try {
		tmpkey = dynamic_cast<VerseKey *>(key);
	}
	catch ( ... ) {}
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
		try {
			tmpkey = dynamic_cast<VerseKey *>(key);
		}
		catch ( ... ) {}
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

