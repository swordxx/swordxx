/******************************************************************************
 *
 *  swmodule.cpp -    code for base class 'SWModule'. SWModule is the basis
 *            for all types of modules (e.g. texts, commentaries,
 *            maps, lexicons, etc.)
 *
 * $Id$
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

#include "swmodule.h"

#include <CLucene.h>
#include <cstdint>
#ifndef _MSC_VER
#include <iostream>
#endif
#include <memory>
#ifdef USECXX11REGEX
#include <regex>
#else
#include <regex.h>    // GNU
#endif
#include <vector>
#include "filemgr.h"
#include "keys/treekeyidx.h" // KLUDGE for Search
#include "keys/versekey.h"   // KLUDGE for Search
#include "stringmgr.h"
#include "swfilter.h"
#include "swlog.h"
#include "swoptfilter.h"
#include "sysdata.h"
#include "utilstr.h"


#if defined(USECXX11REGEX) && !defined(REG_ICASE)
#define REG_ICASE std::regex::icase
#endif

namespace {

void prepareText(std::string &buf) {
    decltype(buf.size()) to, from;
    char space = 0, cr = 0, realdata = 0, nlcnt = 0;
    for (to = from = 0u; buf[from]; from++) {
        switch (buf[from]) {
        case 10:
            if (!realdata)
                continue;
            space = (cr) ? 0 : 1;
            cr = 0;
            nlcnt++;
            if (nlcnt > 1) {
//                *to++ = nl;
                buf[to++] = 10;
//                *to++ = nl[1];
//                nlcnt = 0;
            }
            continue;
        case 13:
            if (!realdata)
                continue;
//            *to++ = nl[0];
            buf[to++] = 10;
            space = 0;
            cr = 1;
            continue;
        }
        realdata = 1;
        nlcnt = 0;
        if (space) {
            space = 0;
            if (buf[from] != ' ') {
                buf[to++] = ' ';
                from--;
                continue;
            }
        }
        buf[to++] = buf[from];
    }
    buf.resize(to);
    while (to > 1) {            // remove trailing excess
        to--;
        if ((buf[to] == 10) || (buf[to] == ' '))
            buf.resize(to);
        else break;
    }
}

} // anonymous namespace

namespace swordxx {

/******************************************************************************
 * SWModule Constructor - Initializes data for instance of SWModule
 *
 * ENT:    imodname - Internal name for module
 *    imoddesc - Name to display to user for module
 *    idisp     - Display object to use for displaying
 *    imodtype - Type of Module (All modules will be displayed with
 *            others of same type under their modtype heading
 *    unicode  - if this module is unicode
 */

SWModule::SWModule(std::unique_ptr<SWKey> key_,
                   const char * imodname,
                   const char * imoddesc,
                   const char * imodtype,
                   TextEncoding encoding,
                   SWTextDirection direction,
                   SWTextMarkup markup,
                   const char * imodlang)
    : m_moduleName(imodname ? imodname : "")
    , m_moduleDescription(imoddesc ? imoddesc : "")
    , m_moduleType(imodtype ? imodtype : "")
    , m_moduleLanguageName(imodlang ? imodlang : "")
    , m_textDirection(direction)
    , m_textMarkup(markup)
    , m_textEncoding(encoding)
    , key(key_.release())
{}


/******************************************************************************
 * SWModule Destructor - Cleans up instance of SWModule
 */

SWModule::~SWModule()
{
    if (key) {
        if (!key->isPersist())
            delete key;
    }

    m_entryAttributes.clear();
}


/******************************************************************************
 * SWModule::popError - Gets and clears error status
 *
 * RET:    error status
 */

char SWModule::popError()
{
    char retval = error;

    error = 0;
    if (!retval) retval = key->popError();
    return retval;
}


/******************************************************************************
 * SWModule::setKey - Sets a key to this module for position to a particular
 *            record
 *
 * ENT:    ikey - key with which to set this module
 *
 * RET:    error status
 */

char SWModule::setKey(const SWKey *ikey) {
    SWKey * oldKey = nullptr;

    if (key) {
        if (!key->isPersist())    // if we have our own copy
            oldKey = key;
    }

    if (!ikey->isPersist()) {        // if we are to keep our own copy
         key = createKey().release();
        key->positionFrom(*ikey);
    }
    else     key = (SWKey *)ikey;        // if we are to just point to an external key

    delete oldKey;

    return error = key->getError();
}

