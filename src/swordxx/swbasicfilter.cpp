/******************************************************************************
 *
 *  swbasicfilter.cpp -    definition of class SWBasicFilter.  An SWFilter
 *              impl that provides some basic methods that
 *              many filters will need and can use as a starting
 *              point.
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include "swbasicfilter.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include "stringmgr.h"
#include "utilstr.h"


namespace swordxx {


typedef std::map<std::string, std::string> DualStringMap;
typedef std::set<std::string> StringSet;


// I hate bridge patterns but this isolates std::map from a ton of filters
class SWBasicFilter::Private {
public:
    DualStringMap tokenSubMap;
    DualStringMap escSubMap;
    StringSet escPassSet;
};


const char SWBasicFilter::INITIALIZE = 1;
const char SWBasicFilter::PRECHAR    = 2;
const char SWBasicFilter::POSTCHAR   = 4;
const char SWBasicFilter::FINALIZE   = 8;


SWBasicFilter::SWBasicFilter() {

    p = new Private;

    processStages = 0;
    tokenStart    = nullptr;
    tokenEnd      = nullptr;
    escStart      = nullptr;
    escEnd        = nullptr;

    setTokenStart("<");
    setTokenEnd(">");
    setEscapeStart("&");
    setEscapeEnd(";");

    escStringCaseSensitive = false;
    tokenCaseSensitive     = false;
    passThruUnknownToken   = false;
    passThruUnknownEsc     = false;
    passThruNumericEsc     = false;
}


SWBasicFilter::~SWBasicFilter() {
    delete[] tokenStart;
    delete[] tokenEnd;
    delete[] escStart;
    delete[] escEnd;
    delete p;
}


void SWBasicFilter::setPassThruUnknownToken(bool val) {
    passThruUnknownToken = val;
}


void SWBasicFilter::setPassThruUnknownEscapeString(bool val) {
    passThruUnknownEsc = val;
}


void SWBasicFilter::setPassThruNumericEscapeString(bool val) {
    passThruUnknownEsc = val;
}


void SWBasicFilter::setTokenCaseSensitive(bool val) {
    tokenCaseSensitive = val;
}


void SWBasicFilter::setEscapeStringCaseSensitive(bool val) {
    escStringCaseSensitive = val;
}


void SWBasicFilter::addTokenSubstitute(const char *findString, const char *replaceString) {
    char * buf = nullptr;

    if (!tokenCaseSensitive) {
        stdstr(&buf, findString);
        toupperstr(buf);
        p->tokenSubMap[buf] = replaceString;
        delete [] buf;
    }
    else p->tokenSubMap[findString] = replaceString;
}


void SWBasicFilter::removeTokenSubstitute(const char *findString) {
    if (p->tokenSubMap.find(findString) != p->tokenSubMap.end()) {
        p->tokenSubMap.erase( p->tokenSubMap.find(findString) );
    }
}


void SWBasicFilter::addAllowedEscapeString(const char *findString) {
    char * buf = nullptr;

    if (!escStringCaseSensitive) {
        stdstr(&buf, findString);
        toupperstr(buf);
        p->escPassSet.insert(StringSet::value_type(buf));
        delete [] buf;
    }
    else p->escPassSet.insert(StringSet::value_type(findString));
}


void SWBasicFilter::removeAllowedEscapeString(const char *findString) {
    if (p->escPassSet.find(findString) != p->escPassSet.end()) {
        p->escPassSet.erase( p->escPassSet.find(findString) );
    }
}


void SWBasicFilter::addEscapeStringSubstitute(const char *findString, const char *replaceString) {
    char * buf = nullptr;

    if (!escStringCaseSensitive) {
        stdstr(&buf, findString);
        toupperstr(buf);
        p->escSubMap.insert(DualStringMap::value_type(buf, replaceString));
        delete [] buf;
    }
    else p->escSubMap.insert(DualStringMap::value_type(findString, replaceString));
}


void SWBasicFilter::removeEscapeStringSubstitute(const char *findString) {
    if (p->escSubMap.find(findString) != p->escSubMap.end()) {
        p->escSubMap.erase( p->escSubMap.find(findString) );
    }
}


bool SWBasicFilter::substituteToken(std::string &buf, const char *token) {
    DualStringMap::iterator it;

    if (!tokenCaseSensitive) {
        char * tmp = nullptr;
        stdstr(&tmp, token);
        toupperstr(tmp);
        it = p->tokenSubMap.find(tmp);
        delete [] tmp;
    } else
    it = p->tokenSubMap.find(token);

    if (it != p->tokenSubMap.end()) {
        buf += it->second;
        return true;
    }
    return false;
}


void SWBasicFilter::appendEscapeString(std::string &buf, const char *escString) {
    buf += escStart;
    buf += escString;
    buf += escEnd;
}


bool SWBasicFilter::passAllowedEscapeString(std::string &buf, const char *escString) {
    StringSet::iterator it;

    if (!escStringCaseSensitive) {
        char * tmp = nullptr;
        stdstr(&tmp, escString);
        toupperstr(tmp);
        it = p->escPassSet.find(tmp);
        delete [] tmp;
    } else
        it = p->escPassSet.find(escString);

    if (it != p->escPassSet.end()) {
        appendEscapeString(buf, escString);
        return true;
    }

    return false;
}


bool SWBasicFilter::handleNumericEscapeString(std::string &buf, const char *escString) {
    if (passThruNumericEsc) {
        appendEscapeString(buf, escString);
        return true;
    }
    return false;
}


bool SWBasicFilter::substituteEscapeString(std::string &buf, const char *escString) {
    DualStringMap::iterator it;

    if (*escString == '#') {
        return handleNumericEscapeString(buf, escString);
    }

    if (passAllowedEscapeString(buf, escString)) {
        return true;
    }

    if (!escStringCaseSensitive) {
        char * tmp = nullptr;
        stdstr(&tmp, escString);
        toupperstr(tmp);
        it = p->escSubMap.find(tmp);
        delete [] tmp;
    } else
    it = p->escSubMap.find(escString);

    if (it != p->escSubMap.end()) {
        buf += it->second;
        return true;
    }
    return false;
}


bool SWBasicFilter::handleToken(std::string &buf, const char *token, BasicFilterUserData * /* userData */) {
    return substituteToken(buf, token);
}


