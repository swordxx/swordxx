/***************************************************************************
 *
 * $Id: swoptfilter.h,v 1.2 2003/07/05 08:53:05 mgruner Exp $
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

#ifndef SWOPTFILTER_H
#define SWOPTFILTER_H

#include <swfilter.h>

SWORD_NAMESPACE_START

  /** Base class for all option filters.
  */
class SWDLLEXPORT SWOptionFilter : public SWFilter {
protected:
	SWBuf optionValue;
	const char *optName;
	const char *optTip;
	const StringList *optValues;
	bool option;
public:
	SWOptionFilter(const char *oName, const char *oTip, const StringList *oValues);
	virtual ~SWOptionFilter();
	virtual const char *getOptionName() { return optName; }
	virtual const char *getOptionTip() { return optTip; }
	virtual void setOptionValue(const char *ival);
	virtual const char *getOptionValue();
	virtual StringList getOptionValues() { return *optValues; }
};

SWORD_NAMESPACE_END
#endif
