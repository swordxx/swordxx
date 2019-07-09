/******************************************************************************
 *
 *  utf8transliterator.h -    Implementation of UTF8Transliterator
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_UTF8TRANSLITERATOR_H
#define SWORDXX_UTF8TRANSLITERATOR_H

#include "../swoptfilter.h"

#include <map>
#include <memory>
#include <list>
#include <string>
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include "../defs.h"


namespace swordxx {

class SWModule;

struct SWTransData {
    icu::UnicodeString resource;
    UTransDirection dir;
};
using SWTransMap = std::map<icu::UnicodeString const, SWTransData>;
using SWTransPair = std::pair<icu::UnicodeString, SWTransData>;

/** This Filter uses ICU for transliteration
*/
class SWDLLEXPORT UTF8Transliterator : public SWOptionFilter {
private:

    bool addTrans(const char* newTrans, std::string* transList);
    std::unique_ptr<icu::Transliterator> createTrans(
            icu::UnicodeString const & ID,
            UTransDirection dir,
            UErrorCode & status);

public:
    UTF8Transliterator();
    ~UTF8Transliterator() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_UTF8TRANSLITERATOR_H */
