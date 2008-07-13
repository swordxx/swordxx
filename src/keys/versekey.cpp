/******************************************************************************
 *  VerseKey.cpp - code for class 'VerseKey'- a standard Biblical verse key
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
#include <localemgr.h>
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
VerseKey::LocaleCache   VerseKey::localeCache;


/******************************************************************************
 * VerseKey::init - initializes instance of VerseKey
 */

void VerseKey::init() {
	myclass = &classdef;

	instance++;
	autonorm = 1;		// default auto normalization to true
	headings = 0;		// default display headings option is false
	upperBound = 0;
	lowerBound = 0;
	boundSet = false;
	testament = 0;
	book = 0;
	chapter = 0;
	verse = 0;
	suffix = 0;
	locale = 0;

	setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
	setVersificationSystem("KJV");
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
 * VerseKey::positionFrom - Positions this VerseKey to another VerseKey
 */

void VerseKey::positionFrom(const VerseKey &ikey) {
	error = 0;
	testament = ikey.Testament();
	book = ikey.Book();
	chapter = ikey.Chapter();
	verse = ikey.Verse();
	suffix = ikey.getSuffix();
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


VerseKey::VerseKey(const char *min, const char *max) : SWKey()
{
	init();
	LowerBound(min);
	UpperBound(max);
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
	if (upperBound)
		delete upperBound;
	if (lowerBound)
		delete lowerBound;
	if (locale)
		delete [] locale;

	--instance;
}


void VerseKey::setVersificationSystem(const char *name) {
	refSys = VerseMgr::getSystemVerseMgr()->getVersificationSystem(name);
	BMAX[0] = refSys->getBMAX()[0];
	BMAX[1] = refSys->getBMAX()[1];
}


const char *VerseKey::getVersificationSystem() const { return refSys->getName(); }

void VerseKey::setLocale(const char *name) {
	bool useCache = false;

	if (localeCache.name)
		useCache = (!strcmp(localeCache.name, name));

	if (!useCache)	{	// if we're setting params for a new locale
		stdstr(&(localeCache.name), name);
	}

	SWLocale *locale = (useCache) ? localeCache.locale : LocaleMgr::getSystemLocaleMgr()->getLocale(name);
	localeCache.locale = locale;

//	setBookAbbrevs((locale)?locale->getBookAbbrevs():builtin_abbrevs, localeCache.abbrevsCnt);
	stdstr(&(this->locale), localeCache.name);

	if (lowerBound)
		LowerBound().setLocale(name);
	if (upperBound)
		UpperBound().setLocale(name);
}




/******************************************************************************
 * VerseKey::parse - parses keytext into testament|book|chapter|verse
 *
 * RET:	error status
 */

char VerseKey::parse(bool checkAutoNormalize)
{


	testament = 2;
	book      = BMAX[1];
	chapter   = 1;
	verse     = 1;
	//int booklen   = 0;

	int error = 0;

	if (keytext) {
		ListKey tmpListKey = VerseKey::ParseVerseList(keytext);
		if (tmpListKey.Count()) {

			this->positionFrom(tmpListKey.getElement(0));
			error = this->error;
/*
			SWKey::setText((const char *)tmpListKey);
			for (int i = 1; i < 3; i++) {
				for (int j = 1; j <= BMAX[i-1]; j++) {
					int matchlen = strlen(books[i-1][j-1].name);
					if (!strncmp(keytext, books[i-1][j-1].name, matchlen)) {
						if (matchlen > booklen) {
							booklen = matchlen;
							testament = i;
							book = j;
						}
					}
				}
			}

			if (booklen) {
				sscanf(&keytext[booklen], "%d:%d", &chapter, &verse);
			}
			else	error = 1;
*/
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
		sprintf(buf, "%s %d:%d", refSys->getBook(((realTest>1)?BMAX[0]:0)+realbook-1)->getLongName(), chapter, verse);
		if (suffix) {
			buf[strlen(buf)+1] = 0;
			buf[strlen(buf)] = suffix;
		}
	}

	stdstr((char **)&keytext, buf);
}



/******************************************************************************
 * VerseKey::getBookAbbrev - Attempts to find a book abbreviation for a buffer
 *
 * ENT:	abbr - key for which to search;
 * RET:	book number or < 0 = not valid
 */

int VerseKey::getBookAbbrev(const char *iabbr)
{
	int diff, abLen, min, max, target, retVal = -1;

	char *abbr = 0;

	int abbrevsCnt;
	bool useCache = false;

	useCache = (localeCache.name && localeCache.locale);

	if (!useCache)	{	// if we're setting params for a new locale
		stdstr(&(localeCache.name), locale);
		localeCache.locale = LocaleMgr::getSystemLocaleMgr()->getLocale(locale);
	}

	const struct abbrev *abbrevs = localeCache.locale->getBookAbbrevs(&abbrevsCnt);

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

			retVal = (!diff) ? abbrevs[target].book : -1;
		}
		if (retVal > 0)
			break;
	}
	delete [] abbr;
	return retVal;
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

ListKey VerseKey::ParseVerseList(const char *buf, const char *defaultKey, bool expandRange) {
	char book[2048];
	char number[2048];
	*book = 0;
	*number = 0;
	int tobook = 0;
	int tonumber = 0;
	char suffix = 0;
	int chap = -1, verse = -1;
	int bookno = 0;
	VerseKey curKey, lBound, lastKey;
	curKey.setLocale(getLocale());
	lBound.setLocale(getLocale());
	lastKey.setLocale(getLocale());
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

	curKey.AutoNormalize(AutoNormalize());
	lastKey.AutoNormalize(0);
	lBound.AutoNormalize(0);
	if (defaultKey) lastKey = defaultKey;

	while (*buf) {
		switch (*buf) {
		case ':':
			if (buf[1] != ' ') {		// for silly Mat 1:1: this verse....
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
						if (isroman(&book[loop+1])) {
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
						chap = lastKey.Chapter();
						*book = 0;
					}
				}
				if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
					strcpy(book, lastKey.getBookName());
				}
				bookno = getBookAbbrev(book);
			}
			if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
				char partial = 0;
				curKey.Verse(1);
				curKey.Chapter(1);
				curKey.Book(1);

				if (bookno < 0) {
					curKey.Testament(lastKey.Testament());
					curKey.Book(lastKey.Book());
				}
				else {
					int t = 1;
					if (bookno > BMAX[0]) {
						t++;
						bookno -= BMAX[0];
					}
					curKey.Testament(t);
					curKey.Book(bookno);
				}

				if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
//				if (comma) {
					curKey.Chapter(lastKey.Chapter());
					curKey.Verse(chap);  // chap because this is the first number captured
				}
				else {
					if (chap >= 0) {
						curKey.Chapter(chap);
					}
					else {
						partial++;
						curKey.Chapter(1);
					}
					if (verse >= 0) {
						curKey.Verse(verse);
						if (suffix) {
							curKey.setSuffix(suffix);
						}
					}
					else {
						partial++;
						curKey.Verse(1);
					}
				}

				// check for '-'
				for (q = 0; ((buf[q]) && (buf[q] == ' ')); q++);
				if ((buf[q] == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
					buf+=q;
					lastKey.LowerBound(curKey);
					lastKey.setPosition(TOP);
					tmpListKey << lastKey;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
				else {
					if (!dash) { 	// if last separator was not a dash just add
						if (expandRange && partial) {
							lastKey.LowerBound(curKey);
							if (partial > 1)
								curKey.setPosition(MAXCHAPTER);
							if (partial > 0)
								curKey = MAXVERSE;
							lastKey.UpperBound(curKey);
							lastKey = TOP;
							tmpListKey << lastKey;
							tmpListKey.GetElement()->userData = (void *)buf;
						}
						else {
							// we store non-range entries as strings so we don't traverse
							// maybe we should consider just setting
							// lowerBound and upperBound to the same value
							lastKey.LowerBound(curKey);
							lastKey.UpperBound(curKey);
							lastKey = TOP;
							tmpListKey << lastKey;
							tmpListKey.GetElement()->userData = (void *)buf;
						}
					}
					else	if (expandRange) {
						VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement());
						if (newElement) {
							if (partial > 1)
								curKey = MAXCHAPTER;
							if (partial > 0)
								curKey = MAXVERSE;
							newElement->UpperBound(curKey);
							*newElement = TOP;
							tmpListKey.GetElement()->userData = (void *)buf;
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
					if (*buf >= 'a' && *buf <= 'z') {
						if (chap >=0) suffix = *buf;
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
				if (isroman(&book[loop+1])) {
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
				chap = lastKey.Chapter();
				*book = 0;
			}
		}

		if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
			strcpy(book, lastKey.getBookName());
		}
		bookno = getBookAbbrev(book);
	}
	if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
		char partial = 0;
		curKey.Verse(1);
		curKey.Chapter(1);
		curKey.Book(1);

		if (bookno < 0) {
			curKey.Testament(lastKey.Testament());
			curKey.Book(lastKey.Book());
		}
		else {
			int t = 1;
			if (bookno > BMAX[0]) {
				t++;
				bookno -= BMAX[0];
			}
			curKey.Testament(t);
			curKey.Book(bookno);
		}

		if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
			curKey.Chapter(lastKey.Chapter());
			curKey.Verse(chap);  // chap because this is the first number captured
		}
		else {
			if (chap >= 0) {
				curKey.Chapter(chap);
			}
			else {
				partial++;
				curKey.Chapter(1);
			}
			if (verse >= 0) {
				curKey.Verse(verse);
				if (suffix) {
					curKey.setSuffix(suffix);
				}
			}
			else {
				partial++;
				curKey.Verse(1);
			}
		}

		if ((*buf == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
			lastKey.LowerBound(curKey);
			lastKey = TOP;
			tmpListKey << lastKey;
			tmpListKey.GetElement()->userData = (void *)buf;
		}
		else {
			if (!dash) { 	// if last separator was not a dash just add
				if (expandRange && partial) {
					lastKey.LowerBound(curKey);
					if (partial > 1)
						curKey = MAXCHAPTER;
					if (partial > 0)
						curKey = MAXVERSE;
					lastKey.UpperBound(curKey);
					lastKey = TOP;
					tmpListKey << lastKey;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
				else {
					lastKey.LowerBound(curKey);
					lastKey.UpperBound(curKey);
					lastKey = TOP;
					tmpListKey << lastKey;
//					tmpListKey << curKey.getText();
					tmpListKey.GetElement()->userData = (void *)buf;
				}
			}
			else if (expandRange) {
				VerseKey *newElement = SWDYNAMIC_CAST(VerseKey, tmpListKey.GetElement());
				if (newElement) {
					if (partial > 1)
						curKey = MAXCHAPTER;
					if (partial > 0)
						curKey = MAXVERSE;
						newElement->UpperBound(curKey);
					*newElement = TOP;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
			}
		}
	}
	*book = 0;
	tmpListKey = TOP;
	internalListKey = tmpListKey;
	internalListKey = TOP;	// Align internalListKey to first element before passing back;

	return internalListKey;
}


/******************************************************************************
 * VerseKey::LowerBound	- sets / gets the lower boundary for this key
 */

VerseKey &VerseKey::LowerBound(const VerseKey &lb)
{
	if (!lowerBound)
		initBounds();

	(*lowerBound) = lb;
// why are we normalizing?
//	lowerBound->Normalize();
	lowerBound->setLocale( this->getLocale() );
	boundSet = true;
	return (*lowerBound);
}


/******************************************************************************
 * VerseKey::UpperBound	- sets / gets the upper boundary for this key
 */

VerseKey &VerseKey::UpperBound(const VerseKey &ub)
{
	if (!upperBound)
		initBounds();

// need to set upperbound parsing to resolve to max verse/chap if not specified
	   (*upperBound) = ub;
	if (*upperBound < *lowerBound)
		*upperBound = *lowerBound;
// why are we normalizing?
//	upperBound->Normalize();
	upperBound->setLocale( this->getLocale() );

// until we have a proper method to resolve max verse/chap use this kludge
/*
	int len = strlen(ub);
	bool alpha = false;
	bool versespec = false;
	bool chapspec = false;
	for (int i = 0; i < len; i++) {
		if (isalpha(ub[i]))
			alpha = true;
		if (ub[i] == ':')	// if we have a : we assume verse spec
			versespec = true;
		if ((isdigit(ub[i])) && (alpha))	// if digit after alpha assume chap spec
			chapspec = true;
	}
	if (!chapspec)
		*upperBound = MAXCHAPTER;
	if (!versespec)
		*upperBound = MAXVERSE;
*/
// -- end kludge

	boundSet = true;
	return (*upperBound);
}


/******************************************************************************
 * VerseKey::LowerBound	- sets / gets the lower boundary for this key
 */

VerseKey &VerseKey::LowerBound() const
{
	if (!lowerBound)
		initBounds();

	return (*lowerBound);
}


/******************************************************************************
 * VerseKey::UpperBound	- sets / gets the upper boundary for this key
 */

VerseKey &VerseKey::UpperBound() const
{
	if (!upperBound)
		initBounds();

	return (*upperBound);
}


/******************************************************************************
 * VerseKey::ClearBounds	- clears bounds for this VerseKey
 */

void VerseKey::ClearBounds()
{
	initBounds();
}


void VerseKey::initBounds() const
{
	if (!upperBound) {
		upperBound = new VerseKey();
		upperBound->AutoNormalize(0);
		upperBound->Headings(1);
	}
	if (!lowerBound) {
		lowerBound = new VerseKey();
		lowerBound->AutoNormalize(0);
		lowerBound->Headings(1);
	}

	lowerBound->Testament(0);
	lowerBound->Book(0);
	lowerBound->Chapter(0);
	lowerBound->Verse(0);

	upperBound->Testament(2);
	upperBound->Book(BMAX[1]);
	const VerseMgr::Book *b = refSys->getBook(BMAX[0]+BMAX[1]-1);
	upperBound->Chapter(b->getChapterMax());
	upperBound->Verse(b->getVerseMax(b->getChapterMax()));
	boundSet = false;
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
	return refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getLongName();
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
	case POS_TOP:
		testament = LowerBound().Testament();
		book      = LowerBound().Book();
		chapter   = LowerBound().Chapter();
		verse     = LowerBound().Verse();
		suffix    = LowerBound().getSuffix();
		break;
	case POS_BOTTOM:
		testament = UpperBound().Testament();
		book      = UpperBound().Book();
		chapter   = UpperBound().Chapter();
		verse     = UpperBound().Verse();
		suffix    = UpperBound().getSuffix();
		break;
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
	return refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getChapterMax();
}

int VerseKey::getVerseMax() const {
	return refSys->getBook(((testament>1)?BMAX[0]:0)+book-1)->getVerseMax(chapter);
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
                    }
                    else	{
                         if (testament > 1) {
						chapter += refSys->getBook(BMAX[0]-1)->getChapterMax();
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

          if (testament > 2) {
               testament = 2;
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
 * VerseKey::Testament - Gets testament
 *
 * RET:	value of testament
 */

char VerseKey::getTestament() const
{
	return testament;
}


/******************************************************************************
 * VerseKey::Book - Gets book
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
 * VerseKey::Testament - Sets/gets testament
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
 * VerseKey::Book - Sets/gets book
 *
 * ENT:	ibook - value which to set book
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of book
 *	if   changed -> previous value of book
 */

void VerseKey::setBook(char ibook)
{
	Chapter(1);
	book = ibook;
	Normalize(1);
}


/******************************************************************************
 * VerseKey::Chapter - Sets/gets chapter
 *
 * ENT:	ichapter - value which to set chapter
 *		[MAXPOS(int)] - only get
 *
 * RET:	if unchanged ->          value of chapter
 *	if   changed -> previous value of chapter
 */

void VerseKey::setChapter(int ichapter)
{
	Verse(1);
	chapter = ichapter;
	Normalize(1);
}


/******************************************************************************
 * VerseKey::Verse - Sets/gets verse
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
 *
 * ENT:	iautonorm - value which to set autonorm
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of autonorm
 *		if   changed -> previous value of autonorm
 */

char VerseKey::AutoNormalize(char iautonorm)
{
	char retval = autonorm;

	if (iautonorm != MAXPOS(char)) {
		autonorm = iautonorm;
		Normalize(1);
	}
	return retval;
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
		verse  = 0;
	}
	else {
		if (!book)
			chapter = 0;
		if (!chapter)
			verse   = 0;

		offset = refSys->getOffsetFromVerse((((testament>1)?BMAX[0]:0)+book-1), chapter, verse);
	}
	return offset;
}


/******************************************************************************
 * VerseKey::Index - Gets index based upon current verse
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

/*
	suffix = 0;
// This is the dirty stuff --------------------------------------------

	if (!testament)
		testament = 1;

	if (iindex < 1) {				// if (-) or module heading
		if (testament < 2) {
			if (iindex < 0) {
				testament = 0;  // previously we changed 0 -> 1
				error     = KEYERR_OUTOFBOUNDS;
			}
			else testament = 0;		// we want module heading
		}
		else {
			testament--;
			const VerseMgr::Book *b = refSys->getBook(BMAX[0]+((testament)?BMAX[1]:0)-1);
			iindex = offsets[testament-1][1][offsize[testament-1][1]-1]
			       + b->getVerseMax(b->getChapterMax())
			       + iindex; // What a doozy! ((offset of last chapter + number of verses in the last chapter) + iindex)
		}
	}

// --------------------------------------------------------------------


	if (testament) {
		if ((!error) && (iindex)) {
			offset  = findindex(offsets[testament-1][1], offsize[testament-1][1], iindex);
			verse   = iindex - offsets[testament-1][1][offset];
			book    = findindex(offsets[testament-1][0], offsize[testament-1][0], offset);
			chapter = offset - offsets[testament-1][0][VerseKey::book];
			verse   = (chapter) ? verse : 0;  // funny check. if we are index=1 (testmt header) all gets set to 0 exept verse.  Don't know why.  Fix if you figure out.  Think its in the offsets table.
			if (verse) {		// only check if -1 won't give negative
				if (verse > getVerseMax()) {
					if (testament > 1) {
						verse = getVerseMax();
						error = KEYERR_OUTOFBOUNDS;
					}
					else {
						testament++;
						const VerseMgr::Book *b = refSys->getBook(book-1);
						Index(verse - b->getVerseMax(chapter-1));
					}
				}
			}
		}
	}
*/
	if (_compare(UpperBound()) > 0) {
		*this = UpperBound();
		error = KEYERR_OUTOFBOUNDS;
	}
	if (_compare(LowerBound()) < 0) {
		*this = LowerBound();
		error = KEYERR_OUTOFBOUNDS;
	}
	return Index();
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
	long keyval1 = 0;
	long keyval2 = 0;

	keyval1 += Testament() * 1000000000;
	keyval2 += ivkey.Testament() * 1000000000;
	keyval1 += Book() * 1000000;
	keyval2 += ivkey.Book() * 1000000;
	keyval1 += Chapter() * 1000;
	keyval2 += ivkey.Chapter() * 1000;
	keyval1 += Verse() * 50;
	keyval2 += ivkey.Verse() * 50;
	keyval1 += (int)getSuffix();
	keyval2 += (int)ivkey.getSuffix();
	keyval1 -= keyval2;
	keyval1 = (keyval1) ? ((keyval1 > 0) ? 1 : -1) /*keyval1/labs(keyval1)*/:0; // -1 | 0 | 1
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
	if (isBoundSet() && (LowerBound() != UpperBound())) {
		char buf[1023];
		sprintf(buf, "%s-%s", (const char *)LowerBound(), (const char *)UpperBound());
		stdstr(&rangeText, buf);
	}
	else stdstr(&rangeText, getText());
	return rangeText;
}


const char *VerseKey::convertToOSIS(const char *inRef, const SWKey *lastKnownKey) {
	static SWBuf outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*lastKnownKey), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		char buf[5120];
		char frag[800];
		char preJunk[800];
		char postJunk[800];
		memset(buf, 0, 5120);
		memset(frag, 0, 800);
		memset(preJunk, 0, 800);
		memset(postJunk, 0, 800);
		while ((*startFrag) && (strchr(" {};,()[].", *startFrag))) {
			outRef += *startFrag;
			startFrag++;
		}
		if (element) {
			memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
			frag[((const char *)element->userData - startFrag) + 1] = 0;
			int j;
			for (j = strlen(frag)-1; j && (strchr(" {};,()[].", frag[j])); j--);
			if (frag[j+1])
				strcpy(postJunk, frag+j+1);
			frag[j+1]=0;
			startFrag += ((const char *)element->userData - startFrag) + 1;
			sprintf(buf, "<reference osisRef=\"%s-%s\">%s</reference>%s", element->LowerBound().getOSISRef(), element->UpperBound().getOSISRef(), frag, postJunk);
		}
		else {
			memmove(frag, startFrag, ((const char *)verses.GetElement(i)->userData - startFrag) + 1);
			frag[((const char *)verses.GetElement(i)->userData - startFrag) + 1] = 0;
			int j;
			for (j = strlen(frag)-1; j && (strchr(" {};,()[].", frag[j])); j--);
			if (frag[j+1])
				strcpy(postJunk, frag+j+1);
			frag[j+1]=0;
			startFrag += ((const char *)verses.GetElement(i)->userData - startFrag) + 1;
			sprintf(buf, "<reference osisRef=\"%s\">%s</reference>%s", VerseKey(*verses.GetElement(i)).getOSISRef(), frag, postJunk);
		}
		outRef+=buf;
	}
	if (startFrag < (inRef + strlen(inRef)))
		outRef+=startFrag;
	return outRef.c_str();
}
SWORD_NAMESPACE_END
