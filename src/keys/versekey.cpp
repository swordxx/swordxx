/******************************************************************************
 *  VerseKey.cpp - code for class 'VerseKey'- a standard Biblical verse key
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */


#include <swmacs.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

#include <utilstr.h>
#include <stringmgr.h>
#include <swkey.h>
#include <swlog.h>
#include <versekey.h>
#include <swlocale.h>
#include <roman.h>
#include <versemgr.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"VerseKey", "SWKey", "SWObject", 0};
SWClass VerseKey::classdef(classes);

/******************************************************************************
 *  Initialize static members of VerseKey
 */

int           VerseKey::instance       = 0;


/******************************************************************************
 * VerseKey::init - initializes instance of VerseKey
 */

void VerseKey::init(const char *v11n) {
	myclass = &classdef;

	instance++;
	autonorm = 1;		// default auto normalization to true
	headings = 0;		// default display headings option is false
	upperBound = 0;
	lowerBound = 0;
	boundSet = false;
	testament = 1;
	book = 1;
	chapter = 1;
	verse = 1;
	suffix = 0;
	tmpClone = 0;
	refSys = 0;

	setVersificationSystem(v11n);
}

/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:	ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(const SWKey &ikey) : SWKey(ikey)
{
	init();
	copyFrom(ikey);
}


VerseKey::VerseKey(const SWKey *ikey) : SWKey(*ikey)
{
	init();
	if (ikey)
		copyFrom(*ikey);
}


/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:	ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(const char *ikeyText) : SWKey(ikeyText)
{
	init();
	if (ikeyText)
		parse();
}


VerseKey::VerseKey(VerseKey const &k) : SWKey(k)
{
	init();
	copyFrom(k);
}


/******************************************************************************
 * VerseKey::setFromOther - Positions this VerseKey to another VerseKey
 */

void VerseKey::setFromOther(const VerseKey &ikey) {
	if (refSys == ikey.refSys) {
		testament = ikey.Testament();
		book = ikey.Book();
		chapter = ikey.Chapter();
		verse = ikey.Verse();
		suffix = ikey.getSuffix();
        }
        // Here is where we will do v11n system conversions in the future
        // when we have a conversion mechanism (Ben Morgan has started
        // thinking about this)
        else {
	        // For now, this is the best we can do
        	setText(ikey.getText());
        }
}


void VerseKey::positionFrom(const SWKey &ikey) {
 	error = 0;
        const SWKey *fromKey = &ikey;
	ListKey *tryList = SWDYNAMIC_CAST(ListKey, fromKey);
	if (tryList) {
		SWKey *k = tryList->getElement();
		if (k) fromKey = k;
	}
	VerseKey *tryVerse = SWDYNAMIC_CAST(VerseKey, fromKey);
	if (tryVerse) {
		setFromOther(*tryVerse);
	}
	else {
		SWKey::positionFrom(*fromKey);
		parse();
	}

 	// should we always perform bounds checks?  Tried but seems to cause infinite recursion
	if (_compare(UpperBound()) > 0) {
		setFromOther(UpperBound());
		error = KEYERR_OUTOFBOUNDS;
	}
	if (_compare(LowerBound()) < 0) {
		setFromOther(LowerBound());
		error = KEYERR_OUTOFBOUNDS;
	}
}


/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another VerseKey
 */

void VerseKey::copyFrom(const VerseKey &ikey) {
	autonorm = ikey.autonorm;
	headings = ikey.headings;
	testament = ikey.Testament();
	book = ikey.Book();
	chapter = ikey.Chapter();
	verse = ikey.Verse();
	suffix = ikey.getSuffix();
	setLocale(ikey.getLocale());
	setVersificationSystem(ikey.getVersificationSystem());
	if (ikey.isBoundSet()) {
		LowerBound(ikey.LowerBound());
		UpperBound(ikey.UpperBound());
	}
}


/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another SWKey
 */

void VerseKey::copyFrom(const SWKey &ikey) {
	// check to see if we can do a more specific copy
	// plus some optimizations
	const SWKey *fromKey = &ikey;
	ListKey *tryList = SWDYNAMIC_CAST(ListKey, fromKey);
	if (tryList) {
		SWKey *k = tryList->getElement();
		if (k) fromKey = k;
	}
	VerseKey *tryVerse = SWDYNAMIC_CAST(VerseKey, fromKey);
	if (tryVerse) {
		copyFrom(*tryVerse);
	}
	else {
		SWKey::copyFrom(*fromKey);
		parse();
	}
}


VerseKey::VerseKey(const char *min, const char *max, const char *v11n) : SWKey()
{
	init(v11n);
	ListKey tmpListKey = ParseVerseList(min);
	if (tmpListKey.Count()) {
		VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement(0));
		LowerBound(*newElement);
	}
	tmpListKey = ParseVerseList(max, min, true);
	if (tmpListKey.Count()) {
		VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement(0));
		UpperBound((newElement->isBoundSet())?newElement->UpperBound():*newElement);
	}
	setPosition(TOP);
}


SWKey *VerseKey::clone() const
{
	return new VerseKey(*this);
}


/******************************************************************************
 * VerseKey Destructor - cleans up instance of VerseKey
 *
 * ENT:	ikey - text key
 */

VerseKey::~VerseKey() {

	delete tmpClone;

	--instance;
}


void VerseKey::setVersificationSystem(const char *name) {
	const VerseMgr::System *newRefSys = VerseMgr::getSystemVerseMgr()->getVersificationSystem(name);
	// TODO: cheese, but what should we do if requested v11n system isn't found?
	if (!newRefSys)   newRefSys = VerseMgr::getSystemVerseMgr()->getVersificationSystem("KJV");
	if (refSys != newRefSys) {
		refSys = newRefSys;
		BMAX[0] = refSys->getBMAX()[0];
		BMAX[1] = refSys->getBMAX()[1];

		// TODO: adjust bounds for versificaion system ???
		// TODO: when we have mapping done, rethink this
		//necessary as our bounds might not mean anything in the new v11n system
		ClearBounds();
	}

}


