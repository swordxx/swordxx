/******************************************************************************
 *
 * $Id: osishtmlhref.h,v 1.5 2003/08/07 23:23:24 chrislit Exp $
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

#ifndef OSISHTMLHREF_H
#define OSISHTMLHREF_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts OSIS text to HTML text with hrefs
 */
class SWDLLEXPORT OSISHTMLHref : public SWBasicFilter {
private:
protected:
	class MyUserData : public SWFilterUserData {
	public:
		bool osisQToTick;
		bool inBold;
		SWBuf w;
		SWBuf fn;
		MyUserData(const SWModule *module, const SWKey *key);
	};
	virtual SWFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
		return new MyUserData(module, key);
	}
	virtual bool handleToken(SWBuf &buf, const char *token, SWFilterUserData *userData);
public:
	OSISHTMLHref();
};

SWORD_NAMESPACE_END
#endif
