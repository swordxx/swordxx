/***************************************************************************
                          gbfosis.h  -  description
                             -------------------
    begin                : 2001-09-03
    copyright            : 2001 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GBFOSIS_H
#define GBFOSIS_H

#include <defs.h>
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
		void pushStartStream(char **to);
	};

	stack<QuoteInstance> quotes;
public:
	QuoteStack();
	virtual ~QuoteStack();
	void handleQuote(char *buf, char *quotePos, char **to);
	void clear();
	bool empty() { return quotes.empty(); }
};

/** this filter converts GBF text to HTML text with hrefs
 */
class SWDLLEXPORT GBFOSIS : public SWFilter {
protected:
	virtual const char *convertToOSIS(const char *, const SWKey *key);
public:
	GBFOSIS();
	virtual ~GBFOSIS();
	char ProcessText(char *text, int maxlen, const SWKey *, const SWModule * = 0);
	static void pushString(char **buf, const char *format, ...);
};

SWORD_NAMESPACE_END
#endif /* THMLOSIS_H */
