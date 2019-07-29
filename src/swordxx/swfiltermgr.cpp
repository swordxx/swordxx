/******************************************************************************
 *
 * Copyright 2018 Jaak Ristioja
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

#include "swfiltermgr.h"


namespace swordxx {

SWFilterMgr::~SWFilterMgr() noexcept = default;

void SWFilterMgr::addGlobalOptions(SWModule &,
                                   ConfigEntMap const &,
                                   ConfigEntMap::const_iterator,
                                   ConfigEntMap::const_iterator)
{}

void SWFilterMgr::addLocalOptions(SWModule &,
                                  ConfigEntMap const &,
                                  ConfigEntMap::const_iterator,
                                  ConfigEntMap::const_iterator)
{}

void SWFilterMgr::addEncodingFilters(SWModule &, ConfigEntMap const &) {}

void SWFilterMgr::addRenderFilters(SWModule &, ConfigEntMap const &) {}

void SWFilterMgr::addStripFilters(SWModule &, ConfigEntMap const &) {}

void SWFilterMgr::addRawFilters(SWModule &, ConfigEntMap const &) {}

} /* namespace swordxx */
