/******************************************************************************
 *
 *  teiplain.h -    Implementation of TEIPlain
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef TEIPLAIN_H
#define TEIPLAIN_H

#include "../swbasicfilter.h"

#include "../utilxml.h"


namespace swordxx {

/** this filter converts TEI text to plain text
 */
class SWDLLEXPORT TEIPlain : public SWBasicFilter {
public:
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        std::string w;
        XMLTag tag;
        MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {}
    };
    virtual BasicFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
        return new MyUserData(module, key);
    }
    virtual bool handleToken(std::string &buf, const char *token, BasicFilterUserData *userData);
public:
    TEIPlain();
};

} /* namespace swordxx */
#endif
