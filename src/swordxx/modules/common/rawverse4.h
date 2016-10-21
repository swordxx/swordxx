/******************************************************************************
 *
 *  rawverse4.h -    code for class 'RawVerse4'- a module that reads raw
 *            text files:  ot and nt using indexs ??.bks ??.cps
 *            ??.vss and provides lookup and parsing functions based
 *            on class VerseKey
 *
 * $Id$
 *
 * Copyright 2007-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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


#ifndef RAWVERSE4_H
#define RAWVERSE4_H

#include "rawversebase.h"

#include "../../defs.h"


namespace swordxx {

class FileDesc;

class SWDLLEXPORT RawVerse4: public RawVerseBase {

protected:

    void doSetText(char testmt, long idxoff, const char *buf, long len = -1);
    void doLinkEntry(char testmt, long destidxoff, long srcidxoff);

public:
    RawVerse4(const char *ipath, int fileMode = -1);

    void findOffset(char testmt, long idxoff, long *start,    unsigned long *end) const;
    static char createModule(const char *path, const char *v11n = "KJV");
};

} /* namespace swordxx */
#endif
