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
	versebuf = 0;
	prefix   = 0;
	stdstr(&prefix, iprefix);
}


/******************************************************************************
 * HREFCom Destructor - Cleans up instance of HREFCom
 */

HREFCom::~HREFCom()
{
	if (versebuf)
		delete [] versebuf;
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

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ size + strlen(prefix) * FILTERPAD ];
	tmpbuf   = new char [ size + 1 ];

	gettext(key->Testament(), start, size + 1, tmpbuf);
	sprintf(versebuf, "%s%s", prefix, tmpbuf);
	preptext(versebuf);

	delete [] tmpbuf;

	if (key != this->key)
		delete key;

	return versebuf;
}
