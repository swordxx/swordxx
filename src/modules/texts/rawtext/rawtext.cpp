/******************************************************************************
 *  rawtext.cpp - code for class 'RawText'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <stdio.h>
#include <fcntl.h>
#include <sysdata.h>

#include <utilstr.h>
#include <rawverse.h>
#include <rawtext.h>
#include <rawstr.h>
#include <filemgr.h>
#include <versekey.h>
#include <stringmgr.h>

#include <regex.h>	// GNU
#include <map>
#include <list>
#include <algorithm>

#ifndef USELUCENE
using std::map;
using std::list;
using std::find;
#endif

SWORD_NAMESPACE_START

#ifndef USELUCENE
typedef  map < SWBuf, list<long> > strlist;
typedef list<long> longlist;
#endif

/******************************************************************************
 * RawText Constructor - Initializes data for instance of RawText
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawText::RawText(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang)
		: SWText(iname, idesc, idisp, enc, dir, mark, ilang),
		RawVerse(ipath) {
		
#ifndef USELUCENE
	SWBuf fname;
	fname = path;
	char ch = fname.c_str()[strlen(fname.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		fname += "/";
	
	for (int loop = 0; loop < 2; loop++) {
		fastSearch[loop] = 0;
		SWBuf fastidxname =(fname + ((loop)?"ntwords.dat":"otwords.dat"));
		if (FileMgr::existsFile(fastidxname.c_str())) {
			fastidxname = (fname + ((loop)?"ntwords.idx":"otwords.idx"));
			if (FileMgr::existsFile(fastidxname.c_str()))
				fastSearch[loop] = new RawStr((fname + ((loop)?"ntwords":"otwords")).c_str());
		}
	}
#endif
}


/******************************************************************************
 * RawText Destructor - Cleans up instance of RawText
 */

RawText::~RawText() {
#ifndef USELUCENE
	if (fastSearch[0])
		delete fastSearch[0];

	if (fastSearch[1])
		delete fastSearch[1];
#endif
}


bool RawText::isWritable() {
	return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawText::getRawEntry	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

SWBuf &RawText::getRawEntryBuf() {
	long  start = 0;
	unsigned short size = 0;
	VerseKey &key = getVerseKey();

	findOffset(key.Testament(), key.TestamentIndex(), &start, &size);
	entrySize = size;        // support getEntrySize call

	entryBuf = "";
	readText(key.Testament(), start, size, entryBuf);

	rawFilter(entryBuf, 0);	// hack, decipher
	rawFilter(entryBuf, &key);

//	if (!isUnicode())
		prepText(entryBuf);

	return entryBuf;
}


void RawText::setEntry(const char *inbuf, long len) {
	VerseKey &key = getVerseKey();
	doSetText(key.Testament(), key.TestamentIndex(), inbuf, len);
}


void RawText::linkEntry(const SWKey *inkey) {
	VerseKey &destkey = getVerseKey();
	const VerseKey *srckey = 0;

	// see if we have a VerseKey * or decendant
	SWTRY {
		srckey = SWDYNAMIC_CAST(VerseKey, inkey);
	}
	SWCATCH ( ... ) {}
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	doLinkEntry(destkey.Testament(), destkey.TestamentIndex(), srckey->TestamentIndex());

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;
}


/******************************************************************************
 * RawText::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawText::deleteEntry() {
	VerseKey &key = getVerseKey();
	doSetText(key.Testament(), key.TestamentIndex(), "");
}

/******************************************************************************
 * RawText::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

void RawText::increment(int steps) {
	long  start;
	unsigned short size;
	VerseKey *tmpkey = &getVerseKey();

	findOffset(tmpkey->Testament(), tmpkey->TestamentIndex(), &start, &size);

	SWKey lastgood = *tmpkey;
	while (steps) {
		long laststart = start;
		unsigned short lastsize = size;
		SWKey lasttry = *tmpkey;
		(steps > 0) ? (*key)++ : (*key)--;
		tmpkey = &getVerseKey();

		if ((error = key->Error())) {
			*key = lastgood;
			break;
		}
		long index = tmpkey->TestamentIndex();
		findOffset(tmpkey->Testament(), index, &start, &size);
		if (
			(((laststart != start) || (lastsize != size))	// we're a different entry
//				&& (start > 0)
				&& (size))	// and we actually have a size
				||(!skipConsecutiveLinks)) {	// or we don't want to skip consecutive links
			steps += (steps < 0) ? 1 : -1;
			lastgood = *tmpkey;
		}
	}
	error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

SWORD_NAMESPACE_END
