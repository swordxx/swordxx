/***************************************************************************
 *
 * $Id: gbfrtf.h,v 1.9 2003/02/20 07:25:19 scribe Exp $
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

#ifndef GBFRTF_H
#define GBFRTF_H

#include <swfilter.h>

SWORD_NAMESPACE_START

/** This filter converts GBF text into RTF text
 */
class SWDLLEXPORT GBFRTF : public SWFilter {
public:
	GBFRTF();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule * = 0);
};

SWORD_NAMESPACE_END
#endif
