/******************************************************************************
 *
 * swoptfilter - SWFilter descendant and base class for all option filters
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

#include <swoptfilter.h>
#include <utilstr.h>

SWORD_NAMESPACE_START


SWOptionFilter::SWOptionFilter() {
	static StringList empty;
	static const char *empty2 = "";
	optName   = empty2;
	optTip    = empty2;
	optValues = &empty;
}

SWOptionFilter::SWOptionFilter(const char *oName, const char *oTip, const StringList *oValues) {
	optName   = oName;
	optTip    = oTip;
	optValues = oValues;
}


SWOptionFilter::~SWOptionFilter() {
}


void SWOptionFilter::setOptionValue(const char *ival) {
	for (StringList::const_iterator loop = optValues->begin(); loop != optValues->end(); loop++) {
		if (!stricmp(loop->c_str(), ival)) {
			optionValue = *loop;
			option = (!strnicmp(ival, "On", 2));	// convenience for boolean filters
			break;
		}
	}
}

const char *SWOptionFilter::getOptionValue() {
	return optionValue;
}


SWORD_NAMESPACE_END
