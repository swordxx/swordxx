/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filters will need and can use as a starting
 *  				point. 
 *
 * $Id: swbasicfilter.cpp,v 1.22 2003/02/20 07:53:15 scribe Exp $
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

SWORD_NAMESPACE_START

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


bool SWBasicFilter::substituteToken(SWBuf &buf, const char *token) {
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
		buf += it->second.c_str();
		return true;
	}
	return false;
}


bool SWBasicFilter::substituteEscapeString(SWBuf &buf, const char *escString) {
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
		buf += it->second.c_str();
		return true;
	}
	return false;
}


bool SWBasicFilter::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
	return substituteToken(buf, token);
}


bool SWBasicFilter::handleEscapeString(SWBuf &buf, const char *escString, DualStringMap &userData) {
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


char SWBasicFilter::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	this->key = key;
	this->module = module;
        const char *from;
        char token[4096];
	int tokpos = 0;
	bool intoken 	= false;
	bool inEsc = false;
	char escStartLen = strlen(escStart);
	char escEndLen   = strlen(escEnd);
	char escStartPos = 0, escEndPos = 0;
	char tokenStartLen = strlen(tokenStart);
	char tokenEndLen   = strlen(tokenEnd);
	char tokenStartPos = 0, tokenEndPos = 0;
	DualStringMap userData;
	std::string lastTextNode;

	bool suspendTextPassThru = false;
	userData["suspendTextPassThru"] = "false";

	SWBuf orig = text;
	from = orig.c_str();
	for (text = ""; *from; from++) {
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
					if ((!handleEscapeString(text, token, userData)) && (passThruUnknownEsc)) {
						text += escStart;
						text += token;
						text += escEnd;
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
					if ((!handleToken(text, token, userData)) && (passThruUnknownToken)) {
						text += tokenStart;
						text += token;
						text += tokenEnd;
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
				text += *from;
			lastTextNode += *from;
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
