/******************************************************************************
 *
 *  hrefcom.cpp -    code for class 'HREFCom'- a module that produces HTML
 *            HREFs pointing to actual text desired.  Uses standard
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
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

HREFCom::HREFCom(char const * ipath,
                 char const * iprefix,
                 char const * iname,
                 char const * idesc)
    : RawVerse(ipath)
    , SWCom(iname, idesc)
    , m_prefix(iprefix)
{}


/******************************************************************************
 * HREFCom::operator char *    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string HREFCom::getRawEntryImpl() const {
    StartType start;
    SizeType size;
    auto const key_(getVerseKey());

    findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size);

    auto entry(readText(key_->getTestament(), start, size));

    return m_prefix + entry;
}

} /* namespace swordxx */
