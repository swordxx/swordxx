/******************************************************************************
 *  swfilter.h	- definition of class SWFilter used to filter text between
 *				different formats
 *
 * $Id: swfilter.h,v 1.15 2003/02/20 07:25:20 scribe Exp $
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

#ifndef SWFILTER_H
#define SWFILTER_H

#include <defs.h>
#include <swbuf.h>
#include <list>
#include <swkey.h>

SWORD_NAMESPACE_START

/**
* The type definitoin for option types
*/
typedef std::list < SWBuf > OptionsList;


/** Base class for all filters in sword.
* Filters are used to filter/convert text between different formats
* like GBF, HTML, RTF ...
*/
class SWModule;

class SWDLLEXPORT  SWFilter {
public:
	virtual ~SWFilter() {}

	/** gets the name of the option of this filter
	 * @return option name
	 */
	virtual const char *getOptionName () { return 0; }

	/** gets a short explanation of the option of this filter;
	 * it could be presented to the user in frontend programs
	 * @return option tip/explanation
	 */
	virtual const char *getOptionTip () { return 0; }

	/** returns a list of the possible option values
	 * might be
	 * @return list of option values
	 */
	virtual OptionsList getOptionValues () {
		OptionsList options;
		return options;
	}

	/** sets the value of the option of this filter,
	 * e.g maybe a strong's filter mioght be set to "on" / "off" -
	 * that would mean to show or not to show the strongs in the text,
	 * see also @ref getOptionValues
	 * @param ival the new option valus
	 */
	virtual void setOptionValue (const char *) { }
	virtual const char *getOptionValue () { return 0; }

	/** This is the main filter function
	 * @param text the text to be filtered/converted
	 * @param maxlen maximal length of text to be processed
	 * @param key sorry I don't know
	 * @return 0
	 */
	virtual char processText(SWBuf &text, const SWKey * = 0, const SWModule * = 0) = 0;
};

	SWORD_NAMESPACE_END
#endif
