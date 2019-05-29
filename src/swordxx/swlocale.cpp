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

#include "keys/versekey.h"
#include "swconfig.h"
#include "utilstr.h"
#include "versificationmgr.h"

namespace swordxx {

char const * SWLocale::DEFAULT_LOCALE_NAME = "en";

SWLocale::SWLocale(const char *ifilename) {
    std::unique_ptr<SWConfig> localeSource;
    if (ifilename) {
        localeSource = std::make_unique<SWConfig>(ifilename);

        /* Build abbreviations map from locale,  */
        m_bookAbbrevs = (*localeSource)["Book Abbrevs"];
    } else {
        localeSource = std::make_unique<SWConfig>();
        auto & meta = (*localeSource)["Meta"];
        meta["Name"] = DEFAULT_LOCALE_NAME;
        meta["Description"] = "English (US)";
    }

    auto const & sections = localeSource->sections();
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
    }

    // Add all text entries:
    for (auto & ep : (*localeSource)["Text"])
        m_textTranslations.emplace(ep.first, ep.second);
}

SWLocale::~SWLocale() noexcept = default;

std::string const & SWLocale::translate(std::string_view text) {
    auto const it(m_textTranslations.find(text));
    return (it != m_textTranslations.end()) ? it->second : std::string(text);
}

void SWLocale::augment(SWLocale const & addFrom) {
    /* Because the other locale augments this one, it makes sense to use the
       augmented name and description as well, which might contain information
       about the augmentation. */
    m_name = addFrom.m_name;
    m_description = addFrom.m_description;

    // Texts in the locale augmenting this one override the texts of this locale
    for (auto const & vp : addFrom.m_textTranslations)
        m_textTranslations[vp.first] = vp.second;

    // Book abbrevations are appended except the ones which would be duplicates:
    auto const bookAbbrevExists =
            [this](auto const & valuePair) {
                for (auto range(m_bookAbbrevs.equal_range(valuePair.first));
                     range.first != range.second;
                     ++range.first)
                    if (range.first->second == valuePair.second)
                        return true;
                return false;
            };
    for (auto const & vp : addFrom.m_bookAbbrevs)
        if (!bookAbbrevExists(vp))
            m_bookAbbrevs.emplace(vp);
}

ConfigEntMap const & SWLocale::builtinBookAbbreviations() {
    static ConfigEntMap const builtinAbbrevs(
                [](){
                    ConfigEntMap abbrevsMap;
                    for (std::size_t j = 0; builtin_abbrevs[j].osis[0]; ++j)
                        abbrevsMap.emplace(builtin_abbrevs[j].ab,
                                           builtin_abbrevs[j].osis);
                    return abbrevsMap;
                }());
    return builtinAbbrevs;

}

} /* namespace swordxx */
