/******************************************************************************
 *
 * $Id: thmlhtmlhref.h,v 1.4 2003/02/20 07:25:20 scribe Exp $
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

#ifndef _THMLHTMLHREF_H
#define _THMLHTMLHREF_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to HTML text with hrefs
 */
class SWDLLEXPORT ThMLHTMLHREF : public SWBasicFilter {
protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	ThMLHTMLHREF();
};
SWORD_NAMESPACE_END
#endif /* _THMLHTMLHREF_H */
