/******************************************************************************
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstring>
#include <iostream>
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>
#include <swordxx/swlocale.h>
#include <swordxx/swlog.h>
#include <swordxx/utilstr.h>

using namespace swordxx;

int main(int argc, char **argv) {
    if ((argc < 2) || (argc > 8)) {
        std::cerr << "usage: " << *argv << " <\"string to parse\"> [locale_name] [v11n] [context] [echo params 1|0] [test-in-set-verse 1|0] [intros 1|0]\n";
        std::exit(-1);
    }

    if (argc > 2)
        LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[2]);

    VerseKey DefaultVSKey;
//    DefaultVSKey.AutoNormalize(0);

    if (argc > 3) {
        if (auto v11n = VersificationMgr::systemVersificationMgr()->getVersificationSystem(argv[3])) {
            DefaultVSKey.setVersificationSystem(std::move(v11n));
        } else {
            std::cerr << "error: " << *argv
                      << ": Requested versification system not found: "
                      << argv[3] << std::endl;
            return -1;
        }
    }

    const char *context = (argc > 4) ? argv[4] : "gen.1.1";

    bool const echo = (argc > 5) && !std::strcmp(argv[5], "1");
    bool const inSetTest = (argc > 6) && !std::strcmp(argv[6], "1");
    bool const intros = (argc > 7) && !std::strcmp(argv[7], "1");

    DefaultVSKey.setIntros(intros);

    SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
    {
        auto const locale(DefaultVSKey.locale());
        auto const refSys(DefaultVSKey.versificationSystem());
        auto const & books = refSys->books();
        for (std::size_t i = 0; i < books.size(); ++i) {
            std::string abbr(locale->translateText(books[i].getLongName()));
            trimString(abbr);
            std::optional<std::size_t> bn;
            {
                auto iabbr(trimmedView(abbr));
                if (!iabbr.empty()) {
                    /* The first iteration of the following loop tries to uppercase the string.
                       If we still fail to match, we try matching the string without any toupper
                       logic. This is useful for, say, Chinese user input on a system that
                       doesn't properly support a true Unicode-toupper function. */
                    auto const upperAbbr(utf8ToUpper(iabbr));
                    auto const scanAbbreviations =
                        [refSys_ = refSys.get()](ConfigEntMap const & abbrevs, std::string_view abbr_) {
                            assert(!abbr_.empty());
                            for (auto it = abbrevs.lower_bound(abbr_); it != abbrevs.end(); ++it)
                            {
                                auto const & foundAbbr = it->first;
                                if (foundAbbr.size() < abbr_.size())
                                    break;
                                if (std::memcmp(abbr_.data(), foundAbbr.c_str(), abbr_.size()))
                                    break;
                                auto const & osis = it->second;
                                if (auto r = refSys_->bookNumberByOSISName(osis.c_str()))
                                    return r;
                            }
                            return std::optional<std::size_t>();
                        };
                    std::string_view abbr_(upperAbbr);
                    for (int i_ = 0; i_ < 2; i_++, abbr_ = iabbr) {
                        if ((bn = scanAbbreviations(locale->bookAbbreviations(), abbr_)))
                            break;
                        if ((bn = scanAbbreviations(SWLocale::builtinBookAbbreviations(), abbr_)))
                            break;
                    }
                }
            }
            // auto const bn = DefaultVSKey.bookFromAbbrev(abbr);
            if (!bn || (bn.value() != i)) {
                if (bn) {
                    SWLog::getSystemLog()->logDebug("VerseKey::Book: %s does not have a matching toupper abbrevs entry! book number returned was: %zu, should be %zu. Required entry to add to locale:", abbr.c_str(), bn.value(), i);
                } else {
                    SWLog::getSystemLog()->logDebug("VerseKey::Book: %s does not have a matching toupper abbrevs entry! book number returned was: -1, should be %zu. Required entry to add to locale:", abbr.c_str(), i);
                }

                abbr = utf8ToUpper(abbr);
                SWLog::getSystemLog()->logDebug("%s=%s\n", abbr.c_str(), books[i].getOSISName().c_str());
            }
        }
    }

    DefaultVSKey.setText(context);

    ListKey verses = DefaultVSKey.parseVerseList(argv[1], DefaultVSKey.getText().c_str(), true);

    if (echo) {
        for (int i = 1; i < argc; i++) {
            if (i > 1) std::cout << " ";
            std::cout << argv[i];
        }
        std::cout << ": ";
    }
    std::cout << verses.getRangeText() << "\n";
/*
    for (int i = 0; i < verses.Count(); i++) {
        std::cout << verses.GetElement(i)->getRangeText() << "\n";
    }
*/

    if (inSetTest) {
        verses.setText(context);
        std::cout << "Verse is" << ((verses.popError()) ? " NOT" : "") << " in set.\n\n";
    }

    return 0;
}
