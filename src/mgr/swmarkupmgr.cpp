/******************************************************************************
 *  swmarkupmgr.cpp   - implementaion of class SWMarkupMgr, subclass of
 *                        used to transcode all module text to a requested
 *                        markup.
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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
#include <plainhtml.h>
#include <thmlhtmlhref.h>
#include <gbfhtmlhref.h>
#include <thmlrtf.h>
#include <gbfrtf.h>

#include <swmarkupmgr.h>
SWMarkupMgr::SWMarkupMgr (SWConfig * iconfig, SWConfig * isysconfig, bool autoload, char enc, char mark)
        : SWEncodingMgr(iconfig, isysconfig, autoload, enc)
{
        markup = mark;

        CreateFilters(markup);
}

SWMarkupMgr::~SWMarkupMgr() {
        if (fromthml)
                delete (fromthml);
        if (fromgbf)
                delete (fromgbf);
        if (fromplain)
                delete (fromplain);
        if (fromosis)
                delete (fromosis);
}

char SWMarkupMgr::Markup(char mark) {
        if (mark && mark != markup) {
                markup = mark;
                ModMap::const_iterator module;

                SWFilter * oldplain = fromplain;
                SWFilter * oldthml = fromthml;
                SWFilter * oldgbf = fromgbf;
                SWFilter * oldosis = fromosis;

                CreateFilters(markup);

                for (module = Modules.begin(); module != Modules.end(); module++)
                        switch (module->second->Markup()) {
                        case FMT_THML:
                                if (oldthml != fromthml) {
                                        if (oldthml) {
                                                if (!fromthml) {
                                                        module->second->RemoveRenderFilter(oldthml);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldthml, fromthml);
                                                }
                                        }
                                        else if (fromthml) {
                                                module->second->AddRenderFilter(fromthml);
                                        }
                                }
                                break;
                        case FMT_GBF:
                                if (oldgbf != fromgbf) {
                                        if (oldgbf) {
                                                if (!fromgbf) {
                                                        module->second->RemoveRenderFilter(oldgbf);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldgbf, fromgbf);
                                                }
                                        }
                                        else if (fromgbf) {
                                                module->second->AddRenderFilter(fromgbf);
                                        }
                                        break;
                                }
                        case FMT_PLAIN:
                                if (oldplain != fromplain) {
                                        if (oldplain) {
                                                if (!fromplain) {
                                                        module->second->RemoveRenderFilter(oldplain);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldplain, fromplain);
                                                }
                                        }
                                        else if (fromplain) {
                                                module->second->AddRenderFilter(fromplain);
                                        }
                                        break;
                                }
                        case FMT_OSIS:
                                if (oldosis != fromosis) {
                                        if (oldosis) {
                                                if (!fromosis) {
                                                        module->second->RemoveRenderFilter(oldosis);
                                                }
                                                else {
                                                        module->second->ReplaceRenderFilter(oldosis, fromosis);
                                                }
                                        }
                                        else if (fromosis) {
                                                module->second->AddRenderFilter(fromosis);
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

void SWMarkupMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
        SWMgr::AddRenderFilters(module, section);
        switch (module->Markup()) {
        case FMT_THML:
                if (fromthml)
                        module->AddRenderFilter(fromthml);
                break;
        case FMT_GBF:
                if (fromgbf)
                        module->AddRenderFilter(fromgbf);
                break;
        case FMT_PLAIN:
                if (fromplain)
                        module->AddRenderFilter(fromplain);
                break;
        case FMT_OSIS:
                if (fromosis)
                        module->AddRenderFilter(fromosis);
                break;
        }
}

void SWMarkupMgr::CreateFilters(char markup) {

                switch (markup) {
                case FMT_PLAIN:
                        fromplain = NULL;
                        fromthml = new ThMLPlain();
                        fromgbf = new GBFPlain();
                        fromosis = NULL;
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
                        fromplain = new PLAINHTML();
                        fromthml = new ThMLHTML();
                        fromgbf = new GBFHTML();
                        fromosis = NULL;
                        break;
                case FMT_HTMLHREF:
                        fromplain = NULL;
                        fromthml = new ThMLHTMLHREF();
                        fromgbf = new GBFHTMLHREF();
                        fromosis = NULL;
                        break;
                case FMT_RTF:
                        fromplain = NULL;
                        fromthml = new ThMLRTF();
                        fromgbf = new GBFRTF();
                        fromosis = NULL;
                        break;
                case FMT_OSIS:
                        fromplain = NULL;
                        fromthml = NULL;
                        fromgbf = NULL;
                        fromosis = NULL;
                        break;
                }

}
