/******************************************************************************
 * listkey.h	- code for base class 'listkey'.  listkey is the basis for all
 *				types of keys for indexing into modules
 *				(e.g. verse, word,
 *				place, etc.)
 *
 * $Id: listkey.h,v 1.11 2001/04/20 02:17:32 scribe Exp $
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

#ifndef SWLSTKEY_H
#define SWLSTKEY_H

#include <swkey.h>

#include <defs.h>

  /** ListKey is the basis for all
  * types of keys that have lists of specified indexes
  * (e.g. a list of verses, place, etc.)
  */
class SWDLLEXPORT ListKey : public SWKey {

  static SWClass classdef;
  void init ();
protected:
  int arraypos;
  int arraymax;
  int arraycnt;
  SWKey **array;
public:
  /** initializes instance of ListKey
  *
  * @param ikey text key
  */
    ListKey (const char *ikey = 0);
    ListKey (ListKey const &k);
  /** cleans up instance of ListKey
  */
    virtual ~ ListKey ();

  virtual SWKey *clone () const;
  /** Clears out elements of list
  */
  virtual void ClearList ();
  /** Returns number of elements in list
  * @return number of elements in list
  */
  virtual int Count ();
  /** Removes current element from list
  */
  virtual void Remove ();
  /** Sets key to element number
  *
  * @param ielement element number to set to
  * @return error status
  */
  virtual char SetToElement (int ielement, SW_POSITION = TOP);
  /** Gets a key element number
  *
  * @param pos element number to get (or default current)
  * @return Key or null on error
  */
 virtual SWKey *GetElement (int pos = -1);
  /** Adds an element to the list
  * @param ikey the element to add
  */
  virtual ListKey & operator << (const SWKey &ikey);
  /** Equates this ListKey to another ListKey object
  *
  * @param ikey other ListKey object
  */
  virtual ListKey & operator = (const ListKey & ikey);
  virtual SWKey & operator = (const char *ikey) { return SWKey::operator =
      (ikey);}
  virtual SWKey & operator = (const SWKey & ikey) { return SWKey::operator =
      (ikey);}
  /** Positions this key
  *
  * @param p position
  * @return *this
  */
  virtual SWKey & operator = (SW_POSITION);
  /** Decrements a number of elements
  */
  virtual SWKey & operator -= (int decrement);
  /** Increments a number of elements
  */
  virtual SWKey & operator += (int increment);
  virtual char Traversable ()
  {
    return 1;
  }
  virtual long Index () const
  {
    return arraypos;
  }
  /**
  * Returns the index for the new one given as as parameter.
  * The first parameter is the new index.
  */
  virtual long Index (long index)
  {
    SetToElement (index);
    return Index ();
  }
};


#endif
