/******************************************************************************
 *  VerseKey2.cpp - code for class 'VerseKey2'- the new version of a standard Biblical verse key
 */

#include <swmacs.h>
#include <utilfuns.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <utilstr.h>
#include <swkey.h>
#include <swlog.h>
#include <versekey2.h>
#include <localemgr.h>
#include <roman.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"VerseKey2", "SWKey", "SWObject", 0};
SWClass VerseKey2::classdef(classes);

/******************************************************************************
 *  Initialize static members of VerseKey2
 */

#include <osisbook.h>	// Initialize static members of canonical books structure

//struct sbook *VerseKey2::builtin_books[2]       = {0,0};
struct sbook2 *VerseKey2::builtin_books       = {0};
//const char    VerseKey2::builtin_BMAX[2]        = {39, 27};
const char    VerseKey2::builtin_BMAX        = OSISBMAX;
//long         *VerseKey2::offsets[2][2]  = {{VerseKey2::otbks, VerseKey2::otcps}, {VerseKey2::ntbks, VerseKey2::ntcps}};
bkref         *VerseKey2::offsets[2]  = {VerseKey2::kjvbks, VerseKey2::kjvcps};
int           VerseKey2::instance       = 0;
//VerseKey2::LocaleCache   VerseKey2::localeCache;
VerseKey2::LocaleCache   *VerseKey2::localeCache = 0;


/******************************************************************************
 * VerseKey2::init - initializes instance of VerseKey2
 */

void VerseKey2::init() {
	myclass = &classdef;
	if (!instance)
		initstatics();

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
	locale = 0;
	abbrevsCnt = BUILTINABBREVCNT;
	oldindexhack = true;

	setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
}

/******************************************************************************
 * VerseKey2 Constructor - initializes instance of VerseKey2
 *
 * ENT:	ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseKey2::parse for more detailed information)
 */

VerseKey2::VerseKey2(const SWKey *ikey) : SWKey(*ikey)
{
	init();
	if (ikey)
		parse();
}


/******************************************************************************
 * VerseKey2 Constructor - initializes instance of VerseKey2
 *
 * ENT:	ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *		VerseKey2::parse for more detailed information)
 */

VerseKey2::VerseKey2(const char *ikey) : SWKey(ikey)
{
	init();
	if (ikey)
		parse();
}


VerseKey2::VerseKey2(VerseKey2 const &k) : SWKey(k)
{
	init();
	autonorm = k.autonorm;
	headings = k.headings;
	testament = k.Testament();
	book = k.Book();
	chapter = k.Chapter();
	verse = k.Verse();
	if (k.isBoundSet()) {
		LowerBound(k.LowerBound());
		UpperBound(k.UpperBound());
	}
}


VerseKey2::VerseKey2(const char *min, const char *max) : SWKey()
{
	init();
	LowerBound(min);
	UpperBound(max);
	setPosition(TOP);
}


SWKey *VerseKey2::clone() const
{
	return new VerseKey2(*this);
}


/******************************************************************************
 * VerseKey2 Destructor - cleans up instance of VerseKey2
 *
 * ENT:	ikey - text key
 */

VerseKey2::~VerseKey2() {
	if (upperBound)
		delete upperBound;
	if (lowerBound)
		delete lowerBound;
	if (locale)
		delete [] locale;

	--instance;
	if (!instance) delete localeCache;
}


void VerseKey2::setLocale(const char *name) {
	char *lBMAX;
	struct sbook2 **lbooks;
	bool useCache = false;

	if (localeCache->name)
		useCache = (!strcmp(localeCache->name, name));

	if (!useCache)	{	// if we're setting params for a new locale
		stdstr(&(localeCache->name), name);
		localeCache->abbrevsCnt = 0;
	}

	SWLocale *locale = (useCache) ? localeCache->locale : LocaleMgr::getSystemLocaleMgr()->getLocale(name);
	localeCache->locale = locale;

	if (locale) {
		#ifdef SPLITLIB
		locale->getBooks2(&lBMAX, &lbooks, this);
		#else
		locale->getBooks(&lBMAX, &lbooks, this);
		#endif
		setBooks(lBMAX, lbooks);
		setBookAbbrevs((struct abbrev2 *)locale->getBookAbbrevs(), localeCache->abbrevsCnt);
		localeCache->abbrevsCnt = abbrevsCnt;
	}
	else {
		setBooks(&builtin_BMAX, &builtin_books);
		setBookAbbrevs(builtin_abbrevs, localeCache->abbrevsCnt);
		localeCache->abbrevsCnt = abbrevsCnt;
	}
	stdstr(&(this->locale), localeCache->name);

	if (lowerBound)
		LowerBound().setLocale(name);
	if (upperBound) 
		UpperBound().setLocale(name);
}

void VerseKey2::setBooks(const char *iBMAX, struct sbook2 **ibooks) {
	BMAX = iBMAX;
	books = ibooks;
}


