/******************************************************************************
 *
 *  rawfiles.h -    code for class 'RawFiles'- a module that produces HTML
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

#ifndef SWORDXX_RAWFILES_H
#define SWORDXX_RAWFILES_H

#include "../../swcom.h"
#include "../common/rawverse.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT RawFiles : public RawVerse, public SWCom {

    std::string getNextFilename();

public:


    RawFiles(char const * ipath,
             char const * iname = nullptr,
             char const * idesc = nullptr,
             TextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * ilang = nullptr);
    ~RawFiles() override;

    bool isWritable() const noexcept override;

    static char createModule(char const *);
    void setEntry(std::string_view text) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

};

} /* namespace swordxx */

#endif /* SWORDXX_RAWFILES_H */
