/******************************************************************************
 *  swbasicfilter.h	- definition of class SWBasicFilter.  An SWFilter
 *  				impl that provides some basic methods that
 *  				many filters will need and can use as a starting
 *  				point. 
 *
 * $Id: swbasicfilter.cpp,v 1.9 2001/11/03 20:54:05 chrislit Exp $
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


void SWBasicFilter::pushString(char **buf, const char *fragToPush, const char *fragToPush2, const char *fragToPush3, const char *fragToPush4, const char *fragToPush5, const char *fragToPush6) {
  while (*fragToPush) {
    **buf = *fragToPush++;
    (*buf)++;
  }
  if (fragToPush2) {
    while (*fragToPush2) {
      **buf = *fragToPush2++;
      (*buf)++;
    }
    if (fragToPush3) {
      while (*fragToPush3) {
	**buf = *fragToPush3++;
	(*buf)++;
      }
      if (fragToPush4) {
	while (*fragToPush4) {
	  **buf = *fragToPush4++;
	  (*buf)++;
	}
	if (fragToPush5) {
	  while (*fragToPush5) {
	    **buf = *fragToPush5++;
	    (*buf)++;
	  }
	  if (fragToPush6) {
	    while (*fragToPush6) {
	      **buf = *fragToPush6++;
	      (*buf)++;
	    }
	  }
	}
      }
    }
  }
}


bool SWBasicFilter::substituteToken(char **buf, const char *token) {
	DualStringMap::iterator it;
	it = tokenSubMap.find(token);
	if (it != tokenSubMap.end()) {
		pushString(buf, it->second.c_str());
		return true;
	}
	return false;
}


bool SWBasicFilter::substituteEscapeString(char **buf, const char *escString) {
	DualStringMap::iterator it;
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


char SWBasicFilter::ProcessText(char *text, int maxlen) {
	char *to, *from, token[2048];
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


	for (to = text; *from; from++) {
		if (*from == tokenStart[tokenStartPos]) {
			if (tokenStartPos == (tokenStartLen - 1)) {
				intoken = true;
				tokpos = 0;
				memset(token, 0, 2048);
				inEsc = false;
			}
			else tokenStartPos++;
			continue;
		}

		if (*from == escStart[escStartPos]) {
			if (escStartPos == (escStartLen - 1)) {
				intoken = true;
				tokpos = 0;
				memset(token, 0, 2048);
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
			if (tokpos < 2047)
				token[tokpos++] = *from;
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

