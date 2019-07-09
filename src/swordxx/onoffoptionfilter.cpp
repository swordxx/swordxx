/***************************************************************************
 *
 * Copyright 2016 Jaak Ristioja
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

#include "onoffoptionfilter.h"

#include <list>
#include <string>


namespace swordxx {

namespace { std::list<std::string> const onOffValues({"On", "Off"}); }

OnOffOptionFilter::OnOffOptionFilter()
    : SWOptionFilter("", "", {"On", "Off"})
{}

OnOffOptionFilter::OnOffOptionFilter(char const * name, char const * tip)
    : SWOptionFilter(name, tip, {"On", "Off"})
{}

OnOffOptionFilter::~OnOffOptionFilter() noexcept = default;

} /* namespace swordxx */