void VerseKey2::setBookAbbrevs(const struct abbrev2 *bookAbbrevs, unsigned int size) {
	abbrevs = bookAbbrevs;
	if (!size) {
			/*
		for (abbrevsCnt = 0; *abbrevs[abbrevsCnt].ab; abbrevsCnt++) {
			if (strcmp(abbrevs[abbrevsCnt-1].ab, abbrevs[abbrevsCnt].ab) > 0) {
				fprintf(stderr, "ERROR: book abbreviation (canon.h or locale) misordered at entry: %s\n", abbrevs[abbrevsCnt].ab);
				exit(-1);
			}
		}
			*/
		for (int i = 0; i <= *BMAX; i++) {
			int bn = getBookAbbrev((*books)[i].name);
			if (bn != i) {
				SWLog::getSystemLog()->logError("Book: %s does not have a matching toupper abbrevs entry! book number returned was: %d", 
					(*books)[i].name, bn);
			}
		}
    }
	else abbrevsCnt = size;
}


/******************************************************************************
 * VerseKey2::initstatics - initializes statics.  Performed only when first
 *						instance on VerseKey2 (or descendent) is created.
 */

void VerseKey2::initstatics() {
	int l1, l2, chaptmp = 0;

	//builtin_books[0] = otbooks;
	//builtin_books[1] = ntbooks;
	builtin_books = osisbooks;
	
	localeCache = new LocaleCache();
/*
	for (l2 = 0; l2 <= builtin_BMAX; l2++) {
			builtin_books[l2].versemax = &(offsets[1][chaptmp]);
			chaptmp += getMaxChaptersInBook(l2);
		}
*/
	/*
	for (l1 = 0; l1 < 2; l1++) {
		for (l2 = 0; l2 < builtin_BMAX[l1]; l2++) {
			builtin_books[l1][l2].versemax = &vm[chaptmp];
			chaptmp += builtin_books[l1][l2].chapmax;
		}
	}
	*/
}


/******************************************************************************
 * VerseKey2::parse - parses keytext into testament|book|chapter|verse
 *
 * RET:	error status
 */

char VerseKey2::parse()
{

	
	testament = 2;
	book      = *BMAX;
	chapter   = 1;
	verse     = 1;
	int booklen   = 0;

	int error = 0;

	if (keytext) {
		ListKey tmpListKey = VerseKey2::ParseVerseList(keytext);
		if (tmpListKey.Count()) {
			SWKey::setText((const char *)tmpListKey);
			for (int j = 0; j <= *BMAX; j++) {
				int matchlen = strlen((*books)[j].name);
				if (!strncmp(keytext, (*books)[j].name, matchlen)) {
					if (matchlen > booklen) {
						booklen = matchlen;
						//!!!WDG This is a temporary hack
						if (j < OTBOOKS)
							testament = 1;
						book = j;
					}
				}
			}
			//}

			if (booklen) {
				sscanf(&keytext[booklen], "%d:%d", &chapter, &verse);
			}
			else	error = KEYERR_FAILEDPARSE;
		} else error = KEYERR_FAILEDPARSE;
	}
	Normalize(1);
	freshtext();

	return (this->error) ? this->error : (this->error = error);
}


/******************************************************************************
 * VerseKey2::freshtext - refreshes keytext based on
 *				testament|book|chapter|verse
 */

void VerseKey2::freshtext() const
{
	SWBuf buf = "";
	int realbook = book;

	if (book < 1) {
		if (testament < 1)
			buf = "[ Module Heading ]";
		else buf.appendFormatted("[ Testament %d Heading ]", (int)testament);
	}
	else {
		if (realbook > *BMAX) {
				realbook = *BMAX;
		}
		buf.appendFormatted("%s %d:%d", (*books)[realbook].name, chapter, verse);
	}

	stdstr((char **)&keytext, buf.c_str());
}



/******************************************************************************
 * VerseKey2::getBookAbbrev - Attempts to find a book from an abbreviation for a buffer
 *
 * ENT:	abbr - key for which to search;
 * RET:	book number or < 0 = not valid
 */

int VerseKey2::getBookAbbrev(const char *iabbr)
{
	int loop, diff, abLen, min, max, target, retVal = -1;

	char *abbr = 0;

	//for (int i = 0; i < 2; i++) {
		stdstr(&abbr, iabbr);
		strstrip(abbr);
		//if (!i)
			toupperstr(abbr);
		abLen = strlen(abbr);

		if (abLen) {
			min = 0;
//			max = abbrevsCnt - 1;
			max 	= abbrevsCnt;
			while(1) {
				target = min + ((max - min) / 2);
				diff = strncmp(abbr, abbrevs[target].ab, abLen);
				if ((!diff)||(target >= max)||(target <= min))
					break;
				if (diff > 0)
					min = target;
				else	max = target;
			}
			for (; target > 0; target--) {
				if (strncmp(abbr, abbrevs[target-1].ab, abLen))
					break;
			}
				
			retVal = (!diff) ? abbrevs[target].book : -1;
		}
		//if (retVal > 0)
			//break;
	//}
	delete [] abbr;
	return retVal;
}

