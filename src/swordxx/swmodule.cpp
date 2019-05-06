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

} /* namespace swordxx */
