/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filter will need and can use as a starting
 *  				point. 
 *
 * $Id: swbasicfilter.h,v 1.1 2001/09/02 08:10:41 scribe Exp $
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

#ifndef SWBASICFILTER_H
#define SWBASICFILTER_H

#include <swfilter.h>

#include <defs.h>
#include <map>
using namespace std;


class SWDLLEXPORT SWBasicFilter : public SWFilter {

	char *tokenStart;
	char *tokenEnd;
	char *escStart;
	char *escEnd;
	bool escStringCaseSensitive;
	bool tokenCaseSensitive;
	bool passThruUnknownToken;
	bool passThruUnknownEsc;

public:
	SWBasicFilter();
	virtual char ProcessText(char *text, int maxlen = -1);
	virtual ~SWBasicFilter();

protected:
	typedef map<string, string> DualStringMap;
	DualStringMap tokenSubMap;
	DualStringMap escSubMap;
	void setEscapeStart(const char *escStart);
	void setEscapeEnd(const char *escEnd);
	void setTokenStart(const char *tokenStart);
	void setTokenEnd(const char *tokenEnd);
	void setPassThruUnknownToken(bool val);
	void setPassThruUnknownEscapeString(bool val);
	void setTokenCaseSensitive(bool val);
	void setEscapeStringCaseSensitive(bool val);
	void addTokenSubstitute(const char *findString, const char *replaceString);
	void addEscapeStringSubstitute(const char *findString, const char *replaceString);
	bool substituteToken(char **buf, const char *token);
	bool substituteEscapeString(char **buf, const char *escString);
	void pushString(char **buf, const char *fragToPush);

	virtual void init();
	virtual bool handleToken(char **buf, const char *token);
	virtual bool handleEscapeString(char **buf, const char *escString);
};

#endif
