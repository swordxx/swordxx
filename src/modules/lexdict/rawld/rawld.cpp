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

RawLD::RawLD(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, char enc, char dir, char mark, const char* ilang) : RawStr(ipath), SWLD(iname, idesc, idisp, enc, dir, mark, ilang)
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
	long  start = 0;
	unsigned short size = 0;
	char *idxbuf = 0;
	char retval = 0;

	char *buf = new char [ strlen(*key) + 6 ];
	strcpy(buf, *key);

	strongsPad(buf);

	*entrybuf = 0;
	if (!(retval = findoffset(buf, &start, &size, away))) {
		entrySize = size;        // support getEntrySize call
		if (entrybuf)
			delete [] entrybuf;
		entrybuf = new char [ ++size * FILTERPAD ];
		idxbuf   = new char [ size * FILTERPAD ];

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

char *RawLD::getRawEntry() {
	if (!getEntry() && !isUnicode()) {
		preptext(entrybuf);
	}

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
 * RawLD::operator =(SW_POSITION)	- Positions this key if applicable
 */

SWModule &RawLD::operator =(SW_POSITION p)
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


SWModule &RawLD::setentry(const char *inbuf, long len) {
	settext(*key, inbuf, len);

	return *this;
}

SWModule &RawLD::operator <<(const char *inbuf) {
	return setentry(inbuf, 0);
}


SWModule &RawLD::operator <<(const SWKey *inkey) {
	linkentry(*key, *inkey);

	return *this;
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawLD::deleteEntry() {
	settext(*key, "");
}
