/******************************************************************************
 *
 *  swfilter.h -    definition of class SWFilter used to filter text between
 *                   different formats
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_SWFILTER_H
#define SWORDXX_SWFILTER_H

#include <string>

#include "defs.h"

namespace swordxx {

class SWKey;
class SWModule;


class SWModule;

/** Base class for all filters in Sword++.
* Filters are used to filter/convert text between different formats
* like GBF, HTML, RTF ...
*/
class SWDLLEXPORT  SWFilter {
public:
    virtual ~SWFilter() noexcept;

    /** This method processes and appropriately modifies the text given it
     *    for a particular filter task
     *
     * @param text The text to be filtered/converted
     * @param key Current key That was used.
     * @param module Current module.
     * @return 0
     */
    virtual char processText(std::string & text,
                             SWKey const * key = nullptr,
                             SWModule const * module = nullptr) = 0;

    /** This method can supply a header associated with the processing done with this filter.
     *    A typical example is a suggested CSS style block for classed containers.
     */
    virtual char const * getHeader() const;
};

    } /* namespace swordxx */

#endif /* SWORDXX_SWFILTER_H */
