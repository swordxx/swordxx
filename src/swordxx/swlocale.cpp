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
    {
        std::shared_ptr<ConfigEntMap> abbrevsMap;
        if (ifilename) {
            localeSource = std::make_unique<SWConfig>(ifilename);

            /* Build abbreviations map from locale,  */
            abbrevsMap = std::make_shared<ConfigEntMap>(
                             (*localeSource)["Book Abbrevs"]);
        } else {
            localeSource = std::make_unique<SWConfig>();
            auto & meta = (*localeSource)["Meta"];
            meta["Name"] = DEFAULT_LOCALE_NAME;
            meta["Description"] = "English (US)";

            // Use english abbrevs by default:
            abbrevsMap = std::make_shared<ConfigEntMap>();
        }

        // Append the built-in (English) abbreviations to the end:
        for (std::size_t j = 0; builtin_abbrevs[j].osis[0]; ++j)
            abbrevsMap->emplace(builtin_abbrevs[j].ab, builtin_abbrevs[j].osis);
        m_bookAbbrevs = std::move(abbrevsMap);
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
    // Texts in the locale augmenting this one override the texts of this locale
    for (auto const & vp : addFrom.m_textTranslations)
        m_textTranslations[vp.first] = vp.second;
}

} /* namespace swordxx */
