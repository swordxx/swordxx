/******************************************************************************
 *
 *  swobject.cpp -	code for SWClass used as lowest base class for many
 *			SWORD objects
 *
 * $Id$
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <swobject.h>

// hack.  remove this when we figure out our link error
#ifndef __CYGWIN__
#include <utilstr.h>
#else
#include <string.h>
#endif


SWORD_NAMESPACE_START


bool SWClass::isAssignableFrom(const char *className) const {
	for (int i = 0; descends[i]; i++) {
#ifndef __CYGWIN__
		if (!sword::stricmp(descends[i], className))
#else
		if (!stricmp(descends[i], className))
#endif
			return true;
	}
	return false;
}


SWORD_NAMESPACE_END