const char *VerseKey::getVersificationSystem() const { return refSys->getName(); }



/******************************************************************************
 * VerseKey::parse - parses keytext into testament|book|chapter|verse
 *
 * RET:	error status
 */

char VerseKey::parse(bool checkAutoNormalize)
{
	testament = BMAX[1]?2:1;
	book      = BMAX[BMAX[1]?1:0];
	chapter   = 1;
	verse     = 1;

	int error = 0;

	if (keytext) {
		ListKey tmpListKey = ParseVerseList(keytext);
		if (tmpListKey.Count()) {
			this->positionFrom(*tmpListKey.getElement(0));
			error = this->error;
		} else error = 1;
	}
	if (checkAutoNormalize) {
		Normalize(1);
	}
	freshtext();

	return (this->error) ? this->error : (this->error = error);
}


/******************************************************************************
 * VerseKey::freshtext - refreshes keytext based on
 *				testament|book|chapter|verse
 */

void VerseKey::freshtext() const
{
	char buf[2024];
	int realTest = testament;
	int realbook = book;

	if (book < 1) {
		if (testament < 1)
			sprintf(buf, "[ Module Heading ]");
		else sprintf(buf, "[ Testament %d Heading ]", (int)testament);
	}
	else {
		if (realbook > BMAX[realTest-1]) {
			realbook -= BMAX[realTest-1];
			if (realTest < 2)
				realTest++;
			if (realbook > BMAX[realTest-1])
				realbook = BMAX[realTest-1];
		}
		sprintf(buf, "%s %d:%d", getBookName(), chapter, verse);
		if (suffix) {
			buf[strlen(buf)+1] = 0;
			buf[strlen(buf)] = suffix;
		}
	}

	stdstr((char **)&keytext, buf);
}



/************************************************************************
 * VerseKey::getBookAbbrev - Attempts to find a book no from a name or
 *                           abbreviation
 *
 * ENT:	abbr - key for which to search;
 * RET:	book number or < 0 = not valid
 */

int VerseKey::getBookAbbrev(const char *iabbr) const
{
	int diff, abLen, min, max, target, retVal = -1;

	char *abbr = 0;

	int abbrevsCnt;

	const struct abbrev *abbrevs = getPrivateLocale()->getBookAbbrevs(&abbrevsCnt);

	StringMgr* stringMgr = StringMgr::getSystemStringMgr();
	const bool hasUTF8Support = StringMgr::hasUTF8Support();

	// The first iteration of this loop tries to uppercase
	// the string. If we still fail to match, we try
	// matching the string without any toupper logic.
	// This is useful for, say, Chinese user input
	// on a system that doesn't properly support
	// a true Unicode-toupper function (!hasUTF8Support)
	for (int i = 0; i < 2; i++) {
		stdstr(&abbr, iabbr, 2);
		strstrip(abbr);

		if (!i) {
			if (hasUTF8Support) { //we have support for UTF-8 handling; we expect UTF-8 encoded locales
				stringMgr->upperUTF8(abbr, strlen(abbr)*2);
			}
			else {
				stringMgr->upperLatin1(abbr);
			}
		}

		abLen = strlen(abbr);

		if (abLen) {
			min = 0;
			max = abbrevsCnt;

			// binary search for a match
			while(1) {
				target = min + ((max - min) / 2);
				diff = strncmp(abbr, abbrevs[target].ab, abLen);
				if ((!diff)||(target >= max)||(target <= min))
					break;
				if (diff > 0)
					min = target;
				else	max = target;
			}

			// lets keep backing up till we find the 'first' valid match
			for (; target > 0; target--) {
				if (strncmp(abbr, abbrevs[target-1].ab, abLen))
					break;
			}

			if (!diff) {
				// lets keep moving forward till we find an abbrev in our refSys
				retVal = refSys->getBookNumberByOSISName(abbrevs[target].osis);
				while ((retVal < 0)  && (target < max) && (!strncmp(abbr, abbrevs[target+1].ab, abLen))) {
					target++;
					retVal = refSys->getBookNumberByOSISName(abbrevs[target].osis);
				}
			}
			else retVal = -1;
		}
		if (retVal > 0)
			break;
	}
	delete [] abbr;
	return retVal;
}


/******************************************************************************
 * VerseKey::validateCurrentLocale - be sure a locale book abbrevs set is complete
 *
 */
void VerseKey::validateCurrentLocale() const {
	if (SWLog::getSystemLog()->getLogLevel() >= SWLog::LOG_DEBUG) { //make sure log is wanted, this loop stuff costs a lot of time
		for (int i = 0; i < refSys->getBookCount(); i++) {
			const int bn = getBookAbbrev(getPrivateLocale()->translate(refSys->getBook(i)->getLongName()));
			if (bn != i+1) {
				char *abbr = 0;
				stdstr(&abbr, getPrivateLocale()->translate(refSys->getBook(i)->getLongName()), 2);
				strstrip(abbr);
				SWLog::getSystemLog()->logDebug("VerseKey::Book: %s does not have a matching toupper abbrevs entry! book number returned was: %d, should be %d. Required entry to add to locale:", abbr, bn, i);

				StringMgr* stringMgr = StringMgr::getSystemStringMgr();
				const bool hasUTF8Support = StringMgr::hasUTF8Support();
				if (hasUTF8Support) { //we have support for UTF-8 handling; we expect UTF-8 encoded locales
					stringMgr->upperUTF8(abbr, strlen(abbr)*2);
				}
				else {
					stringMgr->upperLatin1(abbr);
				}
				SWLog::getSystemLog()->logDebug("%s=%s\n", abbr, refSys->getBook(i)->getOSISName());
				delete [] abbr;
			}
		}
	}
}


