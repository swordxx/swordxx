/******************************************************************************
 *
 *  gbfhtmlhref.h -    Implementation of GBFHTMLHREF
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

#ifndef GBFHTMLHREF_H
#define GBFHTMLHREF_H

#include "../swbasicfilter.h"


namespace swordxx {

/** this filter converts GBF  text to HTML text with hrefs
 */
class SWDLLEXPORT GBFHTMLHREF : public SWBasicFilter {
    bool renderNoteNumbers;
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        MyUserData(const SWModule *module, const SWKey *key);
        bool hasFootnotePreTag;
        std::string version;
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    GBFHTMLHREF();
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
};

} /* namespace swordxx */
#endif
