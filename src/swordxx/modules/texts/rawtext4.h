/******************************************************************************
 *
 *  rawtext4.h -    code for class 'RawText4'- a module that reads raw
 *            text files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
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

#ifndef RAWTEXT4_H
#define RAWTEXT4_H

#include "../../swtext.h"
#include "../common/rawverse4.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT RawText4 : public SWText, public RawVerse4 {

public:

    RawText4(char const * ipath,
             char const * iname = nullptr,
             char const * idesc = nullptr,
             SWTextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * ilang = nullptr,
             char const * versification = "KJV");
    virtual ~RawText4();
    virtual std::string &getRawEntryBuf() const;
    virtual void increment(int steps = 1);
    virtual void decrement(int steps = 1) { increment(-steps); }
    // write interface ----------------------------
    virtual bool isWritable() const;
    static char createModule(const char *path, const char *v11n = "KJV") { return RawVerse4::createModule(path, v11n); }
    virtual void setEntry(const char *inbuf, long len = -1);    // Modify current module entry
    virtual void linkEntry(const SWKey *linkKey);    // Link current module entry to other module entry
    virtual void deleteEntry();    // Delete current module entry
    // end write interface ------------------------

    virtual bool isLinked(const SWKey *k1, const SWKey *k2) const;
    virtual bool hasEntry(const SWKey *k) const;

    SWMODULE_OPERATORS

};

} /* namespace swordxx */
#endif
