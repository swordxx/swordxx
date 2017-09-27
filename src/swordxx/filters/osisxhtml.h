/******************************************************************************
 *
 *  osisxhtml.h -    Render filter for classed XHTML of an OSIS module
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

#ifndef OSISXHTML_H
#define OSISXHTML_H

#include "../swbasicfilter.h"

#include <stack>
#include <string>


namespace swordxx {

/** this filter converts OSIS text to classed XHTML
 */
class SWDLLEXPORT OSISXHTML : public SWBasicFilter {
private:
    bool morphFirst;
    bool renderNoteNumbers;
protected:

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

    // used by derived classes so we have it in the header
    class MyUserData : public BasicFilterUserData {

    public: /* Types: */

        using TagStack = std::stack<std::string>;

    public: /* Methods: */

        MyUserData(SWModule const * module, SWKey const * key);
        ~MyUserData();
        void outputNewline(std::string &buf);

    public: /* Fields: */

        bool osisQToTick;
        bool inXRefNote;
        bool BiblicalText;
        int suspendLevel;
        std::string wordsOfChristStart;
        std::string wordsOfChristEnd;
        std::string interModuleLinkStart;
        std::string interModuleLinkEnd;
        TagStack quoteStack;
        TagStack hiStack;
        TagStack titleStack;
        TagStack lineStack;
        int consecutiveNewlines;
        std::string lastTransChange;
        std::string w;
        std::string fn;
        std::string version;

    };
public:
    OSISXHTML();
    void setMorphFirst(bool val = true) { morphFirst = val; }
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }

    char const * getHeader() const override;

};

} /* namespace swordxx */
#endif
