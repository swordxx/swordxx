/******************************************************************************
 *  rawtext.cpp - code for class 'RawText'- a module that reads raw text files:
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
#include <rawtext.h>


/******************************************************************************
 * RawText Constructor - Initializes data for instance of RawText
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawText::RawText(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : SWText(iname, idesc, idisp), RawVerse(ipath)
{
	versebuf = 0;
}


/******************************************************************************
 * RawText Destructor - Cleans up instance of RawText
 */

RawText::~RawText()
{
	if (versebuf)
		delete [] versebuf;
}


/******************************************************************************
 * RawText::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

RawText::operator char*()
{
	long  start;
	unsigned short size;
	VerseKey *key = 0;
	FilterList::iterator it;

	// see if we have a VerseKey * or decendant
	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {
	}
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ size * 3 ];

	gettext(key->Testament(), start, size + 1, versebuf);

	for (it = rawfilters.begin(); it != rawfilters.end(); it++) {
		(*it)->ProcessText(versebuf, size + 1, key);
	}

	preptext(versebuf);
	RenderText(versebuf, size * 3);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return versebuf;
}
