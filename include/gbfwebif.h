/******************************************************************************
 *
 * $Id: gbfwebif.h,v 1.1 2003/06/01 14:32:09 joachim Exp $
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

#ifndef GBFWEBIF_H
#define GBFWEBIF_H

#include <gbfhtmlhref.h>
#include <string>

SWORD_NAMESPACE_START

/** this filter converts GBF  text to HTML text with hrefs
 */
class SWDLLEXPORT GBFWEBIF : public GBFHTMLHREF {
	const std::string baseURL;
	const std::string passageStudyURL;

protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	GBFWEBIF();
};

SWORD_NAMESPACE_END
#endif
