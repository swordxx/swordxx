/******************************************************************************
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_FILTERS_ThMLWEBIF_H
#define SWORDXX_FILTERS_ThMLWEBIF_H

#include "thmlxhtml.h"


namespace swordxx {

/** this filter converts ThML text to HTML text with hrefs
 */
class SWDLLEXPORT ThMLWEBIF : public ThMLXHTML {

protected:

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    ThMLWEBIF();
};

} /* namespace swordxx */

#endif /* SWORDXX_FILTERS_ThMLWEBIF_H */
