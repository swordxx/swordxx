/******************************************************************************
 *
 *  zld.cpp -    code for class 'zLD'- a module that reads compressed lexicon
 *        and dictionary files
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

#ifndef SWORDXX_ZLD_H
#define SWORDXX_ZLD_H

#include "../../swld.h"
#include "../common/zstr.h"

#include <utility>
#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT zLD : private zStr, public SWLD {
    char getEntry(std::string & entry, long away = 0) const;

public:

    zLD(char const * ipath,
        char const * iname = nullptr,
        char const * idesc = nullptr,
        long m_blockCount = 200,
        std::unique_ptr<SWCompress> = nullptr,
        TextEncoding encoding = ENC_UNKNOWN,
        SWTextDirection dir = DIRECTION_LTR,
        SWTextMarkup markup = FMT_UNKNOWN,
        char const * ilang = nullptr,
        bool m_caseSensitive = false,
        bool strongsPadding = true);
    ~zLD() override;

    void increment(int steps = 1) override;
    void decrement(int steps = 1) override;

    bool isWritable() const noexcept override;
    static char createModule(const char *path) {
        return zStr::createModule(path);
    }

    void setEntry(std::string_view text) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    void rawZFilter(std::string & buf, char direction = 0) const override;

    void flush() override;

    long getEntryCount() const override;
    long getEntryForKey(char const * key) const override;
    std::string getKeyForEntry(long entry) const override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

};

} /* namespace swordxx */

#endif /* SWORDXX_ZLD_H */
