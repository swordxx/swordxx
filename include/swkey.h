/******************************************************************************
 *  swkey.h	- code for base class 'swkey'.  swkey is the basis for all
 *				types of keys for indexing into modules (e.g. verse, word,
 *				place, etc.)
 *
 * $Id: swkey.h,v 1.18 2002/07/28 01:48:38 scribe Exp $
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

#ifndef SWKEY_H
#define SWKEY_H

#include <swobject.h>

#include <defs.h>

#define KEYERR_OUTOFBOUNDS 1

#define SWKEY_OPERATORS \
  SWKey & operator =(const char *ikey) { setText(ikey); return *this; } \
  SWKey & operator =(const SWKey &ikey) { copyFrom(ikey); return *this; } \
  SWKey & operator =(SW_POSITION pos) { setPosition(pos); return *this; } \
  operator const char *() const { return getText(); } \
  bool operator ==(const SWKey & ikey) { return equals(ikey); } \
  bool operator !=(const SWKey & ikey) { return !equals(ikey); } \
  virtual bool operator >(const SWKey & ikey) { return (compare(ikey) > 0); } \
  virtual bool operator <(const SWKey & ikey) { return (compare(ikey) < 0); } \
  virtual bool operator >=(const SWKey & ikey) { return (compare(ikey) > -1); }  \
  virtual bool operator <=(const SWKey & ikey) { return (compare(ikey) < 1); } \
  SWKey & operator -=(int steps) { decrement(steps); return *this; } \
  SWKey & operator +=(int steps) { increment(steps); return *this; } \
  SWKey & operator++(int) { return *this += 1; } \
  SWKey & operator--(int) { return *this -= 1; }


// For use with = operator to position key.

class SW_POSITION {
	char pos;
public:
	SW_POSITION(char ipos) { pos = ipos; }
	operator char() { return pos; }
};

#define POS_TOP ((char)1)
#define POS_BOTTOM ((char)2)

#define TOP SW_POSITION(POS_TOP)
#define BOTTOM SW_POSITION(POS_BOTTOM)

/** SWKey is the basis for all
* types of keys for indexing into modules (e.g. verse, word,
* place, etc.)
*/
class SWDLLEXPORT SWKey : public SWObject {
	long index;
	static SWClass classdef;
	void init();

protected:
	char *keytext;
	char persist;
	char error;

public:

	// misc pointer for whatever
	void *userData;

	/** initializes instance of SWKey
	*
	* @param ikey text key
	*/
	SWKey(const char *ikey = 0);

	/** Copy Constructor
	* @param The SWKey object to copy.
	*/
	SWKey(SWKey const &k);

	/** Destructor, cleans up this instance of SWKey
	*/
	virtual ~SWKey();

	/** Returns a copy of this SWKey object.
	* This is useful to get a 1:1 copy of an SWKey based object.
	* @return SWKey
	*/
	virtual SWKey *clone() const;

	/** Gets whether this object itself persists within a
	* module that it was used to SetKey or just a copy.
	* (1 - persists in module; 0 - a copy is attempted
	*
	* @return value of persist
	*/
	char Persist() const;

	/** Set/gets whether this object itself persists within a
	* module that it was used to SetKey or just a copy.
	* (1 - persists in module; 0 - a copy is attempted
	*
	* @param ipersist value which to set persist;
	* [-1] - only get
	* @return value of persist
	*/
	char Persist(signed char ikey);

	/** Gets and clears error status
	*
	* @return error status
	*/
	virtual char Error();

	/** Equates this SWKey to a character string
	*
	* @param ikey string to set this key to
	*/
	virtual void setText(const char *ikey);

	/** Equates this SWKey to another SWKey object
	*
	* @param ikey other swkey object
	*/
	virtual void copyFrom(const SWKey &ikey);

	/** returns text key if (char *) cast is requested
	*/
	virtual const char *getText() const;
	virtual const char *getShortText() const { return getText(); }

	/** Compares another VerseKey object
	*
	* @param ikey key to compare with this one
	* @return >0 if this key is greater than compare key;
	* <0 if this key is smaller than compare key;
	* 0 if the keys are the same
	*/
	virtual int compare(const SWKey & ikey);

	/** Compares another VerseKey object
	*
	* @param ikey key to compare with this one
	* @return true if the keys are the same
	*/
	virtual bool equals(const SWKey &ikey) { return !compare(ikey); }

	virtual void setPosition(SW_POSITION);

	/** Decrements key a number of entries
	*
	* @param steps Number of entries to jump backward
	* @return *this
	*/
	virtual void decrement(int steps = 1);

	/** Increments key a number of entries
	*
	* @param increment Number of entries to jump forward
	* @return *this
	*/
	virtual void increment(int steps = 1);

	virtual char Traversable() { return 0; }

	/** Use this function to get te current position withing a module.
	* Here's a small example how to use this function and @ref Index(long).
	* This function uses the GerLut module and chooses a random verse from the
	* Bible and returns it.
	* @code
	* const char* randomVerse() {
	*   VerseKey vk;
	*   SWMgr mgr;
	*   LocaleMgr::systemLocaleMgr.setDefaultLocaleName("de");
	*
	*   SWModule* module = mgr->Modules("GerLut");
	*   srand( time(0) );
	*   const double newIndex = (double(rand())/RAND_MAX)*(24108+8224);
	*   vk.Index(newIndex);
	*   module->SetKey(vk);
	*
	*   char* text;
	*   sprintf(text, "%s: %s",(const char*)vk ,module->StripText(&vk));
	*   return text;
	* @endcode
	*/
	virtual long Index() const { return index; }
	virtual long Index(long iindex) { index = iindex; return index; }

	SWKEY_OPERATORS

	};


#endif
