/******************************************************************************
 *	versekey2.h - code for class 'versekey'- a standard Biblical verse key
 *
 * $Id: versekey2.h,v 1.4 2004/04/12 13:49:12 dglassey Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ	85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * General Public License for more details.
 *
 */


#ifndef VERSEKEY2_H
#define VERSEKEY2_H

#include <swkey.h>
#include <swmacs.h>
#include <listkey.h>
#include <ctype.h>

#include <defs.h>

SWORD_NAMESPACE_START

#define POS_MAXVERSE ((char)3)
#define POS_MAXCHAPTER ((char)4)
#define POS_MAXBOOK ((char)5)

#define MAXVERSE SW_POSITION(POS_MAXVERSE)
#define MAXCHAPTER SW_POSITION(POS_MAXCHAPTER)
#define MAXBOOK SW_POSITION(POS_MAXBOOK)

#define KEYERR_FAILEDPARSE 2

struct bkref
{
	long offset;
	unsigned char maxnext;
};


struct sbook2
{
	/**Name of book
	*/
	const char *name;

	/**Preferred Abbreviation
	*/
	const char *prefAbbrev;

	/**Maximum chapters in book
	*/
	//unsigned char chapmax;
	/** Array[chapmax] of maximum verses in chapters
	*/
	//const bkref *versemax;
};

struct abbrev2
{
	const char *ab;
	int book;
};



class SWLocale;

/**
 * Class VerseKey2
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT VerseKey2:public SWKey {
	class LocaleCache {
	public:
		char *name;
		unsigned int abbrevsCnt;
		SWLocale *locale;
		LocaleCache() : name(0), abbrevsCnt(0), locale(0) {};
		virtual ~LocaleCache() {
		if (name)
			delete[]name;
		}
	};

	static SWClass classdef;

	//static long *offsets[2][2];
	static bkref *offsets[2];
	//static int offsize[2][2];
	static int offsize[2];
	/** number of instantiated VerseKey2 objects or derivitives 
	*/
	static int instance;
/*	static struct sbook otbooks[];
	static struct sbook ntbooks[];
	static long otbks[];
	static long otcps[];
	static long ntbks[];
	static long ntcps[];
	static int vm[];
*/
	static struct sbook2 osisbooks[];
	static bkref kjvbks[];
	static bkref kjvcps[];
	static LocaleCache *localeCache;
	ListKey internalListKey;

	const struct abbrev2 *abbrevs;
	char *locale;
	int abbrevsCnt;
	/** The Testament: 0 - Old; 1 - New
	*/
	signed char testament;
	mutable signed char book;
	mutable signed int chapter;
	mutable signed int verse;
	/** flag for auto normalization 
	*/
	char autonorm;
	/** flag for headings on/off
	*/
	char headings;
	/** flag to use old ot/nt indexes for old modules
	*/
	bool oldindexhack;

	int getBookAbbrev(const char *abbr);
	void initBounds() const;
	/** initialize and allocate books array
	*/
	void initstatics();
	/** initializes this VerseKey2()
	*/
	void init();
	/** Refresh keytext based on testament|book|chapter|verse
	* default auto normalization to true
	* default display headings option is false
	*/
	void freshtext() const;
	/**	Parse a character array into testament|book|chapter|verse 
	*
	*/
	virtual char parse();
	/** Binary search to find the index closest, but less
	* than the given value.
	*
	* @param array long * to array to search
	* @param size number of elements in the array
	* @param value value to find
	* @return the index into the array that is less than but closest to value
	*/
	int findindex(bkref *array, int size, long value);
	mutable VerseKey2 *lowerBound, *upperBound;

	static const char builtin_BMAX;
	//static struct sbook *builtin_books[2];
	static struct sbook2 *builtin_books;
	static const struct abbrev2 builtin_abbrevs[];
	const char *BMAX;
	struct sbook2 **books;
