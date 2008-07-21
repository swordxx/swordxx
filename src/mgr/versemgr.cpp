/******************************************************************************
 *  versemgr.cpp	- implementation of class VerseMgr used for managing
 *  					versification systems
 *
 * $Id: versemgr.cpp 2108 2007-10-13 20:35:02Z scribe $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#include <versemgr.h>
#include <vector>
#include <map>
#include <treekey.h>
#include <canon.h>	// KJV internal versification system
#include <algorithm>

using std::vector;
using std::map;

using std::lower_bound;

SWORD_NAMESPACE_START


class VerseMgr::System::Private {
public:
	/** Array[chapmax] of maximum verses in chapters */
	vector<Book> books;
	map<SWBuf, int> osisLookup;

	Private() {
	}
	Private(const VerseMgr::System::Private &other) {
		books = other.books;
		osisLookup = other.osisLookup;
	}
	VerseMgr::System::Private &operator =(const VerseMgr::System::Private &other) {
		books = other.books;
		osisLookup = other.osisLookup;
		return *this;
	}
};


class VerseMgr::Book::Private {
friend struct BookOffsetLess;
public:
	/** Array[chapmax] of maximum verses in chapters */
	vector<int> verseMax;
	vector<long> offsetPrecomputed;

	Private() {
		verseMax.clear();
	}
	Private(const VerseMgr::Book::Private &other) {
		verseMax.clear();
		verseMax = other.verseMax;
		offsetPrecomputed = other.offsetPrecomputed;
	}
	VerseMgr::Book::Private &operator =(const VerseMgr::Book::Private &other) {
		verseMax.clear();
		verseMax = other.verseMax;
		offsetPrecomputed = other.offsetPrecomputed;
		return *this;
	}
};

struct BookOffsetLess {
	bool operator() (const VerseMgr::Book &o1, const long &o2) const { return o1.p->offsetPrecomputed[0] < o2; }
};

void VerseMgr::Book::init() {
	p = new Private();
}

void VerseMgr::System::init() {
	p = new Private();
	BMAX[0] = 0;
	BMAX[1] = 0;
	ntStartOffset = 0;
}


VerseMgr::System::System(const System &other) {
	name = other.name;
	BMAX[0] = other.BMAX[0];
	BMAX[1] = other.BMAX[1];
	init();
	(*p) = *(other.p);
}

VerseMgr::System::System &VerseMgr::System::operator =(const System &other) {
	name = other.name;
	BMAX[0] = other.BMAX[0];
	BMAX[1] = other.BMAX[1];
	init();
	(*p) = *(other.p);
	return *this;
}


VerseMgr::System::~System() {
	delete p;
	p = 0;
}

const VerseMgr::Book *VerseMgr::System::getBook(int number) const {
	return &(p->books[number]);
}


int VerseMgr::System::getBookNumberByOSISName(const char *bookName) const {
	return p->osisLookup[bookName];
}


void VerseMgr::System::loadFromSBook(const sbook *ot, const sbook *nt, int *chMax) {
	int chap = 0;
	int book = 0;
	long offset = 0;	// module heading
	offset++;			// testament heading
	while (ot->chapmax) {
		p->books.push_back(Book(ot->name, ot->osis, ot->prefAbbrev, ot->chapmax));
		offset++;		// book heading
		Book &b = p->books[p->books.size()-1];
		p->osisLookup[b.getOSISName()] = p->books.size();
		for (int i = 0; i < ot->chapmax; i++) {
			b.p->verseMax.push_back(chMax[chap]);
			offset++;		// chapter heading
			b.p->offsetPrecomputed.push_back(offset);
			offset += chMax[chap++];
		}
		ot++;
		book++;
	}
	BMAX[0] = book;
	book = 0;
	ntStartOffset = offset;
	offset++;			// testament heading
	while (nt->chapmax) {
		p->books.push_back(Book(nt->name, nt->osis, nt->prefAbbrev, nt->chapmax));
		offset++;		// book heading
		Book &b = p->books[p->books.size()-1];
		p->osisLookup[b.getOSISName()] = p->books.size();
		for (int i = 0; i < nt->chapmax; i++) {
			b.p->verseMax.push_back(chMax[chap]);
			offset++;		// chapter heading
			b.p->offsetPrecomputed.push_back(offset);
			offset += chMax[chap++];
		}
		nt++;
		book++;
	}
	BMAX[1] = book;

	// TODO: build offset speed array
}


VerseMgr::Book::Book(const Book &other) {
	longName = other.longName;
	osisName = other.osisName;
	prefAbbrev = other.prefAbbrev;
	chapMax = other.chapMax;
	init();
	(*p) = *(other.p);
}

VerseMgr::Book::Book& VerseMgr::Book::operator =(const Book &other) {
	longName = other.longName;
	osisName = other.osisName;
	prefAbbrev = other.prefAbbrev;
	chapMax = other.chapMax;
	init();
	(*p) = *(other.p);
	return *this;
}


VerseMgr::Book::~Book() {
	delete p;
	p = 0;
}


int VerseMgr::Book::getVerseMax(int chapter) const {
	return p->verseMax[chapter-1];
}


