/******************************************************************************
 *
 * $Id: thmlrtf.h,v 1.7 2003/02/20 07:25:20 scribe Exp $
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

#ifndef THMLRTF_H
#define THMLRTF_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to RTF text
 */
class SWDLLEXPORT ThMLRTF : public SWBasicFilter {
protected:
	virtual bool handleToken(SWBuf &buf, const char *token, DualStringMap &userData);
public:
	ThMLRTF();
};

SWORD_NAMESPACE_END
#endif