void SWModule::positionToTop() {
    key->positionToTop();
    char saveError = key->popError();
    this->increment();
    this->decrement();
    error = saveError;
}

void SWModule::positionToBottom() {
    key->positionToBottom();
    char saveError = key->popError();
    this->decrement();
    this->increment();
    error = saveError;
}


/******************************************************************************
 * SWModule::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void SWModule::increment(int steps) {
    key->increment(steps);
    error = key->popError();
}


/******************************************************************************
 * SWModule::decrement    - Decrements module key a number of entries
 *
 * ENT:    decrement    - Number of entries to jump backward
 *
 * RET: *this
 */

void SWModule::decrement(int steps) {
    key->decrement(steps);
    error = key->popError();
}


void SWModule::nullPercent(char, void *) {}

/******************************************************************************
 * SWModule::Search     - Searches a module for a string
 *
 * ENT:    istr        - string for which to search
 *     searchType    - type of search to perform
 *                >=0 - regex
 *                -1  - phrase
 *                -2  - multiword
 *                -3  - entryAttrib (eg. Word//Lemma./G1234/)     (Lemma with dot means check components (Lemma.[1-9]) also)
 *                -4  - clucene
 *     flags        - options flags for search
 *
 * RET: ListKey set to verses that contain istr
 */

ListKey &SWModule::search(char const * istr,
                          int searchType,
                          int flags,
                          SWKey * scope,
                          void (* percent)(char, void *),
                          void * percentUserData)
{
    m_listKey.clear();
    std::string term = istr;
    bool includeComponents = false;    // for entryAttrib e.g., /Lemma.1/

    SWKey * saveKey   = nullptr;
    auto resultKey(createKey());
    auto lastKey(createKey());
    std::string lastBuf = "";

#ifdef USECXX11REGEX
    std::locale oldLocale;
    std::locale::global(std::locale("en_US.UTF-8"));

    std::regex preg;
#else
    regex_t preg;
#endif

    std::vector<std::string> words;
    const char *sres;
    char perc = 1;
    bool savePEA = isProcessEntryAttributes();

    // determine if we might be doing special strip searches.  useful for knowing if we can use shortcuts
    bool specialStrips = (getConfigEntry("LocalStripFilter")
            || (getConfig().has("GlobalOptionFilter", "UTF8GreekAccents"))
            || (getConfig().has("GlobalOptionFilter", "UTF8HebrewPoints"))
            || (getConfig().has("GlobalOptionFilter", "UTF8ArabicPoints"))
            || (std::strchr(istr, '<')));

    setProcessEntryAttributes(searchType == -3);


    if (!key->isPersist()) {
        saveKey = createKey().release();
        saveKey->positionFrom(*key);
    }
    else    saveKey = key;

    std::unique_ptr<SWKey> searchKey;
    if (scope) {
        searchKey = scope->clone();
    } else if (key->isPersist()) {
        searchKey = key->clone();
    }
    if (searchKey) {
        searchKey->setPersist(true);
        setKey(*searchKey);
    }

    (*percent)(perc, percentUserData);

    positionToBottom();
    long highIndex = key->getIndex();
    if (!highIndex)
        highIndex = 1;        // avoid division by zero errors.
    positionToTop();
    if (searchType >= 0) {
#ifdef USECXX11REGEX
        preg = std::regex((std::string(".*")+istr+".*").c_str(), std::regex_constants::extended | searchType | flags);
#else
        flags |=searchType|REG_NOSUB|REG_EXTENDED;
        if (int const err = ::regcomp(&preg, istr, flags)) {
            auto const errorMessageSize = ::regerror(err, &preg, nullptr, 0u);
            auto const errorMessage(std::make_unique<char[]>(errorMessageSize));
            ::regerror(err, &preg, errorMessage.get(), errorMessageSize);
            SWLog::getSystemLog()->logError(
                        "Failed to compile regular expression: %s",
                        errorMessage.get());
            return m_listKey;
        }
#endif
    }

    (*percent)(++perc, percentUserData);


    (*percent)(10, percentUserData);
    if (searchType == -4) {    // indexed search

        lucene::index::IndexReader    * ir = nullptr;
        lucene::search::IndexSearcher * is = nullptr;
        lucene::search::Query         * q  = nullptr;
        lucene::search::Hits          * h  = nullptr;
        try {
            ir = lucene::index::IndexReader::open(searchIndexPath().c_str());
            is = new lucene::search::IndexSearcher(ir);
            static TCHAR const * stopWords[] = { nullptr };
            lucene::analysis::standard::StandardAnalyzer analyzer(stopWords);

            // parse the query
            q = lucene::queryParser::QueryParser::parse(
                    utf8ToWChar(istr).c_str(),
                    _T("content"),
                    &analyzer);
            (*percent)(20, percentUserData);

            // perform the search
            h = is->search(q);
            (*percent)(80, percentUserData);

            // iterate thru each good module position that meets the search
            bool checkBounds = getKey()->isBoundSet();
            for (unsigned long i = 0; i < (unsigned long)h->length(); i++) {
                lucene::document::Document &doc = h->doc(i);
                // set a temporary verse key to this module position
                resultKey->setText(wcharToUTF8(doc.get(_T("key")))); //TODO Does a key always accept utf8?
                uint64_t score = (uint64_t)((uint32_t)(h->score(i)*100));

                // check to see if it sets ok (within our bounds) and if not, skip
                if (checkBounds) {
                    getKey()->positionFrom(*resultKey);
                    if (*getKey() != *resultKey) {
                        continue;
                    }
                }
                m_listKey << *resultKey;
                m_listKey.getElement()->m_userData = score;
            }
            (*percent)(98, percentUserData);
        }
        catch (...) {
            q = nullptr;
            // invalid clucene query
        }
        delete h;
        delete q;

        delete is;
        if (ir) {
            ir->close();
        }
    }

    // some pre-loop processing
    switch (searchType) {

    // phrase
    case -1:
        // let's see if we're told to ignore case.  If so, then we'll touppstr our term
        if ((flags & REG_ICASE) == REG_ICASE) toupperstr(term);
        break;

    // multi-word
    case -2:
        // let's break the term down into our words vector
        while (1) {
            auto const r(getPrefixSize(term, ' '));
            if (!r.first) {
                words.push_back(term);
                break;
            }
            words.emplace_back(term.c_str(), r.second);
            term.erase(0u, r.second + 1u);
        }
        if ((flags & REG_ICASE) == REG_ICASE)
            for (auto & word : words)
                toupperstr(word);
        break;

    // entry attributes
    case -3:
        // let's break the attribute segs down.  We'll reuse our words vector for each segment
        while (1) {
            auto const r(getPrefixSize(term, '/'));
            if (!r.first) {
                words.push_back(term);
                break;
            }
            words.emplace_back(term.c_str(), r.second);
            term.erase(0u, r.second + 1u);
        }
        if ((words.size()>2) && (*words[2].rbegin() == '.')) {
            includeComponents = true;
            words[2].pop_back();
        }
        break;
    }


    // our main loop to iterate the module and find the stuff
    perc = 5;
    (*percent)(perc, percentUserData);


    while ((searchType != -4) && !popError()) {
        long mindex = key->getIndex();
        float per = (float)mindex / highIndex;
        per *= 93;
        per += 5;
        char newperc = (char)per;
        if (newperc > perc) {
            perc = newperc;
            (*percent)(perc, percentUserData);
        }
        else if (newperc < perc) {
#ifndef _MSC_VER
            std::cerr << "Serious error: new percentage complete is less than previous value\n";
            std::cerr << "index: " << (key->getIndex()) << "\n";
            std::cerr << "highIndex: " << highIndex << "\n";
            std::cerr << "newperc ==" << (int)newperc << "%" << "is smaller than\n";
            std::cerr << "perc == "  << (int )perc << "% \n";
#endif
        }
        if (searchType >= 0) {
            auto textBuf(stripText());
#ifdef USECXX11REGEX
            if (std::regex_match(std::string(textBuf.c_str()), preg)) {
#else
            if (!regexec(&preg, textBuf.c_str(), 0, nullptr, 0)) {
#endif
                resultKey->positionFrom(*getKey());
                resultKey->clearBound();
                m_listKey << *resultKey;
                lastBuf = "";
            }
#ifdef USECXX11REGEX
            else if (std::regex_match(std::string((lastBuf + ' ' + textBuf).c_str()), preg)) {
#else
            else if (!regexec(&preg, (lastBuf + ' ' + textBuf).c_str(), 0, nullptr, 0)) {
#endif
                lastKey->clearBound();
                m_listKey << *lastKey;
                lastBuf = textBuf;
            }
            else {
                lastBuf = textBuf;
            }
        }

        // phrase
        else {
            std::string textBuf;
            switch (searchType) {

            // phrase
            case -1:
                textBuf = stripText();
                if ((flags & REG_ICASE) == REG_ICASE) toupperstr(textBuf);
                sres = std::strstr(textBuf.c_str(), term.c_str());
                if (sres) { //it's also in the stripText(), so we have a valid search result item now
                    resultKey->positionFrom(*getKey());
                    resultKey->clearBound();
                    m_listKey << *resultKey;
                }
                break;

            // multiword
            case -2: { // enclose our allocations
                int loopCount = 0;
                decltype(words)::size_type foundWords = 0;
                do {
                    textBuf = ((loopCount == 0)&&(!specialStrips)) ? getRawEntry() : stripText();
                    foundWords = 0;

                    for (auto const & word : words) {
                        if ((flags & REG_ICASE) == REG_ICASE)
                            toupperstr(textBuf);
                        sres = std::strstr(textBuf.c_str(), word.c_str());
                        if (!sres)
                            break; //for loop
                        foundWords++;
                    }

                    loopCount++;
                } while ( (loopCount < 2) && (foundWords == words.size()));

                if ((loopCount == 2) && (foundWords == words.size())) { //we found the right words in both raw and stripped text, which means it's a valid result item
                    resultKey->positionFrom(*getKey());
                    resultKey->clearBound();
                    m_listKey << *resultKey;
                }
                }
                break;

            // entry attributes
            case -3: {
                renderText();    // force parse
                AttributeTypeList const & entryAttribs = getEntryAttributes();
                AttributeTypeList::const_iterator i1Start, i1End;
                AttributeList::const_iterator i2Start, i2End;
                AttributeValue::const_iterator i3Start, i3End;

                if ((words.size()) && (words[0].length())) {
                    i1Start = entryAttribs.find(words[0]);
                    i1End = i1Start;
                    if (i1End != entryAttribs.end()) {
                        i1End++;
                    }
                }
                else {
                    i1Start = entryAttribs.begin();
                    i1End   = entryAttribs.end();
                }
                for (;i1Start != i1End; i1Start++) {
                    if ((words.size()>1) && (words[1].length())) {
                        i2Start = i1Start->second.find(words[1]);
                        i2End = i2Start;
                        if (i2End != i1Start->second.end())
                            i2End++;
                    }
                    else {
                        i2Start = i1Start->second.begin();
                        i2End   = i1Start->second.end();
                    }
                    for (;i2Start != i2End; i2Start++) {
                        if ((words.size()>2) && (words[2].length()) && (!includeComponents)) {
                            i3Start = i2Start->second.find(words[2]);
                            i3End = i3Start;
                            if (i3End != i2Start->second.end())
                                i3End++;
                        }
                        else {
                            i3Start = i2Start->second.begin();
                            i3End   = i2Start->second.end();
                        }
                        for (;i3Start != i3End; i3Start++) {
                            if ((words.size()>3) && (words[3].length())) {
                                if (includeComponents) {
                                    std::string key2(i3Start->first);
                                    auto const sepPos = key2.find_first_of('.');
                                    if (sepPos != std::string::npos)
                                        key2.resize(sepPos);
                                    // we're iterating all 3 level keys, so be sure we match our
                                    // prefix (e.g., Lemma, Lemma.1, Lemma.2, etc.)
                                    if (key2 != words[2]) continue;
                                }
                                if (flags & SEARCHFLAG_MATCHWHOLEENTRY) {
                                    bool found = !(((flags & REG_ICASE) == REG_ICASE) ? !caseInsensitiveEquals(i3Start->second, words[3]) : (i3Start->second != words[3]));
                                    sres = found
                                           ? i3Start->second.c_str()
                                           : nullptr;
                                }
                                else {
                                    sres = ((flags & REG_ICASE) == REG_ICASE)
                                           ? stristr(i3Start->second, words[3])
                                           : std::strstr(i3Start->second.c_str(), words[3].c_str());
                                }
                                if (sres) {
                                    resultKey->positionFrom(*getKey());
                                    resultKey->clearBound();
                                    m_listKey << *resultKey;
                                    break;
                                }
                            }
                        }
                        if (i3Start != i3End)
                            break;
                    }
                    if (i2Start != i2End)
                        break;
                }
                break;
            }
            } // end switch
        }
        lastKey->positionFrom(*getKey());
        increment(1);
    }


    // cleaup work
    if (searchType >= 0) {
#ifdef USECXX11REGEX
        std::locale::global(oldLocale);
#else
        regfree(&preg);
#endif
    }

    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    searchKey.reset();
    resultKey.reset();
    lastKey.reset();

    m_listKey.positionToTop();
    setProcessEntryAttributes(savePEA);


    (*percent)(100, percentUserData);


    return m_listKey;
}

std::string SWModule::getRawEntry() const {
    auto r = getRawEntryImpl();
    if (!r.empty())
        prepareText(r);
    return r;
}


/******************************************************************************
 * SWModule::stripText()     - calls all stripfilters on current text
 *
 * ENT:    buf    - buf to massage instead of this modules current text
 *     len    - max len of buf
 *
 * RET: this module's text at current key location massaged by Strip filters
 */

std::string SWModule::stripText(const char * buf, int len)
{ return renderText(buf, len, false); }


/** SWModule::getRenderHeader()    - Produces any header data which might be
 *    useful which associated with the processing done with this filter.
 *    A typical example is a suggested CSS style block for classed
 *    containers.
 */
char const * SWModule::getRenderHeader() const
{ return m_renderFilters.empty() ? "" : m_renderFilters.front()->getHeader(); }


/******************************************************************************
 * SWModule::renderText     - calls all renderfilters on provided text
 *                or current module position provided text null
 *
 * ENT:    buf    - buffer to render
 *
 * RET: this module's text at current key location massaged by renderText filters
 *
 * NOTES: This method is only truly const if called with a provided text; using
 * module's current position may produce a new entry attributes map which
 * logically violates the const semantic, which is why the above method
 * which takes no params is not const, i.e., don't call this method with
 * null as text param, but instead use non-const method above.  The public
 * interface for this method expects a value for the text param.  We use it
 * internally sometimes calling with null to save duplication of code.
 */

std::string SWModule::renderText(const char *buf, int len, bool render) const {
    bool savePEA = isProcessEntryAttributes();
    if (!buf) {
        m_entryAttributes.clear();
    }
    else {
        setProcessEntryAttributes(false);
    }

    std::string tmpbuf;
    if (buf) {
        tmpbuf = buf;
    } else {
        tmpbuf = getRawEntry();
    }

    SWKey * key2 = nullptr;

    unsigned long size = (len < 0) ? ((getEntrySize()<0) ? tmpbuf.size() : getEntrySize()) : len;
    if (size > 0) {
        key2 = this->getKey();

        optionFilter(tmpbuf, key2);

        if (render) {
            renderFilter(tmpbuf, key2);
            encodingFilter(tmpbuf, key2);
        }
        else    stripFilter(tmpbuf, key2);
    }

    setProcessEntryAttributes(savePEA);

    return tmpbuf;
}


/******************************************************************************
 * SWModule::renderText     - calls all renderfilters on current text
 *
 * ENT:    tmpKey    - key to use to grab text
 *
 * RET: this module's text at current key location massaged by RenderFilers
 */

std::string SWModule::renderText(const SWKey *tmpKey) {
    SWKey *saveKey;

    if (!key->isPersist()) {
        saveKey = createKey().release();
        saveKey->positionFrom(*key);
    }
    else    saveKey = key;

    setKey(*tmpKey);

    auto retVal(renderText());

    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    return retVal;
}


/******************************************************************************
 * SWModule::stripText     - calls all StripTextFilters on current text
 *
 * ENT:    tmpKey    - key to use to grab text
 *
 * RET: this module's text at specified key location massaged by Strip filters
 */

std::string SWModule::stripText(const SWKey *tmpKey) {
    SWKey *saveKey;

    if (!key->isPersist()) {
        saveKey = createKey().release();
        saveKey->positionFrom(*key);
    }
    else    saveKey = key;

    setKey(*tmpKey);

    auto retVal(stripText());

    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    return retVal;
}

const char *SWModule::getConfigEntry(const char *key_) const {
    ConfigEntMap::const_iterator const it(m_config.find(key_));
    return (it != m_config.end()) ? it->second.c_str() : nullptr;
}

bool SWModule::hasSearchIndex()
{ return lucene::index::IndexReader::indexExists(searchIndexPath().c_str()); }

void SWModule::deleteSearchFramework()
{ FileMgr::removeDir(searchIndexPath().c_str()); }


signed char SWModule::createSearchFramework(void (*percent)(char, void *), void *percentUserData) {
    static auto const getWordPartCount =
            [](auto const & wordAttrs_) noexcept {
                auto const it(wordAttrs_.find("PartCount"));
                if (it == wordAttrs_.end())
                    return 1;
                auto const r = std::atoi(it->second.c_str());
                return r ? r : 1;
            };

    const int MAX_CONV_SIZE = 1024 * 1024;
    auto target(searchIndexPath());
    int status = FileMgr::createParent((target+"/dummy").c_str());
    if (status) return -1;

    SWKey * saveKey = nullptr;


    // turn all filters to default values
    std::list<std::string> filterSettings;
    for (auto const & filterPtr : m_optionFilters) {
        filterSettings.push_back(filterPtr->getOptionValue());
        filterPtr->setOptionValue(filterPtr->getOptionValues().begin()->c_str());

        if ((!std::strcmp("Greek Accents", filterPtr->getOptionName())) ||
            (!std::strcmp("Hebrew Vowel Points", filterPtr->getOptionName())) ||
            (!std::strcmp("Arabic Vowel Points", filterPtr->getOptionName()))
           ) {
            filterPtr->setOptionValue("Off");
        }
    }


    // be sure we give CLucene enough file handles
    FileMgr::getSystemFileMgr()->flush();

    // save key information so as not to disrupt original
    // module position
    if (!key->isPersist()) {
        saveKey = createKey().release();
        saveKey->positionFrom(*key);
    }
    else    saveKey = key;

    std::unique_ptr<SWKey> searchKey;
    if (key->isPersist()) {
        searchKey = key->clone();
        searchKey->setPersist(1);
        setKey(*searchKey);
    }

    bool includeKeyInSearch = getConfig().has("SearchOption", "IncludeKeyInSearch");

    // lets create or open our search index
    static TCHAR const * stopWords[] = { nullptr };

    std::unique_ptr<VerseKey> chapMax;
    bool savePEA;
    {
        lucene::analysis::standard::StandardAnalyzer an(stopWords);
        lucene::store::RAMDirectory ramDir;
        lucene::index::IndexWriter coreWriter(&ramDir, &an, true);

        coreWriter.setMaxFieldLength(MAX_CONV_SIZE);

        char perc = 1;
        VerseKey *vkcheck = dynamic_cast<VerseKey *>(key);
        if (vkcheck)
            chapMax.reset(static_cast<VerseKey *>(vkcheck->clone().release()));

        TreeKeyIdx *tkcheck = dynamic_cast<TreeKeyIdx *>(key);


        positionToBottom();
        long highIndex = key->getIndex();
        if (!highIndex)
            highIndex = 1;        // avoid division by zero errors.

        savePEA = isProcessEntryAttributes();
        setProcessEntryAttributes(true);

        // prox chapter blocks
        // position module at the beginning
        positionToTop();

        std::string proxBuf;
        std::string proxLem;
        std::string proxMorph;
        std::string strong;
        std::string morph;

        char err = popError();
        while (!err) {
            long mindex = key->getIndex();

            proxBuf = "";
            proxLem = "";
            proxMorph = "";

            // computer percent complete so we can report to our progress callback
            float per = (float)mindex / highIndex;
            // between 5%-98%
            per *= 93; per += 5;
            char newperc = (char)per;
            if (newperc > perc) {
                perc = newperc;
                (*percent)(perc, percentUserData);
            }

            // get "content" field
            auto content(stripText());

            bool good = false;

            // start out entry

            {
                lucene::document::Document doc;
                // get "key" field
                std::string keyText = (vkcheck) ? vkcheck->getOSISRef() : getKeyText();
                if (!content.empty()) {
                    good = true;


                    // build "strong" field
                    strong="";
                    morph="";
                    auto const wordsIt(getEntryAttributes().find("Word"));
                    if (wordsIt != getEntryAttributes().end()) {
                        for (auto const & wordPair : wordsIt->second) {
                            auto const & wordAttrs = wordPair.second;
                            auto const partCount = getWordPartCount(wordAttrs);
                            for (int i = 0; i < partCount; i++) {
                                std::string tmp = "Lemma";
                                if (partCount > 1) tmp += formatted(".%d", i+1);
                                auto const strongVal(wordAttrs.find(tmp));
                                if (strongVal != wordAttrs.end()) {
                                    // cheeze.  skip empty article tags that weren't assigned to any text
                                    if (strongVal->second == "G3588")
                                        if (wordAttrs.find("Text") == wordAttrs.end())
                                            continue;    // no text? let's skip
                                    strong.append(strongVal->second);
                                    morph.append(strongVal->second);
                                    morph.push_back('@');
                                    std::string tmp2("Morph");
                                    if (partCount > 1) tmp2 += formatted(".%d", i+1);
                                    auto const morphVal(wordAttrs.find(tmp2));
                                    if (morphVal != wordAttrs.end())
                                        morph.append(morphVal->second);
                                    strong.push_back(' ');
                                    morph.push_back(' ');
                                }
                            }
                        }
                    }

                    using lucene::document::Field;
                    doc.add(*_CLNEW Field(_T("key"), utf8ToWChar(keyText.c_str()).c_str(), Field::STORE_YES | Field::INDEX_UNTOKENIZED));

                    if (includeKeyInSearch)
                        content = keyText + " " + content;

                    doc.add(*_CLNEW Field(_T("content"), utf8ToWChar(content.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));

                    if (strong.length() > 0) {
                        doc.add(*_CLNEW Field(_T("lemma"), utf8ToWChar(strong.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
                        doc.add(*_CLNEW Field(_T("morph"), utf8ToWChar(morph.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
        //printf("setting fields (%s).\ncontent: %s\nlemma: %s\n", (const char *)*key, content, strong.c_str());
                    }

        //printf("setting fields (%s).\n", (const char *)*key);
        //fflush(stdout);
                }
                // don't write yet, cuz we have to see if we're the first of a prox block (5:1 or chapter5/verse1

                // for VerseKeys use chapter
                if (vkcheck) {
                    *chapMax = *vkcheck;
                    // we're the first verse in a chapter
                    if (vkcheck->getVerse() == 1) {
                        chapMax->positionToMaxVerse();
                        VerseKey saveKey2 = *vkcheck;
                        while ((!err) && (*vkcheck <= *chapMax)) {
        //printf("building proxBuf from (%s).\nproxBuf.c_str(): %s\n", (const char *)*key, proxBuf.c_str());
        //printf("building proxBuf from (%s).\n", (const char *)*key);

                            content = stripText();
                            if (!content.empty()) {
                                // build "strong" field
                                strong = "";
                                morph = "";
                                auto const wordsIt(
                                            getEntryAttributes().find("Word"));
                                if (wordsIt != getEntryAttributes().end()) {
                                    for (auto const & wordPair : wordsIt->second) {
                                        auto const & wordAttrs =
                                                wordPair.second;
                                        auto const partCount =
                                                getWordPartCount(wordAttrs);
                                        for (int i = 0; i < partCount; i++) {
                                            std::string tmp = "Lemma";
                                            if (partCount > 1) tmp += formatted(".%d", i+1);
                                            auto const strongVal(
                                                        wordAttrs.find(tmp));
                                            if (strongVal != wordAttrs.end()) {
                                                // cheeze.  skip empty article tags that weren't assigned to any text
                                                if (strongVal->second == "G3588")
                                                    if (wordAttrs.find("Text") == wordAttrs.end())
                                                        continue;    // no text? let's skip
                                                strong.append(strongVal->second);
                                                morph.append(strongVal->second);
                                                morph.push_back('@');
                                                std::string tmp2("Morph");
                                                if (partCount > 1) tmp2 += formatted(".%d", i+1);
                                                auto const morphVal(
                                                            wordAttrs.find(tmp2));
                                                if (morphVal != wordAttrs.end())
                                                    morph.append(morphVal->second);
                                                strong.push_back(' ');
                                                morph.push_back(' ');
                                            }
                                        }
                                    }
                                }
                                proxBuf += content;
                                proxBuf.push_back(' ');
                                proxLem += strong;
                                proxMorph += morph;
                                if (proxLem.length()) {
                                    proxLem.append("\n");
                                    proxMorph.append("\n");
                                }
                            }
                            increment(1);
                            err = popError();
                        }
                        err = 0;
                        *vkcheck = saveKey2;
                    }
                }

                // for TreeKeys use siblings if we have no children
                else if (tkcheck) {
                    if (!tkcheck->hasChildren()) {
                        if (!tkcheck->previousSibling()) {
                            do {
        //printf("building proxBuf from (%s).\n", (const char *)*key);
        //fflush(stdout);

                                content = stripText();
                                if (!content.empty()) {
                                    // build "strong" field
                                    strong = "";
                                    morph = "";
                                    auto const wordsIt(
                                                getEntryAttributes().find("Word"));
                                    if (wordsIt != getEntryAttributes().end()) {
                                        for (auto const & wordPair : wordsIt->second) {
                                            auto const & wordAttrs = wordPair.second;
                                            auto const partCount =
                                                    getWordPartCount(wordAttrs);
                                            for (int i = 0; i < partCount; i++) {
                                                std::string tmp = "Lemma";
                                                if (partCount > 1) tmp += formatted(".%d", i+1);
                                                auto const strongVal(wordAttrs.find(tmp));
                                                if (strongVal != wordAttrs.end()) {
                                                    // cheeze.  skip empty article tags that weren't assigned to any text
                                                    if (strongVal->second == "G3588")
                                                        if (wordAttrs.find("Text") == wordAttrs.end())
                                                            continue;    // no text? let's skip
                                                    strong.append(strongVal->second);
                                                    morph.append(strongVal->second);
                                                    morph.push_back('@');
                                                    std::string tmp2("Morph");
                                                    if (partCount > 1) tmp2 += formatted(".%d", i+1);
                                                    auto const morphVal(wordAttrs.find(tmp2));
                                                    if (morphVal != wordAttrs.end())
                                                        morph.append(morphVal->second);
                                                    strong.push_back(' ');
                                                    morph.push_back(' ');
                                                }
                                            }
                                        }
                                    }

                                    proxBuf += content;
                                    proxBuf.push_back(' ');
                                    proxLem += strong;
                                    proxMorph += morph;
                                    if (proxLem.length()) {
                                        proxLem.append("\n");
                                        proxMorph.append("\n");
                                    }
                                }
                            } while (tkcheck->nextSibling());
                            tkcheck->parent();
                            tkcheck->firstChild();
                        }
                        else tkcheck->nextSibling();    // reposition from our previousSibling test
                    }
                }

                using lucene::document::Field;
                if (proxBuf.length() > 0) {
                    doc.add(*_CLNEW Field(_T("prox"), utf8ToWChar(proxBuf.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
                    good = true;
                }
                if (proxLem.length() > 0) {
                    doc.add(*_CLNEW Field(_T("proxlem"), utf8ToWChar(proxLem.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED) );
                    doc.add(*_CLNEW Field(_T("proxmorph"), utf8ToWChar(proxMorph.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED) );
                    good = true;
                }
                if (good) {
        //printf("writing (%s).\n", (const char *)*key);
        //fflush(stdout);
                    coreWriter.addDocument(&doc);
                }
            }

            increment(1);
            err = popError();
        }

        // Optimizing automatically happens with the call to addIndexes
        //coreWriter->optimize();
        coreWriter.close();

        auto * d = lucene::store::FSDirectory::getDirectory(target.c_str());
        bool const createIndex =
                [d,&target]() {
                    if (lucene::index::IndexReader::indexExists(target.c_str())) {
                        if (lucene::index::IndexReader::isLocked(d))
                            lucene::index::IndexReader::unlock(d);
                        return false;
                    }
                    return true;
                }();
        {
            lucene::index::IndexWriter fsWriter(d, &an, createIndex);
            lucene::store::Directory * dirs[] = { &ramDir, nullptr };
            lucene::util::ConstValueArray<lucene::store::Directory *> dirsa(dirs,
                                                                            1);
            fsWriter.addIndexes(dirsa);
            fsWriter.close();
        }
    }

    // reposition module back to where it was before we were called
    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    searchKey.reset();
    chapMax.reset();

    setProcessEntryAttributes(savePEA);

    // reset option filters back to original values
    auto origVal(filterSettings.begin());
    for (auto const & filterPtr : m_optionFilters)
        filterPtr->setOptionValue((origVal++)->c_str());

    return 0;
}

/** OptionFilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(OptionFilterList const & filters,
                            std::string & buf,
                            SWKey const * key_) const
{
    for (auto const & filterPtr : filters)
        filterPtr->processText(buf, key_, this);
}

/** FilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(FilterList const & filters,
                            std::string & buf,
                            SWKey const * key_) const
{
    for (auto const & filterPtr : filters)
        filterPtr->processText(buf, key_, this);
}

std::string SWModule::searchIndexPath() {
    std::string target = getConfigEntry("AbsoluteDataPath");
    addTrailingDirectorySlash(target);
    target.append("lucene");
    return target;
}

} /* namespace swordxx */
