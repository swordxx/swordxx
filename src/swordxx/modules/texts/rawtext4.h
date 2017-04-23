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
             TextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * ilang = nullptr,
             char const * versification = "KJV");
    ~RawText4() override;

    std::string & getRawEntryBuf() const override;
    void increment(int steps = 1) override;
    void decrement(int steps = 1) override { increment(-steps); }
    bool isWritable() const override;
    static char createModule(const char *path, const char *v11n = "KJV") { return RawVerse4::createModule(path, v11n); }
    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    bool isLinked(SWKey const * k1, SWKey const * k2) const override;
    bool hasEntry(SWKey const * k) const override;

};

} /* namespace swordxx */
#endif
