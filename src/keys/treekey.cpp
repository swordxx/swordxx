/******************************************************************************
 *  versekey.h - code for class 'versekey'- a standard Biblical verse key
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


#include <treekey.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"TreeKey", "SWKey", "SWObject", 0};
SWClass TreeKey::classdef(classes);

void TreeKey::init() {
	myclass = &classdef;
}

SWORD_NAMESPACE_END
