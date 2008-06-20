/******************************************************************************
 *	versekey.h - code for class 'versekey'- a standard Biblical verse key
 *
 * $Id$
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


#ifndef VERSEKEY_H
#define VERSEKEY_H

#include <swkey.h>
#include <swmacs.h>
#include <listkey.h>

#include <defs.h>

SWORD_NAMESPACE_START

#define POS_MAXVERSE ((char)3)
#define POS_MAXCHAPTER ((char)4)
#define POS_MAXBOOK ((char)5)

#define MAXVERSE SW_POSITION(POS_MAXVERSE)
#define MAXCHAPTER SW_POSITION(POS_MAXCHAPTER)
#define MAXBOOK SW_POSITION(POS_MAXBOOK)


struct sbook
{
	/**Name of book
	*/
	const char *name;

	/**Preferred Abbreviation
	*/
	const char *prefAbbrev;

	/**Maximum chapters in book
	*/
	unsigned char chapmax;
	/** Array[chapmax] of maximum verses in chapters
	*/
	int *versemax;
};

struct abbrev
{
	const char *ab;
	int book;
};


class SWLocale;

/**
 * Class VerseKey
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT VerseKey : public SWKey {

	class LocaleCache {
	public:
		char *name;
		unsigned int abbrevsCnt;
		SWLocale *locale;
			LocaleCache() {
			name = 0;
			abbrevsCnt = 0;
			locale = 0;
		}
		 virtual ~LocaleCache() {
			if (name)
			delete[]name;
		}
	};

	static SWClass classdef;

	static long *offsets[2][2];
	static int offsize[2][2];
	/** number of instantiated VerseKey objects or derivitives 
	*/
	static int instance;
	static struct sbook otbooks[];
	static struct sbook ntbooks[];
	static const char *osisotbooks[];
	static const char *osisntbooks[];
	static const char **osisbooks[];
#if 1
	static long otbks[];
	static long otcps[];
	static long ntbks[];
	static long ntcps[];
#endif
	static int vm[];
	static LocaleCache localeCache;
	ListKey internalListKey;

	const struct abbrev *abbrevs;
	char *locale;
	int abbrevsCnt;

	/** The Testament: 0 - Old; 1 - New
	*/
	signed char testament;
	mutable signed char book;
	mutable signed int chapter;
	mutable signed int verse;
	mutable char suffix;

	/** flag for auto normalization 
	*/
	char autonorm;

	/** flag for headings on/off
	*/
	char headings;

	int getBookAbbrev(const char *abbr);
	void initBounds() const;

	/** initialize and allocate books array
	*/
	void initstatics();

	/** initializes this VerseKey()
	*/
	void init();

	/** Binary search to find the index closest, but less
	* than the given value.
	*
	* @param array long * to array to search
	* @param size number of elements in the array
	* @param value value to find
	* @return the index into the array that is less than but closest to value
	*/
	int findindex(long *array, int size, long value);

	mutable VerseKey *lowerBound, *upperBound;


protected:

	/** Refresh keytext based on testament|book|chapter|verse
	* default auto normalization to true
	* default display headings option is false
	*/
	void freshtext() const;
	/**	Parse a character array into testament|book|chapter|verse 
	*
	*/
	virtual char parse(bool checkNormalize = true);
public:
#if 0
	static long otbks[];
	static long otcps[];
	static long ntbks[];
	static long ntcps[];
