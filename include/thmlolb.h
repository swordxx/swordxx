/******************************************************************************
 *
 * $Id: thmlolb.h,v 1.3 2003/02/20 07:25:20 scribe Exp $
 *
 * Copyright 2001 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef THMLOLB_H
#define THMLOLB_H

#include <swfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to OLB text
 */
class SWDLLEXPORT ThMLOLB : public SWFilter {
public:
	ThMLOLB();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
};

SWORD_NAMESPACE_END
#endif