/******************************************************************************
 * VerseKey2::ParseVerseList - Attempts to parse a buffer into separate
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

ListKey VerseKey2::ParseVerseList(const char *buf, const char *defaultKey, bool expandRange) {
	SWKey textkey;

	char book[2048];
	char number[2048];
	int tobook = 0;
	int tonumber = 0;
	int chap = -1, verse = -1;
	int bookno = 0;
	VerseKey2 curkey, lBound;
	curkey.setLocale(getLocale());
	lBound.setLocale(getLocale());
	int loop;
	char comma = 0;
	char dash = 0;
	const char *orig = buf;
	int q;
	ListKey tmpListKey;
	ListKey internalListKey;
	SWKey tmpDefaultKey = defaultKey;
	char lastPartial = 0;
	bool inTerm = true;
	int notAllDigits;

	curkey.AutoNormalize(0);
	tmpListKey << tmpDefaultKey;
	tmpListKey.GetElement()->userData = (void *)buf;
	
	while (*buf) {
		switch (*buf) {
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
		case ':':
			if (buf[1] != ' ') {		// for silly Mat 1:1: this verse....
				number[tonumber] = 0;
				tonumber = 0;
				if (*number)
					chap = atoi(number);
				*number = 0;
				break;
			}
			// otherwise drop down to next case

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
						chap = VerseKey2(tmpListKey).Chapter();
						*book = 0;
					}
				}
				if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
					strcpy(book, VerseKey2(tmpListKey).getBookName());
				}
				bookno = getBookAbbrev(book);
			}
			if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
				char partial = 0;
				curkey.Verse(1);
				curkey.Chapter(1);
				curkey.Book(1);

				if (bookno < 0) {
					curkey.Testament(VerseKey2(tmpListKey).Testament());
					curkey.Book(VerseKey2(tmpListKey).Book());
				}
				else {
					curkey.Testament(1);
					curkey.Book(bookno);
				}

				if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
//				if (comma) {
					curkey.Chapter(VerseKey2(tmpListKey).Chapter());
					curkey.Verse(chap);  // chap because this is the first number captured
				}
				else {
					if (chap >= 0) {
						curkey.Chapter(chap);
					}
					else {
						partial++;
						curkey.Chapter(1);
					}
					if (verse >= 0) {
						curkey.Verse(verse);
					}
					else {
						partial++;
						curkey.Verse(1);
					}
				}

				if ((*buf == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
					VerseKey2 newElement;
					newElement.LowerBound(curkey);
					newElement.setPosition(TOP);
					tmpListKey << newElement;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
				else {
					if (!dash) { 	// if last separator was not a dash just add
						if (expandRange && partial) {
							VerseKey2 newElement;
							newElement.LowerBound(curkey);
							if (partial > 1)
								curkey.setPosition(MAXCHAPTER);
							if (partial > 0)
								curkey = MAXVERSE;
							newElement.UpperBound(curkey);
							newElement = TOP;
							tmpListKey << newElement;
							tmpListKey.GetElement()->userData = (void *)buf;
						}
						else {
							tmpListKey << (const SWKey &)(const SWKey)(const char *)curkey;
							tmpListKey.GetElement()->userData = (void *)buf;
						}
					}
					else	if (expandRange) {
						VerseKey2 *newElement = SWDYNAMIC_CAST(VerseKey2, tmpListKey.GetElement());
						if (newElement) {
							if (partial > 1)
								curkey = MAXCHAPTER;
							if (partial > 0)
								curkey = MAXVERSE;
							newElement->UpperBound(curkey);
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
				case 'f':
				case 'F':
					break;
				default:
					number[tonumber] = 0;
					tonumber = 0;
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
				chap = VerseKey2(tmpListKey).Chapter();
				*book = 0;
			}
		}
			
		if ((!stricmp(book, "ch")) || (!stricmp(book, "chap"))) {	// Verse abbrev
			strcpy(book, VerseKey2(tmpListKey).getBookName());
		}
		bookno = getBookAbbrev(book);
	}
	if (((bookno > -1) || (!*book)) && ((*book) || (chap >= 0) || (verse >= 0))) {
		char partial = 0;
		curkey.Verse(1);
		curkey.Chapter(1);
		curkey.Book(1);

		if (bookno < 0) {
			curkey.Testament(VerseKey2(tmpListKey).Testament());
			curkey.Book(VerseKey2(tmpListKey).Book());
		}
		else {
			curkey.Testament(1);
			curkey.Book(bookno);
		}

		if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
//		if (comma) {
			curkey.Chapter(VerseKey2(tmpListKey).Chapter());
			curkey.Verse(chap);  // chap because this is the first number captured
		}
		else {
			if (chap >= 0) {
				curkey.Chapter(chap);
			}
			else {
				partial++;
				curkey.Chapter(1);
			}
			if (verse >= 0) {
				curkey.Verse(verse);
			}
			else {
				partial++;
				curkey.Verse(1);
			}
		}

		if ((*buf == '-') && (expandRange)) {	// if this is a dash save lowerBound and wait for upper
			VerseKey2 newElement;
			newElement.LowerBound(curkey);
			newElement = TOP;
			tmpListKey << newElement;
			tmpListKey.GetElement()->userData = (void *)buf;
		}
		else {
			if (!dash) { 	// if last separator was not a dash just add
				if (expandRange && partial) {
					VerseKey2 newElement;
					newElement.LowerBound(curkey);
					if (partial > 1)
						curkey = MAXCHAPTER;
					if (partial > 0)
						curkey = MAXVERSE;
					newElement.UpperBound(curkey);
					newElement = TOP;
					tmpListKey << newElement;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
				else {
					tmpListKey << (const SWKey &)(const SWKey)(const char *)curkey;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
			}
			else if (expandRange) {
				VerseKey2 *newElement = SWDYNAMIC_CAST(VerseKey2, tmpListKey.GetElement());
				if (newElement) {
					if (partial > 1)
						curkey = MAXCHAPTER;
					if (partial > 0)
						curkey = MAXVERSE;
					newElement->UpperBound(curkey);
					*newElement = TOP;
					tmpListKey.GetElement()->userData = (void *)buf;
				}
			}
		}
	}
	*book = 0;
	tmpListKey = TOP;
	tmpListKey.Remove();	// remove defaultKey
	internalListKey = tmpListKey;
	internalListKey = TOP;	// Align internalListKey to first element before passing back;

	return internalListKey;
}


/******************************************************************************
 * VerseKey2::LowerBound	- sets / gets the lower boundary for this key
 */
