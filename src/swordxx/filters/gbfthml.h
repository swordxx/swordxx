/***************************************************************************
 *
 *  gbfhtml.h -    Implementation of GBFHTML
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef GBFTHML_H
#define GBFTHML_H

#include "../swfilter.h"


namespace swordxx {

/** this filter converts GBF text into ThML text
 */
class SWDLLEXPORT GBFThML : public SWFilter {
public:
    GBFThML();
    virtual char processText(std::string & text,
                             SWKey const * key = nullptr,
                             SWModule const * module = nullptr);
};

} /* namespace swordxx */
#endif
