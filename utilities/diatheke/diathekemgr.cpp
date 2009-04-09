/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

//---------------------------------------------------------------------------
#include <swmodule.h>

#ifdef _ICU_
#include <utf8arshaping.h>
#include <utf8bidireorder.h>
#include <utf8transliterator.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "diathekemgr.h"

//---------------------------------------------------------------------------
DiathekeMgr::DiathekeMgr (SWConfig * iconfig, SWConfig * isysconfig, bool autoload, char enc, char mark, bool ibidi, bool ishape)
        : SWMgr(iconfig, isysconfig, autoload, new DiathekeFilterMgr(mark, enc))
{
	bidi = ibidi;
	shape = ishape;

#ifdef _ICU_
	arshaping = new UTF8arShaping();
	bidireorder = new UTF8BiDiReorder();
	transliterator = new UTF8Transliterator();
#endif
	Load();

#ifdef WIN32
	OSVERSIONINFO osvi;
	memset (&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	platformID = osvi.dwPlatformId;
#endif

}


DiathekeMgr::~DiathekeMgr()
{
#ifdef _ICU_
        if (arshaping)
                delete arshaping;
        if (bidireorder)
                delete bidireorder;
        if (transliterator)
                delete transliterator;
#endif
}


void DiathekeMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section)
{
	SWBuf lang;
	bool rtl;
	ConfigEntMap::iterator entry;

	lang = ((entry = section.find("Lang")) != section.end()) ? (*entry).second : (SWBuf)"en";
	rtl = ((entry = section.find("Direction")) != section.end()) ? ((*entry).second == "RtoL") : false;

#ifdef _ICU_
	if (shape) {
		module->AddRenderFilter(arshaping);
	}
	if (bidi && rtl) {
		module->AddRenderFilter(bidireorder);
	}
#endif
	SWMgr::AddRenderFilters(module, section);
}

signed char DiathekeMgr::Load () {
	signed char retval =  SWMgr::Load();
#ifdef _ICU_
	optionFilters["UTF8Transliterator"] = transliterator;
	options.push_back(transliterator->getOptionName());
#endif
	return retval;
};

void DiathekeMgr::AddGlobalOptions (SWModule * module, ConfigEntMap & section,
                                   ConfigEntMap::iterator start,
                                   ConfigEntMap::iterator end) {

        SWMgr::AddGlobalOptions(module, section, start, end);
#ifdef _ICU_
        module->AddOptionFilter(transliterator);
#endif
};

