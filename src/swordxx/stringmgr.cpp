/******************************************************************************
 *
 *  stringmgr.cpp -    implementation of class StringMgr
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

#include "stringmgr.h"

#include <cassert>
#include <limits>
#include <unicode/locid.h>
#include <unicode/translit.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>
#include "localemgr.h"
#include "utilstr.h"


namespace swordxx {


std::unique_ptr<StringMgr> StringMgr::systemStringMgr;

//here comes our ICUStringMgr reimplementation
class ICUStringMgr : public StringMgr {
public:
    void upperUTF8(std::string & str) const override;

};


/** Default constructor
*/
StringMgr::StringMgr() {
}

/** Copy constructor
*/
StringMgr::StringMgr(const StringMgr & /* m */) {
}

StringMgr::~StringMgr() noexcept = default;

/** Sets the global StringMgr handle
* @param newStringMgr The new global StringMgr. This pointer will be deleted by this StringMgr
*/
void StringMgr::setSystemStringMgr(StringMgr *newStringMgr) {
    systemStringMgr.reset(newStringMgr);

   // TODO: this is magic. apparently we have to reset the system localemgr upon changing stringmgr.
   // setting system stringmgr should be set before localemgr and not possible to change.
   // rework this design.
    LocaleMgr::getSystemLocaleMgr()->setSystemLocaleMgr(
                std::make_shared<LocaleMgr>());
}

/** Returns the global StringMgr handle
* @return The global string handle
*/
StringMgr* StringMgr::getSystemStringMgr() {
    if (!systemStringMgr) {
        systemStringMgr = std::make_unique<ICUStringMgr>();
    }

    return systemStringMgr.get();
}

void ICUStringMgr::upperUTF8(std::string & str) const {
    if (str.empty())
        return;

    static_assert(std::numeric_limits<int32_t>::max()
                  <= std::numeric_limits<std::size_t>::max(), "");

    int32_t lcSizeInUtf16 = 0;
    UErrorCode err = U_ZERO_ERROR;
    u_strFromUTF8(nullptr, 0, &lcSizeInUtf16, str.c_str(), str.size(), &err);
    if ((err != U_BUFFER_OVERFLOW_ERROR) && (err != U_ZERO_ERROR))
        return;
    assert(lcSizeInUtf16 >= 0);
    auto lcInUTF16(std::make_unique<UChar[]>(
                       static_cast<std::size_t>(lcSizeInUtf16)));
    err = U_ZERO_ERROR;
    u_strFromUTF8(lcInUTF16.get(), lcSizeInUtf16, nullptr, str.c_str(), str.size(), &err);
    if ((err != U_STRING_NOT_TERMINATED_WARNING) && (err != U_ZERO_ERROR))
        return;

    err = U_ZERO_ERROR;
    auto const ucSizeInUtf16 =
            u_strToUpper(nullptr, 0, lcInUTF16.get(), lcSizeInUtf16, nullptr, &err);
    if ((err != U_BUFFER_OVERFLOW_ERROR) && (err != U_ZERO_ERROR))
        return;
    assert(ucSizeInUtf16 >= 0);
    auto ucInUTF16(std::make_unique<UChar[]>(
                       static_cast<std::size_t>(ucSizeInUtf16)));
    err = U_ZERO_ERROR;
    u_strToUpper(ucInUTF16.get(), ucSizeInUtf16, lcInUTF16.get(), lcSizeInUtf16, nullptr, &err);
    if ((err != U_STRING_NOT_TERMINATED_WARNING) && (err != U_ZERO_ERROR))
        return;
    lcInUTF16.reset();

    int32_t lcSizeInUtf8 = 0;
    err = U_ZERO_ERROR;
    u_strToUTF8(nullptr, 0, &lcSizeInUtf8, ucInUTF16.get(), ucSizeInUtf16, &err);
    if ((err != U_BUFFER_OVERFLOW_ERROR) && (err != U_ZERO_ERROR))
        return;
    auto lcInUTF8(std::make_unique<char[]>(
                      static_cast<std::size_t>(lcSizeInUtf8)));

    err = U_ZERO_ERROR;
    u_strToUTF8(lcInUTF8.get(), lcSizeInUtf8, nullptr, ucInUTF16.get(), ucSizeInUtf16, &err);
    if ((err != U_STRING_NOT_TERMINATED_WARNING) && (err != U_ZERO_ERROR))
        return;
    ucInUTF16.reset();
    str.assign(lcInUTF8.get(), static_cast<std::size_t>(lcSizeInUtf8));
}

} /* namespace swordxx */
