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


 /******************************************************************************
 * RawFiles Constructor - Initializes data for instance of RawFiles
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawFiles::RawFiles(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : RawVerse(ipath, O_RDWR), SWCom(iname, idesc, idisp)
{
	versebuf = 0;
}


/******************************************************************************
 * RawFiles Destructor - Cleans up instance of RawFiles
 */

RawFiles::~RawFiles()
{
	if (versebuf)
		delete [] versebuf;
}


/******************************************************************************
 * RawFiles::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

RawFiles::operator char*()
{
	FileDesc *datafile;
	long  start;
	unsigned short size;
	char *tmpbuf;
	VerseKey *key = 0;

	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (versebuf)
		delete [] versebuf;

	if (size) {
		tmpbuf   = new char [ size + strlen(path) + 1 ];
		strcpy(tmpbuf, path);
		gettext(key->Testament(), start, size + 1, tmpbuf+strlen(tmpbuf));
		datafile = FileMgr::systemFileMgr.open(tmpbuf, O_RDONLY|O_BINARY);
		delete [] tmpbuf;
		if (datafile->getFd() > 0) {
			size = lseek(datafile->getFd(), 0, SEEK_END);
			versebuf = new char [ size * 2 ];
			memset(versebuf, 0, size * 2);
			lseek(datafile->getFd(), 0, SEEK_SET);
			read(datafile->getFd(), versebuf, size);
			preptext(versebuf);
			RenderText(versebuf, size * 2);
		}
		else {
			versebuf = new char [1];
			versebuf[0] = 0;
		}
		FileMgr::systemFileMgr.close(datafile);
	}
	else {
		versebuf = new char [1];
		versebuf[0] = 0;
	}

	if (key != this->key)
		delete key;

	return versebuf;
}


/******************************************************************************
 * RawFiles::operator << (char *)- Update the modules current key entry with
 *				provided text
 *
 * RET: *this
 */

SWModule &RawFiles::operator <<(const char *inbuf) {

	FileDesc *datafile;
	long  start;
	unsigned short size;
	char *tmpbuf;
	VerseKey *key = 0;

	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		tmpbuf   = new char [ size + strlen(path) + 1 ];
		strcpy(tmpbuf, path);
		gettext(key->Testament(), start, size + 1, tmpbuf+strlen(tmpbuf));
	}
	else {
		tmpbuf   = new char [ 16 + strlen(path) + 1 ];
		sprintf(tmpbuf, "%s%s", path, getnextfilename());
		settext(key->Testament(), key->Index(), tmpbuf+strlen(path));
	}
	datafile = FileMgr::systemFileMgr.open(tmpbuf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
	delete [] tmpbuf;
	if (datafile->getFd() > 0) {
		write(datafile->getFd(), inbuf, strlen(inbuf));
	}
	FileMgr::systemFileMgr.close(datafile);
	
	if (key != this->key)
		delete key;

	return *this;
}


/******************************************************************************
 * RawFiles::operator << (SWKey *)- Link the modules current key entry with
 *				another module entry
 *
 * RET: *this
 */

SWModule &RawFiles::operator <<(const SWKey *inkey) {

	long  start;
	unsigned short size;
	char *tmpbuf;
	const VerseKey *key = 0;

	try {
		key = dynamic_cast<const VerseKey *>(inkey);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		tmpbuf   = new char [ size + 1 ];
		gettext(key->Testament(), start, size + 1, tmpbuf);

		if (key != inkey)
			delete key;
		key = 0;
		try {
			key = dynamic_cast<const VerseKey *>(inkey);
		}
		catch ( ... ) {}
		if (!key)
			key = new VerseKey(this->key);
		settext(key->Testament(), key->Index(), tmpbuf);
	}
	
	if (key != inkey)
		delete key;

	return *this;
}


/******************************************************************************
 * RawFiles::operator << (SWKey *)- Link the modules current key entry with
 *				another module entry
 *
 * RET: *this
 */

void RawFiles::Delete() {

	VerseKey *key = 0;

	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	settext(key->Testament(), key->Index(), "");

	if (key != this->key)
		delete key;
}


/******************************************************************************
 * RawFiles::getnextfilename - generates a valid filename in which to store
 *				an entry
 *
 * RET: filename
 */

char *RawFiles::getnextfilename() {
	static char incfile[255];
	long number;
	FileDesc *datafile;

	sprintf(incfile, "%sincfile", path);
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
