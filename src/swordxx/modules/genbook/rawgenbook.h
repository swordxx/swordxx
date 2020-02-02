/******************************************************************************
 *
 *  rawgenbook.h -    code for class 'RawGenBook'- a module that reads raw
 *            text files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_RAWGENBOOK_H
#define SWORDXX_RAWGENBOOK_H

#include "../../swgenbook.h"

#include <memory>
#include "../../defs.h"
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;

class SWDLLEXPORT RawGenBook : public SWGenBook {
    std::string m_path;
    std::shared_ptr<FileDesc> bdtfd;
    bool verseKey;

public:


    RawGenBook(NormalizedPath const & path,
               char const * iname = nullptr,
               char const * idesc = nullptr,
               TextEncoding encoding = ENC_UNKNOWN,
               SWTextDirection dir = DIRECTION_LTR,
               SWTextMarkup markup = FMT_UNKNOWN,
               char const * ilang = nullptr,
               char const * keyType = "TreeKey");
    ~RawGenBook() override;

    bool isWritable() const noexcept override;
    static char createModule(NormalizedPath const & path);
    void setEntry(std::string_view text) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;
    std::shared_ptr<SWKey> createKey() const override;

    bool hasEntry(SWKey const & k) const override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

};

} /* namespace swordxx */

#endif /* SWORDXX_RAWGENBOOK_H */
