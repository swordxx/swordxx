/******************************************************************************
 *
 *  swlocale.cpp -    implementation of Class SWLocale used for retrieval
 *            of locale lookups
 *
 * $Id$
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swlocale.h"

#include <map>
#include "keys/versekey.h"
#include "swconfig.h"
#include "utilstr.h"
#include "versificationmgr.h"


namespace swordxx {


namespace {
    typedef std::map < std::string, std::string, std::less < std::string > >LookupMap;
}


const char *SWLocale::DEFAULT_LOCALE_NAME="en";


// I hate bridge patterns, but this hides swconfig and map from lots o stuff
class SWLocale::Private {
public:
    LookupMap lookupTable;
    LookupMap mergedAbbrevs;
};


SWLocale::SWLocale(const char *ifilename) {
    m_p = new Private;

    m_bookLongNames  = nullptr;
    m_bookPrefAbbrev = nullptr;
    if (ifilename) {
        m_localeSource   = new SWConfig(ifilename);

        // Assure all english abbrevs are present
        for (int j = 0; builtin_abbrevs[j].osis[0]; j++) {
            m_p->mergedAbbrevs[builtin_abbrevs[j].ab] = builtin_abbrevs[j].osis;
        }
        ConfigEntMap::iterator it = m_localeSource->sections()["Book Abbrevs"].begin();
        ConfigEntMap::iterator end = m_localeSource->sections()["Book Abbrevs"].end();
        for (; it != end; it++) {
            m_p->mergedAbbrevs[it->first.c_str()] = it->second.c_str();
        }
        auto const size = m_p->mergedAbbrevs.size();
        m_bookAbbrevs = std::make_unique<struct abbrev[]>(size + 1);
        int i = 0;
        for (LookupMap::iterator it = m_p->mergedAbbrevs.begin(); it != m_p->mergedAbbrevs.end(); it++, i++) {
            m_bookAbbrevs[i].ab = it->first.c_str();
            m_bookAbbrevs[i].osis = it->second.c_str();
        }

        m_bookAbbrevs[i].ab = "";
        m_bookAbbrevs[i].osis = "";
        m_abbrevsCnt = size;
    }
    else {
        m_localeSource   = new SWConfig();
        (*m_localeSource)["Meta"]["Name"] = DEFAULT_LOCALE_NAME;
        (*m_localeSource)["Meta"]["Description"] = "English (US)";
        for (m_abbrevsCnt = 0; builtin_abbrevs[m_abbrevsCnt].osis[0]; m_abbrevsCnt++);
    }

    auto const & sections = m_localeSource->sections();
    auto const metaSectionIt(sections.find("Meta"));
    if (metaSectionIt != sections.end()) {
        auto const & metaSection = metaSectionIt->second;
        auto const maybeGetEntry =
                [&metaSection](std::string & field, char const * const key) {
                    auto const entry(metaSection.find(key));
                    if (entry != metaSection.end())
                        field = entry->second;
                };
        maybeGetEntry(m_name, "Name");
        maybeGetEntry(m_description, "Description");
        maybeGetEntry(m_encoding, "Encoding");
    }
}


SWLocale::~SWLocale() {
    delete m_localeSource;
    delete m_p;
}


std::string const & SWLocale::translate(const char *text) {
    LookupMap::iterator entry;

    entry = m_p->lookupTable.find(text);

    if (entry == m_p->lookupTable.end()) {
        ConfigEntMap::iterator confEntry;
        confEntry = m_localeSource->sections()["Text"].find(text);
        if (confEntry == m_localeSource->sections()["Text"].end())
            m_p->lookupTable.insert(LookupMap::value_type(text, text));
        else {//valid value found
            /*
            - If Encoding==Latin1 and we have a StringHelper, convert to UTF-8
            - If StringHelper present and Encoding is UTF-8, use UTF8
            - If StringHelper not present and Latin1, use Latin1
            - If StringHelper not present and UTF-8, no idea what to do. Should't happen
            */
/*            if (StringHelper::getSystemStringHelper()) {
                if (!std::strcmp(encoding, "UTF-8")) {
                    p->lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
                }
                else { //latin1 expected, convert to UTF-8
                    std::string t((*confEntry).second);
                    t = StringHelper::getSystemStringHelper()->latin2unicode( t );

                    p->lookupTable.insert(LookupMap::value_type(text, t.c_str()));
                }
            }
            else { //no stringhelper, just insert. Nothing we can do*/
                m_p->lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
//             }

        }
        entry = m_p->lookupTable.find(text);
    }
    return (*entry).second;
}

void SWLocale::augment(SWLocale &addFrom) {
    m_localeSource->augment(*addFrom.m_localeSource);
}

struct abbrev const * SWLocale::getBookAbbrevs(std::size_t * retSize) {
    if (retSize)
        (*retSize) = m_abbrevsCnt;
    return m_bookAbbrevs ? m_bookAbbrevs.get() : builtin_abbrevs;
}

} /* namespace swordxx */
