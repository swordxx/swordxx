/******************************************************************************
 *  rawfiles.cpp - code for class 'RawFiles'- a module that produces HTML HREFs
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
#include <rawfiles.h>
#include <filemgr.h>

#ifndef O_BINARY                // O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0              // If it hasn't been defined than we probably
#endif                          // don't need it.

SWORD_NAMESPACE_START

 /******************************************************************************
 * RawFiles Constructor - Initializes data for instance of RawFiles
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawFiles::RawFiles(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawVerse(ipath, O_RDWR), SWCom(iname, idesc, idisp, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawFiles Destructor - Cleans up instance of RawFiles
 */

RawFiles::~RawFiles()
{
}


/******************************************************************************
 * RawFiles::getRawEntry	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

SWBuf &RawFiles::getRawEntryBuf() {
	FileDesc *datafile;
	long  start = 0;
	unsigned short size = 0;
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

	findOffset(key->Testament(), key->Index(), &start, &size);

	entryBuf = "";
	if (size) {
		SWBuf tmpbuf = path;
		tmpbuf += '/';
		readText(key->Testament(), start, size, entryBuf);
		tmpbuf += entryBuf;
		entryBuf = "";
		datafile = FileMgr::systemFileMgr.open(tmpbuf.c_str(), O_RDONLY|O_BINARY);
		if (datafile->getFd() > 0) {
			size = lseek(datafile->getFd(), 0, SEEK_END);
			char *tmpBuf = new char [ size + 1 ];
			memset(tmpBuf, 0, size + 1);
			lseek(datafile->getFd(), 0, SEEK_SET);
			read(datafile->getFd(), tmpBuf, size);
			entryBuf = tmpBuf;
			delete [] tmpBuf;
//			preptext(entrybuf);
		}
		FileMgr::systemFileMgr.close(datafile);
	}

	if (key != this->key)
		delete key;

	return entryBuf;
}


/******************************************************************************
 * RawFiles::setEntry(char *)- Update the modules current key entry with
 *				provided text
 */

void RawFiles::setEntry(const char *inbuf, long len) {
	FileDesc *datafile;
	long  start;
	unsigned short size;
	VerseKey *key = 0;

	len = (len<0)?strlen(inbuf):len;
	try {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findOffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		SWBuf tmpbuf;
		entryBuf = path;
		entryBuf += '/';
		readText(key->Testament(), start, size, tmpbuf);
		entryBuf += tmpbuf;
	}
	else {
		SWBuf tmpbuf;
		entryBuf = path;
		entryBuf += '/';
		tmpbuf = getNextFilename();
		doSetText(key->Testament(), key->Index(), tmpbuf);
		entryBuf += tmpbuf;
	}
	datafile = FileMgr::systemFileMgr.open(entryBuf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
	if (datafile->getFd() > 0) {
		write(datafile->getFd(), inbuf, len);
	}
	FileMgr::systemFileMgr.close(datafile);
	
	if (key != this->key)
		delete key;
}


/******************************************************************************
 * RawFiles::linkEntry(SWKey *)- Link the modules current key entry with
 *				another module entry
 *
 * RET: *this
 */

void RawFiles::linkEntry(const SWKey *inkey) {

	long  start;
	unsigned short size;
	char *tmpbuf;
	const VerseKey *key = 0;

	try {
		key = SWDYNAMIC_CAST(VerseKey, inkey);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findOffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		SWBuf tmpbuf;
		readText(key->Testament(), start, size + 2, tmpbuf);

		if (key != inkey)
			delete key;
		key = 0;

		try {
			key = SWDYNAMIC_CAST(VerseKey, inkey);
		}
		catch ( ... ) {}
		if (!key)
			key = new VerseKey(this->key);
		doSetText(key->Testament(), key->Index(), tmpbuf.c_str());
	}
	
	if (key != inkey)
		delete key;
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawFiles::deleteEntry() {

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

	doSetText(key->Testament(), key->Index(), "");

	if (key != this->key)
		delete key;
}


/******************************************************************************
 * RawFiles::getNextfilename - generates a valid filename in which to store
 *				an entry
 *
 * RET: filename
 */

char *RawFiles::getNextFilename() {
	static char incfile[255];
	long number;
	FileDesc *datafile;

	sprintf(incfile, "%s/incfile", path);
	datafile = FileMgr::systemFileMgr.open(incfile, O_RDONLY|O_BINARY);
	if (read(datafile->getFd(), &number, 4) != 4)
		number = 0;
	number++;
	FileMgr::systemFileMgr.close(datafile);
	
	datafile = FileMgr::systemFileMgr.open(incfile, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
	write(datafile->getFd(), &number, 4);
	FileMgr::systemFileMgr.close(datafile);
	sprintf(incfile, "%.7ld", number-1);
	return incfile;
}


char RawFiles::createModule (const char *path) {
	char *incfile = new char [ strlen (path) + 16 ];
    static long zero = 0;
	FileDesc *datafile;

	sprintf(incfile, "%s/incfile", path);
	datafile = FileMgr::systemFileMgr.open(incfile, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
    delete [] incfile;
	write(datafile->getFd(), &zero, 4);
	FileMgr::systemFileMgr.close(datafile);

    return RawVerse::createModule (path);
}



SWORD_NAMESPACE_END
