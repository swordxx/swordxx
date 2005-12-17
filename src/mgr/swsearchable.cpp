/******************************************************************************
 *  swsearchable.h	- definition of class SWSearchable used to provide an
 *	interface for objects that be searched.
 *
 * $Id$
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

#include <swsearchable.h>

SWORD_NAMESPACE_START

void SWSearchable::nullPercent(char percent, void *percentUserData) {}

SWSearchable::SWSearchable() {
}


SWSearchable::~SWSearchable() {
}

	// special search framework
signed char SWSearchable::createSearchFramework(void (*percent)(char, void *), void *percentUserData) {
	return 0;
}


void SWSearchable::deleteSearchFramework() {
}


bool SWSearchable::isSearchOptimallySupported(const char *istr, int searchType, int flags, SWKey *scope) {
	bool retVal = false;
	search(istr, searchType, flags, scope, &retVal);
	return retVal;
}

SWORD_NAMESPACE_END
