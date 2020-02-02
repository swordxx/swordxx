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

#ifndef SWORDXX_GBFWEBIF_H
#define SWORDXX_GBFWEBIF_H

#include "gbfxhtml.h"


namespace swordxx {

/** this filter converts GBF text to classed XHTML for web interfaces
 */
class SWDLLEXPORT GBFWEBIF : public GBFXHTML {
    const std::string baseURL;
    const std::string passageStudyURL;

protected:

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    GBFWEBIF();
};

} /* namespace swordxx */

#endif /* SWORDXX_GBFWEBIF_H */
