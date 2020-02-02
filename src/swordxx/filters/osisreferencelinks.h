/******************************************************************************
 *
 *  osisreferencelinks.h -    SWFilter descendant to toggle OSIS reference
 *                links with specified type/subType
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

#ifndef SWORDXX_OSISREFERENCELINKS_H
#define SWORDXX_OSISREFERENCELINKS_H

#include "../onoffoptionfilter.h"


namespace swordxx {

/** This Filter hides reference links with a particular type/subtype in an OSIS text
 */
class SWDLLEXPORT OSISReferenceLinks : public OnOffOptionFilter {
    std::string m_type;
    std::string m_subType;
public:

    /**
     * @param optionName - name of this option
     * @param optionTip - user help tip for this option
     * @param type - which reference types to hide
     * @param subType - which reference subTypes to hide (optional)
     * @param defaultValue - default value of option "On" or "Off" (optional)
     */
    OSISReferenceLinks(std::string m_optionName,
                       std::string m_optionTip,
                       std::string m_type,
                       std::string m_subType = std::string(),
                       std::string_view defaultValue = "On");
    ~OSISReferenceLinks() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_OSISREFERENCELINKS_H */