/*
VerseKey2 &VerseKey2::LowerBound(const char *lb)
{
	if (!lowerBound) 
		initBounds();

	(*lowerBound) = lb;
	lowerBound->Normalize();
	lowerBound->setLocale( this->getLocale() );
	boundSet = true;
	return (*lowerBound);
}
*/
VerseKey2 &VerseKey2::LowerBound(const VerseKey2 & ikey)
{
	if (!lowerBound) 
		initBounds();

	lowerBound->Testament(ikey.Testament());
	lowerBound->Book(ikey.Book());
	lowerBound->Chapter(ikey.Chapter());
	lowerBound->Verse(ikey.Verse());
	#ifdef WDGDEBUG
	printf("setting lower bound to %d:%d:%d\n",lowerBound->Book(), lowerBound->Chapter(), 
		lowerBound->Verse());fflush(NULL);
	#endif
	//lowerBound->Normalize();
	lowerBound->setLocale( this->getLocale() );
	boundSet = true;
	return (*lowerBound);
}


/******************************************************************************
 * VerseKey2::UpperBound	- sets / gets the upper boundary for this key
 */
/*
VerseKey2 &VerseKey2::UpperBound(const char *ub)
{
	if (!upperBound) 
		initBounds();

// need to set upperbound parsing to resolve to max verse/chap if not specified
	   (*upperBound) = ub;
	if (*upperBound < *lowerBound)
		*upperBound = *lowerBound;
	upperBound->Normalize();
	upperBound->setLocale( this->getLocale() );

// until we have a proper method to resolve max verse/chap use this kludge
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
	

// -- end kludge
	boundSet = true;
	return (*upperBound);
}
*/
VerseKey2 &VerseKey2::UpperBound(const VerseKey2 & ikey)
{
	if (!upperBound) 
		initBounds();

	upperBound->Testament(ikey.Testament());
	upperBound->Book(ikey.Book());
	upperBound->Chapter(ikey.Chapter());
	upperBound->Verse(ikey.Verse());
	//upperBound->Normalize();
	upperBound->setLocale( this->getLocale() );
	boundSet = true;
	return (*upperBound);
}


/******************************************************************************
 * VerseKey2::LowerBound	- sets / gets the lower boundary for this key
 */

VerseKey2 &VerseKey2::LowerBound() const
{
	if (!lowerBound) 
		initBounds();

	return (*lowerBound);
}


/******************************************************************************
 * VerseKey2::UpperBound	- sets / gets the upper boundary for this key
 */

VerseKey2 &VerseKey2::UpperBound() const
{
	if (!upperBound) 
		initBounds();

	return (*upperBound);
}


/******************************************************************************
 * VerseKey2::ClearBounds	- clears bounds for this VerseKey2
 */

void VerseKey2::ClearBounds()
{
	initBounds();
}


void VerseKey2::initBounds() const
{
	if (!upperBound) {
		upperBound = new VerseKey2();
		upperBound->AutoNormalize(0);
		upperBound->Headings(1);
	}
	if (!lowerBound) {
		lowerBound = new VerseKey2();
		lowerBound->AutoNormalize(0);
		lowerBound->Headings(1);
	}

	lowerBound->Testament(0);
	lowerBound->Book(0);
	lowerBound->Chapter(0);
	lowerBound->Verse(0);

	upperBound->Testament(2);
	upperBound->Book(*BMAX);
	upperBound->Chapter(getMaxChaptersInBook(upperBound->Book()));
	upperBound->Verse(getMaxVerseInChapter(upperBound->Book(), upperBound->Chapter()));
	boundSet = false;
}


