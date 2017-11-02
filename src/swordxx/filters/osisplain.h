/******************************************************************************
 *
 *  osisplain.h -    Implementation of OSISPlain
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

#ifndef OSISPLAIN_H
#define OSISPLAIN_H

#include "../swbasicfilter.h"

#include "../utilxml.h"


namespace swordxx {

/** this filter converts OSIS text to plain text
 */
class SWDLLEXPORT OSISPlain : public SWBasicFilter {
public:
protected:

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

    bool processStage(char stage,
                      std::string & text,
                      char *& from,
                      BasicFilterUserData * userData) override;

public:
    OSISPlain();
};

} /* namespace swordxx */
#endif
