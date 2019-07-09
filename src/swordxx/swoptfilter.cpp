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

#include <cassert>
#include <utility>
#include "utilstr.h"


namespace swordxx {


SWOptionFilter::SWOptionFilter() {
    static const char *empty2 = "";
    optName   = empty2;
    optTip    = empty2;
}


SWOptionFilter::SWOptionFilter(std::string oName,
                               std::string oTip,
                               std::vector<std::string> oValues)
    : optName(std::move(oName))
    , optTip(std::move(oTip))
    , optValues(std::move(oValues))
    , selectedValueIndex(0u)
{
    if (!optValues.empty())
        setOptionValue(optValues.front().c_str());
    isBooleanVal = optValues.size() == 2 && (optValues.front() == "On" || optValues.front() == "Off");
}


SWOptionFilter::~SWOptionFilter() {
}


void SWOptionFilter::setOptionValue(std::string_view value) {
    for (auto const & optValue : optValues) {
        if (caseInsensitiveEquals(optValue, value)) {
            option = caseInsensitiveEquals(value.substr(0u, 2u), "On"); // convenience for boolean filters
            break;
        }
    }
}

std::string const & SWOptionFilter::getSelectedOptionValue() const noexcept {
    if (optValues.empty()) {
        static std::string const invalid("<INVALID>");
        return invalid;
    }
    assert(selectedValueIndex < optValues.size());
    return optValues[selectedValueIndex];
}


} /* namespace swordxx */
