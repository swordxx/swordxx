/******************************************************************************
 *  zcom.cpp - code for class 'zCom'- a module that reads compressed text
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
#include <zcom.h>


/******************************************************************************
 * zCom Constructor - Initializes data for instance of zCom
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		idisp - Display object to use for displaying
 */

zCom::zCom(char *ipath, char *iname, char *idesc, SWDisplay *idisp) : RawVerse(ipath), SWCom(iname, idesc, idisp), SWCompress()
{
}


/******************************************************************************
 * zCom Destructor - Cleans up instance of zCom
 */

zCom::~zCom()
{
}


/******************************************************************************
 * zCom::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

zCom::operator char*()
{
	long  start;
	unsigned short size;
	char *tmpbuf;
	VerseKey *key = 0;

	try {
		key = (VerseKey *)SWDYNAMIC_CAST(VerseKey, this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);


	findoffset(key->Testament(), key->Index(), &start, &size);

	tmpbuf = new char [ size ];

	gettext(key->Testament(), start, size, tmpbuf);
	zBuf((unsigned long*)&size, tmpbuf);
	preptext(Buf());

	delete [] tmpbuf;

	if (key != this->key)
		delete key;

	return Buf();
}
