/******************************************************************************
 *
 *  markupfiltmgr.cpp -    implementaion of class MarkupFilterMgr, subclass of
 *            used to transcode all module text to a requested
 *            markup
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

#include "filters/gbfhtml.h"
#include "filters/gbfhtmlhref.h"
#include "filters/gbflatex.h"
#include "filters/gbfosis.h"
#include "filters/gbfplain.h"
#include "filters/gbfrtf.h"
#include "filters/gbfthml.h"
#include "filters/gbfwebif.h"
#include "filters/gbfxhtml.h"
#include "filters/osishtmlhref.h"
#include "filters/osislatex.h"
#include "filters/osisosis.h"
#include "filters/osisplain.h"
#include "filters/osisrtf.h"
#include "filters/osiswebif.h"
#include "filters/osisxhtml.h"
#include "filters/teihtmlhref.h"
#include "filters/teilatex.h"
#include "filters/teiplain.h"
#include "filters/teirtf.h"
#include "filters/teixhtml.h"
#include "filters/thmlgbf.h"
#include "filters/thmlhtml.h"
#include "filters/thmlhtmlhref.h"
#include "filters/thmllatex.h"
#include "filters/thmlosis.h"
#include "filters/thmlplain.h"
#include "filters/thmlrtf.h"
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

MarkupFilterMgr::MarkupFilterMgr(char mark, char enc)
        : EncodingFilterMgr(enc) {

    markup = mark;

    CreateFilters(markup);
}


/******************************************************************************
 * MarkupFilterMgr Destructor - Cleans up instance of MarkupFilterMgr
 */

MarkupFilterMgr::~MarkupFilterMgr() {
    delete fromthml;
    delete fromgbf;
    delete fromplain;
    delete fromosis;
    delete fromtei;
}


/******************************************************************************
 * MarkupFilterMgr::Markup    - sets/gets markup
 *
 * ENT:    mark    - new encoding or 0 to simply get the current markup
 *
 * RET: markup
 */
char MarkupFilterMgr::Markup(char mark) {
    if (mark && mark != markup) {
        markup = mark;
        ModMap::const_iterator module;

        SWFilter *oldplain = fromplain;
        SWFilter *oldthml  = fromthml;
        SWFilter *oldgbf   = fromgbf;
        SWFilter *oldosis  = fromosis;
        SWFilter *oldtei   = fromtei;

        CreateFilters(markup);

        for (module = getParentMgr()->Modules.begin(); module != getParentMgr()->Modules.end(); ++module) {
            switch (module->second->getMarkup()) {
            case FMT_THML:
                if (oldthml != fromthml) {
                    if (oldthml) {
                        if (!fromthml) {
                            module->second->removeRenderFilter(oldthml);
                        }
                        else {
                            module->second->replaceRenderFilter(oldthml, fromthml);
                        }
                    }
                    else if (fromthml) {
                        module->second->addRenderFilter(fromthml);
                    }
                }
                break;

            case FMT_GBF:
                if (oldgbf != fromgbf) {
                    if (oldgbf) {
                        if (!fromgbf) {
                            module->second->removeRenderFilter(oldgbf);
                        }
                        else {
                            module->second->replaceRenderFilter(oldgbf, fromgbf);
                        }
                    }
                    else if (fromgbf) {
                        module->second->addRenderFilter(fromgbf);
                    }
                }
                break;

            case FMT_PLAIN:
                if (oldplain != fromplain) {
                    if (oldplain) {
                        if (!fromplain) {
                            module->second->removeRenderFilter(oldplain);
                        }
                        else {
                            module->second->replaceRenderFilter(oldplain, fromplain);
                        }
                    }
                    else if (fromplain) {
                        module->second->addRenderFilter(fromplain);
                    }
                }
                break;

            case FMT_OSIS:
                if (oldosis != fromosis) {
                    if (oldosis) {
                        if (!fromosis) {
                            module->second->removeRenderFilter(oldosis);
                        }
                        else {
                            module->second->replaceRenderFilter(oldosis, fromosis);
                        }
                    }
                    else if (fromosis) {
                        module->second->addRenderFilter(fromosis);
                    }
                }
                break;

            case FMT_TEI:
                if (oldtei != fromtei) {
                    if (oldtei) {
                        if (!fromtei) {
                            module->second->removeRenderFilter(oldtei);
                        }
                        else {
                            module->second->replaceRenderFilter(oldtei, fromtei);
                        }
                    }
                    else if (fromtei) {
                        module->second->addRenderFilter(fromtei);
                    }
                }
                break;
            }
        }

        delete oldthml;
        delete oldgbf;
        delete oldplain;
        delete oldosis;
        delete oldtei;
    }
    return markup;
}