/******************************************************************************
 * VerseKey::ParseVerseList - Attempts to parse a buffer into separate
 *				verse entries returned in a ListKey
 *
 * ENT:	buf		- buffer to parse;
 *	defaultKey	- if verse, chap, book, or testament is left off,
 *				pull info from this key (ie. Gen 2:3; 4:5;
 *				Gen would be used when parsing the 4:5 section)
 *	expandRange	- whether or not to expand eg. John 1:10-12 or just
 *				save John 1:10
 *
 * RET:	ListKey reference filled with verse entries contained in buf
 *
 * COMMENT: This code works but wreaks.  Rewrite to make more maintainable.
 */

ListKey VerseKey::ParseVerseList(const char *buf, const char *defaultKey, bool expandRange, bool useChapterAsVerse) {

	// hold on to our own copy of params, as threads/recursion may change outside values
	const char *bufStart = buf;
	SWBuf iBuf = buf;
	buf = iBuf.c_str();
	SWBuf iDefaultKey = defaultKey;
	if (defaultKey) defaultKey = iDefaultKey.c_str();

	char book[2048];	// TODO: bad, remove
	char number[2048];	// TODO: bad, remove
	*book = 0;
	*number = 0;
	int tobook = 0;
	int tonumber = 0;
	char suffix = 0;
	int chap = -1, verse = -1;
	int bookno = 0;
	int loop;
	char comma = 0;
	char dash = 0;
	const char *orig = buf;
	int q;
	ListKey tmpListKey;
	ListKey internalListKey;
	char lastPartial = 0;
	bool inTerm = true;
	int notAllDigits = 0;

	// assert we have a buffer
	if (!buf) return internalListKey;

	VerseKey *curKey  = (VerseKey *)this->clone();
	VerseKey *lastKey = (VerseKey *)this->clone();
	lastKey->ClearBounds();
	curKey->ClearBounds();

	// some silly checks for corner cases
	if (!strcmp(buf, "[ Module Heading ]")) {
		curKey->Verse(0);
		curKey->Chapter(0);
		curKey->Book(0);
		curKey->Testament(0);
		lastKey->LowerBound(*curKey);
		lastKey->UpperBound(*curKey);
		internalListKey << *lastKey;
		delete curKey;
		delete lastKey;
		return internalListKey;
	}
	if ((!strncmp(buf, "[ Testament ", 12)) &&
	    (isdigit(buf[12])) &&
	    (!strcmp(buf+13, " Heading ]"))) {
		curKey->Verse(0);
		curKey->Chapter(0);
		curKey->Book(0);
		curKey->Testament(buf[12]-48);
		lastKey->LowerBound(*curKey);
		lastKey->UpperBound(*curKey);
		internalListKey << *lastKey;
		delete curKey;
		delete lastKey;
		return internalListKey;
	}

	curKey->AutoNormalize(AutoNormalize());
	lastKey->AutoNormalize(0);
	if (defaultKey) *lastKey = defaultKey;

	while (*buf) {
		switch (*buf) {
		case ':':
			if (buf[1] != ' ') {		// for silly "Mat 1:1: this verse...."
				number[tonumber] = 0;
				tonumber = 0;
				if (*number)
					chap = atoi(number);
				*number = 0;
				comma = 0;
				break;
			}
			// otherwise drop down to next case
		case ' ':
			inTerm = true;
			while (true) {
				if ((!*number) || (chap < 0))
					break;
				for (q = 1; ((buf[q]) && (buf[q] != ' ')); q++);
				if (buf[q] == ':')
					break;
				inTerm = false;
				break;
			}
			if (inTerm) {
				book[tobook++] = ' ';
				break;
			}

		case '-':
		case ',': // on number new verse
		case ';': // on number new chapter
			number[tonumber] = 0;
			tonumber = 0;
			if (*number) {
				if (chap >= 0)
					verse = atoi(number);
				else	chap = atoi(number);
			}
			*number = 0;
			book[tobook] = 0;
			tobook = 0;
			bookno = -1;
			if (*book) {
				for (loop = strlen(book) - 1; loop+1; loop--) {
					if ((isdigit(book[loop])) || (book[loop] == ' ')) {
						book[loop] = 0;
						continue;
					}
					else {
						if ((SW_toupper(book[loop])=='F')&&(loop)) {
							if ((isdigit(book[loop-1])) || (book[loop-1] == ' ') || (SW_toupper(book[loop-1]) == 'F')) {
								book[loop] = 0;
								continue;
							}
						}
					}
					break;
				}

				for (loop = strlen(book) - 1; loop+1; loop--) {
					if (book[loop] == ' ') {
						// "PS C" is ok, but "II C" is not ok
						if (isroman(&book[loop+1]) && !isroman(book,loop)) {
							if (verse == -1) {
								verse = chap;
								chap = from_rom(&book[loop+1]);
								book[loop] = 0;
							}
						}
	        				break;
					}
				}

				if ((!stricmp(book, "V")) || (!stricmp(book, "VER"))) {	// Verse abbrev
					if (verse == -1) {
						verse = chap;
						chap = lastKey->Chapter();
						*book = 0;
					}
				}
				if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
					strcpy(book, lastKey->getBookName());
				}
				bookno = getBookAbbrev(book);
			}
			if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
				char partial = 0;
				curKey->Verse(1);
				curKey->Chapter(1);
				curKey->Book(1);

				if (bookno < 0) {
					curKey->Testament(lastKey->Testament());
					curKey->Book(lastKey->Book());
				}
				else {
					int t = 1;
					if (bookno > BMAX[0]) {
						t++;
						bookno -= BMAX[0];
					}
					curKey->Testament(t);
					curKey->Book(bookno);
				}
				

				if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
//				if (comma) {
					curKey->Chapter(lastKey->Chapter());
					curKey->Verse(chap);  // chap because this is the first number captured
				}
				else {
					if (useChapterAsVerse && verse < 0 && chap > 0 && curKey->getChapterMax() == 1) {
						verse = chap;
						chap = 1;
					}

					
					if (chap >= 0) {
						curKey->Chapter(chap);
					}
					else {
						partial++;
						curKey->Chapter(1);
					}
					if (verse >= 0) {
						curKey->Verse(verse);
						if (suffix) {
							curKey->setSuffix(suffix);
						}
					}
					else {
						partial++;
						curKey->Verse(1);
					}
				}

				// check for '-'
				for (q = 0; ((buf[q]) && (buf[q] == ' ')); q++);
				if ((buf[q] == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
					buf+=q;
					lastKey->LowerBound(*curKey);
					lastKey->setPosition(TOP);
					tmpListKey << *lastKey;
					tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
				}
				else {
					if (!dash) { 	// if last separator was not a dash just add
						if (expandRange && partial) {
							lastKey->LowerBound(*curKey);
							if (partial > 1)
								curKey->setPosition(MAXCHAPTER);
							if (partial > 0)
								*curKey = MAXVERSE;
							lastKey->UpperBound(*curKey);
							*lastKey = TOP;
							tmpListKey << *lastKey;
							tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
						}
						else {
							lastKey->LowerBound(*curKey);
							lastKey->UpperBound(*curKey);
							*lastKey = TOP;
							tmpListKey << *lastKey;
							tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
						}
					}
					else	if (expandRange) {
						VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement());
						if (newElement) {
							if (partial > 1)
								*curKey = MAXCHAPTER;
							if (partial > 0)
								*curKey = MAXVERSE;
							newElement->UpperBound(*curKey);
							*newElement = TOP;
							tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
						}
					}
				}
				lastPartial = partial;
			}
			*book = 0;
			chap = -1;
			verse = -1;
			suffix = 0;
			if (*buf == ',')
				comma = 1;
			else	comma = 0;
			if (*buf == '-')
				dash = 1;
			else	dash = 0;
			break;
		case 10:	// ignore these
		case 13:
		case '[':
		case ']':
		case '(':
		case ')':
		case '{':
		case '}':
			break;
		case '.':
			if (buf > orig)			// ignore (break) if preceeding char is not a digit
				for (notAllDigits = tobook; notAllDigits; notAllDigits--) {
					if ((!isdigit(book[notAllDigits-1])) && (!strchr(" .", book[notAllDigits-1])))
						break;
				}
				if (!notAllDigits)
					break;

			number[tonumber] = 0;
			tonumber = 0;
			if (*number)
				chap = atoi(number);
			*number = 0;
			break;

		default:
			if (isdigit(*buf)) {
				number[tonumber++] = *buf;
			}
			else {
				switch (*buf) {
				case ' ':    // ignore these and don't reset number
				case 'F':
					break;
				default:
					// suffixes (and oddly 'f'-- ff.)
					if ((*buf >= 'a' && *buf <= 'z') && (chap >=0)) {
						suffix = *buf;
					}
					else {
						number[tonumber] = 0;
						tonumber = 0;
					}
					break;
				}
			}
			if (chap == -1)
				book[tobook++] = *buf;
		}
		buf++;
	}
	number[tonumber] = 0;
	tonumber = 0;
	if (*number) {
		if (chap >= 0)
			verse = atoi(number);
		else	chap = atoi(number);
	}
	*number = 0;
	book[tobook] = 0;
	tobook = 0;
	if (*book) {
		for (loop = strlen(book) - 1; loop+1; loop--) {
			if ((isdigit(book[loop])) || (book[loop] == ' ')) {
				book[loop] = 0;
				continue;
			}
			else {
				if ((SW_toupper(book[loop])=='F')&&(loop)) {
					if ((isdigit(book[loop-1])) || (book[loop-1] == ' ') || (SW_toupper(book[loop-1]) == 'F')) {
						book[loop] = 0;
						continue;
					}
				}
			}
			break;
		}

		for (loop = strlen(book) - 1; loop+1; loop--) {
			if (book[loop] == ' ') {
				// "PS C" is ok, but "II C" is not ok
				if (isroman(&book[loop+1]) && !isroman(book,loop)) {
					if (verse == -1) {
						verse = chap;
						chap = from_rom(&book[loop+1]);
						book[loop] = 0;
					}
				}
				break;
			}
		}

		if ((!stricmp(book, "V")) || (!stricmp(book, "VER"))) {	// Verse abbrev.
			if (verse == -1) {
				verse = chap;
				chap = lastKey->Chapter();
				*book = 0;
			}
		}

		if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
			strcpy(book, lastKey->getBookName());
		}
		bookno = getBookAbbrev(book);
	}
	if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
		char partial = 0;
		curKey->Verse(1);
		curKey->Chapter(1);
		curKey->Book(1);

		if (bookno < 0) {
			curKey->Testament(lastKey->Testament());
			curKey->Book(lastKey->Book());
		}
		else {
			int t = 1;
			if (bookno > BMAX[0]) {
				t++;
				bookno -= BMAX[0];
			}
			curKey->Testament(t);
			curKey->Book(bookno);
		}

		if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
			curKey->Chapter(lastKey->Chapter());
			curKey->Verse(chap);  // chap because this is the first number captured
		}
		else {
			if (useChapterAsVerse && verse < 0 && chap > 0 && curKey->getChapterMax() == 1) {
				verse = chap;
				chap = 1;
			}

			
			if (chap >= 0) {
				curKey->Chapter(chap);
			}
			else {
				partial++;
				curKey->Chapter(1);
			}
			if (verse >= 0) {
				curKey->Verse(verse);
				if (suffix) {
					curKey->setSuffix(suffix);
				}
			}
			else {
				partial++;
				curKey->Verse(1);
			}
		}

		if ((*buf == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
			lastKey->LowerBound(*curKey);
			*lastKey = TOP;
			tmpListKey << *lastKey;
			tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
		}
		else {
			if (!dash) { 	// if last separator was not a dash just add
				if (expandRange && partial) {
					lastKey->LowerBound(*curKey);
					if (partial > 1)
						*curKey = MAXCHAPTER;
					if (partial > 0)
						*curKey = MAXVERSE;
					lastKey->UpperBound(*curKey);
					*lastKey = TOP;
					tmpListKey << *lastKey;
					tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
				}
				else {
					lastKey->LowerBound(*curKey);
					lastKey->UpperBound(*curKey);
					*lastKey = TOP;
					tmpListKey << *lastKey;
//					tmpListKey << curKey->getText();
					tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
				}
			}
			else if (expandRange) {
				VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement());
				if (newElement) {
					if (partial > 1)
						*curKey = MAXCHAPTER;
					if (partial > 0)
						*curKey = MAXVERSE;
						newElement->UpperBound(*curKey);
					*newElement = TOP;
					tmpListKey.GetElement()->userData = (void *)(bufStart+(buf-iBuf.c_str()));
				}
			}
		}
	}
	*book = 0;
	tmpListKey = TOP;
	internalListKey = tmpListKey;
	internalListKey = TOP;	// Align internalListKey to first element before passing back;

	delete curKey;
	delete lastKey;

	return internalListKey;
}


