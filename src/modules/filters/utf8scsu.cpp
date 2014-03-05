/******************************************************************************
 *
 *  utf8scsu.cpp -	SWFilter descendant to convert UTF-8 to SCSU
 *
 * $Id$
 *
 * Copyright 2001-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdlib.h>
#include <stdio.h>

#include <sysdata.h>
#include <utf8scsu.h>
#include <utilstr.h>
#include <swbuf.h>


SWORD_NAMESPACE_START


UTF8SCSU::UTF8SCSU() {
}


char UTF8SCSU::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	const unsigned char *from;
	SWBuf orig = text;

	from = (const unsigned char *)orig.c_str();


	return 0;
}

SWORD_NAMESPACE_END
