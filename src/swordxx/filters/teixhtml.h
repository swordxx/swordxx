/******************************************************************************
 *
 * Copyright 2012-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_TEIXHTML_H
#define SWORDXX_TEIXHTML_H

#include "../swbasicfilter.h"


namespace swordxx {

/** this filter converts TEI text to XHTML text
 */
class SWDLLEXPORT TEIXHTML : public SWBasicFilter {
private:
    bool renderNoteNumbers;

protected:
    class MyUserData : public BasicFilterUserData {
    public:
        bool isBiblicalText;
        std::string lastHi;

        std::string version;
        MyUserData(const SWModule *module, const SWKey *key);
        ~MyUserData() noexcept override;
    };

    std::unique_ptr<BasicFilterUserData> createUserData(
            SWModule const * module,
            SWKey const * key) override;

    bool handleToken(std::string & buf,
                     char const * token,
                     BasicFilterUserData * userData) override;

public:
    TEIXHTML();
    void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
    char const * getHeader() const override;
};

} /* namespace swordxx */

#endif /* SWORDXX_TEIXHTML_H */
