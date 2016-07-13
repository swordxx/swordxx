/******************************************************************************
 *
 *  thmlosis.h -    Implementation of ThMLOSIS
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef THMLOSIS_H
#define THMLOSIS_H

#include "swfilter.h"


namespace swordxx {

/** this filter converts ThML text to OSIS text
 */
class SWDLLEXPORT ThMLOSIS : public SWFilter {
public:
    ThMLOSIS();
    virtual ~ThMLOSIS();
    virtual char processText(std::string &text, const SWKey *key = 0, const SWModule *module = 0);
};

} /* namespace swordxx */
#endif /* THMLOSIS_H */