/******************************************************************************
 * VerseKey::LowerBound	- sets / gets the lower boundary for this key
 */

VerseKey &VerseKey::LowerBound(const VerseKey &lb)
{
	initBounds();

	lowerBound = lb.Index();
	lowerBoundComponents.test  = lb.getTestament();
	lowerBoundComponents.book  = lb.getBook();
	lowerBoundComponents.chap  = lb.getChapter();
	lowerBoundComponents.verse = lb.getVerse();

	// both this following check and UpperBound check force upperBound to
	// change allowing LowerBound then UpperBound logic to always flow
	// and set values without restrictions, as expected
	if (upperBound < lowerBound) upperBound = lowerBound;
	boundSet = true;

	return LowerBound();
}


/******************************************************************************
 * VerseKey::UpperBound	- sets / gets the upper boundary for this key
 */

VerseKey &VerseKey::UpperBound(const VerseKey &ub)
{
	initBounds();

	upperBound = ub.Index();
	upperBoundComponents.test  = ub.getTestament();
	upperBoundComponents.book  = ub.getBook();
	upperBoundComponents.chap  = ub.getChapter();
	upperBoundComponents.verse = ub.getVerse();

	// see LowerBound comment, above
	if (upperBound < lowerBound) upperBound = lowerBound;
	boundSet = true;

	return UpperBound();
}