bool SWBasicFilter::handleEscapeString(std::string &buf, const char *escString, BasicFilterUserData * /* userData */) {
    return substituteEscapeString(buf, escString);
}


void SWBasicFilter::setEscapeStart(const char *escStart) {
    stdstr(&(this->escStart), escStart);
    escStartLen = strlen(escStart);
}


void SWBasicFilter::setEscapeEnd(const char *escEnd) {
    stdstr(&(this->escEnd), escEnd);
    escEndLen   = strlen(escEnd);
}


void SWBasicFilter::setTokenStart(const char *tokenStart) {
    stdstr(&(this->tokenStart), tokenStart);
    tokenStartLen = strlen(tokenStart);
}


void SWBasicFilter::setTokenEnd(const char *tokenEnd) {
    stdstr(&(this->tokenEnd), tokenEnd);
    tokenEndLen   = strlen(tokenEnd);
}


char SWBasicFilter::processText(std::string &text, const SWKey *key, const SWModule *module) {
    char *from;
    char token[4096];
    int tokpos = 0;
    bool intoken = false;
    bool inEsc = false;
    int escStartPos = 0, escEndPos = 0;
    int tokenStartPos = 0, tokenEndPos = 0;
    std::string lastTextNode;
    BasicFilterUserData *userData = createUserData(module, key);

    std::string orig = text;
    from = &orig[0u];
    text = "";

    if (processStages & INITIALIZE) {
        if (processStage(INITIALIZE, text, from, userData)) {    // processStage handled it all
            delete userData;
            return 0;
        }
    }

    for (;*from; from++) {

        if (processStages & PRECHAR) {
            if (processStage(PRECHAR, text, from, userData))    // processStage handled this char
                continue;
        }

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
                    intoken = inEsc = false;
                    userData->lastTextNode = lastTextNode;

                    if (!userData->suspendTextPassThru)  { //if text through is disabled no tokens should pass, too
                        if ((!handleEscapeString(text, token, userData)) && (passThruUnknownEsc)) {
                            appendEscapeString(text, token);
                        }
                    }
                    escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
                    lastTextNode = "";
                    continue;
                }
            }
        }

        if (!inEsc) {
            if (*from == tokenEnd[tokenEndPos]) {
                if (tokenEndPos == (tokenEndLen - 1)) {
                    intoken = false;
                    userData->lastTextNode = lastTextNode;
                    if ((!handleToken(text, token, userData)) && (passThruUnknownToken)) {
                        text += tokenStart;
                        text += token;
                        text += tokenEnd;
                    }
                    escEndPos = escStartPos = tokenEndPos = tokenStartPos = 0;
                    lastTextNode = "";
                    continue;
                }
            }
        }

        if (intoken) {
            if (tokpos < 4090) {
                token[tokpos++] = *from;
                token[tokpos+2] = 0;
            }
        }
        else {
             if ((!userData->supressAdjacentWhitespace) || (*from != ' ')) {
                if (!userData->suspendTextPassThru) {
                    text.push_back(*from);
                    userData->lastSuspendSegment.clear();
                }
                else    userData->lastSuspendSegment.push_back(*from);
                lastTextNode.push_back(*from);
             }
            userData->supressAdjacentWhitespace = false;
        }

        if (processStages & POSTCHAR)
            processStage(POSTCHAR, text, from, userData);

    }

    if (processStages & FINALIZE)
        processStage(FINALIZE, text, from, userData);

    delete userData;
    return 0;
}


} /* namespace swordxx */
