/******************************************************************************
*  swobject.cpp  - code for SWObject used as lowest base class for many
*	SWORD objects
*
* $Id: swobject.cpp 1785 2005-04-30 19:11:12Z scribe $
*
* Copyright 2005 CrossWire Bible Society (http://www.crosswire.org)
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
#include <utilstr.h>


SWORD_NAMESPACE_START

bool SWClass::isAssignableFrom(const char *className) const {
	for (int i = 0; descends[i]; i++) {
		if (!stricmp(descends[i], className))
			return true;
	}
	return false;
}

SWORD_NAMESPACE_END
