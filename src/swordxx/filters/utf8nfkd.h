/******************************************************************************
 *
 *  utf8nfkd.h -    Implementation of UTF8NFKD
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

#ifndef SWORDXX_UTF8NFKD_H
#define SWORDXX_UTF8NFKD_H

#include "../swfilter.h"


namespace swordxx {

/** Filter to normalize UTF-8 encoded text using normalization form KD. */
class SWDLLEXPORT UTF8NFKD : public SWFilter {

public: /* Methods: */

    UTF8NFKD() noexcept;
    ~UTF8NFKD() noexcept override;

    char processText(std::string & text,
                     SWKey const * key = nullptr,
                     SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_UTF8NFKD_H */