long VerseMgr::System::getOffsetFromVerse(int book, int chapter, int verse) const {
	long  offset = -1;

	const Book *b = getBook(book);
	offset = b->p->offsetPrecomputed[chapter-1];

/* old code
 *
	offset = offsets[testament-1][0][book];
	offset = offsets[testament-1][1][(int)offset + chapter];
	if (!(offset|verse)) // if we have a testament but nothing else.
		offset = 1;

*/

	return (offset + verse);
}


char VerseMgr::System::getVerseFromOffset(long offset, int *book, int *chapter, int *verse) const {
	// binary search for book
	vector<Book>::iterator b = lower_bound(p->books.begin(), p->books.end(), offset, BookOffsetLess());
	b--;
	vector<long>::iterator c = lower_bound(b->p->offsetPrecomputed.begin(), b->p->offsetPrecomputed.end(), offset);
	c--;

	(*book)    = distance(p->books.begin(), b)+1;
	(*chapter) = distance(b->p->offsetPrecomputed.begin(), c)+1;
	(*verse)   = (offset - *c);

	return 0;
}


/***************************************************
 * VerseMgr
 */

class VerseMgr::Private {
public:
	Private() {
	}
	Private(const VerseMgr::Private &other) {
		systems = other.systems;
	}
	VerseMgr::Private &operator =(const VerseMgr::Private &other) {
		systems = other.systems;
		return *this;
	}
	map<SWBuf, System> systems;
};
// ---------------- statics -----------------
VerseMgr *VerseMgr::systemVerseMgr = 0;

class __staticsystemVerseMgr {
public:
	__staticsystemVerseMgr() { }
	~__staticsystemVerseMgr() { delete VerseMgr::systemVerseMgr; }
} _staticsystemVerseMgr;


void VerseMgr::init() {
	p = new Private();
}


VerseMgr::~VerseMgr() {
	delete p;
	p = 0;
}


VerseMgr *VerseMgr::getSystemVerseMgr() {
	if (!systemVerseMgr) {
		systemVerseMgr = new VerseMgr();
		systemVerseMgr->registerVersificationSystem("KJV", otbooks, ntbooks, vm);
	}

	return systemVerseMgr;
}


void VerseMgr::setSystemVerseMgr(VerseMgr *newVerseMgr) {
	if (systemVerseMgr)
		delete systemVerseMgr;
	systemVerseMgr = newVerseMgr;
}


const VerseMgr::System *VerseMgr::getVersificationSystem(const char *name) const {
	return &(p->systems[name]);
}

void VerseMgr::registerVersificationSystem(const char *name, const sbook *ot, const sbook *nt, int *chMax) {
	p->systems[name] = System(name);
	System &s = p->systems[name];
	s.loadFromSBook(ot, nt, chMax);
}


void VerseMgr::registerVersificationSystem(const char *name, const TreeKey *tk) {
}

// --------------- end statics --------------

/*

long         *VerseKey::offsets[2][2]  = {{VerseKey::otbks, VerseKey::otcps}, {VerseKey::ntbks, VerseKey::ntcps}};
const char    VerseKey::builtin_BMAX[2]        = {39, 27};
void SWLocale::getBooks(char **iBMAX, struct sbook ***ibooks) {
	if (!BMAX) {
		BMAX = new char [2];
		BMAX[0] = VerseKey::builtin_BMAX[0];
		BMAX[1] = VerseKey::builtin_BMAX[1];

		books = new struct sbook *[2];
		books[0] = new struct sbook[BMAX[0]];
		books[1] = new struct sbook[BMAX[1]];

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < BMAX[i]; j++) {
				books[i][j] = VerseKey::builtin_books[i][j];
				books[i][j].name = translate(VerseKey::builtin_books[i][j].name);
			}
		}
	}

	*iBMAX  = BMAX;
	*ibooks = books;
}

~() {
	if (BMAX) {
		for (int i = 0; i < 2; i++)
			delete [] books[i];
		delete [] BMAX;
		delete [] books;
	}
}


******************************************************************************
 * VerseKey::initstatics - initializes statics.  Performed only when first
 *						instance on VerseKey (or descendent) is created.
 */

/*
void VerseKey::initstatics() {
	int l1, l2, chaptmp = 0;

	builtin_books[0] = otbooks;
	builtin_books[1] = ntbooks;

	for (l1 = 0; l1 < 2; l1++) {
		for (l2 = 0; l2 < builtin_BMAX[l1]; l2++) {
			builtin_books[l1][l2].versemax = &vm[chaptmp];
			chaptmp += builtin_books[l1][l2].chapmax;
		}
	}
}


const int VerseKey::getOSISBookNum(const char *bookab) {
	int i;
	for (i=0; i < 39; i++)
	{
		if (!strncmp(bookab, osisotbooks[i], strlen(osisotbooks[i])))
		{
			//printf("VerseKey::getOSISBookNum %s is OT %d\n", bookab, i+1);
			return i+1;
		}
	}
	for (i=0; i < 27; i++)
	{
		if (!strncmp(bookab, osisntbooks[i], strlen(osisotbooks[i])))
		{
			//printf("VerseKey::getOSISBookNum %s is NT %d\n", bookab, i+1);
			return i+1;
		}
	}
	return -1;
}

*/


SWORD_NAMESPACE_END
