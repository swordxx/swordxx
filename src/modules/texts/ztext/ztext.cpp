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

#include <string.h>
#include <utilfuns.h>
#include <rawverse.h>
#include <ztext.h>


/******************************************************************************
 * zText Constructor - Initializes data for instance of zText
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		idisp - Display object to use for displaying
 */

zText::zText(char *ipath, char *iname, char *idesc, SWDisplay *idisp) : RawVerse(ipath), SWText(iname, idesc, idisp), SWCompress()
{
	versebuf = 0;
}


/******************************************************************************
 * zText Destructor - Cleans up instance of zText
 */

zText::~zText()
{
	if (versebuf)
		delete [] versebuf;
}


/******************************************************************************
 * zText::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

zText::operator char*()
{
	long  start;
	short size;
	char *tmpbuf;
	VerseKey lkey = *SWModule::key;
	char sizebuf[3];

	lkey.Verse(0);
	if (chapcache != lkey.Index()) {
		findoffset(lkey.Testament(), lkey.Index(), &start, &size);
		gettext(lkey.Testament(), start, 3, sizebuf);
		memcpy(&size, sizebuf, 2);
		tmpbuf = new char [ size + 1 ];
		gettext(lkey.Testament(), start + 2, size + 1 , tmpbuf);
		zBuf(&size, tmpbuf);
		chapcache = lkey.Index();
		delete [] tmpbuf;
	}
	
	findoffset(key->Testament(), key->Index(), &start, &size);

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ size + 1 ];
	memcpy(versebuf, Buf(), size);

	preptext(versebuf);

	return versebuf;
}
