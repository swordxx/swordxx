/*
 *
 * $Id: gbffootnotes.h,v 1.5 2001/02/08 09:20:48 chrislit Exp $
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

#ifndef GBFFOOTNOTES_H
#define GBFFOOTNOTES_H

#include <swfilter.h>

#include <defs.h>

class SWDLLEXPORT GBFFootnotes : public SWFilter {
	bool option;
	static const char on[];
	static const char off[];
	static const char optName[];
	static const char optTip[];
	OptionsList options;
public:
	GBFFootnotes();
	virtual ~GBFFootnotes();
	virtual char ProcessText(char *text, int maxlen, const SWKey *key);
	virtual const char *getOptionName() { return optName; }
	virtual const char *getOptionTip() { return optTip; }
	virtual void setOptionValue(const char *ival);
	virtual const char *getOptionValue();
	virtual OptionsList getOptionValues() { return options; }
};

#endif
