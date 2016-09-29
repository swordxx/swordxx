/******************************************************************************
 *
 *  encfiltmgr.h -    definition of class SWEncodingMgr, subclass of
 *            used to transcode all module text to a requested
 *            markup
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

#ifndef ENCFILTERMGR_H
#define ENCFILTERMGR_H

#include "swfiltermgr.h"

#include "defs.h"


namespace swordxx {

class SWFilter;

/** This class is like a normal SWMgr,
  * but you can additonally specify which encoding
  * you want to use.
  */

class SWDLLEXPORT EncodingFilterMgr : public SWFilterMgr {

protected:
    SWFilter *latin1utf8;
    SWFilter *scsuutf8;
    SWFilter *utf16utf8;
    SWFilter *targetenc;


    /*
     * current encoding value
     */
    TextEncoding encoding;

public:


    /** Constructor of SWEncodingMgr.
     *
     * @param encoding The desired encoding.
     */
    EncodingFilterMgr(TextEncoding encoding = ENC_UTF8);

    /**
     * The destructor of SWEncodingMgr.
     */
    ~EncodingFilterMgr() override;

    /** Markup sets/gets the encoding after initialization
     *
     * @param enc The new encoding or ENC_UNKNOWN if you just want to get the current markup.
     * @return The current (possibly changed) encoding format.
     */
    TextEncoding Encoding(TextEncoding enc);

    /**
     * Adds the raw filters which are defined in "section" to the SWModule object "module".
     * @param module To this module the raw filter(s) are added
     * @param section We use this section to get a list of filters we should apply to the module
     */
    void AddRawFilters(SWModule * module, ConfigEntMap & section) override;

    /**
     * Adds the encoding filters which are defined in "section" to the SWModule object "module".
     * @param module To this module the encoding filter(s) are added
     * @param section We use this section to get a list of filters we should apply to the module
     */
    void AddEncodingFilters(SWModule * module, ConfigEntMap & section) override;
};

} /* namespace swordxx */
#endif
