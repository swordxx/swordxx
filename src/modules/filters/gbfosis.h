/***************************************************************************
 *
 *  gbfosis.h -    Implementation of GBFOSIS
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef GBFOSIS_H
#define GBFOSIS_H

#include "../../swfilter.h"
#include <stack>
#include <string>


namespace swordxx {


class SWDLLEXPORT QuoteStack {
private:
    class QuoteInstance {
    public:
        char startChar;
        char level;
        std::string uniqueID;
        char continueCount;
        QuoteInstance(char startChar = '\"', char level = 1, std::string uniqueID = "", char continueCount = 0) {
            this->startChar     = startChar;
            this->level         = level;
            this->uniqueID      = uniqueID;
            this->continueCount = continueCount;
        }
        void pushStartStream(std::string &text);
    };

    typedef std::stack<QuoteInstance> QuoteInstanceStack;
    QuoteInstanceStack quotes;
public:
    QuoteStack();
    virtual ~QuoteStack();
    void handleQuote(char *buf, char *quotePos, std::string &text);
    void clear();
    bool empty() { return quotes.empty(); }
};

/** this filter converts GBF text to OSIS text
 */
class SWDLLEXPORT GBFOSIS : public SWFilter {
public:
    GBFOSIS();
    virtual ~GBFOSIS();
    char processText(std::string &text, const SWKey *key = 0, const SWModule *module = 0);
};

} /* namespace swordxx */
#endif /* THMLOSIS_H */
