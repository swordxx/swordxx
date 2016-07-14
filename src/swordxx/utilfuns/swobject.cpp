/******************************************************************************
 *
 *  swobject.cpp -    code for SWClass used as lowest base class for many
 *            Sword++ objects
 *
 * $Id$
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swobject.h"

// hack.  remove this when we figure out our link error
#ifndef __CYGWIN__
#include "utilstr.h"
#else
#include <cstring>
#endif


namespace swordxx {


bool SWClass::isAssignableFrom(const char *className) const {
    for (int i = 0; descends[i]; i++) {
#ifndef __CYGWIN__
        if (!swordxx::stricmp(descends[i], className))
#else
        if (!stricmp(descends[i], className))
#endif
            return true;
    }
    return false;
}


} /* namespace swordxx */

