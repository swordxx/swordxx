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

#if SWORDXX_HAS_CLUCENE
#include <CLucene.h>
#endif
#include <cstdint>
#ifndef _MSC_VER
#include <iostream>
#endif
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
#include "swoptfilter.h"
#include "sysdata.h"
#include "utilstr.h"


#if defined(USECXX11REGEX) && !defined(REG_ICASE)
#define REG_ICASE std::regex::icase
#endif

#if SWORDXX_HAS_CLUCENE
using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;
using namespace lucene::queryParser;
using namespace lucene::search;
#endif

using std::vector;

namespace swordxx {

typedef std::list<std::string> StringList;

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

SWModule::SWModule(SWKey * key_, const char *imodname, const char *imoddesc, const char *imodtype, SWTextEncoding encoding, SWTextDirection direction, SWTextMarkup markup, const char *imodlang) {
    key       = key_;
    entryBuf  = "";
    config    = &ownConfig;
    modname   = 0;
    error     = 0;
    moddesc   = 0;
    modtype   = 0;
    modlang   = 0;
    this->encoding = encoding;
    this->direction = direction;
    this->markup  = markup;
    entrySize= -1;
    stdstr(&modname, imodname);
    stdstr(&moddesc, imoddesc);
    stdstr(&modtype, imodtype);
    stdstr(&modlang, imodlang);
    stripFilters = new FilterList();
    rawFilters = new FilterList();
    renderFilters = new FilterList();
    optionFilters = new OptionFilterList();
    encodingFilters = new FilterList();
    skipConsecutiveLinks = true;
    procEntAttr = true;
}


/******************************************************************************
 * SWModule Destructor - Cleans up instance of SWModule
 */

SWModule::~SWModule()
{
    if (modname)
        delete [] modname;
    if (moddesc)
        delete [] moddesc;
    if (modtype)
        delete [] modtype;
    if (modlang)
        delete [] modlang;

    if (key) {
        if (!key->isPersist())
            delete key;
    }

    stripFilters->clear();
    rawFilters->clear();
    renderFilters->clear();
    optionFilters->clear();
    encodingFilters->clear();
    entryAttributes.clear();

    delete stripFilters;
    delete rawFilters;
    delete renderFilters;
    delete optionFilters;
    delete encodingFilters;
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
    SWKey *oldKey = 0;

    if (key) {
        if (!key->isPersist())    // if we have our own copy
            oldKey = key;
    }

    if (!ikey->isPersist()) {        // if we are to keep our own copy
         key = createKey();
        *key = *ikey;
    }
    else     key = (SWKey *)ikey;        // if we are to just point to an external key

    if (oldKey)
        delete oldKey;

    return error = key->popError();
}


/******************************************************************************
 * SWModule::setPosition(SW_POSITION)    - Positions this modules to an entry
 *
 * ENT:    p    - position (e.g. TOP, BOTTOM)
 *
 * RET: *this
 */

void SWModule::setPosition(SW_POSITION p) {
    *key = p;
    char saveError = key->popError();

    switch (p) {
    case POS_TOP:
        this->increment();
        this->decrement();
        break;

    case POS_BOTTOM:
        this->decrement();
        this->increment();
        break;
    }

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
    (*key) += steps;
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
    (*key) -= steps;
    error = key->popError();
}


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
 *                -5  - multilemma window; flags = window size
 *     flags        - options flags for search
 *    justCheckIfSupported    - if set, don't search, only tell if this
 *                            function supports requested search.
 *
 * RET: ListKey set to verses that contain istr
 */

ListKey &SWModule::search(const char *istr, int searchType, int flags, SWKey *scope, bool *justCheckIfSupported, void (*percent)(char, void *), void *percentUserData) {

    listKey.clear();
    std::string term = istr;
    bool includeComponents = false;    // for entryAttrib e.g., /Lemma.1/

    std::string target = getConfigEntry("AbsoluteDataPath");
    auto const lastChar = *(target.rbegin());
    if (lastChar != '/' && lastChar != '\\')
        target.push_back('/');
#if SWORDXX_HAS_CLUCENE
    target.append("lucene");
#endif
    if (justCheckIfSupported) {
        *justCheckIfSupported = (searchType >= -3);
#if SWORDXX_HAS_CLUCENE
        if ((searchType == -4) && (IndexReader::indexExists(target.c_str()))) {
            *justCheckIfSupported = true;
        }
#endif
        return listKey;
    }

    SWKey *saveKey   = 0;
    SWKey *searchKey = 0;
    SWKey *resultKey = createKey();
    SWKey *lastKey   = createKey();
    std::string lastBuf = "";

#ifdef USECXX11REGEX
    std::locale oldLocale;
    std::locale::global(std::locale("en_US.UTF-8"));

    std::regex preg;
#else
    regex_t preg;
#endif

    vector<std::string> words;
    vector<std::string> window;
    const char *sres;
    terminateSearch = false;
    char perc = 1;
    bool savePEA = isProcessEntryAttributes();

    // determine if we might be doing special strip searches.  useful for knowing if we can use shortcuts
    bool specialStrips = (getConfigEntry("LocalStripFilter")
            || (getConfig().has("GlobalOptionFilter", "UTF8GreekAccents"))
            || (getConfig().has("GlobalOptionFilter", "UTF8HebrewPoints"))
            || (getConfig().has("GlobalOptionFilter", "UTF8ArabicPoints"))
            || (strchr(istr, '<')));

    setProcessEntryAttributes(searchType == -3);


    if (!key->isPersist()) {
        saveKey = createKey();
        *saveKey = *key;
    }
    else    saveKey = key;

    searchKey = (scope)?scope->clone():(key->isPersist())?key->clone():0;
    if (searchKey) {
        searchKey->setPersist(true);
        setKey(*searchKey);
    }

    (*percent)(perc, percentUserData);

    *this = BOTTOM;
    long highIndex = key->getIndex();
    if (!highIndex)
        highIndex = 1;        // avoid division by zero errors.
    *this = TOP;
    if (searchType >= 0) {
#ifdef USECXX11REGEX
        preg = std::regex((std::string(".*")+istr+".*").c_str(), std::regex_constants::extended & flags);
#else
        flags |=searchType|REG_NOSUB|REG_EXTENDED;
        regcomp(&preg, istr, flags);
#endif
    }

    (*percent)(++perc, percentUserData);


#if SWORDXX_HAS_CLUCENE
    (*percent)(10, percentUserData);
    if (searchType == -4) {    // indexed search

        lucene::index::IndexReader    *ir = 0;
        lucene::search::IndexSearcher *is = 0;
        Query                         *q  = 0;
        Hits                          *h  = 0;
        try {
            ir = IndexReader::open(target.c_str());
            is = new IndexSearcher(ir);
            const TCHAR *stopWords[] = { 0 };
            standard::StandardAnalyzer analyzer(stopWords);

            // parse the query
            q = QueryParser::parse(utf8ToWChar(istr).c_str(), _T("content"), &analyzer);
            (*percent)(20, percentUserData);

            // perform the search
            h = is->search(q);
            (*percent)(80, percentUserData);

            // iterate thru each good module position that meets the search
            bool checkBounds = getKey()->isBoundSet();
            for (unsigned long i = 0; i < (unsigned long)h->length(); i++) {
                Document &doc = h->doc(i);
                // set a temporary verse key to this module position
                *resultKey = wcharToUTF8(doc.get(_T("key"))).c_str(); //TODO Does a key always accept utf8?
                uint64_t score = (uint64_t)((uint32_t)(h->score(i)*100));

                // check to see if it sets ok (within our bounds) and if not, skip
                if (checkBounds) {
                    *getKey() = *resultKey;
                    if (*getKey() != *resultKey) {
                        continue;
                    }
                }
                listKey << *resultKey;
                listKey.getElement()->userData = score;
            }
            (*percent)(98, percentUserData);
        }
        catch (...) {
            q = 0;
            // invalid clucene query
        }
        delete h;
        delete q;

        delete is;
        if (ir) {
            ir->close();
        }
    }
#endif

    // some pre-loop processing
    switch (searchType) {

    // phrase
    case -1:
        // let's see if we're told to ignore case.  If so, then we'll touppstr our term
        if ((flags & REG_ICASE) == REG_ICASE) toupperstr(term);
        break;

    // multi-word
    case -2:
    case -5:
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
        if ((flags & REG_ICASE) == REG_ICASE) {
            for (unsigned int i = 0; i < words.size(); i++) {
                toupperstr(words[i]);
            }
        }
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


    while ((searchType != -4) && !popError() && !terminateSearch) {
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
            if (!regexec(&preg, textBuf.c_str(), 0, 0, 0)) {
#endif
                *resultKey = *getKey();
                resultKey->clearBound();
                listKey << *resultKey;
                lastBuf = "";
            }
#ifdef USECXX11REGEX
            else if (std::regex_match(std::string((lastBuf + ' ' + textBuf).c_str()), preg)) {
#else
            else if (!regexec(&preg, (lastBuf + ' ' + textBuf).c_str(), 0, 0, 0)) {
#endif
                lastKey->clearBound();
                listKey << *lastKey;
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
                sres = strstr(textBuf.c_str(), term.c_str());
                if (sres) { //it's also in the stripText(), so we have a valid search result item now
                    *resultKey = *getKey();
                    resultKey->clearBound();
                    listKey << *resultKey;
                }
                break;

            // multiword
            case -2: { // enclose our allocations
                int loopCount = 0;
                unsigned int foundWords = 0;
                do {
                    textBuf = ((loopCount == 0)&&(!specialStrips)) ? getRawEntry() : stripText();
                    foundWords = 0;

                    for (unsigned int i = 0; i < words.size(); i++) {
                        if ((flags & REG_ICASE) == REG_ICASE) toupperstr(textBuf);
                        sres = strstr(textBuf.c_str(), words[i].c_str());
                        if (!sres) {
                            break; //for loop
                        }
                        foundWords++;
                    }

                    loopCount++;
                } while ( (loopCount < 2) && (foundWords == words.size()));

                if ((loopCount == 2) && (foundWords == words.size())) { //we found the right words in both raw and stripped text, which means it's a valid result item
                    *resultKey = *getKey();
                    resultKey->clearBound();
                    listKey << *resultKey;
                }
                }
                break;

            // entry attributes
            case -3: {
                renderText();    // force parse
                AttributeTypeList &entryAttribs = getEntryAttributes();
                AttributeTypeList::iterator i1Start, i1End;
                AttributeList::iterator i2Start, i2End;
                AttributeValue::iterator i3Start, i3End;

                if ((words.size()) && (words[0].length())) {
// cout << "Word: " << words[0] << endl;
                for (i1Start = entryAttribs.begin(); i1Start != entryAttribs.end(); ++i1Start) {
// cout << "stuff: " << i1Start->first.c_str() << endl;
                }
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
                                    std::string key(i3Start->first);
                                    auto const sepPos = key.find_first_of('.');
                                    if (sepPos != std::string::npos)
                                        key.resize(sepPos);
                                    // we're iterating all 3 level keys, so be sure we match our
                                    // prefix (e.g., Lemma, Lemma.1, Lemma.2, etc.)
                                    if (key != words[2]) continue;
                                }
                                if (flags & SEARCHFLAG_MATCHWHOLEENTRY) {
                                    bool found = !(((flags & REG_ICASE) == REG_ICASE) ? swordxx::stricmp(i3Start->second.c_str(), words[3].c_str()) : strcmp(i3Start->second.c_str(), words[3].c_str()));
                                    sres = (found) ? i3Start->second.c_str() : 0;
                                }
                                else {
                                    sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(i3Start->second.c_str(), words[3].c_str()) : strstr(i3Start->second.c_str(), words[3].c_str());
                                }
                                if (sres) {
                                    *resultKey = *getKey();
                                    resultKey->clearBound();
                                    listKey << *resultKey;
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
            case -5:
                AttributeList &words = getEntryAttributes()["Word"];
                std::string kjvWord = "";
                std::string bibWord = "";
                for (AttributeList::iterator it = words.begin(); it != words.end(); it++) {
                    int parts = atoi(it->second["PartCount"].c_str());
                    std::string lemma = "";
                    std::string morph = "";
                    for (int i = 1; i <= parts; i++) {
                        std::string key = "";
                        key = (parts == 1) ? "Lemma" : formatted("Lemma.%d", i).c_str();
                        AttributeValue::iterator li = it->second.find(key);
                        if (li != it->second.end()) {
                            if (i > 1) lemma += " ";
                            key = (parts == 1) ? "LemmaClass" : formatted("LemmaClass.%d", i).c_str();
                            AttributeValue::iterator lci = it->second.find(key);
                            if (lci != it->second.end()) {
                                lemma += lci->second + ":";
                            }
                            lemma += li->second;
                        }
                        key = (parts == 1) ? "Morph" : formatted("Morph.%d", i).c_str();
                        li = it->second.find(key);
                        // silly.  sometimes morph counts don't equal lemma counts
                        if (i == 1 && parts != 1 && li == it->second.end()) {
                            li = it->second.find("Morph");
                        }
                        if (li != it->second.end()) {
                            if (i > 1) morph += " ";
                            key = (parts == 1) ? "MorphClass" : formatted("MorphClass.%d", i).c_str();
                            AttributeValue::iterator lci = it->second.find(key);
                            // silly.  sometimes morph counts don't equal lemma counts
                            if (i == 1 && parts != 1 && lci == it->second.end()) {
                                lci = it->second.find("MorphClass");
                            }
                            if (lci != it->second.end()) {
                                morph += lci->second + ":";
                            }
                            morph += li->second;
                        }
                        // TODO: add src tags and maybe other attributes
                    }
                    while (window.size() < (unsigned)flags) {

                    }
                }
                break;
            } // end switch
        }
        *lastKey = *getKey();
        (*this)++;
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

    if (searchKey)
        delete searchKey;
    delete resultKey;
    delete lastKey;

    listKey = TOP;
    setProcessEntryAttributes(savePEA);


    (*percent)(100, percentUserData);


    return listKey;
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
const char *SWModule::getRenderHeader() const {
    FilterList::const_iterator first = getRenderFilters().begin();
    if (first != getRenderFilters().end()) {
        return (*first)->getHeader();
    }
    return "";
}


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
        entryAttributes.clear();
    }
    else {
        setProcessEntryAttributes(false);
    }

    std::string local;
    if (buf)
        local = buf;

    std::string &tmpbuf = (buf) ? local : getRawEntryBuf();
    SWKey *key = 0;

    unsigned long size = (len < 0) ? ((getEntrySize()<0) ? tmpbuf.size() : getEntrySize()) : len;
    if (size > 0) {
        key = this->getKey();

        optionFilter(tmpbuf, key);

        if (render) {
            renderFilter(tmpbuf, key);
            encodingFilter(tmpbuf, key);
        }
        else    stripFilter(tmpbuf, key);
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
        saveKey = createKey();
        *saveKey = *key;
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
        saveKey = createKey();
        *saveKey = *key;
    }
    else    saveKey = key;

    setKey(*tmpKey);

    auto retVal(stripText());

    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    return retVal;
}

/******************************************************************************
 * SWModule::getBibliography    -Returns bibliographic data for a module in the
 *                                requested format
 *
 * ENT: bibFormat format of the bibliographic data
 *
 * RET: bibliographic data in the requested format as a string (BibTeX by default)
 */

std::string SWModule::getBibliography(unsigned char bibFormat) const {
    std::string s;
    switch (bibFormat) {
    case BIB_BIBTEX:
        s.append("@Book {").append(modname).append(", Title = \"").append(moddesc).append("\", Publisher = \"CrossWire Bible Society\"}");
        break;
    }
    return s;
}

const char *SWModule::getConfigEntry(const char *key) const {
    ConfigEntMap::iterator it = config->find(key);
    return (it != config->end()) ? it->second.c_str() : 0;
}

bool SWModule::hasSearchFramework() {
#if SWORDXX_HAS_CLUCENE
    return true;
#else
    return SWSearchable::hasSearchFramework();
#endif
}

void SWModule::deleteSearchFramework() {
#if SWORDXX_HAS_CLUCENE
    std::string target = getConfigEntry("AbsoluteDataPath");
    char const lastChar = *(target.rbegin());
    if (lastChar != '/' && lastChar != '\\')
        target.push_back('/');
    target.append("lucene");

    FileMgr::removeDir(target.c_str());
#else
    SWSearchable::deleteSearchFramework();
#endif
}


signed char SWModule::createSearchFramework(void (*percent)(char, void *), void *percentUserData) {

#if SWORDXX_HAS_CLUCENE
    std::string target = getConfigEntry("AbsoluteDataPath");
    char const lastChar = *(target.rbegin());
    if (lastChar != '/' && lastChar != '\\')
        target.push_back('/');
    const int MAX_CONV_SIZE = 1024 * 1024;
    target.append("lucene");
    int status = FileMgr::createParent((target+"/dummy").c_str());
    if (status) return -1;

    SWKey *saveKey = 0;
    SWKey *searchKey = 0;


    // turn all filters to default values
    StringList filterSettings;
    for (OptionFilterList::iterator filter = optionFilters->begin(); filter != optionFilters->end(); filter++) {
        filterSettings.push_back((*filter)->getOptionValue());
        (*filter)->setOptionValue((*filter)->getOptionValues().begin()->c_str());

        if ( (!strcmp("Greek Accents", (*filter)->getOptionName())) ||
            (!strcmp("Hebrew Vowel Points", (*filter)->getOptionName())) ||
            (!strcmp("Arabic Vowel Points", (*filter)->getOptionName()))
           ) {
            (*filter)->setOptionValue("Off");
        }
    }


    // be sure we give CLucene enough file handles
    FileMgr::getSystemFileMgr()->flush();

    // save key information so as not to disrupt original
    // module position
    if (!key->isPersist()) {
        saveKey = createKey();
        *saveKey = *key;
    }
    else    saveKey = key;

    searchKey = (key->isPersist())?key->clone():0;
    if (searchKey) {
        searchKey->setPersist(1);
        setKey(*searchKey);
    }

    bool includeKeyInSearch = getConfig().has("SearchOption", "IncludeKeyInSearch");

    // lets create or open our search index
    RAMDirectory *ramDir = 0;
    IndexWriter *coreWriter = 0;
    IndexWriter *fsWriter = 0;
    Directory *d = 0;

    const TCHAR *stopWords[] = { 0 };
    standard::StandardAnalyzer *an = new standard::StandardAnalyzer(stopWords);

    ramDir = new RAMDirectory();
    coreWriter = new IndexWriter(ramDir, an, true);
    coreWriter->setMaxFieldLength(MAX_CONV_SIZE);

    char perc = 1;
    VerseKey *vkcheck = dynamic_cast<VerseKey *>(key);
    VerseKey *chapMax = 0;
    if (vkcheck) chapMax = (VerseKey *)vkcheck->clone();

    TreeKeyIdx *tkcheck = dynamic_cast<TreeKeyIdx *>(key);


    *this = BOTTOM;
    long highIndex = key->getIndex();
    if (!highIndex)
        highIndex = 1;        // avoid division by zero errors.

    bool savePEA = isProcessEntryAttributes();
    setProcessEntryAttributes(true);

    // prox chapter blocks
    // position module at the beginning
    *this = TOP;

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
        Document *doc = new Document();
        // get "key" field
        std::string keyText = (vkcheck) ? vkcheck->getOSISRef() : getKeyText();
        if (!content.empty()) {
            good = true;


            // build "strong" field
            AttributeTypeList::iterator words;
            AttributeList::iterator word;
            AttributeValue::iterator strongVal;
            AttributeValue::iterator morphVal;

            strong="";
            morph="";
            words = getEntryAttributes().find("Word");
            if (words != getEntryAttributes().end()) {
                for (word = words->second.begin();word != words->second.end(); word++) {
                    int partCount = atoi(word->second["PartCount"].c_str());
                    if (!partCount) partCount = 1;
                    for (int i = 0; i < partCount; i++) {
                        std::string tmp = "Lemma";
                        if (partCount > 1) tmp += formatted(".%d", i+1);
                        strongVal = word->second.find(tmp);
                        if (strongVal != word->second.end()) {
                            // cheeze.  skip empty article tags that weren't assigned to any text
                            if (strongVal->second == "G3588") {
                                if (word->second.find("Text") == word->second.end())
                                    continue;    // no text? let's skip
                            }
                            strong.append(strongVal->second);
                            morph.append(strongVal->second);
                            morph.push_back('@');
                            std::string tmp = "Morph";
                            if (partCount > 1) tmp += formatted(".%d", i+1);
                            morphVal = word->second.find(tmp);
                            if (morphVal != word->second.end()) {
                                morph.append(morphVal->second);
                            }
                            strong.push_back(' ');
                            morph.push_back(' ');
                        }
                    }
                }
            }

            doc->add(*_CLNEW Field(_T("key"), utf8ToWChar(keyText.c_str()).c_str(), Field::STORE_YES | Field::INDEX_UNTOKENIZED));

            if (includeKeyInSearch)
                content = keyText + " " + content;

            doc->add(*_CLNEW Field(_T("content"), utf8ToWChar(content.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));

            if (strong.length() > 0) {
                doc->add(*_CLNEW Field(_T("lemma"), utf8ToWChar(strong.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
                doc->add(*_CLNEW Field(_T("morph"), utf8ToWChar(morph.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
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
                *chapMax = MAXVERSE;
                VerseKey saveKey = *vkcheck;
                while ((!err) && (*vkcheck <= *chapMax)) {
//printf("building proxBuf from (%s).\nproxBuf.c_str(): %s\n", (const char *)*key, proxBuf.c_str());
//printf("building proxBuf from (%s).\n", (const char *)*key);

                    content = stripText();
                    if (!content.empty()) {
                        // build "strong" field
                        strong = "";
                        morph = "";
                        AttributeTypeList::iterator words;
                        AttributeList::iterator word;
                        AttributeValue::iterator strongVal;
                        AttributeValue::iterator morphVal;

                        words = getEntryAttributes().find("Word");
                        if (words != getEntryAttributes().end()) {
                            for (word = words->second.begin();word != words->second.end(); word++) {
                                int partCount = atoi(word->second["PartCount"].c_str());
                                if (!partCount) partCount = 1;
                                for (int i = 0; i < partCount; i++) {
                                    std::string tmp = "Lemma";
                                    if (partCount > 1) tmp += formatted(".%d", i+1);
                                    strongVal = word->second.find(tmp);
                                    if (strongVal != word->second.end()) {
                                        // cheeze.  skip empty article tags that weren't assigned to any text
                                        if (strongVal->second == "G3588") {
                                            if (word->second.find("Text") == word->second.end())
                                                continue;    // no text? let's skip
                                        }
                                        strong.append(strongVal->second);
                                        morph.append(strongVal->second);
                                        morph.push_back('@');
                                        std::string tmp = "Morph";
                                        if (partCount > 1) tmp += formatted(".%d", i+1);
                                        morphVal = word->second.find(tmp);
                                        if (morphVal != word->second.end()) {
                                            morph.append(morphVal->second);
                                        }
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
                    (*this)++;
                    err = popError();
                }
                err = 0;
                *vkcheck = saveKey;
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
                            AttributeTypeList::iterator words;
                            AttributeList::iterator word;
                            AttributeValue::iterator strongVal;
                            AttributeValue::iterator morphVal;

                            words = getEntryAttributes().find("Word");
                            if (words != getEntryAttributes().end()) {
                                for (word = words->second.begin();word != words->second.end(); word++) {
                                    int partCount = atoi(word->second["PartCount"].c_str());
                                    if (!partCount) partCount = 1;
                                    for (int i = 0; i < partCount; i++) {
                                        std::string tmp = "Lemma";
                                        if (partCount > 1) tmp += formatted(".%d", i+1);
                                        strongVal = word->second.find(tmp);
                                        if (strongVal != word->second.end()) {
                                            // cheeze.  skip empty article tags that weren't assigned to any text
                                            if (strongVal->second == "G3588") {
                                                if (word->second.find("Text") == word->second.end())
                                                    continue;    // no text? let's skip
                                            }
                                            strong.append(strongVal->second);
                                            morph.append(strongVal->second);
                                            morph.push_back('@');
                                            std::string tmp = "Morph";
                                            if (partCount > 1) tmp += formatted(".%d", i+1);
                                            morphVal = word->second.find(tmp);
                                            if (morphVal != word->second.end()) {
                                                morph.append(morphVal->second);
                                            }
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

        if (proxBuf.length() > 0) {
            doc->add(*_CLNEW Field(_T("prox"), utf8ToWChar(proxBuf.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED));
            good = true;
        }
        if (proxLem.length() > 0) {
            doc->add(*_CLNEW Field(_T("proxlem"), utf8ToWChar(proxLem.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED) );
            doc->add(*_CLNEW Field(_T("proxmorph"), utf8ToWChar(proxMorph.c_str()).c_str(), Field::STORE_NO | Field::INDEX_TOKENIZED) );
            good = true;
        }
        if (good) {
//printf("writing (%s).\n", (const char *)*key);
//fflush(stdout);
            coreWriter->addDocument(doc);
        }
        delete doc;

        (*this)++;
        err = popError();
    }

    // Optimizing automatically happens with the call to addIndexes
    //coreWriter->optimize();
    coreWriter->close();

    d = FSDirectory::getDirectory(target.c_str());
    if (IndexReader::indexExists(target.c_str())) {
        if (IndexReader::isLocked(d)) {
            IndexReader::unlock(d);
        }
        fsWriter = new IndexWriter( d, an, false);
    }
    else {
        fsWriter = new IndexWriter(d, an, true);
    }

    Directory *dirs[] = { ramDir, 0 };
    lucene::util::ConstValueArray< lucene::store::Directory *>dirsa(dirs, 1);
    fsWriter->addIndexes(dirsa);
    fsWriter->close();

    delete ramDir;
    delete coreWriter;
    delete fsWriter;
    delete an;

    // reposition module back to where it was before we were called
    setKey(*saveKey);

    if (!saveKey->isPersist())
        delete saveKey;

    if (searchKey)
        delete searchKey;

    delete chapMax;

    setProcessEntryAttributes(savePEA);

    // reset option filters back to original values
    StringList::iterator origVal = filterSettings.begin();
    for (OptionFilterList::iterator filter = optionFilters->begin(); filter != optionFilters->end(); filter++) {
        (*filter)->setOptionValue((origVal++)->c_str());
    }

#endif
    return 0;
}

/** OptionFilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(OptionFilterList *filters, std::string &buf, const SWKey *key) const {
    OptionFilterList::iterator it;
    for (it = filters->begin(); it != filters->end(); it++) {
        (*it)->processText(buf, key, this);
    }
}

/** FilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(FilterList *filters, std::string &buf, const SWKey *key) const {
    FilterList::iterator it;
    for (it = filters->begin(); it != filters->end(); it++) {
        (*it)->processText(buf, key, this);
    }
}


/******************************************************************************
 * SWModule::prepText    - Prepares the text before returning it to external
 *                    objects
 *
 * ENT:    buf    - buffer where text is stored and where to store the prep'd
 *                text.
 */

void SWModule::prepText(std::string &buf) {
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

} /* namespace swordxx */
