/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filters will need and can use as a starting
 *  				point. 
 *
 * $Id: swbasicfilter.cpp,v 1.18 2002/06/06 21:08:47 scribe Exp $
 *
 * Copyright 2001 CrossWire Bible Society (http://www.crosswire.org)
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
#include <string.h>
#include <swbasicfilter.h>
#include <stdio.h>
#include <stdarg.h>

SWBasicFilter::SWBasicFilter() {
	tokenStart = 0;
	tokenEnd = 0;
	escStart = 0;
	escEnd = 0;

	setTokenStart("<");
	setTokenEnd(">");
	setEscapeStart("&");
	setEscapeEnd(";");

	escStringCaseSensitive = false;
	tokenCaseSensitive = false;
	passThruUnknownToken = false;
	passThruUnknownEsc = false;
}


void SWBasicFilter::setPassThruUnknownToken(bool val) {
	passThruUnknownToken = val;
}


void SWBasicFilter::setPassThruUnknownEscapeString(bool val) {
	passThruUnknownEsc = val;
}


void SWBasicFilter::setTokenCaseSensitive(bool val) {
	tokenCaseSensitive = val;
}


void SWBasicFilter::setEscapeStringCaseSensitive(bool val) {
	escStringCaseSensitive = val;
}


SWBasicFilter::~SWBasicFilter() {
	if (tokenStart)
		delete [] tokenStart;

	if (tokenEnd)
		delete [] tokenEnd;

	if (escStart)
		delete [] escStart;

	if (escEnd)
		delete [] escEnd;
}


void SWBasicFilter::addTokenSubstitute(const char *findString, const char *replaceString) {
	char *buf = 0;

	if (!tokenCaseSensitive) {
		stdstr(&buf, findString);
		toupperstr(buf);
		tokenSubMap.insert(DualStringMap::value_type(buf, replaceString));
		delete [] buf;
	}
	else tokenSubMap.insert(DualStringMap::value_type(findString, replaceString));
}


void SWBasicFilter::addEscapeStringSubstitute(const char *findString, const char *replaceString) {
	char *buf = 0;

	if (!escStringCaseSensitive) {
		stdstr(&buf, findString);
		toupperstr(buf);
		escSubMap.insert(DualStringMap::value_type(buf, replaceString));
		delete [] buf;
	}
	else escSubMap.insert(DualStringMap::value_type(findString, replaceString));
}


void SWBasicFilter::pushString(char **buf, const char *format, ...) {
  va_list argptr;

  va_start(argptr, format);
  *buf += vsprintf(*buf, format, argptr);
  va_end(argptr);

//  *buf += strlen(*buf);
}


bool SWBasicFilter::substituteToken(char **buf, const char *token) {
	DualStringMap::iterator it;

	if (!tokenCaseSensitive) {
	        char *tmp = 0;
		stdstr(&tmp, token);
		toupperstr(tmp);
		it = tokenSubMap.find(tmp);
		delete [] tmp;
	} else
	it = tokenSubMap.find(token);

	if (it != tokenSubMap.end()) {
		pushString(buf, it->second.c_str());
		return true;
	}
	return false;
}


bool SWBasicFilter::substituteEscapeString(char **buf, const char *escString) {
	DualStringMap::iterator it;

	if (!escStringCaseSensitive) {
	        char *tmp = 0;
		stdstr(&tmp, escString);
		toupperstr(tmp);
		it = escSubMap.find(tmp);
		delete [] tmp;
	} else 
	it = escSubMap.find(escString);

	if (it != escSubMap.end()) {
		pushString(buf, it->second.c_str());
		return true;
	}
	return false;
}


bool SWBasicFilter::handleToken(char **buf, const char *token, DualStringMap &userData) {
	return substituteToken(buf, token);
}


bool SWBasicFilter::handleEscapeString(char **buf, const char *escString, DualStringMap &userData) {
	return substituteEscapeString(buf, escString);
}


void SWBasicFilter::setEscapeStart(const char *escStart) {
	stdstr(&(this->escStart), escStart);
}


void SWBasicFilter::setEscapeEnd(const char *escEnd) {
	stdstr(&(this->escEnd), escEnd);
}


void SWBasicFilter::setTokenStart(const char *tokenStart) {
	stdstr(&(this->tokenStart), tokenStart);
}


void SWBasicFilter::setTokenEnd(const char *tokenEnd) {
	stdstr(&(this->tokenEnd), tokenEnd);
}


char SWBasicFilter::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {
	this->key = key;
	this->module = module;
	char *to, *from, token[4096];
	int tokpos = 0;
	bool intoken 	= false;
	int len;
	bool inEsc = false;
	char escStartLen = strlen(escStart);
	char escEndLen   = strlen(escEnd);
	char escStartPos = 0, escEndPos = 0;
	char tokenStartLen = strlen(tokenStart);
	char tokenEndLen   = strlen(tokenEnd);
	char tokenStartPos = 0, tokenEndPos = 0;
	DualStringMap userData;
	string lastTextNode;

	bool suspendTextPassThru = false;
	userData["suspendTextPassThru"] = "false";

	len = strlen(text) + 1;		// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;			// -------------------------------

	resultBuffer = text;

	for (to = text; *from; from++) {
		if (*from == tokenStart[tokenStartPos]) {
			if (tokenStartPos == (tokenStartLen - 1)) {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				inEsc = false;
			}
			else tokenStartPos++;
			continue;
		}

		if (*from == escStart[escStartPos]) {
			if (escStartPos == (escStartLen - 1)) {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				inEsc = true;
			}
			else escStartPos++;
			continue;
		}

		if (inEsc) {
			if (*from == escEnd[escEndPos]) {
				if (escEndPos == (escEndLen - 1)) {
					intoken = false;
					userData["lastTextNode"] = lastTextNode;
					if ((!handleEscapeString(&to, token, userData)) && (passThruUnknownEsc)) {
						pushString(&to, escStart);
						pushString(&to, token);
						pushString(&to, escEnd);
					}
					escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
					lastTextNode = "";
					suspendTextPassThru = (!userData["suspendTextPassThru"].compare("true"));
					continue;
				}
			}
		}

		if (!inEsc) {
			if (*from == tokenEnd[tokenEndPos]) {
				if (tokenEndPos == (tokenEndLen - 1)) {
					intoken = false;
					userData["lastTextNode"] = lastTextNode;
					if ((!handleToken(&to, token, userData)) && (passThruUnknownToken)) {
						pushString(&to, tokenStart);
						pushString(&to, token);
						pushString(&to, tokenEnd);
					}
					escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
					lastTextNode = "";
					suspendTextPassThru = (!userData["suspendTextPassThru"].compare("true"));
					continue;
				}
			}
		}

		if (intoken) {
			if (tokpos < 4090)
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
		}
		else {
			if (!suspendTextPassThru)
				*to++ = *from;
			lastTextNode += *from;
		}
	}
	*to++ = 0;
	*to = 0;
	return 0;
}



