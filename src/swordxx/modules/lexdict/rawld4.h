/******************************************************************************
 *
 *  rawld4.cpp -    code for class 'RawLD4'- a module that reads raw
 *            lexicon and dictionary files: *.dat *.idx
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef RAWLD4_H
#define RAWLD4_H

#include "../../swld.h"
#include "../common/rawstr4.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT RawLD4 : public RawStr4, public SWLD {
    char getEntry(long away = 0) const;

public:


    RawLD4(char const * ipath,
           char const * iname = nullptr,
           char const * idesc = nullptr,
           TextEncoding encoding = ENC_UNKNOWN,
           SWTextDirection dir = DIRECTION_LTR,
           SWTextMarkup markup = FMT_UNKNOWN,
           char const * ilang = nullptr,
           bool caseSensitive = false,
           bool strongsPadding = true);

    ~RawLD4() override;

    std::string &getRawEntryBuf() const override;

    void increment(int steps = 1) override;
    void decrement(int steps = 1) override { increment(-steps); }
    bool isWritable() const override;
    static char createModule(const char *path) { return RawStr4::createModule(path); }

    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const * linkKey) override;
    void deleteEntry() override;
    long getEntryCount() const override;
    long getEntryForKey(char const * key) const override;
    char * getKeyForEntry(long entry) const override;

};

} /* namespace swordxx */
#endif
