/******************************************************************************
 *
 *  swoptfilter.cpp -    SWFilter descendant and base class for all option
 *             filters
 *
 * $Id$
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

#include "swoptfilter.h"

#include "utilstr.h"


namespace swordxx {


SWOptionFilter::SWOptionFilter() {
    static std::list<std::string> empty;
    static const char *empty2 = "";
    optName   = empty2;
    optTip    = empty2;
    optValues = &empty;
}


SWOptionFilter::SWOptionFilter(char const * oName,
                               char const * oTip,
                               std::list<std::string> const * oValues)
{
    optName   = oName;
    optTip    = oTip;
    optValues = oValues;
    if (optValues->begin() != optValues->end())
        setOptionValue(optValues->begin()->c_str());
    isBooleanVal = optValues->size() == 2 && (optionValue == "On" || optionValue == "Off");
}


SWOptionFilter::~SWOptionFilter() {
}


void SWOptionFilter::setOptionValue(const char *ival) {
    for (auto const & optValue : *optValues) {
        if (!stricmp(optValue.c_str(), ival)) {
            optionValue = optValue;
            option = (!strnicmp(ival, "On", 2));    // convenience for boolean filters
            break;
        }
    }
}

const char *SWOptionFilter::getOptionValue() {
    return optionValue.c_str();
}


} /* namespace swordxx */
