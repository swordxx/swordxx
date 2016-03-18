/******************************************************************************
 *
 *  nullim.h -	Implementation of NullIM
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

#ifndef NULLIM_H
#define NULLIM_H

#include <swinputmeth.h>
#include <defs.h>
namespace sword {

class SWDLLEXPORT NullIM : public SWInputMethod {

public:
	NullIM();
	int * translate(char ch);
};

} /* namespace sword */
#endif
