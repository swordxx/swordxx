/******************************************************************************
 *  hrefcom.cpp - code for class 'HREFCom'- a module that produces HTML HREFs
 *			pointing to actual text desired.  Uses standard
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
#include <hrefcom.h>

SWORD_NAMESPACE_START

 /******************************************************************************
 * HREFCom Constructor - Initializes data for instance of HREFCom
 *
 * ENT:	iname - Internal name for module
 *	iprefix - string to prepend to each HREF (e.g. "file://mods/com/jfb/")
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

HREFCom::HREFCom(const char *ipath, const char *iprefix, const char *iname, const char *idesc, SWDisplay *idisp) : RawVerse(ipath), SWCom(iname, idesc, idisp)
{
	prefix   = 0;
	stdstr(&prefix, iprefix);
}


/******************************************************************************
 * HREFCom Destructor - Cleans up instance of HREFCom
 */

HREFCom::~HREFCom()
{
	if (prefix)
		delete [] prefix;
}


/******************************************************************************
 * HREFCom::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

char *HREFCom::getRawEntry() {
	long  start;
	unsigned short size;
	char *tmpbuf;
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

	unsigned long newsize = ((size + 2) + strlen(prefix)) * FILTERPAD;
	if (newsize > entrybufallocsize) {
		if (entrybuf)
			delete [] entrybuf;
		entrybuf = new char [ newsize ];
		entrybufallocsize = newsize;
	}
	tmpbuf   = new char [ size + 10 ];

	readtext(key->Testament(), start, size + 2, tmpbuf);
	sprintf(entrybuf, "%s%s", prefix, tmpbuf);
	preptext(entrybuf);

	delete [] tmpbuf;

	if (key != this->key)
		delete key;

	return entrybuf;
}

SWORD_NAMESPACE_END
