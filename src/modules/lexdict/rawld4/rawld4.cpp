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
#include <rawstr4.h>
#include <rawld4.h>


 /******************************************************************************
 * RawLD Constructor - Initializes data for instance of RawLD
 *
 * ENT:	ipath	- path and filename of files (no extension)
 *		iname	- Internal name for module
 *		idesc	- Name to display to user for module
 *		idisp	- Display object to use for displaying
 */

RawLD4::RawLD4(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawStr4(ipath), SWLD(iname, idesc, idisp, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

RawLD4::~RawLD4()
{
}


/******************************************************************************
 * RawLD4::strongsPad	- Pads a key if it is 100% digits to 5 places
 *
 * ENT: buf -	buffer to check and pad
 */

void RawLD4::strongsPad(char *buf)
{
	const char *check;
	long size = 0;
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
 * RawLD4::getEntry	- Looks up entry from data file.  'Snaps' to closest
 *				entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char RawLD4::getEntry(long away)
{
	long  start = 0;
	unsigned long size = 0;
	char *idxbuf = 0;
	char retval = 0;

	char *buf = new char [ strlen(*key) + 6 ];
	strcpy(buf, *key);

	strongsPad(buf);

	*entrybuf = 0;
	if (!(retval = findoffset(buf, &start, &size, away))) {
		readtext(start, &size, &idxbuf, &entrybuf);
		rawFilter(entrybuf, size, 0);	// hack, decipher
		rawFilter(entrybuf, size, key);
		entrySize = size;        // support getEntrySize call
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
 * RawLD4::getRawEntry	- Returns the correct entry when char * cast
 *							is requested
 *
 * RET: string buffer with entry
 */

char *RawLD4::getRawEntry() {

	char ret = getEntry();
	if (!ret) {
		if (!isUnicode())
			preptext(entrybuf);
	}
	else error = ret;

	return entrybuf;
}


/******************************************************************************
 * RawLD4::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

void RawLD4::increment(int steps) {
	char tmperror;

	if (key->Traversable()) {
		*key += steps;
		error = key->Error();
		steps = 0;
	}
	
	tmperror = (getEntry(steps)) ? KEYERR_OUTOFBOUNDS : 0;
	error = (error)?error:tmperror;
	*key = entkeytxt;
}


void RawLD4::setEntry(const char *inbuf, long len) {
	setText(*key, inbuf, len);
}


void RawLD4::linkEntry(const SWKey *inkey) {
	linkentry(*key, *inkey);
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawLD4::deleteEntry() {
	setText(*key, "");
}