/******************************************************************************
 * VerseKey::LowerBound	- sets / gets the lower boundary for this key
 */

VerseKey &VerseKey::LowerBound() const
{
	initBounds();
	if (!isAutoNormalize()) {
		tmpClone->testament = lowerBoundComponents.test;
		tmpClone->book      = lowerBoundComponents.book;
		tmpClone->chapter   = lowerBoundComponents.chap;
		tmpClone->setVerse   (lowerBoundComponents.verse);
	}
	else tmpClone->Index(lowerBound);

	return (*tmpClone);
}


/******************************************************************************
 * VerseKey::UpperBound	- sets / gets the upper boundary for this key
 */

VerseKey &VerseKey::UpperBound() const
{
	initBounds();
	if (!isAutoNormalize()) {
		tmpClone->testament = upperBoundComponents.test;
		tmpClone->book      = upperBoundComponents.book;
		tmpClone->chapter   = upperBoundComponents.chap;
		tmpClone->setVerse   (upperBoundComponents.verse);
	}
	else tmpClone->Index(upperBound);

	return (*tmpClone);
}


/******************************************************************************
 * VerseKey::ClearBounds	- clears bounds for this VerseKey
 */

void VerseKey::ClearBounds()
{
	delete tmpClone;
	tmpClone = 0;
	boundSet = false;
}


void VerseKey::initBounds() const
{
	if (!tmpClone) {
		tmpClone = (VerseKey *)this->clone();
		tmpClone->AutoNormalize(0);
		tmpClone->Headings(1);
		tmpClone->Testament((BMAX[1])?2:1);
		tmpClone->Book(BMAX[(BMAX[1])?1:0]);
		tmpClone->Chapter(tmpClone->getChapterMax());
		tmpClone->Verse(tmpClone->getVerseMax());
		upperBound = tmpClone->Index();
		upperBoundComponents.test  = tmpClone->getTestament();
		upperBoundComponents.book  = tmpClone->getBook();
		upperBoundComponents.chap  = tmpClone->getChapter();
		upperBoundComponents.verse = tmpClone->getVerse();

		lowerBound = 0;
		lowerBoundComponents.test  = 0;
		lowerBoundComponents.book  = 0;
		lowerBoundComponents.chap  = 0;
		lowerBoundComponents.verse = 0;

	}
	else tmpClone->setLocale(getLocale());
}


/******************************************************************************
 * VerseKey::getText - refreshes keytext before returning if cast to
 *				a (char *) is requested
 */

const char *VerseKey::getText() const {
	freshtext();
	return keytext;
}


