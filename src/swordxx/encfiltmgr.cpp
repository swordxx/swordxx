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
#include "filters/unicodertf.h"
#include "filters/utf16utf8.h"
#include "filters/utf8latin1.h"
#include "filters/utf8html.h"
#if SWORDXX_HAS_ICU
#include "filters/utf8scsu.h"
#endif
#include "filters/utf8utf16.h"
#include "swmgr.h"
#include "swmodule.h"
#include "utilstr.h"


namespace swordxx {
namespace {

std::unique_ptr<SWFilter> makeEncodingFilter(TextEncoding const encoding) {
    switch (encoding) {
        case ENC_LATIN1: return std::make_unique<UTF8Latin1>();
        case ENC_UTF16:  return std::make_unique<UTF8UTF16>();
        case ENC_RTF:    return std::make_unique<UnicodeRTF>();
        case ENC_HTML:   return std::make_unique<UTF8HTML>();
#if SWORDXX_HAS_ICU
        case ENC_SCSU:   return std::make_unique<UTF8SCSU>();
#endif
        default: // i.e. case ENC_UTF8
            return std::unique_ptr<SWFilter>();
    }
}

} // anonymous namespace

EncodingFilterMgr::EncodingFilterMgr(TextEncoding const encoding)
    : m_latin1utf8(std::make_unique<Latin1UTF8>())
    , m_scsuutf8(std::make_unique<SCSUUTF8>())
    , m_utf16utf8(std::make_unique<UTF16UTF8>())
    , m_targetenc(makeEncodingFilter(encoding))
    , m_encoding(encoding)
{}

void EncodingFilterMgr::addRawFilters(SWModule & module,
                                      ConfigEntMap const & section)
{
    auto const entry(section.find("Encoding"));
    if (entry == section.end() || entry->second.empty()) {
        module.addRawFilter(m_latin1utf8.get()); /// \todo pass as shared_ptr?
    } else {
        char const * const rawEncodingStr = entry->second.c_str();
        if (!stricmp(rawEncodingStr, "Latin-1")) {
            /// \todo pass as shared_ptr?
            module.addRawFilter(m_latin1utf8.get());
        } else if (!stricmp(rawEncodingStr, "SCSU")) {
            /// \todo pass as shared_ptr?
            module.addRawFilter(m_scsuutf8.get());
        } else if (!stricmp(rawEncodingStr, "UTF-16")) {
            /// \todo pass as shared_ptr?
            module.addRawFilter(m_utf16utf8.get());
        }
    }
}


void EncodingFilterMgr::addEncodingFilters(SWModule & module,
                                           ConfigEntMap const & /* section */)
{
    if (m_targetenc)
        /// \todo pass as shared_ptr?
        module.addEncodingFilter(m_targetenc.get());
}

void EncodingFilterMgr::setEncoding(TextEncoding const encoding) {
    assert(encoding != ENC_UNKNOWN);
    if (encoding == m_encoding)
        return;

    m_encoding = encoding;
    if (m_targetenc) {
        std::unique_ptr<SWFilter> const oldfilter(std::move(m_targetenc));
        m_targetenc = makeEncodingFilter(encoding);
        if (!m_targetenc) {
            for (auto const & mp : getParentMgr()->Modules)
                 /// \todo pass as shared_ptr?
                mp.second->removeRenderFilter(oldfilter.get());
        } else {
            for (auto const & mp : getParentMgr()->Modules)
                 /// \todo pass as shared_ptr?
                mp.second->replaceRenderFilter(oldfilter.get(),
                                               m_targetenc.get());
        }
    } else {
        m_targetenc = makeEncodingFilter(encoding);
        if (m_targetenc)
            for (auto const & mp : getParentMgr()->Modules)
                 /// \todo pass as shared_ptr?
                mp.second->addRenderFilter(m_targetenc.get());
    }
    return;
}

} /* namespace swordxx */
