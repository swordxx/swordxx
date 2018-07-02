/******************************************************************************
 *
 *  utf8nfc.h -    Implementation of UTF8NFC
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

#ifndef SWORDXX_UTF8NFC_H
#define SWORDXX_UTF8NFC_H

#include "../swfilter.h"

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/unorm.h>
#include <unicode/utypes.h>


namespace swordxx {

/** This filter normalizes UTF-8 encoded text
 */
class SWDLLEXPORT UTF8NFC : public SWFilter {
private:
    UConverter* conv;
    UErrorCode err;
public:
    UTF8NFC();
    ~UTF8NFC() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_UTF8NFC_H */