const char *VerseKey::getShortText() const {
	static char *stext = 0;
	char buf[2047];
	freshtext();
	if (book < 1) {
		if (testament < 1)
			sprintf(buf, "[ Module Heading ]");
		else sprintf(buf, "[ Testament %d Heading ]", (int)testament);
	}
	else {
		sprintf(buf, "%s %d:%d", getBookAbbrev(), chapter, verse);
	}
	stdstr(&stext, buf);
	return stext;
}


const char *VerseKey::getBookName() const {
	return getPrivateLocale()->translate(refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getLongName());
}


const char *VerseKey::getOSISBookName() const {
	return refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getOSISName();
}


const char *VerseKey::getBookAbbrev() const {
	return refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getPreferredAbbreviation();
}


/******************************************************************************
 * VerseKey::setPosition(SW_POSITION)	- Positions this key
 *
 * ENT:	p	- position
 *
 * RET:	*this
 */

void VerseKey::setPosition(SW_POSITION p) {
	switch (p) {
	case POS_TOP: {
		const VerseKey *lb = &LowerBound();
		testament = (lb->Testament() || headings) ? lb->Testament() : 1;
		book      = (lb->Book()      || headings) ? lb->Book() : 1;
		chapter   = (lb->Chapter()   || headings) ? lb->Chapter() : 1;
		verse     = (lb->Verse()     || headings) ? lb->Verse() : 1;
		suffix    = lb->getSuffix();
		break;
	}
	case POS_BOTTOM: {
		const VerseKey *ub = &UpperBound();
		testament = (ub->Testament() || headings) ? ub->Testament() : 1;
		book      = (ub->Book()      || headings) ? ub->Book() : 1;
		chapter   = (ub->Chapter()   || headings) ? ub->Chapter() : 1;
		verse     = (ub->Verse()     || headings) ? ub->Verse() : 1;
		suffix    = ub->getSuffix();
		break;
	}
	case POS_MAXVERSE:
		Normalize();
		verse     = getVerseMax();
		suffix    = 0;
		break;
	case POS_MAXCHAPTER:
		verse     = 1;
		suffix    = 0;
		Normalize();
		chapter   = getChapterMax();
		break;
	}
	Normalize(1);
	Error();	// clear error from normalize
}

int VerseKey::getChapterMax() const {
	const VerseMgr::Book *b = refSys->getBook(((testament>1)?BMAX[0]:0)+book-1);
	return (b) ? b->getChapterMax() : -1;
}

int VerseKey::getVerseMax() const {
	const VerseMgr::Book *b = refSys->getBook(((testament>1)?BMAX[0]:0)+book-1);
	return (b) ? b->getVerseMax(chapter) : -1;
}


/******************************************************************************
 * VerseKey::increment	- Increments key a number of verses
 *
 * ENT:	step	- Number of verses to jump forward
 *
 * RET: *this
 */

void VerseKey::increment(int step) {
	char ierror = 0;
	Index(Index() + step);
	while ((!verse) && (!headings) && (!ierror)) {
		Index(Index() + 1);
		ierror = Error();
	}

	error = (ierror) ? ierror : error;
}


/******************************************************************************
 * VerseKey::decrement	- Decrements key a number of verses
 *
 * ENT:	step	- Number of verses to jump backward
 *
 * RET: *this
 */

void VerseKey::decrement(int step) {
	char ierror = 0;

	Index(Index() - step);
	while ((!verse) && (!headings) && (!ierror)) {
		Index(Index() - 1);
		ierror = Error();
	}
	if ((ierror) && (!headings))
		(*this)++;

	error = (ierror) ? ierror : error;
}


/******************************************************************************
 * VerseKey::Normalize	- checks limits and normalizes if necessary (e.g.
 *				Matthew 29:47 = Mark 2:2).  If last verse is
 *				exceeded, key is set to last Book CH:VS
 * RET: *this
 */

void VerseKey::Normalize(char autocheck)
{

	if (((!autocheck) || (autonorm))	// only normalize if we were explicitely called or if autonorm is turned on
               &&
	          ((!headings) || ((verse) && (chapter)))) {		// this is cheeze and temporary until deciding what actions should be taken; so headings should only be turned on when positioning with Index() or incrementors
		error = 0;

          while ((testament < 3) && (testament > 0)) {

               if (book > BMAX[testament-1]) {
                    book -= BMAX[testament-1];
                    testament++;
                    continue;
               }

               if (book < 1) {
                    if (--testament > 0) {
                         book += BMAX[testament-1];
                    }
                    continue;
               }

               if (chapter > getChapterMax()) {
                    chapter -= getChapterMax();
                    book++;
                    continue;
               }

		if (chapter < 1) {
			if (--book > 0) {
				chapter += getChapterMax();
				verse = getVerseMax();
			}
			else {
				if (testament > 1) {
					chapter += refSys->getBook(BMAX[0]-1)->getChapterMax();
					verse = refSys->getBook(BMAX[0]-1)->getVerseMax(chapter);
				}
			}
			continue;
		}

               if (verse > getVerseMax()) { // -1 because e.g chapter 1 of Matthew is books[1][0].versemax[0]
                    verse -= getVerseMax();
				chapter++;
                    continue;
               }

               if (verse < 1) {
                    if (--chapter > 0) {
                         verse += getVerseMax();
                    }
                    else	{
                         if (book > 1) {
						const VerseMgr::Book *prevBook = refSys->getBook(((testament>1)?BMAX[0]:0)+book-2);
                              verse += prevBook->getVerseMax(prevBook->getChapterMax());
                         }
                         else	{
                              if (testament > 1) {
							const VerseMgr::Book *lastOTBook = refSys->getBook(BMAX[0]-1);
                                   verse += lastOTBook->getVerseMax(lastOTBook->getChapterMax());
                              }
                         }
                    }
                    continue;
               }

               break;  // If we've made it this far (all failure checks continue) we're ok
          }

          if (testament > (BMAX[1]?2:1)) {
               testament = BMAX[1]?2:1;
               book      = BMAX[testament-1];
               chapter   = getChapterMax();
               verse     = getVerseMax();
               error     = KEYERR_OUTOFBOUNDS;
          }

          if (testament < 1) {
               error     = ((!headings) || (testament < 0) || (book < 0)) ? KEYERR_OUTOFBOUNDS : 0;
               testament = ((headings) ? 0 : 1);
               book      = ((headings) ? 0 : 1);
               chapter   = ((headings) ? 0 : 1);
               verse     = ((headings) ? 0 : 1);
          }

          // should we always perform bounds checks?  Tried but seems to cause infinite recursion
          if (_compare(UpperBound()) > 0) {
               positionFrom(UpperBound());
               error = KEYERR_OUTOFBOUNDS;
          }
          if (_compare(LowerBound()) < 0) {
               positionFrom(LowerBound());
               error = KEYERR_OUTOFBOUNDS;
          }
     }
}


