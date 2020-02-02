/******************************************************************************
 *
 *  hrefcom.h -    code for class 'HREFCom'- a module that produces HTML HREFs
 *               pointing to actual text desired.  Uses standard
 *        files:    ot and nt using indexs ??.bks ??.cps ??.vss
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

#ifndef SWORDXX_HREFCOM_H
#define SWORDXX_HREFCOM_H

#include "../../swcom.h"
#include "../common/rawverse.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT HREFCom: public RawVerse, public SWCom {

public: /* Methods: */

    HREFCom(char const * ipath,
            char const * prefix,
            char const * iname = nullptr,
            char const * idesc = nullptr);

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

private: /* Fields: */

    std::string m_prefix;

};

} /* namespace swordxx */

#endif /* SWORDXX_HREFCOM_H */
