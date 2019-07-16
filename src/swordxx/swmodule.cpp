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

template <typename Filters>
void filterBuffer(swordxx::SWModule const & module,
                  std::string & buf,
                  swordxx::SWKey const * key_,
                  Filters const & filters)
{
    for (auto const & filterPtr : filters)
        filterPtr->processText(buf, key_, &module);
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

SWModule::SWModule(std::shared_ptr<SWKey> key_,
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
    , m_currentKey(std::move(key_))
{}


/******************************************************************************
 * SWModule Destructor - Cleans up instance of SWModule
 */

SWModule::~SWModule() = default;


/******************************************************************************
 * SWModule::popError - Gets and clears error status
 *
 * RET:    error status
 */

char SWModule::popError()
{
    char retval = error;

    error = 0;
    if (!retval) retval = m_currentKey->popError();
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

void SWModule::setKey(std::shared_ptr<SWKey> ikey) noexcept
{ m_currentKey = std::move(ikey); }

char SWModule::setKey(std::string_view position) {
    m_currentKey->positionFrom(std::string(position));
    return error = m_currentKey->popError();
}

void SWModule::positionToTop() {
    m_currentKey->positionToTop();
    char saveError = m_currentKey->popError();
    this->increment();
    this->decrement();
    error = saveError;
}

void SWModule::positionToBottom() {
    m_currentKey->positionToBottom();
    char saveError = m_currentKey->popError();
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
    m_currentKey->increment(steps);
    error = m_currentKey->popError();
}


/******************************************************************************
 * SWModule::decrement    - Decrements module key a number of entries
 *
 * ENT:    decrement    - Number of entries to jump backward
 *
 * RET: *this
 */

void SWModule::decrement(int steps) {
    m_currentKey->decrement(steps);
    error = m_currentKey->popError();
}


std::string SWModule::getRawEntry() const {
    auto r = getRawEntryImpl();
    if (!r.empty())
        prepareText(r);
    return r;
}

bool SWModule::isWritable() const noexcept { return false; }

void SWModule::rawFilter(std::string & buf, SWKey const * key_) const
{ filterBuffer(*this, buf, key_, m_rawFilters); }

std::string SWModule::stripText() {
    m_entryAttributes.clear();
    std::string buf(getRawEntry());
    if (!buf.empty()) {
        filterBuffer(*this, buf, m_currentKey.get(), m_optionFilters);
        filterBuffer(*this, buf, m_currentKey.get(), m_stripFilters);
    }
    return buf;
}

std::string SWModule::stripText(std::string buf) {
    bool savePEA = m_processEntryAttributes;
    m_processEntryAttributes = false;
    try {
        if (!buf.empty()) {
            filterBuffer(*this, buf, m_currentKey.get(), m_optionFilters);
            filterBuffer(*this, buf, m_currentKey.get(), m_stripFilters);
        }
    } catch (...) {
        m_processEntryAttributes = savePEA;
        throw;
    }
    m_processEntryAttributes = savePEA;
    return buf;
}


/** SWModule::getRenderHeader()    - Produces any header data which might be
 *    useful which associated with the processing done with this filter.
 *    A typical example is a suggested CSS style block for classed
 *    containers.
 */
char const * SWModule::getRenderHeader() const
{ return m_renderFilters.empty() ? "" : m_renderFilters.front()->getHeader(); }

std::string SWModule::renderText() const {
    m_entryAttributes.clear();
    std::string buf(getRawEntry());
    if (!buf.empty()) {
        filterBuffer(*this, buf, m_currentKey.get(), m_optionFilters);
        filterBuffer(*this, buf, m_currentKey.get(), m_renderFilters);
        filterBuffer(*this, buf, m_currentKey.get(), m_encodingFilters);
    }
    return buf;
}

std::string SWModule::renderText(std::string buf) const {
    bool const savePEA = m_processEntryAttributes;
    m_processEntryAttributes = false;
    try {
        if (!buf.empty()) {
            filterBuffer(*this, buf, m_currentKey.get(), m_optionFilters);
            filterBuffer(*this, buf, m_currentKey.get(), m_renderFilters);
            filterBuffer(*this, buf, m_currentKey.get(), m_encodingFilters);
        }
    } catch (...) {
        m_processEntryAttributes = savePEA;
        throw;
    }
    m_processEntryAttributes = savePEA;
    return buf;
}

const char *SWModule::getConfigEntry(const char *key_) const {
    ConfigEntMap::const_iterator const it(m_config.find(key_));
    return (it != m_config.end()) ? it->second.c_str() : nullptr;
}

} /* namespace swordxx */
