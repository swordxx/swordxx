/******************************************************************************
 *  swkey.h	- code for base class 'swkey'.  swkey is the basis for all
 *				types of keys for indexing into modules (e.g. verse, word,
 *				place, etc.)
 *
 * $Id: swkey.h,v 1.9 2001/04/08 09:03:37 chrislit Exp $
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

class SW_POSITION
{
  char pos;
public:
    SW_POSITION (char ipos)
  {
    pos = ipos;
  }
  operator char ()
  {
    return pos;
  }
};

#define POS_TOP ((char)1)
#define POS_BOTTOM ((char)2)

#define TOP SW_POSITION(POS_TOP)
#define BOTTOM SW_POSITION(POS_BOTTOM)

  /** SWKey is the basis for all
  * types of keys for indexing into modules (e.g. verse, word,
  * place, etc.)
  */
class SWDLLEXPORT SWKey:public SWObject
{
  long index;
  static SWClass classdef;
  void init ();
protected:
  char *keytext;
  char persist;
  char error;

public:
  /** initializes instance of SWKey
  *
  * @param ikey text key
  */
  SWKey (const char *ikey = 0);
  /** Copy Constructor
  */
  SWKey (SWKey const &k);
  /** cleans up instance of SWKey
  */
  virtual ~ SWKey ();


  virtual SWKey *clone () const;
  /** Gets whether this object itself persists within a
  * module that it was used to SetKey or just a copy.
  * (1 - persists in module; 0 - a copy is attempted
  *
  * @return value of persist
  */
  char Persist () const;
  /** Set/gets whether this object itself persists within a
  * module that it was used to SetKey or just a copy.
  * (1 - persists in module; 0 - a copy is attempted
  *
  * @param ipersist value which to set persist;
  * [-1] - only get
  * @return value of persist
  */
  char Persist (char ikey);
  /** Gets and clears error status
  *
  * @return error status
  */
  virtual char Error ();
  /** Equates this SWKey to a character string
  *
  * @param ikey string to set this key to
  */
  virtual SWKey & operator = (const char *ikey);
  /** Equates this SWKey to another SWKey object
  *
  * @param ikey other swkey object
  */
  virtual SWKey & operator = (const SWKey & ikey);
  /** returns text key if (char *) cast is requested
  */
  virtual operator const char *() const;
  /** Compares another VerseKey object
  *
  * @param ikey key to compare with this one
  * @return >0 if this key is greater than compare key;
  * <0 if this key is smaller than compare key;
  * 0 if the keys are the same
  */
  virtual int compare (const SWKey & ikey);
  /** Compares another VerseKey object
  *
  * @param ikey key to compare with this one
  * @return <> 0 if the keys are the same
  */
  virtual bool operator == (const SWKey & ikey) { return !compare (ikey);
  }
  /** Compares another VerseKey object
  *
  * @param ikey key to compare with this one
  * @return 0 if the keys are the same
  */
  virtual bool operator != (const SWKey & ikey)
  {
    return compare (ikey);
  }
  /**
  * see @ref compare
  */
  virtual bool operator > (const SWKey & ikey)
  {
    return (compare (ikey) > 0);
  }
  /**
  * see @ref compare
  */
  virtual bool operator < (const SWKey & ikey)
  {
    return (compare (ikey) < 0);
  }
  /**
  * see @ref compare
  */
  virtual bool operator >= (const SWKey & ikey)
  {
    return (compare (ikey) > -1);
  }
  /**
  * see @ref compare
  */
  virtual bool operator <= (const SWKey & ikey)
  {
    return (compare (ikey) < 1);
  }
  /** Positions this key if applicable
  */
  virtual SWKey & operator = (SW_POSITION);
  /** Decrements key a number of entries
  *
  * @param decrement Number of entries to jump backward
  * @return *this
  */
  virtual SWKey & operator -= (int decrement);
  /** Increments key a number of entries
  *
  * @param increment Number of entries to jump forward
  * @return *this
  */
  virtual SWKey & operator += (int increment);
  /** Increments key by 1 entry
  *
  * @return *this
  */
  virtual SWKey & operator++ (int)
  {
    return *this += 1;
  }
  /** Decrements key by 1 entry
  *
  * @return *this
  */
  virtual SWKey & operator-- (int)
  {
    return *this -= 1;
  }
  virtual char Traversable ()
  {
    return 0;
  }
  virtual long Index () const
  {
    return index;
  }
  virtual long Index (long iindex)
  {
    index = iindex;
    return index;
  }
};


#endif
