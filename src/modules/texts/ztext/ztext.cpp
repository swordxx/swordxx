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
//#include <rawverse.h>
#include <ztext.h>
//#include <zlib.h>


/******************************************************************************
 * zText Constructor - Initializes data for instance of zText
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		blockType - verse, chapter, book, etc. of index chunks
 *		icomp - Compressor object
 *		idisp - Display object to use for displaying
 */

zText::zText(const char *ipath, const char *iname, const char *idesc, int blockType, SWCompress *icomp, SWDisplay *idisp) : zVerse(ipath, O_RDONLY, blockType, icomp), SWText(iname, idesc, idisp)/*, SWCompress()*/
{
	versebuf = 0;
	printf("init ztext\n");
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
/*
	long  start;
	unsigned long size;
	unsigned long destsize;
	char *tmpbuf;
	char *dest;
	VerseKey *lkey = (VerseKey *) SWModule::key;
	char sizebuf[3];

	lkey->Verse(0);
	if (chapcache != lkey->Index()) {
		findoffset(lkey->Testament(), lkey->Index(), &start, &((unsigned short) size));
		gettext(lkey->Testament(), start, 3, sizebuf);
		memcpy(&size, sizebuf, 2);
		tmpbuf = new char [ size + 1 ];
		gettext(lkey->Testament(), start + 2, size + 1 , tmpbuf);
		//zBuf(&size, tmpbuf);
		dest = new char [ (size*4) + 1 ];
		uncompress((Bytef *)dest, &destsize, (Bytef *) tmpbuf, size);
		chapcache = lkey->Index();
		delete [] tmpbuf;
	}

	//findoffset(key->Testament(), key->Index(), &start, &size);
	findoffset(lkey->Testament(), lkey->Index(), &start, &((unsigned short) size));

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ size + 1 ];
	//memcpy(versebuf, Buf(), size);
	memcpy(versebuf, dest, destsize);
	delete [] dest;

	preptext(versebuf);

	return versebuf;
*/

	long  start;
	unsigned short size;
	VerseKey *key = 0;
	FilterList::iterator it;

	//printf ("zText char *\n");

	// see if we have a VerseKey * or decendant
	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {
	}
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	//printf ("checking cache\n");
	if (chapcache != key->Index())
	{
		//printf ("finding offset\n");
		findoffset(key->Testament(), key->Index(), &start, &size);

		//printf ("deleting previous buffer\n");
		if (versebuf)
			delete [] versebuf;
		versebuf = new char [ size * 3 ];

		//printf ("getting text\n");
		swgettext(key->Testament(), start, size + 1, versebuf);
		//printf ("got text\n");
		chapcache = key->Index();
	}

	//printf ("processing text\n");
	for (it = rawfilters.begin(); it != rawfilters.end(); it++) {
		(*it)->ProcessText(versebuf, size + 1);
	}

	//printf ("preparing text\n");
	preptext(versebuf);
	RenderText(versebuf, size * 3);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	//printf ("returning text\n");
	return versebuf;

}
