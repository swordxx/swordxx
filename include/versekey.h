/****************************************************************************** 
 *  versekey.h - code for class 'versekey'- a standard Biblical verse key
 *
 * $Id: versekey.h,v 1.3 2000/03/13 09:36:02 scribe Exp $
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


#ifndef VERSEKEY_H
#define VERSEKEY_H

#include <swkey.h>
#include <swmacs.h>
#include <listkey.h>


#define POS_MAXVERSE ((char)3)
#define POS_MAXCHAPTER ((char)4)

#define MAXVERSE POSITION(POS_MAXVERSE)
#define MAXCHAPTER POSITION(POS_MAXCHAPTER)

struct sbook {
	const char *name;		// Name of book
	unsigned char chapmax;	// Maximum chapters in book
	int *versemax;		// Array[chapmax] of maximum verses in chapters
};

struct abbrev {
	const char *ab;
	int book;
};

class VerseKey : public SWKey {

	static long *offsets[2][2];
	static int offsize[2][2];
	static int instance;	// number of instantiated VerseKey objects or derivitives
	static struct sbook otbooks[];
	static struct sbook ntbooks[];
	static long otbks[];
	static long otcps[];
	static long ntbks[];
	static long ntcps[];
	static int vm[];
	static ListKey internalListKey;

	const struct abbrev *abbrevs;
	int abbrevsCnt;

	char testament;		// 0 - Old; 1 - New
	mutable char book;
	mutable int chapter;
	mutable int verse;
	char autonorm;		// flag for auto normalization
	char headings;		// flag for headings on/off

	int getBookAbbrev(char *abbr);
	void initBounds() const;
	void initstatics();	// initialize and allocate books array
	void init();		// initializes this VerseKey()
	void freshtext() const;	// refresh keytext based on testament|book|chapter|verse
	virtual char parse();	// Parse a character array into testament|book|chapter|verse
	int findindex(long *array, int size, long value);	// finds the index of a given value
	mutable VerseKey *lowerBound, *upperBound;
public:
	static const char builtin_BMAX[2];
	static struct sbook *builtin_books[2];
	static const struct abbrev builtin_abbrevs[];
	const char *BMAX;
	struct sbook **books;

	VerseKey(const char *ikey = 0);
	VerseKey(const SWKey *ikey);
	VerseKey(const char *min, const char *max);
 	VerseKey(VerseKey const &k);
	virtual ~VerseKey();

	VerseKey &LowerBound(const char *lb);
	VerseKey &UpperBound(const char *ub);
	VerseKey &LowerBound() const;
	VerseKey &UpperBound() const;
	void ClearBounds();
 	virtual SWKey *clone() const;
	virtual operator const char*() const;
	virtual SWKey &operator =(const char * ikey) {SWKey &retval = SWKey::operator =(ikey); parse(); return retval; }
	virtual SWKey &operator =(const SWKey &ikey);
	virtual SWKey &operator =(const VerseKey &ikey);
	virtual SWKey &operator =(POSITION);
	virtual SWKey &operator -=(int decrement);
	virtual SWKey &operator +=(int increment);
/*
	virtual VerseKey &operator ++(int) { return *this += 1; }
	virtual VerseKey &operator --(int) { return *this -= 1; }
*/
	virtual char Traversable() { return 1; }

	virtual char Testament() const;
	virtual char Book() const;
	virtual int  Chapter() const;
	virtual int  Verse() const;
	virtual char Testament(char itestament);
	virtual char Book(char ibook);
	virtual int  Chapter(int ichapter);
	virtual int  Verse(int iverse);
	virtual void Normalize(char autocheck = 0);
	virtual char AutoNormalize(char iautonorm = MAXPOS(char));
	virtual char Headings(char iheadings = MAXPOS(char));
	virtual long Index() const;
	virtual long Index(long iindex);

	virtual ListKey &ParseVerseList(char *buf, const char *defaultKey = "Genesis 1:1", char max = 0);
	virtual int compare(const SWKey &ikey);
	virtual int _compare(const VerseKey &ikey);
	virtual void setBookAbbrevs(const struct abbrev *bookAbbrevs);
	virtual void setBooks(const char *iBMAX, struct sbook **ibooks);
	virtual void setLocale(const char *name);
};


#endif
