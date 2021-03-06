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

#include "offonoptionfilter.h"

#include <list>
#include <string>


namespace swordxx {

OffOnOptionFilter::OffOnOptionFilter(std::string name, std::string tip)
    : SWOptionFilter(std::move(name), std::move(tip), {"Off", "On"})
{}

OffOnOptionFilter::~OffOnOptionFilter() noexcept = default;

} /* namespace swordxx */
