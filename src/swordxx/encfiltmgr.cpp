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


/******************************************************************************
 * EncodingFilterMgr Constructor - initializes instance of EncodingFilterMgr
 *
 * ENT:
 *      enc - Encoding format to emit
 */
EncodingFilterMgr::EncodingFilterMgr(TextEncoding enc)
        : SWFilterMgr() {

    m_scsuutf8   = new SCSUUTF8();
    m_latin1utf8 = new Latin1UTF8();
    m_utf16utf8  = new UTF16UTF8();

    m_encoding = enc;

    switch (m_encoding) {
        case ENC_LATIN1: m_targetenc = new UTF8Latin1(); break;
        case ENC_UTF16:  m_targetenc = new UTF8UTF16();  break;
        case ENC_RTF:    m_targetenc = new UnicodeRTF(); break;
        case ENC_HTML:   m_targetenc = new UTF8HTML();   break;
#if SWORDXX_HAS_ICU
        case ENC_SCSU:   m_targetenc = new UTF8SCSU();   break;
#endif
        default: // i.e. case ENC_UTF8
            m_targetenc = nullptr;
    }
}


/******************************************************************************
 * EncodingFilterMgr Destructor - Cleans up instance of EncodingFilterMgr
 */
EncodingFilterMgr::~EncodingFilterMgr() {
    delete m_scsuutf8;
    delete m_latin1utf8;
    delete m_utf16utf8;
    delete m_targetenc;
}


void EncodingFilterMgr::AddRawFilters(SWModule *module, ConfigEntMap &section) {

    ConfigEntMap::iterator entry;

    std::string encoding = ((entry = section.find("Encoding")) != section.end()) ? (*entry).second : (std::string)"";
    if (!encoding.length() || !stricmp(encoding.c_str(), "Latin-1")) {
        module->addRawFilter(m_latin1utf8);
    }
    else if (!stricmp(encoding.c_str(), "SCSU")) {
        module->addRawFilter(m_scsuutf8);
    }
    else if (!stricmp(encoding.c_str(), "UTF-16")) {
        module->addRawFilter(m_utf16utf8);
    }
}


void EncodingFilterMgr::AddEncodingFilters(SWModule *module, ConfigEntMap & /* section */) {
    if (m_targetenc)
        module->addEncodingFilter(m_targetenc);
}


/******************************************************************************
 * EncodingFilterMgr::Encoding    - sets/gets encoding
 *
 * ENT:    enc    - new encoding or 0 to simply get the current encoding
 *
 * RET: encoding
 */
TextEncoding EncodingFilterMgr::Encoding(TextEncoding enc) {
    if (enc != ENC_UNKNOWN && enc != m_encoding) {
        m_encoding = enc;
        SWFilter * const oldfilter = m_targetenc;

        switch (m_encoding) {
            case ENC_LATIN1: m_targetenc = new UTF8Latin1(); break;
            case ENC_UTF16:  m_targetenc = new UTF8UTF16();  break;
            case ENC_RTF:    m_targetenc = new UnicodeRTF(); break;
            case ENC_HTML:   m_targetenc = new UTF8HTML();   break;
#if SWORDXX_HAS_ICU
            case ENC_SCSU:   m_targetenc = new UTF8SCSU();   break;
#endif
            default: // i.e. case ENC_UTF8
                m_targetenc = nullptr;
        }

        if (oldfilter != m_targetenc) {
            if (oldfilter) {
                if (!m_targetenc) {
                    for (auto const & mp : getParentMgr()->Modules)
                        mp.second->removeRenderFilter(oldfilter);
                } else {
                    for (auto const & mp : getParentMgr()->Modules)
                        mp.second->replaceRenderFilter(oldfilter, m_targetenc);
                }
                delete oldfilter;
            } else if (m_targetenc) {
                for (auto const & mp : getParentMgr()->Modules)
                    mp.second->addRenderFilter(m_targetenc);
            }
        }
    }
    return m_encoding;
}


} /* namespace swordxx */
