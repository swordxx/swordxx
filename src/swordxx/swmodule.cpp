/******************************************************************************
 *
 *  swmodule.cpp -    code for base class 'SWModule'. SWModule is the basis
 *            for all types of modules (e.g. texts, commentaries,
 *            maps, lexicons, etc.)
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

#include <cstddef>
#include "swfilter.h"
#include "swkey.h"
#include "swoptfilter.h"
#include "utilstr.h"


namespace {

void prepareText(std::string &buf) {
    decltype(buf.size()) to, from;
    bool space = false;
    bool cr = false;
    bool realdata = false;
    std::size_t nlcnt = 0u;
    for (to = from = 0u; buf[from]; ++from) {
        switch (buf[from]) {
        case '\n':
            if (!realdata)
                continue;
            space = !cr;
            cr = false;
            if (++nlcnt > 1) {
                buf[to++] = 10;
            }
            continue;
        case '\r':
            if (!realdata)
                continue;
            buf[to++] = '\n';
            space = false;
            cr = true;
            continue;
        }
        realdata = true;
        nlcnt = 0u;
        if (space) {
            space = false;
            if (buf[from] != ' ') {
                buf[to++] = ' ';
                --from;
                continue;
            }
        }
        buf[to++] = buf[from];
    }
    buf.resize(to);
    while (to > 1u) { // remove trailing excess
        --to;
        if ((buf[to] == '\n') || (buf[to] == ' '))
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

void SWModule::flush() {}


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


bool SWModule::isUnicode() const noexcept
{ return (m_textEncoding == ENC_UTF8 || m_textEncoding == ENC_SCSU); }


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

std::string SWModule::getKeyText() const { return m_currentKey->getText(); }

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

void SWModule::setEntry(std::string_view) {}

void SWModule::linkEntry(SWKey const &) {}

void SWModule::deleteEntry() {}

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

bool SWModule::isLinked(SWKey const &, SWKey const &) const { return false; }
bool SWModule::hasEntry(SWKey const &) const { return false; }

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
