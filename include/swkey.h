/******************************************************************************
 *  swkey.h	- code for base class 'swkey'.  swkey is the basis for all
 *				types of keys for indexing into modules (e.g. verse, word,
 *				place, etc.)
 *
 * $Id: swkey.h,v 1.6 2001/02/08 09:20:48 chrislit Exp $
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

// For use with = operator to position key.

class POSITION {
	char pos;
public:
	POSITION(char ipos) { pos = ipos; }
	operator char() { return pos; }
};

#define POS_TOP ((char)1)
#define POS_BOTTOM ((char)2)

#define TOP POSITION(POS_TOP)
#define BOTTOM POSITION(POS_BOTTOM)

class SWDLLEXPORT SWKey : public SWObject {
	long index;
	static SWClass classdef;
	void init();
protected:
	char *keytext;
	char persist;
	char error;

public:
	SWKey(const char *ikey = 0);
	SWKey(SWKey const &k);
	virtual ~SWKey();


	virtual SWKey *clone() const;
	char Persist() const;
	char Persist(char ikey);

	virtual char Error();
	virtual SWKey &operator =(const char *ikey);
	virtual SWKey &operator =(const SWKey &ikey);
	virtual operator const char*() const;

	virtual int compare(const SWKey &ikey);
	virtual bool operator ==(const SWKey &ikey) { return !compare(ikey); }
	virtual bool operator !=(const SWKey &ikey) { return compare(ikey); }
	virtual bool operator  >(const SWKey &ikey) { return (compare(ikey) > 0); }
	virtual bool operator  <(const SWKey &ikey) { return (compare(ikey) < 0); }
	virtual bool operator >=(const SWKey &ikey) { return (compare(ikey) > -1); }
	virtual bool operator <=(const SWKey &ikey) { return (compare(ikey) < 1); }
	virtual SWKey &operator =(POSITION);
	virtual SWKey &operator -=(int);
	virtual SWKey &operator +=(int);
	virtual SWKey &operator ++(int) { return *this += 1; }
	virtual SWKey &operator --(int) { return *this -= 1; }
	virtual char Traversable() { return 0; }
	virtual long Index() const { return index; }
	virtual long Index(long iindex) { index = iindex; return index; }
};


#endif
