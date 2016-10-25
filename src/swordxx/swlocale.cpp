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
    p = new Private;
    ConfigEntMap::iterator confEntry;

    name           = nullptr;
    description    = nullptr;
    encoding       = nullptr;
    bookAbbrevs    = nullptr;
    bookLongNames  = nullptr;
    bookPrefAbbrev = nullptr;
    if (ifilename) {
        localeSource   = new SWConfig(ifilename);
    }
    else {
        localeSource   = new SWConfig();
        (*localeSource)["Meta"]["Name"] = DEFAULT_LOCALE_NAME;
        (*localeSource)["Meta"]["Description"] = "English (US)";
        bookAbbrevs = (struct abbrev *)builtin_abbrevs;
        for (abbrevsCnt = 0; builtin_abbrevs[abbrevsCnt].osis[0]; abbrevsCnt++);
    }

    confEntry = localeSource->sections()["Meta"].find("Name");
    if (confEntry != localeSource->sections()["Meta"].end())
        stdstr(&name, (*confEntry).second.c_str());

    confEntry = localeSource->sections()["Meta"].find("Description");
    if (confEntry != localeSource->sections()["Meta"].end())
        stdstr(&description, (*confEntry).second.c_str());

    confEntry = localeSource->sections()["Meta"].find("Encoding"); //Either empty (==Latin1) or UTF-8
    if (confEntry != localeSource->sections()["Meta"].end())
        stdstr(&encoding, (*confEntry).second.c_str());
}


SWLocale::~SWLocale() {

    delete localeSource;
    delete[] encoding;
    delete[] description;
    delete[] name;

    if (bookAbbrevs != builtin_abbrevs)
        delete [] bookAbbrevs;

    delete p;
}


std::string const & SWLocale::translate(const char *text) {
    LookupMap::iterator entry;

    entry = p->lookupTable.find(text);

    if (entry == p->lookupTable.end()) {
        ConfigEntMap::iterator confEntry;
        confEntry = localeSource->sections()["Text"].find(text);
        if (confEntry == localeSource->sections()["Text"].end())
            p->lookupTable.insert(LookupMap::value_type(text, text));
        else {//valid value found
            /*
            - If Encoding==Latin1 and we have a StringHelper, convert to UTF-8
            - If StringHelper present and Encoding is UTF-8, use UTF8
            - If StringHelper not present and Latin1, use Latin1
            - If StringHelper not present and UTF-8, no idea what to do. Should't happen
            */
/*            if (StringHelper::getSystemStringHelper()) {
                if (!strcmp(encoding, "UTF-8")) {
                    p->lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
                }
                else { //latin1 expected, convert to UTF-8
                    std::string t((*confEntry).second);
                    t = StringHelper::getSystemStringHelper()->latin2unicode( t );

                    p->lookupTable.insert(LookupMap::value_type(text, t.c_str()));
                }
            }
            else { //no stringhelper, just insert. Nothing we can do*/
                p->lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
//             }

        }
        entry = p->lookupTable.find(text);
    }
    return (*entry).second;
}


const char *SWLocale::getName() {
    return name;
}


const char *SWLocale::getDescription() {
    return description;
}


const char *SWLocale::getEncoding() {
    return encoding;
}


void SWLocale::augment(SWLocale &addFrom) {
    localeSource->augment(*addFrom.localeSource);
}


const struct abbrev *SWLocale::getBookAbbrevs(int *retSize) {
    static const char *nullstr = "";
    if (!bookAbbrevs) {
        // Assure all english abbrevs are present
        for (int j = 0; builtin_abbrevs[j].osis[0]; j++) {
            p->mergedAbbrevs[builtin_abbrevs[j].ab] = builtin_abbrevs[j].osis;
        }
        ConfigEntMap::iterator it = localeSource->sections()["Book Abbrevs"].begin();
        ConfigEntMap::iterator end = localeSource->sections()["Book Abbrevs"].end();
        for (; it != end; it++) {
            p->mergedAbbrevs[it->first.c_str()] = it->second.c_str();
        }
        int size = p->mergedAbbrevs.size();
        bookAbbrevs = new struct abbrev[size + 1];
        int i = 0;
        for (LookupMap::iterator it = p->mergedAbbrevs.begin(); it != p->mergedAbbrevs.end(); it++, i++) {
            bookAbbrevs[i].ab = it->first.c_str();
            bookAbbrevs[i].osis = it->second.c_str();
        }

        bookAbbrevs[i].ab = nullstr;
        bookAbbrevs[i].osis = nullstr;
        abbrevsCnt = size;
    }

    *retSize = abbrevsCnt;
    return bookAbbrevs;
}


} /* namespace swordxx */

