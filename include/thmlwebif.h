/******************************************************************************
 *
 * $Id: thmlwebif.h,v 1.1 2003/06/01 14:32:09 joachim Exp $
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

#ifndef _ThMLWEBIF_H
#define _ThMLWEBIF_H

#include <thmlhtmlhref.h>
#include <string>

SWORD_NAMESPACE_START

/** this filter converts ThML text to HTML text with hrefs
 */
class SWDLLEXPORT ThMLWEBIF : public ThMLHTMLHREF {
	const std::string baseURL;
	const std::string passageStudyURL;

protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	ThMLWEBIF();
};
SWORD_NAMESPACE_END
#endif /* _ThMLWEBIF_H */
