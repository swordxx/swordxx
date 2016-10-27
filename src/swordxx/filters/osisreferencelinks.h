/******************************************************************************
 *
 *  osisreferencelinks.h -    SWFilter descendant to toggle OSIS reference
 *                links with specified type/subType
 *
 * $Id$
 *
 * Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
 *      CrossWire Bible Society
 *      P. O. Box 2528
 *      Tempe, AZ  85280-2528
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

#ifndef OSISREFERENCELINKS_H
#define OSISREFERENCELINKS_H

#include "../onoffoptionfilter.h"


namespace swordxx {

/** This Filter hides reference links with a particular type/subtype in an OSIS text
 */
class SWDLLEXPORT OSISReferenceLinks : public OnOffOptionFilter {
    std::string optionName;
    std::string optionTip;
    std::string type;
    std::string subType;
public:

    /**
     * @param optionName - name of this option
     * @param optionTip - user help tip for this option
     * @param type - which reference types to hide
     * @param subType - which reference subTypes to hide (optional)
     * @param defaultValue - default value of option "On" or "Off" (optional)
     */
    OSISReferenceLinks(char const * optionName,
                       char const * optionTip,
                       char const * type,
                       char const * subType = nullptr,
                       char const * defaultValue = "On");
    ~OSISReferenceLinks() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */
#endif


