/******************************************************************************
 *  ztext.cpp - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#include <sysdata.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <utilfuns.h>
#include <ztext.h>

#include <regex.h>	// GNU


#ifdef USELUCENE
#include <CLucene.h>
using namespace lucene::search;
using namespace lucene::queryParser;
#endif

SWORD_NAMESPACE_START

/******************************************************************************
 * zText Constructor - Initializes data for instance of zText
 *
 * ENT:	ipath - path to data files
 *		iname - Internal name for module
 *		idesc - Name to display to user for module
 *		iblockType - verse, chapter, book, etc. of index chunks
 *		icomp - Compressor object
 *		idisp - Display object to use for displaying
 */

zText::zText(const char *ipath, const char *iname, const char *idesc, int iblockType, SWCompress *icomp, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang)
		: zVerse(ipath, -1, iblockType, icomp), SWText(iname, idesc, idisp, enc, dir, mark, ilang) {
	blockType = iblockType;
	lastWriteKey = 0;
#ifdef USELUCENE
	SWBuf fname;
	fname = path;
	ir = 0;
	is = 0;
	char ch = fname.c_str()[strlen(fname.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		fname += "/lucene";
	if (IndexReader::indexExists(fname.c_str())) {
		ir = &IndexReader::open(fname);
		is = new IndexSearcher(*ir);
	}
#endif
}


/******************************************************************************
 * zText Destructor - Cleans up instance of zText
 */

zText::~zText()
{
	flushCache();

	if (lastWriteKey)
		delete lastWriteKey;

#ifdef USELUCENE
	if (is)
		is->close();

	if (ir)
		delete ir;
#endif
}


/******************************************************************************
 * zText::getRawEntry	- Returns the current verse buffer
 *
 * RET: buffer with verse
 */

SWBuf &zText::getRawEntryBuf() {
	long  start = 0;
	unsigned short size = 0;
	VerseKey &key = getVerseKey();

	findOffset(key.Testament(), key.Index(), &start, &size);
	entrySize = size;        // support getEntrySize call
			  
	entryBuf = "";
	zReadText(key.Testament(), start, size, entryBuf);

	rawFilter(entryBuf, &key);

//	if (!isUnicode())
		prepText(entryBuf);

	return entryBuf;
}


bool zText::sameBlock(VerseKey *k1, VerseKey *k2) {
	if (k1->Testament() != k2->Testament())
		return false;

	switch (blockType) {
	case VERSEBLOCKS:
		if (k1->Verse() != k2->Verse())
			return false;
	case CHAPTERBLOCKS:
		if (k1->Chapter() != k2->Chapter())
			return false;
	case BOOKBLOCKS:
		if (k1->Book() != k2->Book())
			return false;
	}
	return true;
}


void zText::setEntry(const char *inbuf, long len) {
	VerseKey &key = getVerseKey();

	// see if we've jumped across blocks since last write
	if (lastWriteKey) {
		if (!sameBlock(lastWriteKey, &key)) {
			flushCache();
		}
		delete lastWriteKey;
	}

	doSetText(key.Testament(), key.Index(), inbuf, len);

	lastWriteKey = (VerseKey *)key.clone();	// must delete
}


void zText::linkEntry(const SWKey *inkey) {
	VerseKey &destkey = getVerseKey();
	const VerseKey *srckey = 0;

	// see if we have a VerseKey * or decendant
	SWTRY {
		srckey = (const VerseKey *) SWDYNAMIC_CAST(VerseKey, inkey);
	}
	SWCATCH ( ... ) {
	}
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	doLinkEntry(destkey.Testament(), destkey.Index(), srckey->Index());

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;
}


/******************************************************************************
 * zFiles::deleteEntry	- deletes this entry
 *
 */

void zText::deleteEntry() {

	VerseKey &key = getVerseKey();

	doSetText(key.Testament(), key.Index(), "");
}


/******************************************************************************
 * zText::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 */

void zText::increment(int steps) {
	long  start;
	unsigned short size;
	VerseKey *tmpkey = &getVerseKey();

	findOffset(tmpkey->Testament(), tmpkey->Index(), &start, &size);

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
		long index = tmpkey->Index();
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


VerseKey &zText::getVerseKey() {
	static VerseKey tmpVK;
	VerseKey *key;
	// see if we have a VerseKey * or decendant
	SWTRY {
		key = SWDYNAMIC_CAST(VerseKey, this->key);
	}
	SWCATCH ( ... ) {	}
	if (!key) {
		ListKey *lkTest = 0;
		SWTRY {
			lkTest = SWDYNAMIC_CAST(ListKey, this->key);
		}
		SWCATCH ( ... ) {	}
		if (lkTest) {
			SWTRY {
				key = SWDYNAMIC_CAST(VerseKey, lkTest->GetElement());
			}
			SWCATCH ( ... ) {	}
		}
	}
	if (!key) {
		tmpVK = *(this->key);
		return tmpVK;
	}
	else	return *key;
}


#ifndef O_BINARY
#define O_BINARY 0
#endif

signed char zText::createSearchFramework() {
#ifdef USELUCENE
	SWKey *savekey = 0;
	SWKey *searchkey = 0;
	SWKey textkey;
	char *word = 0;
	char *wordBuf = 0;


	// save key information so as not to disrupt original
	// module position
	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	searchkey = (key->Persist())?key->clone():0;
	if (searchkey) {
		searchkey->Persist(1);
		setKey(*searchkey);
	}

	// position module at the beginning
	*this = TOP;

	VerseKey *lkey = (VerseKey *)key;

	// iterate thru each entry in module

	IndexWriter* writer = NULL;
	Directory* d = NULL;
 
	lucene::analysis::SimpleAnalyzer& an = *new lucene::analysis::SimpleAnalyzer();
	SWBuf target = path;
	char ch = target.c_str()[strlen(target.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		target += "/lucene";

	if (IndexReader::indexExists(target.c_str())) {
		d = &FSDirectory::getDirectory(target.c_str(), false);
		if (IndexReader::isLocked(*d)) {
			IndexReader::unlock(*d);
		}
																		   
		writer = new IndexWriter(*d, an, false);
	} else {
		d = &FSDirectory::getDirectory(target.c_str(), true);
		writer = new IndexWriter( *d ,an, true);
	}


 
	while (!Error()) {
		Document &doc = *new Document();
		doc.add( Field::Text(_T("key"), (const char *)*lkey ) );
		doc.add( Field::Text(_T("content"), StripText()) );
		writer->addDocument(doc);
		delete &doc;

		(*this)++;
	}

	writer->optimize();
	writer->close();
	delete writer;
	delete &an;

	// reposition module back to where it was before we were called
	setKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	
#endif
	return 0;
}


/******************************************************************************
 * SWModule::Search 	- Searches a module for a string
 *
 * ENT:	istr		- string for which to search
 * 	searchType	- type of search to perform
 *				>=0 - regex
 *				-1  - phrase
 *				-2  - multiword
 * 	flags		- options flags for search
 *	justCheckIfSupported	- if set, don't search, only tell if this
 *							function supports requested search.
 *
 * RET: listkey set to verses that contain istr
 */

ListKey &zText::search(const char *istr, int searchType, int flags, SWKey *scope, bool *justCheckIfSupported, void (*percent)(char, void *), void *percentUserData) {
#ifdef USELUCENE
	listkey.ClearList();

	if ((is) && (ir)) {

		switch (searchType) {
		case -2: {	// let lucene replace multiword for now


			// test to see if our scope for this search is bounded by a
			// VerseKey
			VerseKey *testKeyType = 0, vk;
			SWTRY {
				testKeyType = SWDYNAMIC_CAST(VerseKey, ((scope)?scope:key));
			}
			SWCATCH ( ... ) {}
			// if we don't have a VerseKey * decendant we can't handle
			// because of scope.
			// In the future, add bool SWKey::isValid(const char *tryString);
			if (!testKeyType)
				break;


			// check if we just want to see if search is supported.
			// If we've gotten this far, then it is supported.
			if (justCheckIfSupported) {
				*justCheckIfSupported = true;
				return listkey;
			}

			(*percent)(10, percentUserData);

			standard::StandardAnalyzer analyzer;
			Query &q =  QueryParser::Parse(istr, _T("content"), analyzer);
			(*percent)(20, percentUserData);
			Hits &h = is->search(q);
			(*percent)(80, percentUserData);


			// iterate thru each good module position that meets the search
			for (long i = 0; i < h.Length(); i++) {
				Document &doc = h.doc(i);

				// set a temporary verse key to this module position
				vk = doc.get(_T("key"));

				// check scope
				// Try to set our scope key to this verse key
				if (scope) {
					*testKeyType = vk;

					// check to see if it set ok and if so, add to our return list
					if (*testKeyType == vk) {
						listkey << (const char *) vk;
						listkey.GetElement()->userData = reinterpret_cast<void *>((int)(h.score(i)*100));
					}
				}
				else {
					listkey << (const char*) vk;
					listkey.GetElement()->userData = reinterpret_cast<void *>((int)(h.score(i)*100));
				}
			}
			(*percent)(98, percentUserData);

			delete &h;
			delete &q;

			listkey = TOP;
			(*percent)(100, percentUserData);
			return listkey;
		}

		default:
			break;
		}
	}

	// check if we just want to see if search is supported
	if (justCheckIfSupported) {
		*justCheckIfSupported = false;
		return listkey;
	}
#endif
	// if we don't support this search, fall back to base class
	return SWModule::search(istr, searchType, flags, scope, justCheckIfSupported, percent, percentUserData);
}


SWORD_NAMESPACE_END
