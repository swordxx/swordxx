/******************************************************************************
 *  swld.cpp   - code for base class 'SWLD'.  SWLD is the basis for all
 *		 types of Lexicon and Dictionary modules (hence the 'LD').
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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


#include <swgenbook.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWLD Constructor - Initializes data for instance of SWLD
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

SWGenBook::SWGenBook(const char *imodname, const char *imoddesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : SWModule(imodname, imoddesc, idisp, (char *)"Generic Books", enc, dir, mark, ilang) {
}


/******************************************************************************
 * SWLD Destructor - Cleans up instance of SWLD
 */

SWGenBook::~SWGenBook() {
}

SWORD_NAMESPACE_END
