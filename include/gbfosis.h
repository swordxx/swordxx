/***************************************************************************
 *
 * $Id: gbfosis.h,v 1.7 2003/02/27 00:56:30 scribe Exp $
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

#ifndef GBFOSIS_H
#define GBFOSIS_H

#include <swfilter.h>
#include <string>
#include <stack>

using std::string;
using std::stack;

SWORD_NAMESPACE_START

class QuoteStack {
private:
	class QuoteInstance {
	public:
		char startChar;
		char level;
		string uniqueID;
		char continueCount;
		QuoteInstance(char startChar = '\"', char level = 1, string uniqueID = "", char continueCount = 0) {
			this->startChar     = startChar;
			this->level         = level;
			this->uniqueID      = uniqueID;
			this->continueCount = continueCount;
		}
		void pushStartStream(SWBuf &text);
	};

	stack<QuoteInstance> quotes;
public:
	QuoteStack();
	virtual ~QuoteStack();
	void handleQuote(char *buf, char *quotePos, SWBuf &text);
	void clear();
	bool empty() { return quotes.empty(); }
};

/** this filter converts GBF text to OSIS text
 */
class SWDLLEXPORT GBFOSIS : public SWFilter {
protected:
	virtual const char *convertToOSIS(const char *, const SWKey *key);
public:
	GBFOSIS();
	virtual ~GBFOSIS();
	char processText(SWBuf &text, const SWKey * = 0, const SWModule * = 0);
};

SWORD_NAMESPACE_END
#endif /* THMLOSIS_H */
