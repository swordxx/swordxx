/******************************************************************************
 *  rawtext.cpp - code for class 'RawGenBook'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <stdio.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <rawgenbook.h>
#include <rawstr.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

/******************************************************************************
 * RawGenBook Constructor - Initializes data for instance of RawGenBook
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawGenBook::RawGenBook(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang)
		: SWGenBook(iname, idesc, idisp, enc, dir, mark, ilang) {
	char tries = 2;
	int fileMode = O_RDWR;
	char *buf = new char [ strlen (ipath) + 20 ];

	entryBuf = 0;
	path = 0;
	stdstr(&path, ipath);


	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	delete key;
	key = CreateKey();


	int i;
	for (i = 0; i < tries; i++) {
		sprintf(buf, "%s.bdt", path);
		bdtfd = FileMgr::systemFileMgr.open(buf, ((!i)?fileMode:O_RDONLY)|O_BINARY);

		if (bdtfd->getFd() >= 0)
			break;
	}

	delete [] buf;

}


/******************************************************************************
 * RawGenBook Destructor - Cleans up instance of RawGenBook
 */

RawGenBook::~RawGenBook() {

	FileMgr::systemFileMgr.close(bdtfd);

	if (path)
		delete [] path;

	if (entryBuf)
		delete [] entryBuf;
}


/******************************************************************************
 * RawGenBook::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

char *RawGenBook::getRawEntry() {

	__u32 offset = 0;
	__u32 size = 0;

	TreeKeyIdx *key = 0;
#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(TreeKeyIdx, (this->key));
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif

	if (!key) {
		key = (TreeKeyIdx *)CreateKey();
		(*key) = *(this->key);
	}

	if (entryBuf)
		delete [] entryBuf;

	int dsize;
	key->getUserData(&dsize);
	if (dsize > 7) {
		memcpy(&offset, key->getUserData(), 4);
		offset = swordtoarch32(offset);

		memcpy(&size, key->getUserData() + 4, 4);
		size = swordtoarch32(size);

		entrySize = size;        // support getEntrySize call

		entryBuf = new char [ (size + 2) * FILTERPAD ];
		*entryBuf = 0;
		lseek(bdtfd->getFd(), offset, SEEK_SET);
		read(bdtfd->getFd(), entryBuf, size);

		rawFilter(entryBuf, size, key);

		   if (!isUnicode())
			RawStr::preptext(entryBuf);
	}
	else {
		entryBuf = new char [2];
		entryBuf[0] = 0;
		entryBuf[1] = 0;
		entrySize = 0;
	}

	if (key != this->key) // free our key if we created a VerseKey
		delete key;

	return entryBuf;
}



#ifdef _MSC_VER
SWModule &RawGenBook::operator =(SW_POSITION p) {
#else
RawGenBook &RawGenBook::operator =(SW_POSITION p) {
#endif
	SWModule::operator =(p);
	return *this;
}

SWModule &RawGenBook::setentry(const char *inbuf, long len) {

	__u32 offset = archtosword32(lseek(bdtfd->getFd(), 0, SEEK_END));
	__u32 size = 0;
	TreeKeyIdx *key = ((TreeKeyIdx *)this->key);

	char userData[8];

	if (!len)
		len = strlen(inbuf);

	write(bdtfd->getFd(), inbuf, len);

	size = archtosword32(len);
	memcpy(userData, &offset, 4);
	memcpy(userData+4, &size, 4);
	key->setUserData(userData, 8);
	key->save();

	return *this;
}

SWModule &RawGenBook::operator <<(const char *inbuf) {
        return setentry(inbuf, 0);
}


SWModule &RawGenBook::operator <<(const SWKey *inkey) {
	TreeKeyIdx *srckey = 0;
	TreeKeyIdx *key = ((TreeKeyIdx *)this->key);
	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		srckey = SWDYNAMIC_CAST(TreeKeyIdx, inkey);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey) {
		srckey = (TreeKeyIdx *)CreateKey();
		(*srckey) = *inkey;
	}

	key->setUserData(srckey->getUserData(), 8);
	key->save();

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;

	return *this;
}


/******************************************************************************
 * RawGenBook::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawGenBook::deleteEntry() {
	TreeKeyIdx *key = ((TreeKeyIdx *)this->key);
	key->remove();
}


char RawGenBook::createModule(const char *ipath) {
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd;
	signed char retval;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s.bdt", path);
	unlink(buf);
	fd = FileMgr::systemFileMgr.open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::systemFileMgr.close(fd);

	retval = TreeKeyIdx::create(path);
	delete [] path;
	return retval;	
}


SWKey *RawGenBook::CreateKey() {
	TreeKeyIdx *newKey = new TreeKeyIdx(path);
	return newKey;
}
