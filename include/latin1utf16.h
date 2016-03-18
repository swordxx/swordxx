/***************************************************************************
 *
 *  latin1utf16.h -	Implementation of Latin1UTF16
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef LATIN1UTF16_H
#define LATIN1UTF16_H

#include <swfilter.h>

namespace sword {

/** This filter converts Latin-1 encoded text to UTF-16
 */
class SWDLLEXPORT Latin1UTF16 : public SWFilter {
public:
	Latin1UTF16();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
};

} /* namespace sword */
#endif