/******************************************************************************
 * VerseKey2::copyFrom - Equates this VerseKey2 to another VerseKey2
 */

void VerseKey2::copyFrom(const VerseKey2 &ikey) {
	//SWKey::copyFrom(ikey);

	//parse();
	testament = ikey.Testament();
	book = ikey.Book();
	chapter = ikey.Chapter();
	verse = ikey.Verse();
	LowerBound(ikey.LowerBound());
	UpperBound(ikey.UpperBound());
	//Normalize();
}


/******************************************************************************
 * VerseKey2::copyFrom - Equates this VerseKey2 to another SWKey
 */

void VerseKey2::copyFrom(const SWKey &ikey) {
	SWKey::copyFrom(ikey);

	parse();
}


/******************************************************************************
 * VerseKey2::getText - refreshes keytext before returning if cast to
 *				a (char *) is requested
 */

const char *VerseKey2::getText() const {
	freshtext();
	return keytext;
}


const char *VerseKey2::getShortText() const {
	static SWBuf buf = "";
	freshtext();
	/*
	if (book < 1) {
		if (testament < 1)
			sprintf(buf, "[ Module Heading ]");
		else sprintf(buf, "[ Testament %d Heading ]", (int)testament);
	}
	else {
		*/
	if (book < 0)
		buf = "[ Module Heading ]";
	else if (getMaxChaptersInBook(book) == TESTAMENT_HEADING) // !!!WDG needs testament support
		buf.appendFormatted("[ Testament %d Heading ]", (int)testament);
	else
		buf.appendFormatted("%s %d:%d", (*books)[book].prefAbbrev, chapter, verse);
	return buf.c_str();
}


const char *VerseKey2::getBookName() const {
	return (*books)[book].name;
}


const char *VerseKey2::getBookAbbrev() const {
	return (*books)[book].prefAbbrev;
}
/******************************************************************************
 * VerseKey2::setPosition(SW_POSITION)	- Positions this key
 *
 * ENT:	p	- position
 *
 * RET:	*this
 */

void VerseKey2::setPosition(SW_POSITION p) {
	switch (p) {
	case POS_TOP:
		testament = LowerBound().Testament();
		book      = LowerBound().Book();
		chapter   = LowerBound().Chapter();
		verse     = LowerBound().Verse();
		break;
	case POS_BOTTOM:
		testament = UpperBound().Testament();
		book      = UpperBound().Book();
		chapter   = UpperBound().Chapter();
		verse     = UpperBound().Verse();
		break;
	case POS_MAXVERSE:
		Normalize();
		verse     = getMaxVerseInChapter(book, chapter);
		break;
	case POS_MAXCHAPTER:
		verse     = 1;
		Normalize();
		chapter   = getMaxChaptersInBook(book);
		break;
	} 
	Normalize(1);
	Error();	// clear error from normalize
}


/******************************************************************************
 * VerseKey2::increment	- Increments key a number of verses
 *
 * ENT:	step	- Number of verses to jump forward
 *
 * RET: *this
 */

void VerseKey2::increment(int step) {
	char ierror = 0;
	printf("incrementing by %d\n", step);
	Index(Index() + step);
	while ((!verse) && (!headings) && (!ierror)) {
		Index(Index() + 1);
		ierror = Error();
	}

	error = (ierror) ? ierror : error;
}


/******************************************************************************
 * VerseKey2::decrement	- Decrements key a number of verses
 *
 * ENT:	step	- Number of verses to jump backward
 *
 * RET: *this
 */

void VerseKey2::decrement(int step) {
	char ierror = 0;

	printf("decrementing by %d\n", step);
	Index(Index() - step);
	while ((!verse) && (!headings) && (!ierror)) {
		Index(Index() - 1);
		ierror = Error();
	}
	if ((ierror) && (!headings))
	{
		Normalize();
	}

	error = (ierror) ? ierror : error;
}


/******************************************************************************
 * VerseKey2::Normalize	- checks limits and normalizes if necessary (e.g.
 *				Matthew 29:47 = Mark 2:2).  If last verse is
 *				exceeded, key is set to last Book CH:VS
 * RET: *this
 */

