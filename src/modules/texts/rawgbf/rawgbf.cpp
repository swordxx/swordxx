/******************************************************************************
 *  rawgbf.cpp - code for class 'RawGBF'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
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
#include <rawgbf.h>


/******************************************************************************
 * RawGBF Constructor - Initializes data for instance of RawGBF
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawGBF::RawGBF(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : SWText(iname, idesc, idisp), RawVerse(ipath)
{
	versebuf = 0;
}


/******************************************************************************
 * RawGBF Destructor - Cleans up instance of RawGBF
 */

RawGBF::~RawGBF()
{
	if (versebuf)
		delete [] versebuf;
    this->SWText::~SWText();
    this->RawVerse::~RawVerse();
}


/******************************************************************************
 * RawGBF::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

RawGBF::operator char*()
{
	long  start;
	unsigned short size;
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
	versebuf = new char [ size * 3 ];		// extra for conversion to RTF or other.

	gettext(key->Testament(), start, size + 1, versebuf);
	preptext(versebuf);
	RenderText(versebuf, size * 3);
	
	if (key != this->key)
		delete key;

	return versebuf;
}
