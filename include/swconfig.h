/******************************************************************************
 *  swconfig.h   - definition of Class SWConfig used for saving and retrieval
 *				of configuration information
 *
 * $Id: swconfig.h,v 1.11 2001/04/06 04:30:46 scribe Exp $
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

#ifndef SWCONFIG_H
#define SWCONFIG_H

#include <stdio.h>

#include <string>
#include <map>

#include <defs.h>

using namespace std;


// multmap that still lets you use [] to reference FIRST
// entry of a key if multiples exist
template <class Key, class T, class Compare>
class multimapwithdefault : public multimap<Key, T, Compare> {
public:
  typedef pair<const Key, T> value_type;
    T& operator[](const Key& k) {
        if (find(k) == end()) {
            insert(value_type(k, T()));
        }
        return (*(find(k))).second;
    }
};

typedef multimapwithdefault < string, string, less < string > >ConfigEntMap;
typedef map < string, ConfigEntMap, less < string > >SectionMap;


class SWDLLEXPORT SWConfig
{
private:
  char getline (FILE * fp, string & line);
public:
    string filename;
  SectionMap Sections;

    SWConfig (const char *ifilename);
    virtual ~ SWConfig ();

  virtual void Load ();
  virtual void Save ();
  virtual SWConfig & operator += (SWConfig & addFrom);
  virtual ConfigEntMap & operator [] (const char *section);
};

#endif