/******************************************************************************
 * VerseKey::getTestament - Gets testament
 *
 * RET:	value of testament
 */

char VerseKey::getTestament() const
{
	return testament;
}


/******************************************************************************
 * VerseKey::getBook - Gets book
 *
 * RET:	value of book
 */

char VerseKey::getBook() const
{
	return book;
}


/******************************************************************************
 * VerseKey::Chapter - Gets chapter
 *
 * RET:	value of chapter
 */

int VerseKey::getChapter() const
{
	return chapter;
}


/******************************************************************************
 * VerseKey::Verse - Gets verse
 *
 * RET:	value of verse
 */

int VerseKey::getVerse() const
{
	return verse;
}


/******************************************************************************
 * VerseKey::setTestament - Sets/gets testament
 *
 * ENT:	itestament - value which to set testament
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of testament
 *	if   changed -> previous value of testament
 */

void VerseKey::setTestament(char itestament)
{
	if (itestament != MAXPOS(char)) {
		testament = itestament;
		Normalize(1);
	}
}


/******************************************************************************
 * VerseKey::setBook - Sets/gets book
 *
 * ENT:	ibook - value which to set book
 */

void VerseKey::setBook(char ibook)
{
	verse   = ibook ? 1 : 0;
	chapter = ibook ? 1 : 0;
	book    = ibook;
	Normalize(1);
}



/******************************************************************************
 * VerseKey::setBookName - Sets/gets book by name
 *
 * ENT:	bname - book name/abbrev
 */

void VerseKey::setBookName(const char *bname)
{
	int bnum = getBookAbbrev(bname);
	if (bnum > -1) {
		if (bnum > BMAX[0]) {
			bnum -= BMAX[0];
			testament = 2;
		}
		else	testament = 1;
		setBook(bnum);
	}
	else error = KEYERR_OUTOFBOUNDS;
}
	

/******************************************************************************
 * VerseKey::setChapter - Sets/gets chapter
 *
 * ENT:	ichapter - value which to set chapter
 */

void VerseKey::setChapter(int ichapter)
{
	verse   = ichapter ? 1 : 0;
	chapter = ichapter;
	Normalize(1);
}


/******************************************************************************
 * VerseKey::setVerse - Sets/gets verse
 *
 * ENT:	iverse - value which to set verse
 *		[MAXPOS(int)] - only get
 *
 * RET:	if unchanged ->          value of verse
 *	if   changed -> previous value of verse
 */

void VerseKey::setVerse(int iverse)
{
	setSuffix(0);
	verse = iverse;
	Normalize(1);
}


char VerseKey::getSuffix() const {
	return suffix;
}

void VerseKey::setSuffix(char suf) {
	suffix = suf;
}

/******************************************************************************
 * VerseKey::AutoNormalize - Sets/gets flag that tells VerseKey to auto-
 *				matically normalize itself when modified
 */

bool VerseKey::isAutoNormalize() const
{
	return autonorm;
}

void VerseKey::setAutoNormalize(bool iautonorm)
{
	autonorm = iautonorm?1:0;
	Normalize(1);
}


/******************************************************************************
 * VerseKey::Headings - Sets/gets flag that tells VerseKey to include
 *					chap/book/testmnt/module headings
 *
 * ENT:	iheadings - value which to set headings
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of headings
 *		if   changed -> previous value of headings
 */

char VerseKey::Headings(char iheadings)
{
	char retval = headings;

	if (iheadings != MAXPOS(char)) {
		headings = iheadings;
		Normalize(1);
	}
	return retval;
}


/******************************************************************************
 * VerseKey::findindex - binary search to find the index closest, but less
 *						than the given value.
 *
 * ENT:	array	- long * to array to search
 *		size		- number of elements in the array
 *		value	- value to find
 *
 * RET:	the index into the array that is less than but closest to value
 */

int VerseKey::findindex(long *array, int size, long value)
{
	int lbound, ubound, tval;

	lbound = 0;
	ubound = size - 1;
	while ((ubound - lbound) > 1) {
		tval = lbound + (ubound-lbound)/2;
		if (array[tval] <= value)
			lbound = tval;
		else ubound = tval;
	}
	return (array[ubound] <= value) ? ubound : lbound;
}


/******************************************************************************
 * VerseKey::Index - Gets index based upon current verse
 *
 * RET:	offset
 */

long VerseKey::Index() const
{
	long  offset;

	if (!testament) { // if we want module heading
		offset = 0;
	}
	else if (!book) {	// we want testament heading
		offset = ((testament == 2) ? refSys->getNTStartOffset():0) + 1;
	}
	else {
		offset = refSys->getOffsetFromVerse((((testament>1)?BMAX[0]:0)+book-1), chapter, verse);
	}
	return offset;
}


/******************************************************************************
 * VerseKey::TestamentIndex - Gets index based upon current verse
 *
 * RET:	offset
 */

