/******************************************************************************
 *  utilxml.h   - definition of class that deal with xml constructs 
 *
 * $Id: utilweb.h,v 1.2 2003/06/27 01:41:07 scribe Exp $
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

#ifndef UTILWEB_H
#define UTILWEB_H

#include <defs.h>
#include <swbuf.h>

SWORD_NAMESPACE_START

/** Encodes and URL
* Encodes a string into a valid URL, e.g. changes http://www.crosswire.org/test.jsp?force=1&help=1 into
* http://www.crosswire.org/test.jsp?force=1&amp;help=1
* This function works on the data of the buf parameter.
*/
const SWBuf encodeURL( const SWBuf& buf );

SWORD_NAMESPACE_END

#endif

