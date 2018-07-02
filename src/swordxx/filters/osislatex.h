/******************************************************************************
 *
 *  osislatex.h -    Render filter for LaTeX of an OSIS module
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

#ifndef SWORDXX_OSISLATEX_H
#define SWORDXX_OSISLATEX_H

#include "../swbasicfilter.h"

#include <stack>
#include <string>


namespace swordxx {

/** this filter converts OSIS text to classed XHTML
 */
class SWDLLEXPORT OSISLaTeX : public SWBasicFilter {
private:
    bool morphFirst;
    bool renderNoteNumbers;
protected:
    // used by derived classes so we have it in the header

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

    class MyUserData: public BasicFilterUserData {

    public: /* Types: */

        using TagStack = std::stack<std::string>;

    public: /* Methods: */

        MyUserData(SWModule const * module, SWKey const * key);
        ~MyUserData();
        void outputNewline(std::string &buf);

    public: /* Fields: */

        bool osisQToTick;
        bool inXRefNote;
        int suspendLevel;
        bool firstCell; // for tables, indicates whether a cell is the first one in a row
        std::string wordsOfChristStart;
        std::string wordsOfChristEnd;
        std::string divLevel;  // divLevel "module", "testament, "bookgroup", "book", "majorsection", "section", "paragraph" , ignore others.

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
    OSISLaTeX();
    void setMorphFirst(bool val = true) { morphFirst = val; }
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
    char const * getHeader() const override;
};

} /* namespace swordxx */

#endif /* SWORDXX_OSISLATEX_H */
