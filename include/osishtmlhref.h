/******************************************************************************
 *
 * $Id: osishtmlhref.h,v 1.2 2003/06/24 12:19:56 tbiggs Exp $
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

#include <string>

using std::string;

SWORD_NAMESPACE_START

/** this filter converts OSIS text to HTML text with hrefs
 */
class SWDLLEXPORT OSISHTMLHref : public SWBasicFilter {
private:
	string tagData;
	string::size_type pos1;
	string::size_type pos2;

protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	OSISHTMLHref();
};

SWORD_NAMESPACE_END
#endif
