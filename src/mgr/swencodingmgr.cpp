/******************************************************************************
 *  swencodingmgr.cpp   - implementaion of class SWEncodingMgr, subclass of
 *                        used to transcode all module text to a requested
 *                        encoding.
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

#include <swencodingmgr.h>

#include <scsuutf8.h>
#include <latin1utf8.h>

#include <unicodertf.h>
#include <utf8latin1.h>
#include <utf8utf16.h>
#include <utf8html.h>

SWEncodingMgr::SWEncodingMgr (SWConfig * iconfig, SWConfig * isysconfig, bool autoload, char enc)
        : SWMgr(iconfig, isysconfig, autoload)
{
        scsuutf8 = new SCSUUTF8();
        latin1utf8 = new Latin1UTF8();

        encoding = enc;

        switch (encoding) {
        case ENC_LATIN1:
                targetenc = new UTF8Latin1();
                break;
        case ENC_UTF16:
                targetenc = new UTF8UTF16();
                break;
        case ENC_RTF:
                targetenc = new UnicodeRTF();
                break;
        case ENC_HTML:
                targetenc = new UTF8HTML();
                break;
        default: // i.e. case ENC_UTF8
                targetenc = NULL;
        }
}

SWEncodingMgr::~SWEncodingMgr() {
        if (scsuutf8)
                delete scsuutf8;
        if (latin1utf8)
                delete latin1utf8;
        if (targetenc)
                delete targetenc;
}

void SWEncodingMgr::AddRawFilters(SWModule *module, ConfigEntMap &section) {
        SWMgr::AddRawFilters(module, section);

	ConfigEntMap::iterator entry;

	string encoding = ((entry = section.find("Encoding")) != section.end()) ? (*entry).second : (string)"";
	if (encoding.empty() || !stricmp(encoding.c_str(), "Latin-1")) {
                module->AddRawFilter(latin1utf8);
	}
        else if (!stricmp(encoding.c_str(), "SCSU")) {
                module->AddRawFilter(scsuutf8);
	}
}

void SWEncodingMgr::AddEncodingFilters(SWModule *module, ConfigEntMap &section) {
        SWMgr::AddEncodingFilters(module, section);
        if (targetenc)
                module->AddEncodingFilter(targetenc);
}

char SWEncodingMgr::Encoding(char enc) {
        if (enc && enc != encoding) {
                encoding = enc;
                SWFilter * oldfilter = targetenc;
                
                switch (encoding) {
                case ENC_LATIN1:
                        targetenc = new UTF8Latin1();
                        break;
                case ENC_UTF16:
                        targetenc = new UTF8UTF16();
                        break;
                case ENC_RTF:
                        targetenc = new UnicodeRTF();
                        break;
                case ENC_HTML:
                        targetenc = new UTF8HTML();
                        break;
                default: // i.e. case ENC_UTF8
                        targetenc = NULL;
                }

                ModMap::const_iterator module;

                if (oldfilter != targetenc) {
                        if (oldfilter) {
                                if (!targetenc) {
                                        for (module = Modules.begin(); module != Modules.end(); module++)
                                                module->second->RemoveRenderFilter(oldfilter);
                                }
                                else {
                                        for (module = Modules.begin(); module != Modules.end(); module++)
                                                module->second->ReplaceRenderFilter(oldfilter, targetenc);
                                }
                                delete oldfilter;
                        }
                        else if (targetenc) {
                                for (module = Modules.begin(); module != Modules.end(); module++)
                                        module->second->AddRenderFilter(targetenc);
                        }
                }

        }
        return encoding;
}
