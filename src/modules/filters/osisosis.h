/******************************************************************************
 *
 *  osisosis.h -    Implementation of OSISOSIS
 *
 * $Id$
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef OSISOSIS_H
#define OSISOSIS_H

#include "swbasicfilter.h"

#include "../../utilfuns/utilxml.h"


namespace swordxx {

/** this filter converts internal OSIS text to public OSIS text
 */
class SWDLLEXPORT OSISOSIS : public SWBasicFilter {
private:
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        bool osisQToTick;
        bool inBold;
        std::string lastTransChange;
        std::string w;
        std::string fn;
        XMLTag startTag;
        MyUserData(const SWModule *module, const SWKey *key);
    };
    virtual BasicFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
        return new MyUserData(module, key);
    }
    virtual char processText(std::string &text, const SWKey *key, const SWModule *module);
    virtual bool handleToken(std::string &buf, const char *token, BasicFilterUserData *userData);
public:
    OSISOSIS();
};

} /* namespace swordxx */
#endif