void VerseKey2::Normalize(char autocheck)
{
	error = 0;
	bool valid = false;

	if ((autocheck) && (!autonorm))	// only normalize if we were explicitely called or if autonorm is turned on
		return;

	if ((headings) && (!verse))		// this is cheeze and temporary until deciding what actions should be taken.
		return;					// so headings should only be turned on when positioning with Index() or incrementors

	#ifdef WDGDEBUG
	printf("normalizing %d:%d:%d\n", book, chapter, verse);fflush(NULL);
	#endif
	while (!valid) {
		
		if (book <= 0 || book > *BMAX) break;
		
		while (chapter > getMaxChaptersInBook(book) && book <= *BMAX) {
			chapter -= getMaxChaptersInBook(book);
			book++;
		}

		while (chapter < !headings && (--book > 0)) {
			chapter += getMaxChaptersInBook(book);
		}

		while (verse > getMaxVerseInChapter(book, chapter)) {
			verse -= getMaxVerseInChapter(book, chapter);
			chapter++;
		}

		while (verse < !headings && (--chapter > 0)) {
			verse += getMaxVerseInChapter(book, chapter);
		}
		
		if (book > 0 && book <= *BMAX && 
			chapter >= !headings && chapter <= getMaxChaptersInBook(book) && 
			verse >= !headings && verse <= getMaxVerseInChapter(book, chapter))
				valid = true;
		
	}
	
	if (book >= 0 && book <= OTBOOKS)
		testament = 1;
	else if (book >= OTBOOKS+1 && book <= *BMAX)
		testament = 2;
	
	if (book > *BMAX) {
		testament = 2;
		book      = *BMAX;
		chapter   = getMaxChaptersInBook(book);
		verse     = getMaxVerseInChapter(book, chapter);
		error     = KEYERR_OUTOFBOUNDS;
	}

	if (book <= 0) {
		error     = ((!headings) || (book < 0)) ? KEYERR_OUTOFBOUNDS : 0;
		testament = ((headings) ? 0 : 1);
		book      = ((headings) ? 0 : 1);
		chapter   = ((headings) ? 0 : 1);
		verse     = ((headings) ? 0 : 1);
	}
	if (_compare(UpperBound()) > 0) {
		*this = UpperBound();
		#ifdef WDGDEBUG
		printf("error beyond upper bound in normalize\n");fflush(NULL);
		#endif
		error = KEYERR_OUTOFBOUNDS;
	}
	if (_compare(LowerBound()) < 0) {
		*this = LowerBound();
		#ifdef WDGDEBUG
		printf("error beyond lower bound in normalize\n");fflush(NULL);
		#endif
		error = KEYERR_OUTOFBOUNDS;
	}
	#ifdef WDGDEBUG
	printf("normalized %d:%d:%d\n", book, chapter, verse);fflush(NULL);
	#endif
}

//!!!WDG once it is working and becomes core we need to change these to get/set
/******************************************************************************
 * VerseKey2::Testament - Gets testament
 *
 * RET:	value of testament
 */

char VerseKey2::Testament() const
{
	return testament;
}


/******************************************************************************
 * VerseKey2::Book - Gets book
 *
 * RET:	value of book
 */

char VerseKey2::Book() const
{
	return book;
}


/******************************************************************************
 * VerseKey2::Chapter - Gets chapter
 *
 * RET:	value of chapter
 */

int VerseKey2::Chapter() const
{
	return chapter;
}


/******************************************************************************
 * VerseKey2::Verse - Gets verse
 *
 * RET:	value of verse
 */

int VerseKey2::Verse() const
{
	return verse;
}


/******************************************************************************
 * VerseKey2::Testament - Sets/gets testament
 *
 * ENT:	itestament - value which to set testament
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of testament
 *	if   changed -> previous value of testament
 */

char VerseKey2::Testament(char itestament)
{
	char retval = testament;

	if (itestament != MAXPOS(char)) {
		testament = itestament;
		Normalize(1);
	}
	return retval;
}


/******************************************************************************
 * VerseKey2::Book - Sets/gets book
 *
 * ENT:	ibook - value which to set book
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of book
 *	if   changed -> previous value of book
 */

char VerseKey2::Book(char ibook)
{
	char retval = book;

	Chapter(1);
	book = ibook;
	Normalize(1);

	return retval;
}


/******************************************************************************
 * VerseKey2::Chapter - Sets/gets chapter
 *
 * ENT:	ichapter - value which to set chapter
 *		[MAXPOS(int)] - only get
 *
 * RET:	if unchanged ->          value of chapter
 *	if   changed -> previous value of chapter
 */

int VerseKey2::Chapter(int ichapter)
{
	int retval = chapter;

	Verse(1);
	chapter = ichapter;
	Normalize(1);

	return retval;
}


/******************************************************************************
 * VerseKey2::Verse - Sets/gets verse
 *
 * ENT:	iverse - value which to set verse
 *		[MAXPOS(int)] - only get
 *
 * RET:	if unchanged ->          value of verse
 *	if   changed -> previous value of verse
 */

int VerseKey2::Verse(int iverse)
{
	int retval = verse;

	verse = iverse;
	Normalize(1);

	return retval;
}


/******************************************************************************
 * VerseKey2::AutoNormalize - Sets/gets flag that tells VerseKey2 to auto-
 *				matically normalize itself when modified
 *
 * ENT:	iautonorm - value which to set autonorm
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of autonorm
 *		if   changed -> previous value of autonorm
 */

char VerseKey2::AutoNormalize(char iautonorm)
{
	char retval = autonorm;

	if (iautonorm != MAXPOS(char)) {
		autonorm = iautonorm;
		Normalize(1);
	}
	return retval;
}


