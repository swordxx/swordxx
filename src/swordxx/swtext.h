/******************************************************************************
 *
 *  swtext.h -    code for base class 'SWText'.  SWText is the basis for all
 *        types of text modules
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWTEXT_H
#define SWTEXT_H

#include "swmodule.h"

#include "defs.h"


namespace swordxx {

class VerseKey;

/** The basis for all text modules
 */
class SWDLLEXPORT SWText : public SWModule {

    // for conversion if we have been set with a different internal key type
    mutable VerseKey *tmpVK1;
    mutable VerseKey *tmpVK2;
        mutable bool tmpSecond;
    char *versification;

protected:
    VerseKey & getVerseKey(SWKey const * key = nullptr) const;

public:

    SWText(char const * imodname = nullptr,
           char const * imoddesc = nullptr,
           TextEncoding encoding = ENC_UNKNOWN,
           SWTextDirection dir = DIRECTION_LTR,
           SWTextMarkup markup = FMT_UNKNOWN,
           char const * ilang = nullptr,
           char const * versification = "KJV");

    ~SWText() override;

    SWKey * createKey() const override;

    long getIndex() const override;
    void setIndex(long iindex) override;

};

} /* namespace swordxx */

#endif
