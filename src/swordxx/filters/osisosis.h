/******************************************************************************
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

#ifndef SWORDXX_OSISOSIS_H
#define SWORDXX_OSISOSIS_H

#include "../swbasicfilter.h"

#include "../utilxml.h"


namespace swordxx {

/** this filter converts internal OSIS text to public OSIS text
 */
class SWDLLEXPORT OSISOSIS : public SWBasicFilter {
private:
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        bool osisQToTick;
        std::string lastTransChange;
        std::string w;
        std::string fn;
        XMLTag startTag;
        MyUserData(const SWModule *module, const SWKey *key);
        ~MyUserData() noexcept override;
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    char processText(std::string & text,
                     SWKey const * key = nullptr,
                     SWModule const * module = nullptr) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    OSISOSIS();
};

} /* namespace swordxx */

#endif /* SWORDXX_OSISOSIS_H */
