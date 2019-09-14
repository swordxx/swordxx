/******************************************************************************
 *
 *  encfiltmgr.cpp -    implementaion of class EncodingFilterMgr, subclass of
 *            SWFilterMgr, used to transcode all module text to a
 *            requested encoding
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "encfiltmgr.h"

#include <cassert>
#include "filters/latin1utf8.h"
#include "filters/scsuutf8.h"
#include "filters/utf16utf8.h"
#include "filters/utf8latin1.h"
#include "filters/utf8html.h"
#include "filters/utf8scsu.h"
#include "filters/utf8utf16.h"
#include "swmodule.h"
#include "utilstr.h"


namespace swordxx {

struct EncodingFilterMgr::EncodingFilter: SWFilter {

/* Methods: */

    EncodingFilter(TextEncoding const encoding) noexcept
        : m_encoding(encoding)
    {}


    char processText(std::string & text,
                     SWKey const * key = nullptr,
                     SWModule const * module = nullptr) override;

/* Fields: */

    TextEncoding m_encoding;

};

char EncodingFilterMgr::EncodingFilter::processText(std::string & text,
                                                    SWKey const * key,
                                                    SWModule const * module)
{
    switch (m_encoding) {
        case ENC_LATIN1: return UTF8Latin1().processText(text, key, module);
        case ENC_UTF16:  return UTF8UTF16().processText(text, key, module);
        case ENC_HTML:   return UTF8HTML().processText(text, key, module);
        case ENC_SCSU:   return UTF8SCSU().processText(text, key, module);
        default: // i.e. case ENC_UTF8
            return 0;
    }
}

EncodingFilterMgr::EncodingFilterMgr(TextEncoding const encoding)
    : m_latin1utf8(std::make_shared<Latin1UTF8>())
    , m_scsuutf8(std::make_shared<SCSUUTF8>())
    , m_utf16utf8(std::make_shared<UTF16UTF8>())
    , m_targetenc(std::make_shared<EncodingFilter>(encoding))
{}

void EncodingFilterMgr::addRawFilters(SWModule & module,
                                      ConfigEntMap const & section)
{
    auto const entry(section.find("Encoding"));
    if (entry == section.end() || entry->second.empty()) {
        module.addRawFilter(m_latin1utf8);
    } else {
        auto const & rawEncodingStr = entry->second;
        if (caseInsensitiveEquals(rawEncodingStr, "Latin-1")) {
            module.addRawFilter(m_latin1utf8);
        } else if (caseInsensitiveEquals(rawEncodingStr, "SCSU")) {
            module.addRawFilter(m_scsuutf8);
        } else if (caseInsensitiveEquals(rawEncodingStr, "UTF-16")) {
            module.addRawFilter(m_utf16utf8);
        }
    }
}


void EncodingFilterMgr::addEncodingFilters(SWModule & module,
                                           ConfigEntMap const & /* section */)
{ module.addEncodingFilter(m_targetenc); }

void EncodingFilterMgr::setEncoding(TextEncoding const encoding) {
    assert(encoding != ENC_UNKNOWN);
    m_targetenc->m_encoding = encoding;
}

TextEncoding EncodingFilterMgr::encoding() const noexcept
{ return m_targetenc->m_encoding; }

} /* namespace swordxx */
