/******************************************************************************
 *  swversion.h   - definition of class SWVersion used to compare version info
 *
 * $Id: swversion.h,v 1.9 2002/10/22 01:28:49 scribe Exp $
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

#ifndef SWORDVER
#define SWORDVER "1.5.5"
#endif

#ifndef SWVERSION_H
#define SWVERSION_H

#include <defs.h>
SWORD_NAMESPACE_START

class SWVersion {
	public:
	int major, minor, minor2, minor3;
	
	SWVersion(const char *version = "0.0");
	int compare(const SWVersion &vi) const;
	const char *getText() const;
	operator const char *() const { return getText(); }
	bool operator>(const SWVersion &vi) const {return (compare(vi) > 0);}
	bool operator<(const SWVersion &vi) const {return (compare(vi) < 0);}
	bool operator==(const SWVersion &vi) const {return (compare(vi) == 0);}

	// current sword library version
	static SWVersion currentVersion;
};

SWORD_NAMESPACE_END
#endif