public:
	//static const char builtin_BMAX[2];

	/**
	* VerseKey2 Constructor - initializes Instance of VerseKey2
	*
	* @param ikey text key (will take various forms of 'BOOK CH:VS'.
	* See parse() for more detailed information)
	*/
	VerseKey2(const char *ikey = 0);
	
	/**
	* VerseKey2 Constructor - initializes instance of VerseKey2
	*
	* @param ikey base key (will take various forms of 'BOOK CH:VS'.
	*	See parse() for more detailed information)
	*/	
	VerseKey2(const SWKey * ikey);
	
	/** VerseKey2 Constructor - initializes instance of VerseKey2
	* with boundariess - see also LowerBound()
	* and UpperBound()
	* @param min the lower boundary of the new	VerseKey2
	* @param max the upper boundary of the new	VerseKey2
	*/	
	VerseKey2(const char *min, const char *max);
	
	/**	VerseKey2 Copy Constructor - will create a new	VerseKey2
	* based on an existing one
	*
	* @param k the	VerseKey2 to copy from
	*/
	VerseKey2(const VerseKey2 &k);
	
	/**	VerseKey2 Destructor
	* Cleans up an instance of VerseKey2
	*/
	virtual ~ VerseKey2();

	/** sets the lower boundary for this	VerseKey2
	* and returns the new boundary
	*
	* @param lb the new lower boundary for this	VerseKey2
	* @return the lower boundary the key was set to
	*/
	//VerseKey2 & LowerBound(const char *lb);
	VerseKey2 & LowerBound(const VerseKey2 & ikey);
	
	/** sets the upper boundary for this	VerseKey2
	* and returns the new boundary
	* @param ub the new upper boundary for this	VerseKey2
	* @return the upper boundary the key was set to
	*/
	//VerseKey2 & UpperBound(const char *ub);
	VerseKey2 & UpperBound(const VerseKey2 & ikey);
	
	/** gets the lower boundary of this	VerseKey2
	* @return the lower boundary of this	VerseKey2
	*/
	VerseKey2 & LowerBound() const;
	
	/** gets the upper boundary of this	VerseKey2
	* @return the upper boundary of this	VerseKey2
	*/
	VerseKey2 & UpperBound() const;
	
	/** clears the boundaries of this	VerseKey2
	*/
	void ClearBounds();
	
	/** Creates a new	SWKey based on the current	VerseKey2
	* see also the Copy Constructor
	*/
	virtual SWKey *clone() const;
	
	/** refreshes keytext before returning if cast to
	* a (char *) is requested
	*/
	virtual const char *getText() const;
	virtual const char *getShortText() const;
	virtual void setText(const char *ikey) { SWKey::setText(ikey); parse (); }
	virtual void copyFrom(const SWKey & ikey);
	
	/** Equates this VerseKey2 to another VerseKey2
	*/
	virtual void copyFrom(const VerseKey2 & ikey);
	
	/** Positions this key
	*
	* @param newpos Position to set to.
	* @return *this
	*/
	virtual void setPosition(SW_POSITION newpos);
	
	/** Decrements key a number of verses
	*
	* @param steps Number of verses to jump backward
	* @return *this
	*/
	virtual void decrement(int steps);
	
	/** Increments key a number of verses
	*
	* @param steps Number of verses to jump forward
	* @return *this
	*/
	virtual void increment(int steps);
	virtual char Traversable() { return 1; }

	virtual const char *getBookName() const;
	virtual const char *getBookAbbrev() const;
	/** Gets testament
	*
	* @return value of testament
	*/
	virtual char Testament() const;
	
	/** Gets book
	*
	* @return value of book
	*/
	virtual char Book() const;
	
	/** Gets chapter
	*
	* @return value of chapter
	*/
	virtual int Chapter() const;
	
	/** Gets verse
	*
	* @return value of verse
	*/
	virtual int Verse() const;
	
	/** Sets/gets testament
	*
	* @param itestament value which to set testament
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of testament,
	* if changed -> previous value of testament
	*/
	virtual char Testament(char itestament);
	
	/** Sets/gets book
	*
	* @param ibook value which to set book
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of book,
	* if changed -> previous value of book
	*/
	virtual char Book(char ibook);
	
	/** Sets/gets chapter
	*
	* @param ichapter value which to set chapter
	* [MAXPOS(int)] - only get
	* @return if unchanged -> value of chapter,
	* if changed -> previous value of chapter
	*/
	virtual int Chapter(int ichapter);
	
	/** Sets/gets verse
	*
	* @param iverse value which to set verse
	* [MAXPOS(int)] - only get
	* @return if unchanged -> value of verse,
	* if changed -> previous value of verse
	*/
	virtual int Verse(int iverse);
	
	/** checks limits and normalizes if necessary (e.g.
	* Matthew 29:47 = Mark 2:2).	If last verse is
	* exceeded, key is set to last Book CH:VS
	*
	* @return *this
	*/
	virtual void Normalize(char autocheck = 0);
	
	/** Sets/gets flag that tells VerseKey2 to
	* automatically normalize itself when modified
	*
	* @param iautonorm value which to set autonorm
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of autonorm,
	* if changed -> previous value of autonorm
	*/
	virtual char AutoNormalize(char iautonorm = MAXPOS (char));
	
	/** Sets/gets flag that tells VerseKey2 to include
	* chapter/book/testament/module headings
	*
	* @param iheadings value which to set headings
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of headings,
	* if changed -> previous value of headings
	*/
	virtual char Headings(char iheadings = MAXPOS (char));
	
	virtual long NewIndex() const;
	
	/** Gets index based upon current verse
	*
	* @return offset
	*/
	virtual long Index() const;
	
	/** Sets index based upon current verse
	*
	* @param iindex value to set index to
	* @return offset
	*/
	virtual long Index(long iindex);

	virtual const char *getOSISRef() const;
	
	virtual const char getMaxBooks() const;
	virtual const char *getNameOfBook(char book) const;
	virtual const char *getPrefAbbrev(char book) const;
	virtual const int getMaxChaptersInBook(char book) const;
	virtual const int getMaxVerseInChapter(char book, int chapter) const;

	virtual ListKey ParseVerseList(const char *buf, const char *defaultKey = "Genesis 1:1", bool expandRange = false);
	virtual const char *getRangeText() const;
	/** Compares another	SWKey object
	*
	* @param ikey key to compare with this one
	* @return >0 if this	VerseKey2 is greater than compare	SWKey,
	* <0 if this	VerseKey2 is smaller than compare	SWKey,
	* 0 if the keys are the same
	*/
	virtual int compare(const SWKey & ikey);
	
	/** Compares another	VerseKey2 object
	*
	* @param ikey key to compare with this one
	* @return >0 if this	VerseKey2 is greater than compare	VerseKey2,
	* <0 if this	VerseKey2 is smaller than compare	VerseKey2,
	* 0 if the keys are the same
	*/
	virtual int _compare(const VerseKey2 & ikey);
	
	virtual void setBookAbbrevs(const struct abbrev2 *bookAbbrevs, unsigned int size = 0 /* default determine size */ );
	virtual void setBooks(const char *iBMAX, struct sbook2 **ibooks);
	virtual void setLocale(const char *name);
	virtual const char *getLocale() const { return locale; }



	// OPERATORS --------------------------------------------------------------------


	SWKEY_OPERATORS

	virtual SWKey & operator = (const VerseKey2 & ikey) { copyFrom(ikey); return *this; }
};

#ifdef VK2
typedef VerseKey2 VerseKey;
typedef sbook2 sbook;
typedef abbrev2 abbrev;
#endif

SWORD_NAMESPACE_END
#endif
