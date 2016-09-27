/******************************************************************************
 *
 *  swfiltermgr.cpp -    Implementation of SWFilterMgr, used as an interface
 *            to manage filters on a module
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

#include "swfiltermgr.h"


namespace swordxx {


SWFilterMgr::SWFilterMgr() {
}


SWFilterMgr::~SWFilterMgr() {
}


void SWFilterMgr::setParentMgr(SWMgr *parentMgr) {
    this->parentMgr = parentMgr;
}


SWMgr *SWFilterMgr::getParentMgr() {
    return parentMgr;
}


void SWFilterMgr::AddGlobalOptions(SWModule * /* module */, ConfigEntMap & /* section */, ConfigEntMap::iterator /* start */, ConfigEntMap::iterator /* end */) {
}


void SWFilterMgr::AddLocalOptions(SWModule * /* module */, ConfigEntMap & /* section */, ConfigEntMap::iterator /* start */, ConfigEntMap::iterator /* end */) {
}


/**
* Adds the encoding filters which are defined in "section" to the SWModule object "module".
* @param module To this module the encoding filter(s) are added
* @param section We use this section to get a list of filters we should apply to the module
*/

void SWFilterMgr::AddEncodingFilters(SWModule * /* module */, ConfigEntMap & /* section */) {
}


/**
* Adds the render filters which are defined in "section" to the SWModule object "module".
* @param module To this module the render filter(s) are added
* @param section We use this section to get a list of filters we should apply to the module
*/

void SWFilterMgr::AddRenderFilters(SWModule * /* module */, ConfigEntMap & /*section */) {
}


/**
* Adds the strip filters which are defined in "section" to the SWModule object "module".
* @param module To this module the strip filter(s) are added
* @param section We use this section to get a list of filters we should apply to the module
*/

void SWFilterMgr::AddStripFilters(SWModule * /* module */, ConfigEntMap & /* section */) {
}


/**
* Adds the raw filters which are defined in "section" to the SWModule object "module".
* @param module To this module the raw filter(s) are added
* @param section We use this section to get a list of filters we should apply to the module
*/

void SWFilterMgr::AddRawFilters(SWModule * /* module */, ConfigEntMap & /* section */) {
}


} /* namespace swordxx */

