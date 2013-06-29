/******************************************************************************
 *
 *  diafiltmgr.cpp -	DiathekeFilterMgr
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#include <thmlplain.h>
#include <gbfplain.h>
#include <thmlgbf.h>
#include <gbfthml.h>
#include <thmlhtml.h>
#include <gbfhtml.h>
#include <thmlhtmlhref.h>
#include <gbfhtmlhref.h>
#include <thmlrtf.h>
#include <gbfrtf.h>
#include <osisrtf.h>
#include <osishtmlhref.h>
#include <swmodule.h>
#include <osisplain.h>
#include <thmlosis.h>
#include <gbfosis.h>

#include <swmgr.h>

#include "diafiltmgr.h"
#include "thmlcgi.h"
#include "gbfcgi.h"
#include "osiscgi.h"

DiathekeFilterMgr::DiathekeFilterMgr (char mark, char enc)
		   : EncodingFilterMgr(enc) {

        markup = mark;

        CreateFilters(markup);
}

DiathekeFilterMgr::~DiathekeFilterMgr() {
        if (fromthml)
                delete (fromthml);
        if (fromgbf)
                delete (fromgbf);
        if (fromplain)
                delete (fromplain);
        if (fromosis)
                delete (fromosis);
}

void DiathekeFilterMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
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
        }
	EncodingFilterMgr::AddRenderFilters(module, section);
}


char DiathekeFilterMgr::Markup(char mark) {
        if (mark && mark != markup) {
                markup = mark;
                ModMap::const_iterator module;

                SWFilter * oldplain = fromplain;
                SWFilter * oldthml = fromthml;
                SWFilter * oldgbf = fromgbf;
                SWFilter * oldosis = fromosis;

                CreateFilters(markup);

                for (module = getParentMgr()->Modules.begin(); module != getParentMgr()->Modules.end(); module++)
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
                                        break;
                                }
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
                                        break;
                                }
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
                                        break;
                                }
                        }

                if (oldthml)
                        delete oldthml;
                if (oldgbf)
                        delete oldgbf;
                if (oldplain)
                        delete oldplain;
                if (oldosis)
                        delete oldosis;
        }
        return markup;
}

void DiathekeFilterMgr::CreateFilters(char markup) {

                switch (markup) {
		case FMT_CGI:
			fromplain = NULL;
			fromthml = new ThMLCGI();
			fromgbf = new GBFCGI();
			fromosis = new OSISCGI();
			break;
                case FMT_PLAIN:
                        fromplain = NULL;
                        fromthml = new ThMLPlain();
                        fromgbf = new GBFPlain();
                        fromosis = new OSISPlain();
                        break;
                case FMT_THML:
                        fromplain = NULL;
                        fromthml = NULL;
                        fromgbf = new GBFThML();
                        fromosis = NULL;
                        break;
                case FMT_GBF:
                        fromplain = NULL;
                        fromthml = new ThMLGBF();
                        fromgbf = NULL;
                        fromosis = NULL;
                        break;
                case FMT_HTML:
                        fromplain = NULL;
                        fromthml = new ThMLHTML();
                        fromgbf = new GBFHTML();
                        fromosis = NULL;
                        break;
                case FMT_HTMLHREF:
                        fromplain = NULL;
                        fromthml = new ThMLHTMLHREF();
                        fromgbf = new GBFHTMLHREF();
                        fromosis = new OSISHTMLHREF();
                        break;
                case FMT_RTF:
                        fromplain = NULL;
                        fromthml = new ThMLRTF();
                        fromgbf = new GBFRTF();
                        fromosis = new OSISRTF();
                        break;
                case FMT_OSIS:
                        fromplain = NULL;
                        fromthml = new ThMLOSIS();
                        fromgbf = new GBFOSIS();
                        fromosis = NULL;
                        break;
                }

}
