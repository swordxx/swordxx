/******************************************************************************
 *
 *  thmlxhtml.h -    Implementation of ThMLXHTML
 *
 * $Id$
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef _THMLXHTML_H
#define _THMLXHTML_H

#include "../swbasicfilter.h"

#include "../utilxml.h"


namespace swordxx {

/** this filter converts ThML text to classed XHTML
 */
class SWDLLEXPORT ThMLXHTML : public SWBasicFilter {
    std::string imgPrefix;
    bool renderNoteNumbers;
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        MyUserData(const SWModule *module, const SWKey *key);//: BasicFilterUserData(module, key) {}
        bool inscriptRef;
        char sectionHeaderLevel;
        bool isBiblicalText;
        std::string version;
        XMLTag startTag;
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    ThMLXHTML();
    virtual const char *getImagePrefix() { return imgPrefix.c_str(); }
    virtual void setImagePrefix(const char *newImgPrefix) { imgPrefix = newImgPrefix; }
    char const * getHeader() const override;
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
};

} /* namespace swordxx */

#endif
