/******************************************************************************
 *
 * $Id: thmlolb.h,v 1.4 2003/02/22 07:24:20 chrislit Exp $
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

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to OLB text
 */
class SWDLLEXPORT ThMLOLB : public SWBasicFilter {
protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	ThMLOLB();
};

SWORD_NAMESPACE_END
#endif
