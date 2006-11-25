/******************************************************************************
 *  rawfiles.cpp - code for class 'RawFiles'- a module that produces HTML HREFs
 *			pointing to actual text desired.  Uses standard
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#include <rawverse.h>
#include <rawfiles.h>
#include <filemgr.h>
#include <versekey.h>

SWORD_NAMESPACE_START

 /******************************************************************************
 * RawFiles Constructor - Initializes data for instance of RawFiles
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawFiles::RawFiles(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawVerse(ipath, FileMgr::RDWR), SWCom(iname, idesc, idisp, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawFiles Destructor - Cleans up instance of RawFiles
 */

RawFiles::~RawFiles()
{
}


/** Is the module writable? :)
* @return yes or no
*/
bool RawFiles::isWritable() {
	return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
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
	VerseKey *key = 0;

	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
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
		datafile = FileMgr::getSystemFileMgr()->open(tmpbuf.c_str(), FileMgr::RDONLY);
		if (datafile->getFd() > 0) {
			size = datafile->seek(0, SEEK_END);
			char *tmpBuf = new char [ size + 1 ];
			memset(tmpBuf, 0, size + 1);
			datafile->seek(0, SEEK_SET);
			datafile->read(tmpBuf, size);
			entryBuf = tmpBuf;
			delete [] tmpBuf;
//			preptext(entrybuf);
		}
		FileMgr::getSystemFileMgr()->close(datafile);
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
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
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
	datafile = FileMgr::getSystemFileMgr()->open(entryBuf, FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
	if (datafile->getFd() > 0) {
		datafile->write(inbuf, len);
	}
	FileMgr::getSystemFileMgr()->close(datafile);
	
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
	const VerseKey *key = 0;

	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, inkey);
	}
	SWCATCH ( ... ) {}
	if (!key)
		key = new VerseKey(this->key);

	findOffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		SWBuf tmpbuf;
		readText(key->Testament(), start, size + 2, tmpbuf);

		if (key != inkey)
			delete key;
		key = 0;

		SWTRY {
			key = SWDYNAMIC_CAST(VerseKey, inkey);
		}
		SWCATCH ( ... ) {}
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

	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {}
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
	datafile = FileMgr::getSystemFileMgr()->open(incfile, FileMgr::RDONLY);
	if (datafile->read(&number, 4) != 4)
		number = 0;
	number++;
	FileMgr::getSystemFileMgr()->close(datafile);
	
	datafile = FileMgr::getSystemFileMgr()->open(incfile, FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
	datafile->write(&number, 4);
	FileMgr::getSystemFileMgr()->close(datafile);
	sprintf(incfile, "%.7ld", number-1);
	return incfile;
}


char RawFiles::createModule (const char *path) {
	char *incfile = new char [ strlen (path) + 16 ];
    static long zero = 0;
	FileDesc *datafile;

	sprintf(incfile, "%s/incfile", path);
	datafile = FileMgr::getSystemFileMgr()->open(incfile, FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
    delete [] incfile;
	datafile->write(&zero, 4);
	FileMgr::getSystemFileMgr()->close(datafile);

    return RawVerse::createModule (path);
}



SWORD_NAMESPACE_END
