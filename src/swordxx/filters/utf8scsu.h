/******************************************************************************
 *
 *  utf8scsu.h -    Implementation of UTF8SCSU
 *
 * $Id$
 *
 * Copyright 2001-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef UTF8SCSU_H
#define UTF8SCSU_H

#include "../swfilter.h"

#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>


namespace swordxx {

/** This filter converts UTF-8 encoded text to SCSU
 */
class SWDLLEXPORT UTF8SCSU : public SWFilter {
private:
    UConverter* scsuConv;
    UConverter* utf8Conv;
    UErrorCode err;
public:
    UTF8SCSU();
    ~UTF8SCSU() override;
    char processText(std::string & text,
                     SWKey const * key = nullptr,
                     SWModule const * module = nullptr) override;
};

} /* namespace swordxx */
#endif
