/******************************************************************************
 *
 *  markupfiltmgr.h -    definition of class SWMarkupMgr, subclass of
 *            used to transcode all module text to a requested
 *            markup
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

#ifndef MARKUPFILTMGR_H
#define MARKUPFILTMGR_H

#include "encfiltmgr.h"


namespace swordxx {

/** This class is like a normal SWEncodingMgr,
* but you can additonally specify which markup
* you want to use.
*/
class SWDLLEXPORT MarkupFilterMgr : public EncodingFilterMgr {
protected:
        SWFilter* fromthml;
        SWFilter* fromgbf;
        SWFilter* fromplain;
        SWFilter* fromosis;
        SWFilter* fromtei;

                /**
                 * current markup value
                 */
        char markup;

        void CreateFilters(char markup);
public:
    /** Constructor of SWMarkupMgr.
     *
     * @param encoding The desired encoding.
     * @param markup The desired markup format.
     */
        MarkupFilterMgr(char markup = FMT_THML, TextEncoding encoding = ENC_UTF8);

    /**
     * The destructor of SWMarkupMgr.
     */
        ~MarkupFilterMgr() override;

    /** Markup sets/gets the markup after initialization
     *
     * @param m The new markup or FMT_UNKNOWN if you just want to get the current markup.
     * @return The current (possibly changed) markup format.
     */
        char Markup(char m = FMT_UNKNOWN);

        void AddRenderFilters(SWModule * module, ConfigEntMap & section) override;
};

} /* namespace swordxx */
#endif
