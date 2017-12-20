/******************************************************************************
 *
 *  osishtmlhref.h -    Implementation of OSISHTMLHREF
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

#ifndef OSISHTMLHREF_H
#define OSISHTMLHREF_H

#include "../swbasicfilter.h"

#include <stack>
#include <string>


namespace swordxx {

/** this filter converts OSIS text to HTML text with hrefs
 */
class SWDLLEXPORT OSISHTMLHREF : public SWBasicFilter {
private:
    bool morphFirst;
    bool renderNoteNumbers;
protected:
    using TagStack = std::stack<std::string>;
    // used by derived classes so we have it in the header
    struct TagStacks {
        TagStack quoteStack;
        TagStack hiStack;
    };
    class SWDLLEXPORT MyUserData : public BasicFilterUserData {
    public:
        bool osisQToTick;
        bool inBold;    // TODO: obsolete. left for binary compat for 1.6.x
        bool inXRefNote;
        bool isBiblicalText;
        int suspendLevel;
        std::string wordsOfChristStart;
        std::string wordsOfChristEnd;
        TagStacks tagStacks;
        std::string lastTransChange;
        std::string w;
        std::string fn;
        std::string version;
        MyUserData(const SWModule *module, const SWKey *key);
        ~MyUserData();
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    OSISHTMLHREF();
    void setMorphFirst(bool val = true) { morphFirst = val; }
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
};

} /* namespace swordxx */
#endif
