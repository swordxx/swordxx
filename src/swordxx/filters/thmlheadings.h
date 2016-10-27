/******************************************************************************
 *
 *  thmlheadings.h -    Implementation of ThMLHeadings
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

#ifndef THMLHEADINGS_H
#define THMLHEADINGS_H

#include "../offonoptionfilter.h"


namespace swordxx {

/** This Filter shows/hides headings in a ThML text
 */
class SWDLLEXPORT ThMLHeadings: public OffOnOptionFilter {
public:
    ThMLHeadings();
    ~ThMLHeadings() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */
#endif
