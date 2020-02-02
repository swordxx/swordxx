/******************************************************************************
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_GBFHTML_H
#define SWORDXX_GBFHTML_H

#include "../swbasicfilter.h"


namespace swordxx {

/** this filter converts GBF  text to HTML text
 */
class SWDLLEXPORT GBFHTML : public SWBasicFilter {
protected:
    class MyUserData : public BasicFilterUserData {
    public:
        using BasicFilterUserData::BasicFilterUserData;
        ~MyUserData() noexcept override;
        bool hasFootnotePreTag;
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    GBFHTML();
};

} /* namespace swordxx */

#endif /* SWORDXX_GBFHTML_H */
