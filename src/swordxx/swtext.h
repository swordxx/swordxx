/******************************************************************************
 *
 *  swtext.h -    code for base class 'SWText'.  SWText is the basis for all
 *        types of text modules
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

#ifndef SWORDXX_SWTEXT_H
#define SWORDXX_SWTEXT_H

#include "swmodule.h"

#include "defs.h"
#include "keys/versekey.h"


namespace swordxx {

/** The basis for all text modules
 */
class SWDLLEXPORT SWText : public SWModule {

protected:
    std::shared_ptr<VerseKey const> getVerseKey(
            std::shared_ptr<SWKey const> key = nullptr) const;

public:

    SWText(char const * imodname = nullptr,
           char const * imoddesc = nullptr,
           TextEncoding encoding = ENC_UNKNOWN,
           SWTextDirection dir = DIRECTION_LTR,
           SWTextMarkup markup = FMT_UNKNOWN,
           char const * ilang = nullptr,
           std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    std::shared_ptr<SWKey> createKey() const override;

private: /* Methods: */

    SWText(char const * imodname,
           char const * imoddesc,
           TextEncoding encoding,
           SWTextDirection dir,
           SWTextMarkup markup,
           char const * ilang,
           std::shared_ptr<VerseKey> key);

private: /* Fields: */

    std::shared_ptr<VersificationMgr::System const> const m_v11n;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWTEXT_H */
