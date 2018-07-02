/******************************************************************************
 *
 *  thmllatex.h -    Implementation of ThMLLaTeX
 *
 * $Id$
 *
 * Copyright 2011-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_THMLLATEX_H
#define SWORDXX_THMLLATEX_H

#include "../swbasicfilter.h"

#include "../utilxml.h"


namespace swordxx {

/** this filter converts ThML text to LaTeX
 */
class SWDLLEXPORT ThMLLaTeX : public SWBasicFilter {
    std::string imgPrefix;
    bool renderNoteNumbers;
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        MyUserData(const SWModule *module, const SWKey *key);//: BasicFilterUserData(module, key) {}
        ~MyUserData() noexcept override;
        bool isSecHead = false;
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
    ThMLLaTeX();
    virtual const char *getImagePrefix() { return imgPrefix.c_str(); }
    virtual void setImagePrefix(const char *newImgPrefix) { imgPrefix = newImgPrefix; }
    char const * getHeader() const override;
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
};

} /* namespace swordxx */

#endif /* SWORDXX_THMLLATEX_H */
