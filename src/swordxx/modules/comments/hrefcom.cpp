/******************************************************************************
 *
 *  hrefcom.cpp -    code for class 'HREFCom'- a module that produces HTML
 *            HREFs pointing to actual text desired.  Uses standard
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "hrefcom.h"

#include <cctype>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include "../../keys/versekey.h"
#include "../../utilstr.h"
#include "../common/rawverse.h"


namespace swordxx {

 /******************************************************************************
 * HREFCom Constructor - Initializes data for instance of HREFCom
 *
 * ENT:    iname - Internal name for module
 *    iprefix - string to prepend to each HREF (e.g. "file://mods/com/jfb/")
 *    idesc - Name to display to user for module
 */

HREFCom::HREFCom(const char *ipath, const char *iprefix, const char *iname, const char *idesc) : RawVerse(ipath), SWCom(iname, idesc)
{
    prefix   = nullptr;
    stdstr(&prefix, iprefix);
}


/******************************************************************************
 * HREFCom Destructor - Cleans up instance of HREFCom
 */

HREFCom::~HREFCom() { delete [] prefix; }


/******************************************************************************
 * HREFCom::operator char *    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string &HREFCom::getRawEntryBuf() const {
    long  start;
    unsigned short size;
    VerseKey * key = nullptr;

        key = &getVerseKey();

    findOffset(key->getTestament(), key->getTestamentIndex(), &start, &size);
    entrySize = size;        // support getEntrySize call

    std::string tmpbuf;

    readText(key->getTestament(), start, size, tmpbuf);
    entryBuf = prefix;
    entryBuf += tmpbuf.c_str();
    prepText(entryBuf);

    if (key != this->key)
        delete key;

    return entryBuf;
}

} /* namespace swordxx */
