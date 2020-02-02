/******************************************************************************
 *
 *  markupfiltmgr.cpp -    implementaion of class MarkupFilterMgr, subclass of
 *            used to transcode all module text to a requested
 *            markup
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

#include "filters/gbfhtml.h"
#include "filters/gbfhtmlhref.h"
#include "filters/gbfosis.h"
#include "filters/gbfplain.h"
#include "filters/gbfthml.h"
#include "filters/gbfwebif.h"
#include "filters/gbfxhtml.h"
#include "filters/osishtmlhref.h"
#include "filters/osisosis.h"
#include "filters/osisplain.h"
#include "filters/osiswebif.h"
#include "filters/osisxhtml.h"
#include "filters/teihtmlhref.h"
#include "filters/teiplain.h"
#include "filters/teixhtml.h"
#include "filters/thmlgbf.h"
#include "filters/thmlhtml.h"
#include "filters/thmlhtmlhref.h"
#include "filters/thmlosis.h"
#include "filters/thmlplain.h"
#include "filters/thmlwebif.h"
#include "filters/thmlxhtml.h"
#include "markupfiltmgr.h"
#include "swmgr.h"
#include "swmodule.h"


namespace swordxx {


/******************************************************************************
 * MarkupFilterMgr Constructor - initializes instance of MarkupFilterMgr
 *
 * ENT:
 *      enc - Encoding format to emit
 *      mark - Markup format to emit
 */

MarkupFilterMgr::MarkupFilterMgr(char mark, TextEncoding enc)
    : EncodingFilterMgr(enc)
    , markup(mark)
{
    switch (mark) {
    case FMT_PLAIN:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLPlain>();
        m_fromgbf = std::make_shared<GBFPlain>();
        m_fromosis = std::make_shared<OSISPlain>();
        m_fromtei = std::make_shared<TEIPlain>();
        break;

    case FMT_THML:
        m_fromplain.reset();
        m_fromthml.reset();
        m_fromgbf = std::make_shared<GBFThML>();
        m_fromosis.reset();
        m_fromtei.reset();
        break;

    case FMT_GBF:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLGBF>();
        m_fromgbf.reset();
        m_fromosis.reset();
        m_fromtei.reset();
        break;

    case FMT_HTML:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLHTML>();
        m_fromgbf = std::make_shared<GBFHTML>();
        m_fromosis.reset();
        m_fromtei.reset();
        break;

    case FMT_HTMLHREF:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLHTMLHREF>();
        m_fromgbf = std::make_shared<GBFHTMLHREF>();
        m_fromosis = std::make_shared<OSISHTMLHREF>();
        m_fromtei = std::make_shared<TEIHTMLHREF>();
        break;

    case FMT_OSIS:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLOSIS>();
        m_fromgbf = std::make_shared<GBFOSIS>();
        m_fromosis = std::make_shared<OSISOSIS>();
        m_fromtei.reset();
        break;

    case FMT_WEBIF:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLWEBIF>();
        m_fromgbf = std::make_shared<GBFWEBIF>();
        m_fromosis = std::make_shared<OSISWEBIF>();
        m_fromtei.reset();
        break;

    case FMT_TEI:
        m_fromplain.reset();
        m_fromthml.reset();
        m_fromgbf.reset();
        m_fromosis.reset();
        m_fromtei.reset();
        break;

    case FMT_XHTML:
        m_fromplain.reset();
        m_fromthml = std::make_shared<ThMLXHTML>();
        m_fromgbf = std::make_shared<GBFXHTML>();
        m_fromosis = std::make_shared<OSISXHTML>();
        m_fromtei = std::make_shared<TEIXHTML>();
        break;
    }
}

void MarkupFilterMgr::addRenderFilters(SWModule & module,
                                       ConfigEntMap const & /* section */)
{
    switch (module.getMarkup()) {
    case FMT_THML:
        if (m_fromthml)
            module.addRenderFilter(m_fromthml);
        break;
    case FMT_GBF:
        if (m_fromgbf)
            module.addRenderFilter(m_fromgbf);
        break;
    case FMT_PLAIN:
        if (m_fromplain)
            module.addRenderFilter(m_fromplain);
        break;
    case FMT_OSIS:
        if (m_fromosis)
            module.addRenderFilter(m_fromosis);
        break;
    case FMT_TEI:
        if (m_fromtei)
            module.addRenderFilter(m_fromtei);
        break;
    }
}

} /* namespace swordxx */