void MarkupFilterMgr::AddRenderFilters(SWModule *module, ConfigEntMap & /* section */) {
    switch (module->getMarkup()) {
    case FMT_THML:
        if (fromthml)
            module->addRenderFilter(fromthml);
        break;
    case FMT_GBF:
        if (fromgbf)
            module->addRenderFilter(fromgbf);
        break;
    case FMT_PLAIN:
        if (fromplain)
            module->addRenderFilter(fromplain);
        break;
    case FMT_OSIS:
        if (fromosis)
            module->addRenderFilter(fromosis);
        break;
    case FMT_TEI:
        if (fromtei)
            module->addRenderFilter(fromtei);
        break;
    }
}


void MarkupFilterMgr::CreateFilters(char markup) {

    switch (markup) {
    case FMT_PLAIN:
        fromplain = nullptr;
        fromthml  = new ThMLPlain();
        fromgbf   = new GBFPlain();
        fromosis  = new OSISPlain();
        fromtei   = new TEIPlain();
        break;

    case FMT_THML:
        fromplain = nullptr;
        fromthml  = nullptr;
        fromgbf   = new GBFThML();
        fromosis  = nullptr;
        fromtei   = nullptr;
        break;

    case FMT_GBF:
        fromplain = nullptr;
        fromthml  = new ThMLGBF();
        fromgbf   = nullptr;
        fromosis  = nullptr;
        fromtei   = nullptr;
        break;

    case FMT_HTML:
        fromplain = nullptr;
        fromthml  = new ThMLHTML();
        fromgbf   = new GBFHTML();
        fromosis  = nullptr;
        fromtei   = nullptr;
        break;

    case FMT_HTMLHREF:
        fromplain = nullptr;
        fromthml  = new ThMLHTMLHREF();
        fromgbf   = new GBFHTMLHREF();
        fromosis  = new OSISHTMLHREF();
        fromtei   = new TEIHTMLHREF();
        break;

    case FMT_RTF:
        fromplain = nullptr;
        fromthml  = new ThMLRTF();
        fromgbf   = new GBFRTF();
        fromosis  = new OSISRTF();
        fromtei   = new TEIRTF();
        break;

    case FMT_LATEX:
        fromplain = nullptr;
        fromthml  = new ThMLLaTeX();
        fromgbf   = new GBFLaTeX();
        fromosis  = new OSISLaTeX();
        fromtei   = new TEILaTeX();
        break;

    case FMT_OSIS:
        fromplain = nullptr;
        fromthml  = new ThMLOSIS();
        fromgbf   = new GBFOSIS();
        fromosis  = new OSISOSIS();
        fromtei   = nullptr;
        break;

    case FMT_WEBIF:
        fromplain = nullptr;
        fromthml  = new ThMLWEBIF();
        fromgbf   = new GBFWEBIF();
        fromosis  = new OSISWEBIF();
        fromtei   = nullptr;
        break;

    case FMT_TEI:
        fromplain = nullptr;
        fromthml  = nullptr;
        fromgbf   = nullptr;
        fromosis  = nullptr;
        fromtei   = nullptr;
        break;

    case FMT_XHTML:
        fromplain = nullptr;
        fromthml  = new ThMLXHTML();
        fromgbf   = new GBFXHTML();
        fromosis  = new OSISXHTML();
        fromtei   = new TEIXHTML();
        break;
    }

}


} /* namespace swordxx */