/******************************************************************************
 * VerseKey2::Headings - Sets/gets flag that tells VerseKey2 to include
 *					chap/book/testmnt/module headings
 *
 * ENT:	iheadings - value which to set headings
 *		[MAXPOS(char)] - only get
 *
 * RET:	if unchanged ->          value of headings
 *		if   changed -> previous value of headings
 */

char VerseKey2::Headings(char iheadings)
{
	char retval = headings;

	if (iheadings != MAXPOS(char)) {
		headings = iheadings;
		Normalize(1);
	}
	return retval;
}


/******************************************************************************
 * VerseKey2::findindex - binary search to find the index closest, but less
 *						than the given value.
 *
 * ENT:	array	- long * to array to search
 *		size		- number of elements in the array
 *		value	- value to find
 *
 * RET:	the index into the array that is less than but closest to value
 */

int VerseKey2::findindex(bkref *array, int size, long value)
{
	int lbound, ubound, tval;

	lbound = 0;
	ubound = size - 1;
	while ((ubound - lbound) > 1) {
		tval = lbound + (ubound-lbound)/2;
		if (array[tval].offset <= value)
			lbound = tval;
		else ubound = tval;
	}
	return (array[ubound].offset <= value) ? ubound : lbound;
}


/******************************************************************************
 * VerseKey2::Index - Gets index based upon current verse
 *
 * RET:	offset
 */

long VerseKey2::Index() const
{
	long  loffset;

	//printf("getting index");fflush(NULL);
	if (!testament) { // if we want module heading
		loffset = 0;
		verse  = 0;
	}
	else {
		if (!book)
			chapter = 0;
		if (!chapter)
			verse   = 0;

		loffset = offsets[0][book].offset;
		loffset = offsets[1][(int)loffset + chapter].offset;
		if (offsets[0][book].maxnext == TESTAMENT_HEADING) // if we want testament heading
		{
			chapter = 0;
			verse = 0;
		}
	}
	if (oldindexhack && loffset >= NTOFFSET)
	{
		//Testament(2);
		loffset -= NTOFFSET;
	}
	#ifdef WDGDEBUG
	printf("returning index %d\n", loffset+verse);fflush(NULL);
	#endif
	return (loffset + verse);
}


/******************************************************************************
 * VerseKey2::Index - Gets index based upon current verse
 *
 * RET:	offset
 */

long VerseKey2::NewIndex() const
{
	//static long otMaxIndex = 32300 - 8245;  // total positions - new testament positions
//	static long otMaxIndex = offsets[0][1][(int)offsets[0][0][BMAX[0]] + books[0][BMAX[0]].chapmax];
	return oldindexhack ? ((testament-1) * NTOFFSET) + Index() : Index();
}


/******************************************************************************
 * VerseKey2::Index - Sets index based upon current verse
 *
 * ENT:	iindex - value to set index to
 *
 * RET:	offset
 */

long VerseKey2::Index(long iindex)
{
	long  offset;

// This is the dirty stuff --------------------------------------------

	//if (!testament)
	//	testament = 1;
/*
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
			iindex = (offsets[testament-1][1][offsize[testament-1][1]-1] + 
				books[testament-1][BMAX[testament-1]-1].versemax[books[testament-1][BMAX[testament-1]-1].chapmax-1]) + 
				iindex; // What a doozy! ((offset of last chapter + number of verses in the last chapter) + iindex)
		}
	}
*/
if (oldindexhack)
{
	if (iindex < 1) {				// if (-) or module heading
		if (testament < 2) {
			if (iindex < 0) {
				testament = 0;  // previously we changed 0 -> 1
				error     = KEYERR_OUTOFBOUNDS;
				verse = iindex;
			}
			else testament = 0;		// we want module heading
		}
		else {
			testament--;
			iindex += NTOFFSET;
		}
	}
	if (testament==2)
		iindex += NTOFFSET;
	if (iindex >= NTOFFSET) 
		testament = 2;
}
else
{
	if (iindex < 1) {				// if (-) or module heading
		if (iindex < 0) {
			testament = 0;  // previously we changed 0 -> 1
			error     = KEYERR_OUTOFBOUNDS;
		}
		else testament = 0;		// we want module heading
	}
}	
	#ifdef WDGDEBUG
	printf("setting index %d(%d)\n", iindex, testament);fflush(NULL);
	#endif

	// --------------------------------------------------------------------

/*
	if (testament) {
		if ((!error) && (iindex)) {
			offset  = findindex(offsets[testament-1][1], offsize[testament-1][1], iindex);
			verse   = iindex - offsets[testament-1][1][offset];
			book    = findindex(offsets[testament-1][0], offsize[testament-1][0], offset);
			chapter = offset - offsets[testament-1][0][VerseKey2::book];
			verse   = (chapter) ? verse : 0;  
				// funny check. if we are index=1 (testmt header) all gets set to 0 exept verse.  
				//Don't know why.  Fix if you figure out.  Think its in the offsets table.
			if (verse) {		// only check if -1 won't give negative
				if (verse > books[testament-1][book-1].versemax[chapter-1]) {
					if (testament > 1) {
						verse = books[testament-1][book-1].versemax[chapter-1];
						error = KEYERR_OUTOFBOUNDS;
					}
					else {
						testament++;
						Index(verse - books[testament-2][book-1].versemax[chapter-1]);
					}
				}
			}
		}
	}
*/
	if (testament) {
		if ((!error) && (iindex)) {
			offset  = findindex(offsets[1], offsize[1], iindex);
			verse   = iindex - offsets[1][offset].offset;
			book    = findindex(offsets[0], offsize[0], offset);
			chapter = offset - offsets[0][book].offset;
			#ifdef WDGDEBUG
			printf("offset %d bcv %d:%d:%d\n", offset, book, chapter, verse);fflush(NULL);
			#endif
			//verse   = (chapter) ? verse : 0;  
				// funny check. if we are index=1 (testmt header) all gets set to 0 exept verse.  
				//Don't know why.  Fix if you figure out.  Think its in the offsets table. !!!WDG fix
		}
	}

	if (_compare(UpperBound()) > 0) {
		#ifdef WDGDEBUG
		printf("beyond upper bound in Index\n");fflush(NULL);
		#endif
		*this = UpperBound();
		error = KEYERR_OUTOFBOUNDS;
	}
	if (_compare(LowerBound()) < 0) {
		#ifdef WDGDEBUG
		printf("beyond lower bound in Index\n");fflush(NULL);
		#endif
		*this = LowerBound();
		#ifdef WDGDEBUG
		printf("now set to %d:%d:%d\n", book, chapter, verse);fflush(NULL);
		#endif
		error = KEYERR_OUTOFBOUNDS;
	}
	return Index();
}


