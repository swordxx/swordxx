/******************************************************************************
 *
 * $Id: osisplain.h,v 1.3 2003/06/27 01:41:06 scribe Exp $
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

#ifndef OSISPLAIN_H
#define OSISPLAIN_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts OSIS text to plain text
 */
class SWDLLEXPORT OSISPlain : public SWBasicFilter {
public:
protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	OSISPlain();
};

SWORD_NAMESPACE_END
#endif
