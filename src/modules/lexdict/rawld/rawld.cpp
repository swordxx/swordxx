/******************************************************************************
 *  rawld.cpp - code for class 'RawLD'- a module that reads raw lexicon and
 *				dictionary files: *.dat *.idx
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
#include <rawstr.h>
#include <rawld.h>


 /******************************************************************************
 * RawLD Constructor - Initializes data for instance of RawLD
 *
 * ENT:	ipath	- path and filename of files (no extension)
 *		iname	- Internal name for module
 *		idesc	- Name to display to user for module
 *		idisp	- Display object to use for displaying
 */

RawLD::RawLD(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : RawStr(ipath), SWLD(iname, idesc, idisp)
{
}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

RawLD::~RawLD()
{
}


/******************************************************************************
 * RawLD::strongsPad	- Pads a key if it is 100% digits to 5 places
 *
 * ENT: buf -	buffer to check and pad
 */

void RawLD::strongsPad(char *buf)
{
	const char *check;
	int size = 0;
	int len = strlen(buf);
	if ((len < 5) && (len > 0)) {
		for (check = buf; *check; check++) {
			if (!isdigit(*check))
				break;
			else size++;
		}

		if ((size == len) && size) 
			sprintf(buf, "%.5d", atoi(buf));
	}
}


/******************************************************************************
 * RawLD::getEntry	- Looks up entry from data file.  'Snaps' to closest
 *				entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char RawLD::getEntry(long away)
{
	long  start;
	unsigned short size;
	char *idxbuf = 0;
	char retval = 0;

	char *buf = new char [ strlen(*key) + 6 ];
	strcpy(buf, *key);

	strongsPad(buf);
	
	if (entrybuf)
		delete [] entrybuf;
	if (!(retval = findoffset(buf, &start, &size, away))) {
		entrybuf = new char [ size * 2 ];	// give filters x2 buffer space
		idxbuf   = new char [ size + 1 ];

		gettext(start, size + 1, idxbuf, entrybuf);
		if (!key->Persist())			// If we have our own key
			*key = idxbuf;				// reset it to entry index buffer

		stdstr(&entkeytxt, idxbuf);	// set entry key text that module 'snapped' to.
		delete [] idxbuf;
	}
	else {
		entrybuf = new char [ 5 ];
		*entrybuf = 0;
	}
	delete [] buf;
	return retval;
}


/******************************************************************************
 * RawLD::operator char *	- Returns the correct entry when char * cast
 *							is requested
 *
 * RET: string buffer with entry
 */

RawLD::operator char*()
{
	getEntry();
	preptext(entrybuf);
	RenderText(entrybuf, strlen(entrybuf) * 2);

	return entrybuf;
}


/******************************************************************************
 * RawLD::operator +=	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWModule &RawLD::operator +=(int increment)
{
	char tmperror;

	if (key->Traversable()) {
		*key += increment;
		error = key->Error();
		increment = 0;
	}
	
	tmperror = (getEntry(increment)) ? KEYERR_OUTOFBOUNDS : 0;
	error = (error)?error:tmperror;
	*key = entkeytxt;
	return *this;
}


/******************************************************************************
 * RawLD::operator =(POSITION)	- Positions this key if applicable
 */

SWModule &RawLD::operator =(POSITION p)
{
	if (!key->Traversable()) {
		switch (p) {
		case POS_TOP:
			*key = "";
			break;
		case POS_BOTTOM:
			*key = "zzzzzzzzz";
			break;
		} 
	}
	else	*key = p;
	return *this;
}