/******************************************************************************
 * VerseKey2::compare	- Compares another SWKey object
 *
 * ENT:	ikey - key to compare with this one
 *
 * RET:	>0 if this VerseKey2 is greater than compare VerseKey2
 *	<0 <
 *	 0 =
 */

int VerseKey2::compare(const SWKey &ikey)
{
	VerseKey2 ivkey = (const char *)ikey;
	return _compare(ivkey);
}


/******************************************************************************
 * VerseKey2::_compare	- Compares another VerseKey2 object
 *
 * ENT:	ikey - key to compare with this one
 *
 * RET:	>0 if this VerseKey2 is greater than compare VerseKey2
 *	<0 <
 *	 0 =
 */

int VerseKey2::_compare(const VerseKey2 &ivkey)
{
	long keyval1 = 0;
	long keyval2 = 0;

	//keyval1 += Testament() * 1000000000;
	//keyval2 += ivkey.Testament() * 1000000000;
	keyval1 += Book() * 1000000;
	keyval2 += ivkey.Book() * 1000000;
	keyval1 += Chapter() * 1000;
	keyval2 += ivkey.Chapter() * 1000;
	keyval1 += Verse();
	keyval2 += ivkey.Verse();
	#ifdef WDGDEBUG
	printf("comparing %d to %d\n", keyval1, keyval2);fflush(NULL);
	#endif
	keyval1 -= keyval2;
	keyval1 = (keyval1) ? ((keyval1 > 0) ? 1 : -1) /*keyval1/labs(keyval1)*/:0; // -1 | 0 | 1
	return keyval1;
}


const char *VerseKey2::getOSISRef() const {
	static SWBuf buf = "";
	freshtext();
	if (Verse())
		buf.appendFormatted("%s.%d.%d", osisbooks[book].prefAbbrev, chapter, verse);
	else if (Chapter())
		buf.appendFormatted("%s.%d", osisbooks[book].prefAbbrev, chapter);
	else if (Book())
		buf.appendFormatted("%s", osisbooks[book].prefAbbrev);
	else	buf = "";
	
	return buf.c_str();
}

const char VerseKey2::getMaxBooks() const {
	return *BMAX;
}

const char *VerseKey2::getNameOfBook(char book) const {
	return osisbooks[book].name;
}

const char *VerseKey2::getPrefAbbrev(char book) const {
	return osisbooks[book].prefAbbrev;
}

const int VerseKey2::getMaxChaptersInBook(char book) const {
	return (offsets[0][book].maxnext == TESTAMENT_HEADING ? 0 : offsets[0][book].maxnext);
}

const int VerseKey2::getMaxVerseInChapter(char book, int chapter) const {
	return offsets[1][(offsets[0][book].offset)+chapter].maxnext;
}



/******************************************************************************
 * VerseKey2::getRangeText - returns parsable range text for this key
 */

const char *VerseKey2::getRangeText() const {
	if (isBoundSet()) {
		SWBuf buf;
		buf.appendFormatted("%s-%s", (const char *)LowerBound(), (const char *)UpperBound());
		stdstr(&rangeText, buf.c_str());
	}
	else stdstr(&rangeText, getText());
	return rangeText;
}

SWORD_NAMESPACE_END