#endif
	static const char builtin_BMAX[2];
	static struct sbook *builtin_books[2];
	static const struct abbrev builtin_abbrevs[];
	const char *BMAX;
	struct sbook **books;

	/**
	* VerseKey Constructor - initializes Instance of VerseKey
	*
	* @param ikey text key (will take various forms of 'BOOK CH:VS'.
	* See parse() for more detailed information)
	*/
	VerseKey(const char *ikey = 0);
	
	/**
	* VerseKey Constructor - initializes instance of VerseKey
	*
	* @param ikey base key (will take various forms of 'BOOK CH:VS'.
	*	See parse() for more detailed information)
	*/	
	VerseKey(const SWKey *ikey);
	
	/** VerseKey Constructor - initializes instance of VerseKey
	* with boundariess - see also LowerBound()
	* and UpperBound()
	* @param min the lower boundary of the new	VerseKey
	* @param max the upper boundary of the new	VerseKey
	*/	
	VerseKey(const char *min, const char *max);
	
	/**	VerseKey Copy Constructor - will create a new VerseKey
	* based on an existing SWKey
	*
	* @param k the	VerseKey to copy from
	*/
	VerseKey(const SWKey &k);

	/**	VerseKey Copy Constructor - will create a new VerseKey
	* based on an existing one
	*
	* @param k the	VerseKey to copy from
	*/
	VerseKey(const VerseKey &k);
	
	/**	VerseKey Destructor
	* Cleans up an instance of VerseKey
	*/
	virtual ~VerseKey();

	/** sets the lower boundary for this	VerseKey
	* and returns the new boundary
	*
	* @param lb the new lower boundary for this	VerseKey
	* @return the lower boundary the key was set to
	*/
	VerseKey &LowerBound(const VerseKey &ub);
	
	/** sets the upper boundary for this	VerseKey
	* and returns the new boundary
	* @param ub the new upper boundary for this	VerseKey
	* @return the upper boundary the key was set to
	*/
	VerseKey &UpperBound(const VerseKey &ub);
	
	/** gets the lower boundary of this	VerseKey
	* @return the lower boundary of this	VerseKey
	*/
	VerseKey &LowerBound() const;
	
	/** gets the upper boundary of this	VerseKey
	* @return the upper boundary of this	VerseKey
	*/
	VerseKey &UpperBound() const;
	
	/** clears the boundaries of this	VerseKey
	*/
	void ClearBounds();
	
	/** Creates a new	SWKey based on the current	VerseKey
	* see also the Copy Constructor
	*/
	virtual SWKey *clone() const;
	
	/** refreshes keytext before returning if cast to
	* a (char *) is requested
	*/
	virtual const char *getText() const;
	virtual const char *getShortText() const;
	virtual void setText(const char *ikey, bool checkNormalize) { SWKey::setText(ikey); parse(checkNormalize); }
	virtual void setText(const char *ikey) { SWKey::setText(ikey); parse(); }
	virtual void copyFrom(const SWKey &ikey);
	
	/** Equates this VerseKey to another VerseKey
	*/
	virtual void copyFrom(const VerseKey &ikey);
	
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
	virtual bool isTraversable() const { return true; }

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
	
	/** Gets verse suffix
	*
	* @return value of verse suffix
	*/
	virtual char getSuffix() const;
	
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
	
	/** Sets/gets verse suffix
	*
	* @param isuffix value which to set verse suffix
	*/
	virtual void setSuffix(char isuffix);
	
	/** checks limits and normalizes if necessary (e.g.
	* Matthew 29:47 = Mark 2:2).	If last verse is
	* exceeded, key is set to last Book CH:VS
	*
	* @return *this
	*/
	virtual void Normalize(char autocheck = 0);
	
	/** Sets/gets flag that tells VerseKey to
	* automatically normalize itself when modified
	*
	* @param iautonorm value which to set autonorm
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of autonorm,
	* if changed -> previous value of autonorm
	*/
	virtual char AutoNormalize(char iautonorm = MAXPOS(char));
	
	/** Sets/gets flag that tells VerseKey to include
	* chapter/book/testament/module headings
	*
	* @param iheadings value which to set headings
	* [MAXPOS(char)] - only get
	* @return if unchanged -> value of headings,
	* if changed -> previous value of headings
	*/
	virtual char Headings(char iheadings = MAXPOS(char));
	
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
	virtual const char *getOSISBookName() const;
	static const int getOSISBookNum(const char *bookab);

	/** Tries to parse a string and convert it into an OSIS reference
	 * @param inRef reference string to try to parse
	 * @param defaultKey @see ParseVerseList(..., defaultKey, ...)
	 */
	static const char *convertToOSIS(const char *inRef, const SWKey *defaultKey);

	virtual ListKey ParseVerseList(const char *buf, const char *defaultKey = 0, bool expandRange = false);
	virtual const char *getRangeText() const;
	/** Compares another	SWKey object
	*
	* @param ikey key to compare with this one
	* @return >0 if this	VerseKey is greater than compare	SWKey,
	* <0 if this	VerseKey is smaller than compare	SWKey,
	* 0 if the keys are the same
	*/
	virtual int compare(const SWKey & ikey);
	
	/** Compares another	VerseKey object
	*
	* @param ikey key to compare with this one
	* @return >0 if this	VerseKey is greater than compare	VerseKey,
	* <0 if this	VerseKey is smaller than compare	VerseKey,
	* 0 if the keys are the same
	*/
	virtual int _compare(const VerseKey & ikey);
	
	virtual void setBookAbbrevs(const struct abbrev *bookAbbrevs, unsigned int size = 0 /* default determine size */ );
	virtual void setBooks(const char *iBMAX, struct sbook **ibooks);
	virtual void setLocale(const char *name);
	virtual const char *getLocale() const { return locale; }



	// OPERATORS --------------------------------------------------------------------


	SWKEY_OPERATORS

	virtual SWKey & operator =(const VerseKey & ikey) { copyFrom(ikey); return *this; }
};

SWORD_NAMESPACE_END

#endif //VERSEKEY_H
