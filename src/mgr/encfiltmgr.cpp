/******************************************************************************
 *
 *  encfiltmgr.cpp -	implementaion of class EncodingFilterMgr, subclass of
 *			SWFilterMgr, used to transcode all module text to a
 *			requested encoding
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

#include <encfiltmgr.h>
#include <utilstr.h>

#include <scsuutf8.h>
#include <latin1utf8.h>

#include <unicodertf.h>
#include <utf8latin1.h>
#include <utf8utf16.h>
#include <utf8html.h>
#include <swmodule.h>

#include <swmgr.h>


SWORD_NAMESPACE_START


/******************************************************************************
 * EncodingFilterMgr Constructor - initializes instance of EncodingFilterMgr
 *
 * ENT:
 *      enc - Encoding format to emit
 */
EncodingFilterMgr::EncodingFilterMgr(char enc)
		: SWFilterMgr() {

	scsuutf8   = new SCSUUTF8();
	latin1utf8 = new Latin1UTF8();

	encoding = enc;

	switch (encoding) {
		case ENC_LATIN1: targetenc = new UTF8Latin1(); break;
		case ENC_UTF16:  targetenc = new UTF8UTF16();  break;
		case ENC_RTF:    targetenc = new UnicodeRTF(); break;
		case ENC_HTML:   targetenc = new UTF8HTML();   break;
		default: // i.e. case ENC_UTF8
			targetenc = NULL;
	}
}


/******************************************************************************
 * EncodingFilterMgr Destructor - Cleans up instance of EncodingFilterMgr
 */
EncodingFilterMgr::~EncodingFilterMgr() {
	delete scsuutf8;
	delete latin1utf8;
	delete targetenc;
}


void EncodingFilterMgr::AddRawFilters(SWModule *module, ConfigEntMap &section) {

	ConfigEntMap::iterator entry;

	SWBuf encoding = ((entry = section.find("Encoding")) != section.end()) ? (*entry).second : (SWBuf)"";
	if (!encoding.length() || !stricmp(encoding.c_str(), "Latin-1")) {
                module->addRawFilter(latin1utf8);
	}
	else if (!stricmp(encoding.c_str(), "SCSU")) {
		module->addRawFilter(scsuutf8);
	}
}


void EncodingFilterMgr::AddEncodingFilters(SWModule *module, ConfigEntMap &section) {
	if (targetenc)
		module->addEncodingFilter(targetenc);
}


/******************************************************************************
 * EncodingFilterMgr::Encoding	- sets/gets encoding
 *
 * ENT:	enc	- new encoding or 0 to simply get the current encoding
 *
 * RET: encoding
 */
char EncodingFilterMgr::Encoding(char enc) {
	if (enc && enc != encoding) {
		encoding = enc;
		SWFilter *oldfilter = targetenc;

		switch (encoding) {
			case ENC_LATIN1: targetenc = new UTF8Latin1(); break;
			case ENC_UTF16:  targetenc = new UTF8UTF16();  break;
			case ENC_RTF:    targetenc = new UnicodeRTF(); break;
			case ENC_HTML:   targetenc = new UTF8HTML();   break;
			default: // i.e. case ENC_UTF8
				targetenc = NULL;
		}

		ModMap::const_iterator module;

		if (oldfilter != targetenc) {
			if (oldfilter) {
				if (!targetenc) {
					for (module = getParentMgr()->Modules.begin(); module != getParentMgr()->Modules.end(); module++)
						module->second->removeRenderFilter(oldfilter);
					}
				else {
					for (module = getParentMgr()->Modules.begin(); module != getParentMgr()->Modules.end(); module++)
						module->second->replaceRenderFilter(oldfilter, targetenc);
				}
				delete oldfilter;
			}
			else if (targetenc) {
				for (module = getParentMgr()->Modules.begin(); module != getParentMgr()->Modules.end(); module++)
					module->second->addRenderFilter(targetenc);
			}
		}
	}
	return encoding;
}


SWORD_NAMESPACE_END