long VerseKey::TestamentIndex() const
{
	long offset = Index();
	return (testament > 1) ? offset - refSys->getNTStartOffset() : offset;
}


/******************************************************************************
 * VerseKey::Index - Sets index based upon current verse
 *
 * ENT:	iindex - value to set index to
 *
 * RET:	offset
 */

long VerseKey::Index(long iindex)
{
	int b;
	error = refSys->getVerseFromOffset(iindex, &b, &chapter, &verse);
	book = (unsigned char)b;
	testament = 1;
	if (book > BMAX[0]) {
		book -= BMAX[0];
		testament = 2;
	}
	// special case for Module and Testament heading
	if (book < 0) { testament = 0; book = 0; }
	if (chapter < 0) { book = 0; chapter = 0; }

	long i = Index();

	initBounds();
	if (i > upperBound) {
		i = Index(upperBound);
		error = KEYERR_OUTOFBOUNDS;
	}
	if (i < lowerBound) {
		i = Index(lowerBound);
		error = KEYERR_OUTOFBOUNDS;
	}

	return i;
}


/******************************************************************************
 * VerseKey::compare	- Compares another SWKey object
 *
 * ENT:	ikey - key to compare with this one
 *
 * RET:	>0 if this versekey is greater than compare versekey
 *	<0 <
 *	 0 =
 */

int VerseKey::compare(const SWKey &ikey)
{
	const SWKey *testKey = &ikey;
	const VerseKey *vkey = (const VerseKey *)SWDYNAMIC_CAST(VerseKey, testKey);
	if (vkey) {
		return _compare(*vkey);
	}
	const VerseKey ivkey = (const char *)ikey;
	return _compare(ivkey);
}


/******************************************************************************
 * VerseKey::_compare	- Compares another VerseKey object
 *
 * ENT:	ikey - key to compare with this one
 *
 * RET:	>0 if this versekey is greater than compare versekey
 *	<0 <
 *	 0 =
 */

int VerseKey::_compare(const VerseKey &ivkey)
{
	unsigned long keyval1 = 0;
	unsigned long keyval2 = 0;

	keyval1 += Testament()       * 1000000000;
	keyval2 += ivkey.Testament() * 1000000000;
	keyval1 += Book()            * 10000000;
	keyval2 += ivkey.Book()      * 10000000;
	keyval1 += Chapter()         * 10000;
	keyval2 += ivkey.Chapter()   * 10000;
	keyval1 += Verse()           * 50;
	keyval2 += ivkey.Verse()     * 50;
	keyval1 += (int)getSuffix();
	keyval2 += (int)ivkey.getSuffix();
	keyval1 = (keyval1 != keyval2) ? ((keyval1 > keyval2) ? 1 : -1) : 0; // -1 | 0 | 1
	return keyval1;
}


const char *VerseKey::getOSISRef() const {
	static char buf[5][254];
	static int loop = 0;

	if (loop > 4)
		loop = 0;

	if (Verse())
		sprintf(buf[loop], "%s.%d.%d", getOSISBookName(), (int)Chapter(), (int)Verse());
	else if (Chapter())
		sprintf(buf[loop], "%s.%d", getOSISBookName(), (int)Chapter());
	else if (Book())
		sprintf(buf[loop], "%s", getOSISBookName());
	else	buf[loop][0] = 0;
	return buf[loop++];
}


/******************************************************************************
 * VerseKey::getRangeText - returns parsable range text for this key
 */

const char *VerseKey::getRangeText() const {
	if (isBoundSet() && lowerBound != upperBound) {
		SWBuf buf = (const char *)LowerBound();
		buf += "-";
		buf += (const char *)UpperBound();
		stdstr(&rangeText, buf.c_str());
	}
	else stdstr(&rangeText, getText());
	return rangeText;
}


/******************************************************************************
 * VerseKey::getOSISRefRangeText - returns parsable range text for this key
 */

const char *VerseKey::getOSISRefRangeText() const {
	if (isBoundSet() && (lowerBound != upperBound)) {
		SWBuf buf = LowerBound().getOSISRef();
		buf += "-";
		buf += UpperBound().getOSISRef();
		stdstr(&rangeText, buf.c_str());
	}
	else stdstr(&rangeText, getOSISRef());
	return rangeText;
}


// TODO:  this is static so we have no context.  We can only parse KJV v11n now
// 		possibly add a const char *versification = KJV param?
const char *VerseKey::convertToOSIS(const char *inRef, const SWKey *lastKnownKey) {
	static SWBuf outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*lastKnownKey), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		SWKey *element = verses.GetElement(i);
//		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		SWBuf buf;
		// TODO: This code really needs to not use fixed size arrays
		char frag[800];
		char preJunk[800];
		char postJunk[800];
		memset(frag, 0, 800);
		memset(preJunk, 0, 800);
		memset(postJunk, 0, 800);
		while ((*startFrag) && (strchr(" {};,()[].", *startFrag))) {
			outRef += *startFrag;
			startFrag++;
		}
		memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
		frag[((const char *)element->userData - startFrag) + 1] = 0;
		int j;
		for (j = strlen(frag)-1; j && (strchr(" {};,()[].", frag[j])); j--);
		if (frag[j+1])
			strcpy(postJunk, frag+j+1);
		frag[j+1]=0;
		startFrag += ((const char *)element->userData - startFrag) + 1;
		buf = "<reference osisRef=\"";
		buf += element->getOSISRefRangeText();
		buf += "\">";
		buf += frag;
		buf += "</reference>";
		buf += postJunk;

		outRef += buf;

	}
	if (startFrag < (inRef + strlen(inRef)))
		outRef += startFrag;
	return outRef.c_str();
}
SWORD_NAMESPACE_END
